#include<cstddef>
#include"bytes.hpp"
#include"itch.hpp"
#include"reflect_decode.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include<algorithm>
#include<chrono>

const int mx = 1e6;

std::uint64_t run_reflect(const std::uint8_t* p, std::size_t size,std::size_t& decoded1){
     std::uint64_t sum = 0;
    decoded1 = 0;
    std::size_t pos = 0;

    while (pos + 2 <= size) {
        std::size_t length = read_u16_be(p + pos);
        if (pos + 2 + length > size) break;           // last message cut short: stop

        const std::uint8_t* msg = p + pos + 2;        // first byte of this message
        char type = char(msg[0]);

        if (type == 'A') {
            AddOrder m = decode<AddOrder>(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.shares + m.price;
            ++decoded1;
        } else if (type == 'E') {
            OrderExecuted m = decode<OrderExecuted>(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.executed_shares + m.match_number;
            ++decoded1;
        } else if (type == 'X') {
            OrderCancel m = decode<OrderCancel>(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.cancelled_shares;
            ++decoded1;
        } else if (type == 'D') {
            OrderDelete m = decode<OrderDelete>(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference;
            ++decoded1;
        } else if (type == 'U') {
            OrderReplace m = decode<OrderReplace>(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.new_order_reference + m.shares + m.price;
            ++decoded1;
        }

        pos += 2 + length;
    }
    return sum;
}

std::uint64_t run_hand(const std::uint8_t* p, std::size_t size, std::size_t& decoded) {
    std::uint64_t sum = 0;
    decoded = 0;
    std::size_t pos = 0;

    while (pos + 2 <= size) {
        std::size_t length = read_u16_be(p + pos);
        if (pos + 2 + length > size) break;           // last message cut short: stop

        const std::uint8_t* msg = p + pos + 2;        // first byte of this message
        char type = char(msg[0]);

        if (type == 'A') {
            AddOrder m = decode_add_order(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.shares + m.price;
            ++decoded;
        } else if (type == 'E') {
            OrderExecuted m = decode_order_executed(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.executed_shares + m.match_number;
            ++decoded;
        } else if (type == 'X') {
            OrderCancel m = decode_order_cancel(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.cancelled_shares;
            ++decoded;
        } else if (type == 'D') {
            OrderDelete m = decode_order_delete(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference;
            ++decoded;
        } else if (type == 'U') {
            OrderReplace m = decode_order_replace(msg);
            sum += m.stock_locate + m.tracking_number + m.timestamp
                 + m.order_reference + m.new_order_reference + m.shares + m.price;
            ++decoded;
        }

        pos += 2 + length;
    }
    return sum;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: bench <file>\n";
        return 2;
    }
    const char* path = argv[1];

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cerr << "cannot open " << path << '\n';
        return 1;
    }
    in.seekg(0, std::ios::end);
    std::size_t size = in.tellg();
    in.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> buffer(size);
    in.read(reinterpret_cast<char*>(buffer.data()), size);

    std::size_t decoded_hand = 0, decoded_reflect = 0;
    std::uint64_t sum_hand = 0,sum_reflect = 0;

    std::vector<double> hand_ms,reflect_ms;

    for(int r = 0; r<5;++r){
        auto t0 = std::chrono::steady_clock::now();
        sum_hand = run_hand(buffer.data(),size,decoded_hand);
        auto t1 = std::chrono::steady_clock::now();
        sum_reflect = run_reflect(buffer.data(),size,decoded_reflect);
        auto t2 = std::chrono::steady_clock::now();
        hand_ms.push_back(std::chrono::duration<double,std::milli>(t1 - t0).count());
        reflect_ms.push_back(std::chrono::duration<double,std::milli>(t2 - t1).count());
    }
    std::sort(hand_ms.begin(),hand_ms.end());
    std::sort(reflect_ms.begin(),reflect_ms.end());

    if(sum_hand != sum_reflect){
        std::cerr << "MISMATCH" << '\n';
        return 1; 
    }
    std::cout << "file: " << path << '\n';
    std::cout << "bytes: " << size << '\n';
    std::cout << "decoded: " << decoded_hand << '\n';
    std::cout << "checksum_hand: " << sum_hand << '\n';
    std::cout << "checksum_reflect: " << sum_reflect << '\n';
    std::cout << "hand_ms: " << hand_ms[0] << " (median " << hand_ms[2] << ")\n";
    std::cout << "reflect_ms: " << reflect_ms[0] << " (median " << reflect_ms[2] << ")\n";
    std::cout << "hand_ns_per_msg: " << hand_ms[0]*mx/decoded_hand << '\n';
    std::cout << "reflect_ns_per_msg: " << reflect_ms[0]*mx/decoded_reflect << '\n';

    return 0;
}