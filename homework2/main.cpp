#include <iostream>
#include <vector>
#include <list>

// Typelist

struct NullType {};

template <typename ...Args>
struct TypeList {
    using Head = NullType;
    using Tail = NullType;
};

typedef TypeList<> EmptyList;

template <typename H, typename ...T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
};

// Fibonacci sequence

template <typename T, class Derive>
struct Unit : public Derive {
    T* value;
};

template <typename TList, template <typename Base, typename Derive> class Unit>
struct LinHierarchy;

template <  typename Head,
            typename ...Tail,
            template <typename Base, typename Derive> class Unit>
struct LinHierarchy<TypeList<Head, Tail...>, Unit> : public Unit<Head, LinHierarchy<TypeList<Tail...>, Unit>> {};

template <template <typename Base, typename Derive> class Unit>
struct LinHierarchy<EmptyList, Unit> : public Unit<NullType, NullType> {};

template <size_t n>
struct FibonacciNumber {
    static const size_t value = FibonacciNumber<n-1>::value + FibonacciNumber<n-2>::value;
};

template <>
struct FibonacciNumber<0> {
    static const size_t value = 1;
};

template <>
struct FibonacciNumber<1> {
    static const size_t value = 1;
};

template <typename TList,
            typename AccTList,
            size_t i,
            size_t acc_size,
            size_t capacity,
            template <typename Base, typename Derive> class Unit>
struct FibonacciHierarchy;

template <typename Head,
            typename ...Tail,
            typename ...AccTail,
            size_t i,
            size_t acc_size,
            size_t capacity,
            template <typename Base, typename Derive> class Unit>
struct FibonacciHierarchy
<TypeList<Head, Tail...>,
TypeList<AccTail...>,
i, acc_size, capacity, Unit> : public FibonacciHierarchy<TypeList<Tail...>,
                                                            TypeList<Head, AccTail...>,
                                                            i, acc_size+1, capacity, Unit>
{};

template <typename Head,
            typename ...Tail,
            typename ...AccTail,
            size_t i,
            size_t capacity,
            template <typename Base, typename Derive> class Unit>
struct FibonacciHierarchy
<TypeList<Head, Tail...>,
TypeList<AccTail...>,
i, capacity, capacity, Unit> : public LinHierarchy<TypeList<AccTail...>, Unit>,
                               public FibonacciHierarchy<TypeList<Head, Tail...>,
                                                            EmptyList,
                                                            i+1, 0, FibonacciNumber<i+1>::value+1, Unit>
{};

template <typename ...AccTail,
            size_t i,
            size_t acc_size,
            size_t capacity,
            template <typename Base, typename Derive> class Unit>
struct FibonacciHierarchy<EmptyList,
                            TypeList<AccTail...>,
                            i, acc_size, capacity, Unit> : public LinHierarchy<TypeList<AccTail...>, Unit>
{};

template <typename ...AccTail,
            size_t i,
            size_t capacity,
            template <typename Base, typename Derive> class Unit>
struct FibonacciHierarchy<EmptyList,
                            TypeList<AccTail...>,
                            i, capacity, capacity, Unit> : public LinHierarchy<TypeList<AccTail...>, Unit>
{};

template <typename TList, template <typename Base, typename Derive> class Unit>
struct GenFibonacciHierarchy : public FibonacciHierarchy<TList,
                                                            EmptyList,
                                                            0, 0,
                                                            FibonacciNumber<0>::value+1, Unit>
{};

int main() {
    using Hierarchy = GenFibonacciHierarchy<
                        TypeList<int, size_t, double, char,
                                    std::vector<int>,
                                    std::vector<size_t>,
                                    std::vector<double>,
                                    std::vector<char>,
                                    std::list<int>,
                                    std::list<size_t>,
                                    std::list<double>,
                                    std::list<char>>, Unit>;
    
    std::cout << FibonacciNumber<10>::value << std::endl;
    
    return 0;
}
