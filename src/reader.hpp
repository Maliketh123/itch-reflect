#pragma once
#include"itch.hpp"

struct Summary{
std::uint64_t add_orders = 0;
std::uint64_t orders_executed = 0;
std::uint64_t orders_cancelled = 0;
std::uint64_t orders_deleted = 0;
std::uint64_t orders_replaced = 0;
std::uint64_t other = 0;
std::uint64_t total_added_shares = 0;
std::uint64_t total_executed_shares = 0;
};

inline Summary summarize(const std:: uint8_t* p,std::size_t size){
    std::uint64_t length=0;
    std::uint64_t pos=0;
    Summary b;
    while(pos + 2 <= size){
        length = read_u16_be(p+pos);
        if(pos + 2 + length > size){
            break;
        }
        if(char(p[pos+2]) == 'A'){
            b.add_orders++;
            b.total_added_shares += decode_add_order(p+pos+2).shares;
        }
        else if(char(p[pos+2]) == 'E'){
            b.orders_executed++;
            b.total_executed_shares += decode_order_executed(p+pos+2).executed_shares;
        }
        else if(char(p[pos+2]) == 'D'){
            b.orders_deleted++;
        }
        else if(char(p[pos+2]) == 'U'){
            b.orders_replaced++;
        }
        else if(char(p[pos+2]) == 'X'){
            b.orders_cancelled++;
        }
        else{
            b.other++;
        }
        pos += length + 2;
    }
return  b;
}