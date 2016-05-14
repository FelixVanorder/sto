#include <iostream>
#include "sto.hpp"



namespace
{
    template< class NumType > inline constexpr char const * num_type_name( NumType const & );
    
    inline constexpr char const * num_type_name(  int8_t  const & ) { return  "int8_t"; }
    inline constexpr char const * num_type_name( uint8_t  const & ) { return "uint8_t"; }
    inline constexpr char const * num_type_name(  int16_t const & ) { return  "int16_t"; }
    inline constexpr char const * num_type_name( uint16_t const & ) { return "uint16_t"; }
    inline constexpr char const * num_type_name(  int32_t const & ) { return  "int32_t"; }
    inline constexpr char const * num_type_name( uint32_t const & ) { return "uint32_t"; }
    inline constexpr char const * num_type_name(  int64_t const & ) { return  "int64_t"; }
    inline constexpr char const * num_type_name( uint64_t const & ) { return "uint64_t"; }
}


template< class NumType >
void empty_string_test()
{
    bool catched = false;

    try { vanorder::sto< NumType >( "" ); }
    catch( std::length_error ) { catched = true; }

    if( not catched ) { throw std::runtime_error( "Empty string passed for " + std::string( num_type_name(NumType()) ) + "." ); }
}

template< class NumType >
void invalid_argument_test( const std::string & invalid_argument )
{
    bool catched = false;

    try { vanorder::sto< NumType >( invalid_argument ); }
    catch( std::invalid_argument ) { catched = true; }

    if( not catched )
    {
        throw std::runtime_error( "Invalid string passed for " + std::string(num_type_name(NumType())) + ": \"" + std::string(invalid_argument) + "\"" );
    }
}

template< class NumType >
typename std::enable_if<
                        std::numeric_limits< NumType >::is_specialized
                        and std::numeric_limits< NumType >::is_signed == false
                        , void
         >::type
domain_error_test( const std::string & domain_error )
{
    bool catched = false;

    try { vanorder::sto< NumType >( domain_error ); }
    catch( std::domain_error ) { catched = true; }

    if( not catched )
    {
        throw std::runtime_error( "Domain error string passed for " + std::string(num_type_name(NumType())) + ": \"" + std::string(domain_error) + "\"" );
    }

}



void basic_sanity_tests()
{
    {
        empty_string_test<  int8_t  >();
        empty_string_test< uint8_t  >();
        empty_string_test<  int16_t >();
        empty_string_test< uint16_t >();
        empty_string_test<  int32_t >();
        empty_string_test< uint32_t >();
        empty_string_test<  int64_t >();
        empty_string_test< uint64_t >();
    }

    {
        constexpr char const * invalid_arguments[] =
        {
            // invalid symbols
            " 12", "1 2", "12 ",
            "+1", "1+",
            "+12", "1+2", "12+"
            "+123", "1+23", "12+3", "123+",

            // no numbers at all
            "text", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
        };

        for( auto invalid_argument : invalid_arguments )
        {
            invalid_argument_test<  int8_t  >( invalid_argument );
            invalid_argument_test< uint8_t  >( invalid_argument );
            invalid_argument_test<  int16_t >( invalid_argument );
            invalid_argument_test< uint16_t >( invalid_argument );
            invalid_argument_test<  int32_t >( invalid_argument );
            invalid_argument_test< uint32_t >( invalid_argument );
            invalid_argument_test<  int64_t >( invalid_argument );
            invalid_argument_test< uint64_t >( invalid_argument );
        }

        constexpr char const * signed_invalid_arguments[] =
        {
            "-",
            "-5a", "- 5", "- 5a",
        };

        for( auto signed_invalid_argument : signed_invalid_arguments )
        {
            invalid_argument_test<  int8_t  >( signed_invalid_argument );
            invalid_argument_test<  int16_t >( signed_invalid_argument );
            invalid_argument_test<  int32_t >( signed_invalid_argument );
            invalid_argument_test<  int64_t >( signed_invalid_argument );
        }
    }

    {
        constexpr char const * domain_errors[] =
        {
            "-1", "-15", "-0",
        };

        for( auto domain_error : domain_errors )
        {
            domain_error_test< uint8_t  >( domain_error );
            domain_error_test< uint16_t >( domain_error );
            domain_error_test< uint32_t >( domain_error );
            domain_error_test< uint64_t >( domain_error );
        }
    }

}

template< class NumType >
typename std::enable_if<
                        std::is_same< intmax_t, NumType >::value
                        or std::is_same< uintmax_t, NumType >::value
                        , void
         >::type
extended_test()
{

    std::cout << "Sorry, can't test " << num_type_name(NumType()) << " right now." << std::endl;
}


template< class NumType >
typename std::enable_if<
                        std::numeric_limits< NumType >::is_specialized
                        and std::numeric_limits< NumType >::is_signed
                        and std::is_same< intmax_t, NumType >::value == false
                        and std::is_same< uintmax_t, NumType >::value == false
                        , void
         >::type
extended_test()
{
    std::cout << "Parse all values of " << num_type_name(NumType()) << "." << std::endl;
    std::string str_value;
    for( intmax_t value = std::numeric_limits< NumType >::min()
         ; value < (intmax_t)std::numeric_limits< NumType >::max() + 1
         ; ++value
         )
    {
        str_value = std::to_string( value );
        auto const parsed = vanorder::sto< NumType >( str_value );
        if( parsed != value )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Invalid parsing of " + str_value + ", parsed: " + std::to_string( parsed )) ;
        }
    }

    std::cout << "Test underflow values of " << num_type_name(NumType()) << "." << std::endl;
    for( intmax_t value = 30*(intmax_t)std::numeric_limits< NumType >::min()
         ; value < std::numeric_limits< NumType >::min()
         ; ++value )
    {
        bool catched = false;
        str_value = std::to_string( value );
        NumType must_be_not_parsed = 0;

        try
        {
            must_be_not_parsed = vanorder::sto< NumType >( str_value );
        }
        catch( std::underflow_error )
        {
            catched = true;
        }

        if( not catched )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Underflow value parsed: "
                                      "\"" + str_value + "\", parsed: " + std::to_string( must_be_not_parsed ) );
        }
    }

    std::cout << "Test overflow values of " << num_type_name(NumType()) << "." << std::endl;
    for( intmax_t value = 1 + (intmax_t)std::numeric_limits< NumType >::max()
         ; value < 30*(intmax_t)std::numeric_limits< NumType >::max()
         ; ++value )
    {
        bool catched = false;
        str_value = std::to_string( value );
        NumType must_be_not_parsed = 0;

        try
        {
            must_be_not_parsed = vanorder::sto< NumType >( str_value );
        }
        catch( std::overflow_error )
        {
            catched = true;
        }

        if( not catched )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Overflow value parsed: "
                                      "\"" + str_value + "\", parsed: " + std::to_string( must_be_not_parsed ) );
        }
    }

    std::cout << "Done with " << num_type_name(NumType()) << "." << std::endl;
}



template< class NumType >
typename std::enable_if<
                        std::numeric_limits< NumType >::is_specialized
                        and std::numeric_limits< NumType >::is_signed == false
                        and std::is_same< intmax_t, NumType >::value == false
                        and std::is_same< uintmax_t, NumType >::value == false
                        , void
         >::type
extended_test()
{
    std::string str_value;

    std::cout << "Parse all values of " << num_type_name(NumType()) << "." << std::endl;
    for( intmax_t value = 0; value < 1 + (intmax_t)std::numeric_limits< NumType >::max(); ++value )
    {
        str_value = std::to_string( value );
        auto const parsed = vanorder::sto< NumType >( str_value );
        if( parsed != value )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Invalid parsing of " + str_value + ", parsed: " + std::to_string( parsed )) ;
        }
    }

    std::cout << "Test domain error values of " << num_type_name(NumType()) << "." << std::endl;
    for( intmax_t value = -(intmax_t)std::numeric_limits< NumType >::max()*30; value < 0; ++value )
    {
        bool catched = false;
        str_value = std::to_string( value );
        NumType must_be_not_parsed = 0;

        try
        {
            must_be_not_parsed = vanorder::sto< NumType >( str_value );
        }
        catch( std::domain_error )
        {
            catched = true;
        }

        if( not catched )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Underflow value parsed: "
                                      "\"" + str_value + "\", parsed: " + std::to_string( must_be_not_parsed ) );
        }
    }

    std::cout << "Test overflow values of " << num_type_name(NumType()) << "." << std::endl;
    for( intmax_t value = 1 + (intmax_t)std::numeric_limits< NumType >::max()
         ; value < (intmax_t)std::numeric_limits< NumType >::max()*30
         ; ++value )
    {
        bool catched = false;
        str_value = std::to_string( value );
        NumType must_be_not_parsed = 0;

        try
        {
            must_be_not_parsed = vanorder::sto< NumType >( str_value );
        }
        catch( std::overflow_error )
        {
            catched = true;
        }

        if( not catched )
        {
            throw std::runtime_error( "vanorder::sto<" + std::string( num_type_name(NumType()) ) + "> : Overflow value parsed: "
                                      "\"" + str_value + "\", parsed: " + std::to_string( must_be_not_parsed ) );
        }
    }

    std::cout << "Done with " << num_type_name(NumType()) << "." << std::endl;
}


template< class NumType >
void extended_test_wrapper()
{
    std::cout << "\n\nStart extended test for " << num_type_name(NumType()) << "." << std::endl;
    try { extended_test< NumType >(); }
    catch( std::length_error const & ex )       { std::cout << "Unexpected length_error: " << ex.what() << std::endl; }
    catch( std::invalid_argument const & ex )   { std::cout << "Unexpected invalid_argument: " << ex.what() << std::endl; }
    catch( std::domain_error const & ex )       { std::cout << "Unexpected domain_error: " << ex.what() << std::endl; }
    catch( std::underflow_error const & ex )    { std::cout << "Unexpected underflow_error: " << ex.what() << std::endl; }
    catch( std::overflow_error const & ex )     { std::cout << "Unexpected overflow_error: " << ex.what() << std::endl; }
    catch( std::out_of_range const & ex )       { std::cout << "Unexpected out_of_range: " << ex.what() << std::endl; }
    catch( std::exception const & ex )          { std::cout << "Test failure: " << ex.what() << std::endl; }
}

int main( int argc, char *argv[] )
{


    std::cout << "Start basic sanity test." << std::endl;
    try { basic_sanity_tests(); }
    catch( std::length_error const & ex )       { std::cout << "Unexpected length_error: " << ex.what() << std::endl; }
    catch( std::invalid_argument const & ex )   { std::cout << "Unexpected invalid_argument: " << ex.what() << std::endl; }
    catch( std::domain_error const & ex )       { std::cout << "Unexpected domain_error: " << ex.what() << std::endl; }
    catch( std::underflow_error const & ex )    { std::cout << "Unexpected underflow_error: " << ex.what() << std::endl; }
    catch( std::overflow_error const & ex )     { std::cout << "Unexpected overflow_error: " << ex.what() << std::endl; }
    catch( std::out_of_range const & ex )       { std::cout << "Unexpected out_of_range: " << ex.what() << std::endl; }
    catch( std::exception const & ex )          { std::cout << "Test failure: " << ex.what() << std::endl; }



    extended_test_wrapper<  int8_t  >();
    extended_test_wrapper< uint8_t  >();
    extended_test_wrapper<  int16_t >();
    extended_test_wrapper< uint16_t >();
    extended_test_wrapper<  int32_t >();
    extended_test_wrapper< uint32_t >();
    extended_test_wrapper<  int64_t >();
    extended_test_wrapper< uint64_t >();


    return 0;
}
