#include "itch.hpp"
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

// Compare two pieces of text; print a line only when they differ.
void check_text(const std::string& name, const std::string& got, const std::string& want) {
  if (got != want) {
    std::cout << "FAIL " << name << ": got '" << got << "', want '" << want << "'\n";
    ++failures;
  }
}

// The five fields every message starts with. Works for any of the message structs.
template <class Message>
void check_common(const std::string& prefix, const Message& m, char type) {
  check_text(prefix + ".message_type", std::string(1, m.message_type), std::string(1, type));
  check_num(prefix + ".stock_locate", m.stock_locate, 1);
  check_num(prefix + ".tracking_number", m.tracking_number, 2);
  check_num(prefix + ".timestamp", m.timestamp, 1000);
  check_num(prefix + ".order_reference", m.order_reference, 42);
}

void test_add_order() {
  const std::uint8_t msg[36] = {
      0x41,                                            // message type 'A'
      0x00, 0x01,                                      // stock locate = 1
      0x00, 0x02,                                      // tracking number = 2
      0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,              // timestamp = 1000
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,  // order reference = 42
      0x42,                                            // side 'B'
      0x00, 0x00, 0x00, 0x64,                          // shares = 100
      0x41, 0x41, 0x50, 0x4C, 0x20, 0x20, 0x20, 0x20,  // stock "AAPL    "
      0x00, 0x16, 0xE3, 0x60                           // price = 1500000
  };
  AddOrder m = decode_add_order(msg);
  check_common("add_order", m, 'A');
  check_text("add_order.side", std::string(1, m.side), "B");
  check_num("add_order.shares", m.shares, 100);
  check_text("add_order.stock", std::string(m.stock.begin(), m.stock.end()), "AAPL    ");
  check_num("add_order.price", m.price, 1500000);
}

void test_order_executed() {
  const std::uint8_t msg[31] = {
      0x45,                                            // message type 'E'
      0x00, 0x01,                                      // stock locate = 1
      0x00, 0x02,                                      // tracking number = 2
      0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,              // timestamp = 1000
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,  // order reference = 42
      0x00, 0x00, 0x00, 0x32,                          // executed shares = 50
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07   // match number = 7
  };
  OrderExecuted m = decode_order_executed(msg);
  check_common("order_executed", m, 'E');
  check_num("order_executed.executed_shares", m.executed_shares, 50);
  check_num("order_executed.match_number", m.match_number, 7);
}

void test_order_cancel() {
  const std::uint8_t msg[23] = {
      0x58,                                            // message type 'X'
      0x00, 0x01,                                      // stock locate = 1
      0x00, 0x02,                                      // tracking number = 2
      0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,              // timestamp = 1000
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,  // order reference = 42
      0x00, 0x00, 0x00, 0x19                           // cancelled shares = 25
  };
  OrderCancel m = decode_order_cancel(msg);
  check_common("order_cancel", m, 'X');
  check_num("order_cancel.cancelled_shares", m.cancelled_shares, 25);
}

void test_order_delete() {
  const std::uint8_t msg[19] = {
      0x44,                                            // message type 'D'
      0x00, 0x01,                                      // stock locate = 1
      0x00, 0x02,                                      // tracking number = 2
      0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,              // timestamp = 1000
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A   // order reference = 42
  };
  OrderDelete m = decode_order_delete(msg);
  check_common("order_delete", m, 'D');
}

void test_order_replace() {
  const std::uint8_t msg[35] = {
      0x55,                                            // message type 'U'
      0x00, 0x01,                                      // stock locate = 1
      0x00, 0x02,                                      // tracking number = 2
      0x00, 0x00, 0x00, 0x00, 0x03, 0xE8,              // timestamp = 1000
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A,  // original order reference = 42
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2B,  // new order reference = 43
      0x00, 0x00, 0x00, 0xC8,                          // shares = 200
      0x00, 0x16, 0xE7, 0x48                           // price = 1501000
  };
  OrderReplace m = decode_order_replace(msg);
  check_common("order_replace", m, 'U');
  check_num("order_replace.new_order_reference", m.new_order_reference, 43);
  check_num("order_replace.shares", m.shares, 200);
  check_num("order_replace.price", m.price, 1501000);
}

int main() {
  test_add_order();
  test_order_executed();
  test_order_cancel();
  test_order_delete();
  test_order_replace();

  if (failures == 0) std::cout << "all tests passed\n";
  return failures == 0 ? 0 : 1;
}
