#include <type_traits>
#include <iostream>
#include <vector>
#include <list>
#include <tuple>

/// <summary>
/// Prints the IP address representation of an integral value.
/// </summary>
/// <typeparam name="T">The type of the input value. Must be an integral type</typeparam>
/// <param name="value">The integral value to be printed as an IP address.</param>
///  
/// Example usage:
/// \code{.cpp}
/// print_ip(int8_t{-1}); // 255
/// print_ip(int16_t{0}); // 0.0
/// print_ip(int32_t{2130706433}); // 127.0.0.1
/// print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
/// \endcode
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_ip(T value)
{
    size_t size_T = sizeof(T);
    
    using uT = std::make_unsigned_t<T>;

    uT u_value = static_cast<uT>(value);

    if (size_T == 1)
    {
        std::cout << static_cast<std::uint16_t>(u_value) << std::endl;
    }
    else
    {  
        for (size_t i = size_T-1; i > 0; i--)
        {
            uint8_t h = static_cast<uint8_t>(u_value >> 0 >> 8 * i);        
            std::cout << static_cast<std::uint16_t>(h) << ".";
        }
       
        uint8_t l = static_cast<uint8_t>(u_value);
        std::cout << static_cast<std::uint16_t>(l) << std::endl;
    }
    
}

/// <summary>
/// Prints the IP address representation of an string value.
/// </summary>
/// <typeparam name="T">The type of the input value. Must be an string type</typeparam>
/// <param name="value">The string value to be printed as an IP address.</param>
template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value, void>::type
print_ip(T value) 
{
    std::cout << value << std::endl;
}

/// <summary>
/// Prints the IP address representation of elements in a container.
/// </summary>
/// <typeparam name="T">The type of the container. Must be either std::vector or std::list.</typeparam>
/// <param name="container">The container whose elements are to be printed as an IP address.</param>
template <typename T>
typename std::enable_if<
    std::disjunction_v<
        std::is_same<T, std::vector<typename T::value_type>>,
        std::is_same<T, std::list<typename T::value_type>>
    >, void>::type
print_ip(const T& container)
{
    auto back_it = container.end()--;    

    for (auto it = container.begin(); it != back_it; it++)
    {
        std::cout << *it << ".";
    }    
    std::cout << container.back() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Trait to determine if a type is a tuple.
/// </summary>
/// <typeparam name="Tuple">The type to check</typeparam>
template <typename Tuple>
struct is_tuple : std::false_type {};

/// <summary>
/// Specialization of is_tuple for std::tuple types.
/// </summary>
/// <typeparam name="...Types">The types contained in the tuple</typeparam>
template <typename... Types>
struct is_tuple<std::tuple<Types...>> : std::true_type {};

/// <summary>
/// Trait to determine if all elements in a tuple are of the same type.
/// </summary>
/// <typeparam name="...Types">Types The types contained in the tuple</typeparam>
template <typename... Types>
struct is_same_type;


/// <summary>
/// Specialization of is_same_type for a tuple with a single type. 
/// This specialization sets the value to `true_type` for a tuple with a single type.
/// </summary>
/// <typeparam name="Type">Type The single type contained in the tuple.</typeparam>
template <typename Type>
struct is_same_type<std::tuple<Type>> : std::true_type {};

/// <summary>
/// Recursive specialization of is_same_type to check if all types in a tuple are the same.
/// This specialization recursively checks each type in the tuple to ensure they are all the same.
/// </summary>
/// <typeparam name="Type">The current type to compare.</typeparam>
/// <typeparam name="...Types">The remaining types to compare.</typeparam>
template <typename Type, typename... Types>
struct is_same_type<std::tuple<Type, Type, Types...>> : is_same_type<std::tuple<Type, Types...>> {};

/// <summary>
/// Specialization of is_same_type for tuples with different types.
/// This specialization sets the value to `false_type` if any two types in the tuple are different.
/// </summary>
/// <typeparam name="Type">The first type to compare.</typeparam>
/// <typeparam name="EnotherType">The second type to compare.</typeparam>
/// <typeparam name="...Types">The remaining types to compare.</typeparam>
template <typename Type, typename EnotherType, typename... Types>
struct is_same_type<std::tuple<Type, EnotherType, Types...>> : std::false_type {};

/// <summary>
/// Prints the elements of a tuple as an IP address.
/// This helper function prints each element of a tuple in dot-separated format.
/// </summary>
/// <typeparam name="Tuple">The type of the tuple.</typeparam>
/// <typeparam name="...index">The indices of the tuple elements.</typeparam>
/// <param name="t">The tuple to be printed.</param>
template <typename Tuple, std::size_t... index>
void print_tuple(const Tuple& t, std::index_sequence<index...>)
{
    //print index0.index1.index2.index3 
    ((std::cout << (index == 0 ? "" : ".") << std::get<index>(t)), ...);
    std::cout << std::endl;
}

/// <summary>
/// Prints a tuple as an IP address if it contains elements of the same type.
/// This function ensures that the tuple is printed only if all its elements are of the same type.
/// This function uses `std::enable_if` to ensure it is only instantiated with tuple types. It also
/// uses a static assertion to ensure all elements in the tuple are of the same type.
/// </summary>
/// <typeparam name="Tuple">The type of the tuple.</typeparam>
/// <param name="value">The tuple to be printed.</param>
template <typename Tuple>
typename std::enable_if<is_tuple<Tuple>::value, void>::type
print_ip(const Tuple& value)
{   
    static_assert(is_same_type<Tuple>::value, "Tuple has different types");
    print_tuple(value, std::make_index_sequence<std::tuple_size<Tuple>::value>{});
}

////////////////////////////////////////////////////////////////////////////////

int main() 
{
    print_ip(int8_t{-1}); // 255
    print_ip(int16_t{0}); // 0.0
    print_ip(int32_t{2130706433}); // 127.0.0.1
    print_ip(int64_t{8875824491850138409});// 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"}); // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100  
    print_ip(std::make_tuple(123, 456, 789, 0)); // 123.456.789.0

    return 0;
}
