#pragma once
#include <cstdint>
#include<cstddef>

template<class T,std::size_t N>
constexpr T read_be(const uint8_t* p){
	static_assert(N <= sizeof(T),"read_be : N bytes do no fit in T");
int it = 0;
		T k = 0;
  	for(int i = N-1;i >= 0;--i){
  		k += (std::uint64_t(p[i]) << it);
  		it += 8;
  	}
  	return k;
}

inline std::uint16_t read_u16_be(const std::uint8_t* p) {
	return read_be<std::uint16_t,2>(p);
}

inline std::uint32_t read_u32_be(const std::uint8_t* p) {
	return read_be<std::uint32_t,4>(p);
}
inline std::uint64_t read_u48_be(const std::uint8_t* p) {
  return read_be<std::uint64_t,6>(p);
}

inline std::uint64_t read_u64_be(const std::uint8_t* p) {
	return read_be<std::uint64_t,8>(p);
}
