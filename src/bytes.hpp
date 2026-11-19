#pragma once
#include <cstdint>
#include<cstddef>
#include<cstring>
#include<bit>

template<class T,std::size_t N>
constexpr T read_be(const std::uint8_t* p){
	static_assert(N <= sizeof(T),"read_be : N bytes do no fit in T");
	static_assert(N >= 1 ,"read_be : N should at least be 1");
	static_assert(std::endian::native == std::endian::little, "read_be: needs a little-endian machine");
		if consteval{
			int it = 0;
			T k = 0;
  	for(int i = N-1;i >= 0;--i){
  		k += (std::uint64_t(p[i]) << it);
  		it += 8;
  	}
	return k;
}
else{
	std::uint64_t rot{};
	std::memcpy(&rot,p,N);
	auto it = std::byteswap(rot);
	std::size_t shift = 8*(8-N);
	it >>= shift;
	return static_cast<T>(it);

}
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
