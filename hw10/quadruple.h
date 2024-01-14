#pragma once

#include <array>

// TODO: Implement the Quadruple struct, with the required specialisations
// - The general case should store types and have getters for each called `get_first`, `get_second`,
// etc
// - Implement once specialization if all types are ints. Store them in a publicly accessible
// `std::array`, again provide the getters.
// - Implement once specialization if all types are the same. Store them in a publicly accessible
// `std::array`, again provide the getters.
template <typename T, typename U, typename V, typename W> struct Quadruple {
public:
    Quadruple(T f, U s, V t, W fo): first(f), second(s), third(t), fourth(fo){};

    T get_first(){return first;}
    U get_second(){return second;}
    V get_third(){return third;}
    W get_fourth(){return fourth;}

private:
    T first;
    U second;
    V third;
    W fourth;
};
template <>
struct Quadruple<int, int, int, int>{
public:
    Quadruple(int f, int s, int t, int fo): members({f, s, t, fo}){};

    std::array<int, 4> members;

    int get_first(){return members[0];}
    int get_second(){return members[1];}
    int get_third(){return members[2];}
    int get_fourth(){return members[3];}

};
template <typename T>
struct Quadruple<T, T, T, T>{
public:
    Quadruple(T f, T s, T t, T fo): members({f, s, t, fo}){};

    std::array<T, 4> members;

    T get_first(){return members[0];}
    T get_second(){return members[1];}
    T get_third(){return members[2];}
    T get_fourth(){return members[3];}
};
