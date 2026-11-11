#include "itch.hpp"
#include <cstdint>
#include <iostream>
#include <string>

static int failures = 0;

// Compare two numbers; print a line only when they differ.
void check_num(const char* name, std::uint64_t got, std::uint64_t want) {
  if (got != want) {
    std::cout << "FAIL " << name << ": got " << got << ", want " << want << '\n';
    ++failures;
  }
}

// Compare two pieces of text; print a line only when they differ.
void check_text(const char* name, const std::string& got, const std::string& want) {
  if (got != want) {
    std::cout << "FAIL " << name << ": got '" << got << "', want '" << want << "'\n";
    ++failures;
  }
}

int main() {
  // The 36-byte sample Add Order message.
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

  check_text("message_type", std::string(1, m.message_type), "A");
  check_num("stock_locate", m.stock_locate, 1);
  check_num("tracking_number", m.tracking_number, 2);
  check_num("timestamp", m.timestamp, 1000);
  check_num("order_reference", m.order_reference, 42);
  check_text("side", std::string(1, m.side), "B");
  check_num("shares", m.shares, 100);
  check_text("stock", std::string(m.stock.begin(), m.stock.end()), "AAPL    ");
  check_num("price", m.price, 1500000);

  if (failures == 0) std::cout << "all tests passed\n";
  return failures == 0 ? 0 : 1;
}