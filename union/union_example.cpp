#include <cstdint>
#include <typeinfo>
#include <spdlog/spdlog.h>

/*
 * GCC/Clang `typeid(T).name()` Quick Reference (Itanium C++ ABI Mangling)
 * =====================================================================
 * 
 * Basic Types:
 * -----------
 * bool                -> "b"
 * char                -> "c"
 * signed char         -> "a"
 * unsigned char       -> "h"
 * short               -> "s"
 * unsigned short      -> "t"
 * int                 -> "i"
 * unsigned int        -> "j"
 * long                -> "l"
 * unsigned long       -> "m"
 * long long           -> "x"
 * unsigned long long  -> "y"
 * float               -> "f"
 * double              -> "d"
 * long double         -> "e"
 * void                -> "v"
 * 
 * Pointers & Arrays:
 * ------------------
 * int*                -> "Pi"
 * const int*          -> "PKi"
 * char*               -> "Pc"
 * const char*         -> "PKc"
 * int[5]              -> "A5_i"
 * 
 * Standard Library Types:
 * ----------------------
 * std::string         -> "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE"
 * std::vector<int>    -> "St6vectorIiSaIiEE"
 * 
 * Demangling:
 * -----------
 * Use `c++filt` (Linux/macOS) to decode mangled names, e.g.:
 *   $ c++filt NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
 *   Output: std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >
 */

// Taken from mavlink_types.h 
// Union helps a lot when you are trying to save memory in your application
// In this example, you will see that all of the definitions scoped within the union
// are placed at the same memory, hence accessible
typedef struct param_union {
	union {
		float param_float;
		int32_t param_int32;
		uint32_t param_uint32;
		int16_t param_int16;
		uint16_t param_uint16;
		int8_t param_int8;
		uint8_t param_uint8;
		uint8_t bytes[4];
	};
} mavlink_param_union_t;

int main() {
    
    // All int type (both unsigned and signed) will output the correct value
    uint8_t tmp = 1;
    mavlink_param_union_t mav_type;
    mav_type.param_uint8 = tmp;
    // Cast it as a generic pointer representation to be able to print the address
    spdlog::info("Union address: {}", (void *)&mav_type);
    spdlog::info("Size: {} bytes", sizeof(mav_type));
    spdlog::info("Current value as float: {}", mav_type.param_float);
    spdlog::info("Current value as uint8_t: {}", mav_type.param_uint8);
    spdlog::info("Current value as uint16_t: {}", mav_type.param_uint16);
    spdlog::info("Current value as int16_t: {}", mav_type.param_int16);

    // Both uint8 and int16 will overflow, giving garbage values
    // float will be wrong in this case
    uint16_t tmp_uint16 = 40000;
    mav_type.param_uint16 = tmp_uint16;
    // Cast it as a generic pointer representation to be able to print the address
    spdlog::info("Union address: {}", (void *)&mav_type);
    spdlog::info("Size: {} bytes", sizeof(mav_type));
    spdlog::info("Current value as float: {}", mav_type.param_float);
    spdlog::info("Current value as uint8_t: {}", mav_type.param_uint8);
    spdlog::info("Current value as uint16_t: {}", mav_type.param_uint16);
    spdlog::info("Current value as int16_t: {}", mav_type.param_int16);

    float tmp_float = 2.0f;
    mav_type.param_float = tmp_float;
    spdlog::info("Union address: {}", (void *)&mav_type);
    spdlog::info("Size: {} bytes", sizeof(mav_type));
    spdlog::info("Current value as float: {}", mav_type.param_float);
    spdlog::info("Current value as uint8_t: {}", mav_type.param_uint8);
    spdlog::info("Current value as uint16_t: {}", mav_type.param_uint16);
    spdlog::info("Current value as int16_t: {}", mav_type.param_int16);

    // float will return something wrong because the float bit decoding will be different
    // uint8 and uint16 will overflow
    // int16 will output the correct value
    int16_t tmp_int16 = -1;
    mav_type.param_int16 = tmp_int16;
    spdlog::info("Union address: {}", (void *)&mav_type);
    spdlog::info("Size: {} bytes", sizeof(mav_type));
    spdlog::info("Current value as float: {}", mav_type.param_float);
    spdlog::info("Current value as uint8_t: {}", mav_type.param_uint8);
    spdlog::info("Current value as uint16_t: {}", mav_type.param_uint16);
    spdlog::info("Current value as int16_t: {}", mav_type.param_int16);

    return 0;
}