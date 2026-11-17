#pragma once
#include<meta>
#include<vector>
#include<print>
#include<map>
#include"bytes.hpp"

template<class T>
inline T decode(const std::uint8_t* p){
    T obj{};
    constexpr auto ctx = std::meta::access_context::unchecked();
    template for(constexpr auto j : std::define_static_array(std::meta::nonstatic_data_members_of(^^T,ctx))){
        constexpr auto alpha = std::meta::size_of(j);
        using beta = typename [:std::meta::type_of(j):];
        obj.[:j:] = read_be<beta,alpha>(p);
        p  = p + alpha ;
    }
    return obj;
}
