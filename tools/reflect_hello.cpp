#include<iostream>
#include<meta>
#include<print>
#include<string>
#include"itch.hpp"

template<typename T>
consteval auto count_names(){
    constexpr auto ctx = std::meta::access_context::unchecked();
    return std::meta::nonstatic_data_members_of(^^T,ctx).size();
}

template<typename T>
consteval auto field_names(){
    std::string out;
    constexpr auto ctx = std::meta::access_context::unchecked();
    for(auto m : std::meta::nonstatic_data_members_of(^^T,ctx)){
        out += std::meta::identifier_of(m);
        out += '\n';
    }
    return out;
}

int main(){
  static constexpr const char* names = std::define_static_string(field_names<AddOrder>());
  static constexpr auto count = count_names<AddOrder>();
    std::print("{}",names);
    std::println("members: {}", count);
    return 0;
}

