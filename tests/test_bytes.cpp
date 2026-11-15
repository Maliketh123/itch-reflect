#include"bytes.hpp"
#include<iostream>

static int failures =0 ;

template <class T>
void check(const char* name,T got,T want){
	if(got != want){
		std::cout << name << " " << (unsigned long long)got << " " << (unsigned long long)want;
		++failures;
	}
}

int main(){
	const std::uint8_t b[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  check<std::uint16_t>("u16", read_u16_be(b), 0x0102);
  check<std::uint32_t>("u32", read_u32_be(b), 0x01020304u);
  check<std::uint64_t>("u48", read_u48_be(b), 0x010203040506ull);
  check<std::uint64_t>("u64", read_u64_be(b), 0x0102030405060708ull);

  const std::uint8_t price[4] = {0x00, 0x00, 0x01, 0xF4};
  check<std::uint32_t>("u32 = 500", read_u32_be(price), 500u);

  // High bytes set: catches shifting before widening.
  const std::uint8_t hi[8] = {0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88};
  check<std::uint16_t>("u16 high", read_u16_be(hi), 0xFFEE);
  check<std::uint32_t>("u32 high", read_u32_be(hi), 0xFFEEDDCCu);
  check<std::uint64_t>("u48 high", read_u48_be(hi), 0xFFEEDDCCBBAAull);
  check<std::uint64_t>("u64 high", read_u64_be(hi), 0xFFEEDDCCBBAA9988ull);

  // The generic reader: any result type T and any byte count N.
  const std::uint8_t one[1] = {0x7F};
  check<std::uint8_t>("read_be u8,1", read_be<std::uint8_t, 1>(one), 127);
  check<std::uint16_t>("read_be u16,2", read_be<std::uint16_t, 2>(b), 258);
  check<std::uint32_t>("read_be u32,3", read_be<std::uint32_t, 3>(b), 66051u);
  check<std::uint32_t>("read_be u32,4", read_be<std::uint32_t, 4>(price), 500u);
  check<std::uint64_t>("read_be u64,6", read_be<std::uint64_t, 6>(hi), 281401388481450ull);
  check<std::uint64_t>("read_be u64,8", read_be<std::uint64_t, 8>(b), 72623859790382856ull);

  // Compile-time use: these are evaluated by the compiler, not when the program runs.
  constexpr std::uint8_t cb[4] = {0x00, 0x00, 0x01, 0xF4};
  static_assert(read_be<std::uint32_t, 4>(cb) == 500, "read_be must work at compile time");
  static_assert(read_be<std::uint16_t, 2>(cb + 2) == 500, "read_be must work at compile time");

  if (failures == 0) std::printf("all tests passed\n");
  return failures == 0 ? 0 : 1;
}