#ifndef VANORDER_STO_UTILS_HPP
#define VANORDER_STO_UTILS_HPP

#include <cstdint>
#include <type_traits>

namespace vanorder
{
    namespace utils
    {
        template< class NumType > constexpr char const * sto_func_name( NumType const & );
        template<> inline constexpr char const *  sto_func_name<  int8_t>(   int8_t const & ) { return    "vanorder::sto<int8_t>"; }
        template<> inline constexpr char const *  sto_func_name< uint8_t>(  uint8_t const & ) { return   "vanorder::sto<uint8_t>"; }
        template<> inline constexpr char const *  sto_func_name< int16_t>(  int16_t const & ) { return   "vanorder::sto<int16_t>"; }
        template<> inline constexpr char const *  sto_func_name<uint16_t>( uint16_t const & ) { return  "vanorder::sto<uint16_t>"; }
        template<> inline constexpr char const *  sto_func_name< int32_t>(  int32_t const & ) { return   "vanorder::sto<int32_t>"; }
        template<> inline constexpr char const *  sto_func_name<uint32_t>( uint32_t const & ) { return  "vanorder::sto<uint32_t>"; }
        template<> inline constexpr char const *  sto_func_name< int64_t>(  int64_t const & ) { return   "vanorder::sto<int64_t>"; }
        template<> inline constexpr char const *  sto_func_name<uint64_t>( uint64_t const & ) { return  "vanorder::sto<uint64_t>"; }
        
        template< class NumType > constexpr char const * xsto_func_name( NumType const & );
        template<> inline constexpr char const * xsto_func_name<  int8_t>(   int8_t const & ) { return   "vanorder::xsto<int8_t>"; }
        template<> inline constexpr char const * xsto_func_name< uint8_t>(  uint8_t const & ) { return  "vanorder::xsto<uint8_t>"; }
        template<> inline constexpr char const * xsto_func_name< int16_t>(  int16_t const & ) { return  "vanorder::xsto<int16_t>"; }
        template<> inline constexpr char const * xsto_func_name<uint16_t>( uint16_t const & ) { return "vanorder::xsto<uint16_t>"; }
        template<> inline constexpr char const * xsto_func_name< int32_t>(  int32_t const & ) { return  "vanorder::xsto<int32_t>"; }
        template<> inline constexpr char const * xsto_func_name<uint32_t>( uint32_t const & ) { return "vanorder::xsto<uint32_t>"; }
        template<> inline constexpr char const * xsto_func_name< int64_t>(  int64_t const & ) { return  "vanorder::xsto<int64_t>"; }
        template<> inline constexpr char const * xsto_func_name<uint64_t>( uint64_t const & ) { return "vanorder::xsto<uint64_t>"; }
        
        template< class NumType > constexpr char const * osto_func_name( NumType const & );
        template<> inline constexpr char const * osto_func_name<  int8_t>(   int8_t const & ) { return   "vanorder::osto<int8_t>"; }
        template<> inline constexpr char const * osto_func_name< uint8_t>(  uint8_t const & ) { return  "vanorder::osto<uint8_t>"; }
        template<> inline constexpr char const * osto_func_name< int16_t>(  int16_t const & ) { return  "vanorder::osto<int16_t>"; }
        template<> inline constexpr char const * osto_func_name<uint16_t>( uint16_t const & ) { return "vanorder::osto<uint16_t>"; }
        template<> inline constexpr char const * osto_func_name< int32_t>(  int32_t const & ) { return  "vanorder::osto<int32_t>"; }
        template<> inline constexpr char const * osto_func_name<uint32_t>( uint32_t const & ) { return "vanorder::osto<uint32_t>"; }
        template<> inline constexpr char const * osto_func_name< int64_t>(  int64_t const & ) { return  "vanorder::osto<int64_t>"; }
        template<> inline constexpr char const * osto_func_name<uint64_t>( uint64_t const & ) { return "vanorder::osto<uint64_t>"; }
    }
    
    namespace utils
    {
        template< class NumType >
        inline constexpr
        typename std::enable_if<
                                std::numeric_limits< NumType >::is_specialized
                                , NumType
                 >::type
        maxval( NumType )
        {
            return std::numeric_limits< NumType >::max();
        }
    }
    
    namespace utils
    {
        constexpr char const invalid_hexdigit = maxval( char() );
        constexpr char const invalid_octdigit = maxval( char() );
    }
    
    namespace utils
    {
        inline constexpr 
        char hexdigit_value( char c )
        {
            return c >= '0' and c <= '9'
                ? c - '0'
                : c >= 'a' and c <= 'f'
                    ? c - 'a' 
                    : c >= 'A' and c <= 'F'
                        ? c - 'A'
                        : invalid_hexdigit
                ;
        }
        
        inline constexpr 
        char octdigit_value( char c )
        {
            return c >= '0' and c <= '7'
                ? c - '0'
                : invalid_octdigit
                ;
        }
    }
} // namespace vanorder

#endif // VANORDER_STO_UTILS_HPP