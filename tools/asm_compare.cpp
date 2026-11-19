#include"itch.hpp"
#include"reflect_decode.hpp"

extern "C" void hand_add_order(const std::uint8_t* p,AddOrder* out){

    *out = decode_add_order(p);

}

extern "C" void gen_add_order(const std::uint8_t* p,AddOrder* out){
    *out = decode<AddOrder>(p);
}