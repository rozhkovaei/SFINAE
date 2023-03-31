#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>

/**
 * Template function to print ip address.
 * @brief Outputs each element of the container as is separated with dots.
 * @param container to print.
*/
template <
    template < typename, typename > typename Container,
    typename Type,
    typename Allocator = std::allocator< Type >,
    typename = std::enable_if_t<
        std::is_same_v< Container< Type, Allocator >, std::vector< Type, Allocator > > ||
        std::is_same_v<Container< Type, Allocator >, std::list< Type, Allocator > > > >
void print_ip( Container< Type, Allocator > &&container )
{
    for( auto iter = std::begin( container ); iter != std::end( container ); ++iter )
    {
        if ( iter != std::begin( container ) )
            std::cout << ".";
        std::cout << *iter;
    }
    std::cout << std::endl;
}

template< typename T >
using IsString = std::enable_if_t< std::is_same_v< T, std::string >, bool >;

template< typename T >
using IsInteger = std::enable_if_t< std::is_integral_v< T > && !std::is_same_v< T, bool >, bool >;

/**
 * Template function to print ip address.
 * @brief Outputs string as is.
 * @param value to print.
*/
template < typename T, IsString< T > = true >
void print_ip( T&& value )
{
    std::cout << value << std::endl;
}

/**
 * Template function to print ip address.
 * @brief Outputs each byte of the value separated with dots.
 * @param value to print.
*/
template < typename T, IsInteger< T > = true >
void print_ip( T&& value )
{
    unsigned char* bytes = ( unsigned char * )( &value );
    for ( int i = sizeof( value ) - 1; i >= 0 ; i-- )
    {
        if ( i != sizeof( value ) - 1 )
        {
            std::cout << ".";
        }
        std::cout << ( int )bytes[ i ];
    }
    std::cout << std::endl;
}

template < typename Tuple, size_t i = 0 >
static constexpr bool check_visitor()
{
    if constexpr ( std::tuple_size_v< Tuple > == 1 )
        return true;
    static_assert( std::is_same_v< std::tuple_element_t< i, Tuple >,
                                   std::tuple_element_t< i + 1, Tuple > > );
    if constexpr ( i + 1 == std::tuple_size_v< Tuple > - 1 )
        return true;
    else
        return check_visitor< Tuple, i + 1 >();
    return true;
}

template< class Type, size_t... Is >
void print_tuple( const Type& tuple, std::index_sequence< Is... >)
{
    ( ..., ( std::cout << ( Is != 0? "." : "" ) << std::get< Is >( tuple ) ) );
    std::cout << std::endl;
}

/**
 * Template function to print ip address.
 * @brief Outputs each tuple element as is separated with dots.
 * @param tuple_value to print.
*/
template <
    template < typename ... > typename Tuple,
    typename ...Type,
    typename = std::enable_if_t< std::is_same_v< Tuple< Type... >, std::tuple< Type... > > > >
void print_ip( Tuple< Type... >&& tuple_value ) {
    check_visitor< Tuple< Type... > >();
    static constexpr size_t tuple_size = std::tuple_size_v< Tuple< Type... > >;
    static_assert( tuple_size > 0 );
    print_tuple( tuple_value, std::make_index_sequence< tuple_size >{} );
}

int main( [[maybe_unused]] int argc, [[maybe_unused]] char const* argv[] )
{
    print_ip( int8_t{ -1 } );
    print_ip( int16_t{ 0 } );
    print_ip( int32_t{ 2130706433 } );
    print_ip( int64_t{ 8875824491850138409 } );
    print_ip( std::string{ "Hello, World!" } );
    print_ip( std::vector< int >{ 100, 200, 300, 400 } );
    print_ip( std::list< short >{ 400, 300, 200, 100 } );
    print_ip( std::make_tuple( 123, 456, 789, 0 ) );
	return 0;
}

