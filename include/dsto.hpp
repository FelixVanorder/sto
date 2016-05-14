#ifndef VANORDER_DSTO_HPP
#define VANORDER_DSTO_HPP

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "sto_utils.hpp"

namespace vanorder
{

    template< class NumType >
    typename std::enable_if<
                            std::numeric_limits< NumType >::is_integer
                            and std::numeric_limits< NumType >::is_signed
                            , NumType
             >::type
    sto( const std::string & str )
    {
        static_assert( std::numeric_limits< NumType >::is_specialized,
                       "Can work only with specialized numbers." );

        static_assert( std::numeric_limits< NumType >::min()
                       == -std::numeric_limits< NumType >::max() - 1,
                       "We relay on ususal integer representation where min = -max - 1"
                       // int8_t -> [-128; 127]
                       // int16_t -> [-32768; 32767]
                       // And so on.
                       );


        typename std::remove_cv<NumType>::type result = 0;

        if( str.empty() )
        {
            throw std::length_error( utils::sto_func_name(result) );
        }

        bool const is_neg = (str.front() == '-');

        auto iter = str.begin() + ( is_neg ? 1 : 0 );

        if( iter == str.end() )
        {
            throw std::invalid_argument( utils::sto_func_name(result) );
        }

        for( ; iter != str.end() ; ++iter )
        {
            signed c = *iter;
            if( std::isdigit( c ) )
            {
                c -= '0';
            }
            else
            {
                throw std::invalid_argument( utils::sto_func_name(result) );
            }

            constexpr NumType cutoff =    std::numeric_limits< NumType >::min() / 10;
            constexpr NumType cutlim = -( std::numeric_limits< NumType >::min() % 10 );

            if( result < cutoff or ( result == cutoff and c > cutlim ) )
            {
                if( is_neg )
                {
                    throw std::underflow_error( utils::sto_func_name(result) );
                }
                else
                {
                    throw std::overflow_error( utils::sto_func_name(result) );
                }
            }
            result *= 10;
            result -= c;
        }

        if( not is_neg )
        {
            if( result == std::numeric_limits<NumType>::min() )
            {
                throw std::overflow_error( utils::sto_func_name(result) );
            }
            result = -result;
        }

        return result;
    }



    template< class NumType >
    typename std::enable_if<
                            std::numeric_limits< NumType >::is_integer
                            and std::numeric_limits< NumType >::is_signed == false
                            , NumType
             >::type
    sto( const std::string & str )
    {
        static_assert( std::numeric_limits< NumType >::is_specialized,
                       "Can work only with specialized numbers." );

        typename std::remove_cv<NumType>::type result = 0;

        if( str.empty() )
        {
            throw std::length_error( utils::sto_func_name(result) );
        }
        else if( str.front() == '-' )
        {
            throw std::domain_error( utils::sto_func_name(result) );
        }

        for( auto c : str )
        {
            if( std::isdigit( c ) )
            {
                c -= '0';
            }
            else
            {
                throw std::invalid_argument( utils::sto_func_name(result) );
            }

            constexpr NumType cutoff = std::numeric_limits< NumType >::max() / 10;
            constexpr NumType cutlim = std::numeric_limits< NumType >::max() % 10;

            if( result > cutoff or ( result == cutoff and c > cutlim ) )
            {
                throw std::overflow_error( utils::sto_func_name(result) );
            }
            result *= 10;
            result += c;
        }

        return result;
    }

    template< class NumType >
    typename std::enable_if<
                            std::is_same<
                                         float
                                         , typename std::remove_cv< NumType >::type
                            >::value
                            , NumType
             >::type
    sto( const std::string & str )
    {
        return std::stof( str );
    }

    template< class NumType >
    typename std::enable_if<
                            std::is_same<
                                         double
                                         , typename std::remove_cv< NumType >::type
                            >::value
                            , NumType
             >::type
    sto( const std::string & str )
    {
        return std::stod( str );
    }


    template< class NumType >
    typename std::enable_if<
                            std::is_same<
                                         long double
                                         , typename std::remove_cv< NumType >::type
                            >::value
                            , NumType
             >::type
    sto( const std::string & str )
    {
        return std::stold( str );
    }
    
} // namespace vanorder

#endif // VANORDER_DSTO_HPP
