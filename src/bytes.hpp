
#pragma once
#include <cstdint>

inline std::uint16_t read_u16_be(const std::uint8_t* p) {
	int it = 0;
		uint16_t k = 0;
  	for(int i = 1;i >= 0;--i){
  		k += (std::uint64_t(p[i]) << it);
  		it += 8;
  	}
  	return k;
}

inline std::uint32_t read_u32_be(const std::uint8_t* p) {
	int it = 0;
	uint32_t k = 0;
	for(int i = 3;i>=0;--i){
		k += (std::uint64_t(p[i]) << it);
		it += 8;
	}
	return k;
}
inline std::uint64_t read_u48_be(const std::uint8_t* p) {
  int it = 0;
	uint64_t k = 0;
	for(int i = 5;i>=0;--i){
		k += (std::uint64_t(p[i]) << it);
		it += 8;
	}
	return k;
}

inline std::uint64_t read_u64_be(const std::uint8_t* p) {
int it = 0;
	uint64_t k = 0;
	for(int i = 7;i>=0;--i){
		k += (std::uint64_t(p[i]) << it);
		it += 8;
	}
	return k;
}
