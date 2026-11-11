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

  if (failures == 0) std::printf("all tests passed\n");
  return failures == 0 ? 0 : 1;
}