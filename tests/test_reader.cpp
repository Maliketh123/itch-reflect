#include "reader.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

static int failures = 0;

// Compare two numbers; print a line only when they differ.
void check_num(const std::string& name, std::uint64_t got, std::uint64_t want) {
  if (got != want) {
    std::cout << "FAIL " << name << ": got " << got << ", want " << want << '\n';
    ++failures;
  }
}

// Check every member of a Summary against the expected values.
void check_summary(const std::string& prefix, const Summary& got, const Summary& want) {
  check_num(prefix + ".add_orders", got.add_orders, want.add_orders);
  check_num(prefix + ".orders_executed", got.orders_executed, want.orders_executed);
  check_num(prefix + ".orders_cancelled", got.orders_cancelled, want.orders_cancelled);
  check_num(prefix + ".orders_deleted", got.orders_deleted, want.orders_deleted);
  check_num(prefix + ".orders_replaced", got.orders_replaced, want.orders_replaced);
  check_num(prefix + ".other", got.other, want.other);
  check_num(prefix + ".total_added_shares", got.total_added_shares, want.total_added_shares);
  check_num(prefix + ".total_executed_shares", got.total_executed_shares, want.total_executed_shares);
}

// Four records, each a 2-byte length followed by the message. 106 bytes in total.
static const std::uint8_t block[106] = {
    0x00, 0x24,  // length 36: Add Order
    0x41, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x42,
    0x00, 0x00, 0x00, 0x64, 0x41, 0x41, 0x50, 0x4C, 0x20, 0x20, 0x20, 0x20,
    0x00, 0x16, 0xE3, 0x60,

    0x00, 0x1F,  // length 31: Order Executed
    0x45, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,
    0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,

    0x00, 0x13,  // length 19: Order Delete
    0x44, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,

    0x00, 0x0C,  // length 12: System Event, a type we do not decode
    0x53, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F
};

int main() {
  // Sample 1: the whole block.
  Summary want1;
  want1.add_orders = 1;
  want1.orders_executed = 1;
  want1.orders_deleted = 1;
  want1.other = 1;
  want1.total_added_shares = 100;
  want1.total_executed_shares = 50;
  check_summary("sample1", summarize(block, 106), want1);

  // Sample 2: the last message is cut short, so it must not be counted.
  Summary want2 = want1;
  want2.other = 0;
  check_summary("sample2", summarize(block, 101), want2);

  // Sample 3: an empty block gives all zeros.
  check_summary("sample3", summarize(block, 0), Summary{});

  if (failures == 0) std::cout << "all tests passed\n";
  return failures == 0 ? 0 : 1;
}
