#pragma once 
#include"bytes.hpp"
#include<array>

struct AddOrder{
	char message_type;//1
	std::uint16_t stock_locate;//2
	std::uint16_t tracking_number;//2
	std::uint64_t timestamp;//6
	std::uint64_t order_reference;//8
	char side;//1
	std::uint32_t shares;//4
	std::array<char,8> stock;
	std::uint32_t price; //4
};

inline AddOrder decode_add_order(const std::uint8_t* p){
	AddOrder system;
	
	system.message_type = char(p[0]);
	system.stock_locate = read_u16_be(p+1);
	system.tracking_number = read_u16_be(p+3);
	system.timestamp = read_u48_be(p+5);
	system.order_reference = read_u64_be(p + 11);
	system.side = char(p[19]);
	system.shares = read_u32_be(p+20);
	for(int i = 24,j = 0;i<32;++i,++j){
		system.stock[j] = char(p[i]);
	}
	system.price = read_u32_be(p+32);
	
	return system;
}