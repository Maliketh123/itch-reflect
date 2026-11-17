#include<meta>
#include<print>
#include<string>
#include"itch.hpp"

consteval void append_number(std::string& out, std::size_t n) {
    std::string digits;
    do {
        digits.insert(digits.begin(), char('0' + n % 10));
        n /= 10;
    } while (n > 0);
    out += digits;
}

template<typename T>
consteval auto count_names(){
    constexpr auto ctx = std::meta::access_context::unchecked();
    return std::meta::nonstatic_data_members_of(^^T,ctx).size();
}

template<typename T>
consteval auto field_names(){
    std::string out;
    std::size_t total = 0;
    constexpr auto ctx = std::meta::access_context::unchecked();
    for(auto m : std::meta::nonstatic_data_members_of(^^T,ctx)){
        out += std::meta::identifier_of(m);
        out += ": ";
        out += std::meta::display_string_of(std::meta::type_of(m));
        out += ", ";
      append_number(out,std::meta::size_of(m));
      total += std::meta::size_of(m);
      out += " bytes";
        out += '\n';
    }
    out += "total: ";
    append_number(out,total);
    out += " bytes\n";
    return out;
}

int main(){
    static constexpr const char* names = std::define_static_string(field_names<AddOrder>());
    static auto count = count_names<AddOrder>();
    std::print("{}",names);
    return 0;
}