#include"reader.hpp"
#include<fstream>
#include<chrono>
#include<cstdint>
#include<iostream>
#include<vector>
#include<cstddef>
int main(int argc,char** argv){
    if(argc!=2){
        std::cerr << "usage: itch_summary <file>\n";
        return 2;
    }
    const char* path = argv[1];

    std::ifstream in(path,std::ios::binary);
    if(!in){
        std::cerr << "cannot open " << path <<'\n';
        return 1;
    }

    in.seekg(0,std::ios::end);
    std::size_t size = in.tellg();

    in.seekg(0,std::ios::beg);
    std::vector<std::uint8_t> buffer(size);
    in.read(reinterpret_cast<char*>(buffer.data()),size);

    auto start = std::chrono::steady_clock::now();
    Summary M = summarize(buffer.data(),size);
    auto stop = std::chrono::steady_clock::now();

    double ms = std::chrono::duration<double,std::milli>(stop - start).count();

    std::cout << "file: " << path << '\n';
    std::cout << "bytes: " << size << '\n';
    std::cout << "add_orders: " << M.add_orders << '\n';
    std::cout << "orders_executed: "<<M.orders_executed<< '\n';
    std::cout << "orders_cancelled: "<<M.orders_cancelled<< '\n';
    std::cout << "orders_deleted: " <<M.orders_deleted << '\n';
    std::cout << "orders_replaced: "<<M.orders_replaced << '\n';
    std::cout << "other: " << M.other << '\n';
    std::cout << "total_added_shares: " << M.total_added_shares << '\n';
    std::cout << "total_executed_shares: " << M.total_executed_shares << '\n';
    std::cout << "messages: "
          << M.add_orders + M.orders_executed + M.orders_cancelled +
             M.orders_deleted + M.orders_replaced + M.other
          << '\n';
    std::cout << "elapsed_ms: " << ms << '\n';
    
    return 0;
}