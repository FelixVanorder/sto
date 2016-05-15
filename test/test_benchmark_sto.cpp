#include <algorithm>
#include <array>
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


template< class NumType > NumType standard_sto( std::string const & str );

template<> int8_t    standard_sto( std::string const & str ) { return std::stoi( str ); }
template<> int16_t   standard_sto( std::string const & str ) { return std::stoi( str ); }
template<> int       standard_sto( std::string const & str ) { return std::stoi( str ); }

template<>
std::enable_if<
    std::is_same< long, long long >::value == false
    , long
>::type
standard_sto( std::string const & str )
{
    return std::stol( str );
}

template<> long long standard_sto( std::string const & str ) { return std::stoll( str ); }


template<> uint8_t    standard_sto( std::string const & str ) { return std::stoul( str ); }
template<> uint16_t   standard_sto( std::string const & str ) { return std::stoul( str ); }
template<> uint32_t   standard_sto( std::string const & str ) { return std::stoul( str ); }

template<>
std::enable_if<
    std::is_same< unsigned long, unsigned long long >::value == false
    , unsigned long
>::type
standard_sto( std::string const & str )
{
    return std::stoul( str );
}

template<> unsigned long long standard_sto( std::string const & str ) { return std::stoull( str ); }




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
using parser_type = NumType (*)( std::string const & );

template< class NumType >
parser_type< NumType > get_vanorder_parser()
{
    constexpr parser_type< NumType > parser = vanorder::sto< NumType >;
    return parser;
}


template< class NumType >
constexpr char const * parser_name( parser_type<NumType> fptr )
{
    return fptr == &standard_sto< NumType >
            ? stdsto_func_name(NumType())
            : fptr == get_vanorder_parser< NumType >()
                ? vanorder::utils::sto_func_name(NumType())
                : throw std::runtime_error( "Unknown parser function: " + std::to_string( (size_t)(void*)fptr ) )
        ;
}


template< class NumType, size_t calls, typename Parser >
std::chrono::nanoseconds benchmark( Parser parser )
{
    using ClearNumType = typename std::remove_cv<NumType>::type;

    volatile ClearNumType numvalue = 0;

    const std::string strnum = std::numeric_limits<NumType>::is_signed
                          ? min_cstr( decltype(numvalue)() ) 
                          : max_cstr( decltype(numvalue)() );

    auto start = std::chrono::high_resolution_clock::now();
    for( size_t i = 0; i < calls; ++i )
    {
        numvalue = parser( strnum );
    }
    auto finish = std::chrono::high_resolution_clock::now();

    return finish - start;
 }
 

template< class NumType >
bool performance_test( NumType )
{
    enum : size_t { calls = 10*1000*1000 };

    std::array< std::chrono::nanoseconds, 9 > test_durations;
    std::chrono::nanoseconds standard_ns = {};
    std::chrono::nanoseconds vanorder_ns = {};
    {
        constexpr parser_type<NumType> parser = standard_sto< NumType >;
        auto & ns = standard_ns;
        for( auto & duration : test_durations )
        {
            duration = benchmark< NumType, calls >( parser );
        }

        std::sort( test_durations.begin(), test_durations.end() );

        ns = test_durations[ test_durations.size() / 2 ];
        auto ms = std::chrono::duration_cast< std::chrono::milliseconds >( ns );

        std::cout << parser_name<NumType>( parser )
            << " for " << calls << " calls took: " << ms.count() << "ms. "
            << ((double)(ns.count()) / calls) << "ns per call."
            << std::endl;
    }
    {
        constexpr parser_type<NumType> parser = vanorder::sto< NumType >;
        auto & ns = vanorder_ns;
        for( auto & duration : test_durations )
        {
            duration = benchmark< NumType, calls >( parser );
        }

        std::sort( test_durations.begin(), test_durations.end() );

        ns = test_durations[ test_durations.size() / 2 ];
        auto ms = std::chrono::duration_cast< std::chrono::milliseconds >( ns );

        std::cout << parser_name<NumType>( parser )
            << " for " << calls << " calls took: " << ms.count() << "ms. "
            << ((double)(ns.count()) / calls) << "ns per call."
            << std::endl;
    }

    if( standard_ns > vanorder_ns )
    {
        std::cout << std::endl;
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
