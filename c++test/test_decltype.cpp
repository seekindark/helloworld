#include <vector>
#include <iostream>
using namespace std;

struct A { double x; };
const A* a;
 
decltype(a->x) y;       // y 的类型是 double（其声明类型）
decltype((a->x)) z = y; // z 的类型是 const double&（左值表达式）
 
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) // 返回类型依赖于模板形参  
{                                     // C++14 开始可以推导返回类型
    return t+u;                       // auto 的类型由后面的 “ -> decltype(t + u)” 推导
}
 
int main() 
{
    int i = 33;
    decltype(i) j = i * 2;
 
    std::cout << "i = " << i << ", "
              << "j = " << j << '\n';
 
    auto f = [](int a, int b) -> int
    {
        return a * b;
    };
 
    decltype(f) g = f; // lambda 的类型是独有且无名的
    i = f(2, 2);
    j = g(3, 3);
 
    std::cout << "i = " << i << ", "
              << "j = " << j << '\n';
}

//===========================
