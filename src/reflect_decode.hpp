#pragma once
#include<meta>
#include<type_traits>
#include<array>
#include<cstddef>
#include"wire.hpp"
#include"bytes.hpp"

consteval std::size_t wire_width(std::meta::info j){
     auto a = std::meta::annotations_of(j);
    if(a.empty()){
        return std::meta::size_of(j);
    }
   return std::meta::extract<Wire>(a[0]).bytes;
}
template<class T>
inline T decode(const std::uint8_t* p){
    T obj{};
    constexpr auto ctx = std::meta::access_context::unchecked();
    template for(constexpr auto j : std::define_static_array(std::meta::nonstatic_data_members_of(^^T,ctx))){
        constexpr auto alpha = wire_width(j);
        using beta = typename [:std::meta::type_of(j):];
        if constexpr(std::is_same_v<beta,std::array<char,8>>){
            for(size_t i = 0;i<alpha;++i){
                obj.[:j:][i] = char(p[i]);
            }
        }
         else{
                obj.[:j:] = read_be<beta,alpha>(p);
            }
        p  = p + alpha ;
    }
    return obj;
}
