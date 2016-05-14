#include <chrono>
#include <iostream>
#include <string>

#include "sto.hpp"




template< class NumType > constexpr char const * min_cstr( NumType );

template<> constexpr char const * min_cstr<int8_t>( int8_t ) { return "-128"; }
template<> constexpr char const * min_cstr<int16_t>( int16_t ) { return "-32768"; }
template<> constexpr char const * min_cstr<int32_t>( int32_t ) { return "-2147483648"; }
template<> constexpr char const * min_cstr<int64_t>( int64_t ) { return "-9223372036854775808"; }

template<> constexpr char const * min_cstr<uint8_t>( uint8_t ) { return "0"; }
template<> constexpr char const * min_cstr<uint16_t>( uint16_t ) { return "0"; }
template<> constexpr char const * min_cstr<uint32_t>( uint32_t ) { return "0"; }
template<> constexpr char const * min_cstr<uint64_t>( uint64_t ) { return "0"; }


template< class NumType > constexpr char const * max_cstr( NumType );

template<> constexpr char const * max_cstr<int8_t>( int8_t ) { return "127"; }
template<> constexpr char const * max_cstr<int16_t>( int16_t ) { return "32767"; }
template<> constexpr char const * max_cstr<int32_t>( int32_t ) { return "2147483647"; }
template<> constexpr char const * max_cstr<int64_t>( int64_t ) { return "9223372036854775807"; }

template<> constexpr char const * max_cstr<uint8_t>( uint8_t ) { return "255"; }
template<> constexpr char const * max_cstr<uint16_t>( uint16_t ) { return "65535"; }
template<> constexpr char const * max_cstr<uint32_t>( uint32_t ) { return "4294967295"; }
template<> constexpr char const * max_cstr<uint64_t>( uint64_t ) { return "18446744073709551615"; }


template< class NumType > NumType standard_sto( const char * cstr );

template<> int8_t    standard_sto( const char * cstr ) { return std::stoi( cstr ); }
template<> int16_t   standard_sto( const char * cstr ) { return std::stoi( cstr ); }
template<> int       standard_sto( const char * cstr ) { return std::stoi( cstr ); }

template<>
std::enable_if<
    std::is_same< long, long long >::value == false
    , long
>::type
standard_sto( const char * cstr )
{
    return std::stol( cstr );
}

template<> long long standard_sto( const char * cstr ) { return std::stoll( cstr ); }


template<> uint8_t    standard_sto( const char * cstr ) { return std::stoul( cstr ); }
template<> uint16_t   standard_sto( const char * cstr ) { return std::stoul( cstr ); }
template<> uint32_t   standard_sto( const char * cstr ) { return std::stoul( cstr ); }

template<>
std::enable_if<
    std::is_same< unsigned long, unsigned long long >::value == false
    , unsigned long
>::type
standard_sto( const char * cstr )
{
    return std::stoul( cstr );
}

template<> unsigned long long standard_sto( const char * cstr ) { return std::stoull( cstr ); }




namespace
{
    template< class NumType > constexpr char const * stdsto_func_name( NumType const & );
    template<> inline constexpr char const *  stdsto_func_name<  int8_t>(   int8_t const & ) { return    "     std::sto<int8_t>"; }
    template<> inline constexpr char const *  stdsto_func_name< uint8_t>(  uint8_t const & ) { return   "     std::sto<uint8_t>"; }
    template<> inline constexpr char const *  stdsto_func_name< int16_t>(  int16_t const & ) { return   "     std::sto<int16_t>"; }
    template<> inline constexpr char const *  stdsto_func_name<uint16_t>( uint16_t const & ) { return  "     std::sto<uint16_t>"; }
    template<> inline constexpr char const *  stdsto_func_name< int32_t>(  int32_t const & ) { return   "     std::sto<int32_t>"; }
    template<> inline constexpr char const *  stdsto_func_name<uint32_t>( uint32_t const & ) { return  "     std::sto<uint32_t>"; }
    template<> inline constexpr char const *  stdsto_func_name< int64_t>(  int64_t const & ) { return   "     std::sto<int64_t>"; }
    template<> inline constexpr char const *  stdsto_func_name<uint64_t>( uint64_t const & ) { return  "     std::sto<uint64_t>"; }
}
template< class NumType >
constexpr char const * parser_namespace( decltype(standard_sto< NumType >) ) { return stdsto_func_name(NumType()); }

template< class NumType >
constexpr char const * parser_namespace( decltype(vanorder::sto< NumType >) ) { return vanorder::utils::sto_func_name(NumType()); }



template< class NumType, size_t cycles, typename Parser >
std::chrono::nanoseconds ppt( Parser parser )
{
    using ClearNumType = typename std::remove_cv<NumType>::type;

    volatile ClearNumType numvalue = 0;
    const char * strnum = std::numeric_limits<NumType>::is_signed 
                          ? min_cstr( decltype(numvalue)() ) 
                          : max_cstr( decltype(numvalue)() );

    auto start = std::chrono::high_resolution_clock::now();
    for( size_t i = 0; i < cycles; ++i )
    {
        numvalue = parser( strnum );
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::nanoseconds ns = end - start;
    auto ms = std::chrono::duration_cast< std::chrono::milliseconds >( ns );
    std::cout << parser_namespace<NumType>(parser)
        << " for " << cycles << " cycles took: " << ms.count() << "ms. "
        << ((double)(ns.count()) / cycles) << "ns per cycle." 
        << std::endl;

    return ns;
 }
 

template< class NumType >
bool performance_test( NumType )
{
    enum : size_t { cycles = 100*1000*1000 };
    
    auto standard_ns = ppt< NumType, cycles >( standard_sto< NumType > );
    auto vanorder_ns = ppt< NumType, cycles >( vanorder::sto< NumType > );
    
    if( standard_ns > vanorder_ns )
    {
        std::cout << "Test success.\n" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Test failure.\n" << std::endl;
        return false;
    }
}

int main( int ac, char * av[] )
{
    int success = 1;
    
    success &= performance_test( int8_t() );
    success &= performance_test( int16_t() );
    success &= performance_test( int32_t() );
    success &= performance_test( int64_t() );
    
    success &= performance_test( uint8_t() );
    success &= performance_test( uint16_t() );
    success &= performance_test( uint32_t() );
    success &= performance_test( uint64_t() );
    
    return success ? 0 : 1 ;
}
