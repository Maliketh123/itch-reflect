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

struct OrderExecuted{
	char message_type;
	std::uint16_t stock_locate;//2
	std::uint16_t tracking_number;//2
	std::uint64_t timestamp;//6
	std::uint64_t order_reference;//8
	std::uint32_t executed_shares;
	std::uint64_t match_number;
};

inline OrderExecuted decode_order_executed(const std::uint8_t* p){
	OrderExecuted m;
	m.message_type = char(p[0]);
	m.stock_locate = read_u16_be(p+1);
	m.tracking_number = read_u16_be(p+3);
	m.timestamp = read_u48_be(p+5);
	m.order_reference = read_u64_be(p+11);
	m.executed_shares = read_u32_be(p+19);
	m.match_number = read_u64_be(p+23);
	return	m;
}

struct OrderCancel{
	char message_type;
	std::uint16_t stock_locate;//2
	std::uint16_t tracking_number;//2
	std::uint64_t timestamp;//6
	std::uint64_t order_reference;//8
	std::uint32_t cancelled_shares;
};

inline OrderCancel decode_order_cancel(const std::uint8_t* p){
	OrderCancel	m;
	m.message_type = char(p[0]);
	m.stock_locate = read_u16_be(p+1);
	m.tracking_number = read_u16_be(p+3);
	m.timestamp = read_u48_be(p+5);
	m.order_reference = read_u64_be(p+11);
	m.cancelled_shares = read_u32_be(p+19);

	return	m;
}
struct OrderDelete{
	char message_type;
	std::uint16_t stock_locate;//2
	std::uint16_t tracking_number;//2
	std::uint64_t timestamp;//6
	std::uint64_t order_reference;//8
};
inline OrderDelete decode_order_delete(const std::uint8_t* p){
	OrderDelete	m;
	m.message_type = char(p[0]);
	m.stock_locate = read_u16_be(p+1);
	m.tracking_number = read_u16_be(p+3);
	m.timestamp = read_u48_be(p+5);
	m.order_reference = read_u64_be(p+11);

	return	m;
}
struct OrderReplace{
	char message_type;
	std::uint16_t stock_locate;//2
	std::uint16_t tracking_number;//2
	std::uint64_t timestamp;//6
	std::uint64_t order_reference;//8
	std::uint64_t new_order_reference;
	std::uint32_t shares;
	std::uint32_t price;
};
inline OrderReplace decode_order_replace(const std::uint8_t* p){
	OrderReplace m;
	m.message_type = char(p[0]);
	m.stock_locate = read_u16_be(p+1);
	m.tracking_number = read_u16_be(p+3);
	m.timestamp = read_u48_be(p+5);
	m.order_reference = read_u64_be(p+11);
	m.new_order_reference = read_u64_be(p+19);
	m.shares = read_u32_be(p+27);
	m.price = read_u32_be(p+31);
	
	return	m;
}

inline AddOrder decode_add_order(const std::uint8_t* p){
	AddOrder m;
	m.message_type = char(p[0]);
	m.stock_locate = read_u16_be(p+1);
	m.tracking_number = read_u16_be(p+3);
	m.timestamp = read_u48_be(p+5);
	m.order_reference = read_u64_be(p + 11);
	m.side = char(p[19]);
	m.shares = read_u32_be(p+20);
	for(int i = 24,j = 0;i<32;++i,++j){
		m.stock[j] = char(p[i]);
	}
	m.price = read_u32_be(p+32);
	
	return	m;
}

