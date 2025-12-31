#include <iostream>

template <unsigned int N>
struct Factorial
{
    static constexpr int value = N * Factorial<N-1>::value;
};

template <>
struct Factorial<0>
{
    static constexpr int value = 1;
};

int main()
{
    std::cout << "Factorial<4>::value = " << Factorial<4>::value << "\n"; // compile time recursive calculation with template metaprogramming (TMP)
    return 0;
};