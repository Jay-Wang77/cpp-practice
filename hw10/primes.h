#pragma once

#include <cmath>

// TODO:
// Exercise 2:
// Implement greated common divisor using constexpr, such that it can be evaluated at compile time.
// Your implementation should use variadic templates, to enable calling it
// with any number of arguments, ensure that at least two arguments are provided.
template <typename T>
constexpr T gcd(T a, T b) {
    if (b == 0) {
        return a;
    }
    return gcd(b, a % b);
}
template <typename T, typename... Args>
constexpr T gcd(T a, T b, const Args&... args){
    return gcd(gcd(a, b), args...);
};
//
// Exercise 3:
// Implement minimum common multiple using constexpr, such that it can be evaluated at compile time.
// As with GCD, your implementation should use variadic templates, to enable calling it
// with arbitrary number of arguments, but at least two
//
template <typename T, typename... Args>
constexpr T mcm(T a, T b) {
    return a / gcd(a, b) * b;
}
template <typename T, typename... Args>
constexpr T mcm(T a, T b, const Args&... args){
    return mcm(mcm(a, b), args...);
};
// Exercise 4:
// Implement power using template metaprogramming. The template struct must be
// called `Power`, and be callable with `Power<base, exponent, modulus>::value`.
// I.e. it needs a compile time evaluation `value`, which holds the result of
// the computation.
// Think about your base cases

template<int base, int exponent, int modulus>
struct Power {
    static const int value = exponent == 0 ? 1 : static_cast<int>((static_cast<long long>(base) * Power<base, exponent - 1, modulus>::value) % modulus);
};

template<int base, int modulus>
struct Power<base, 0, modulus> {
    static const int value = modulus == 1 ? 0 : 1;
};