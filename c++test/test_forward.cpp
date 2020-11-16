#include<iostream>
using namespace std;
template<typename T>
void print(T& t)
{
    cout << "lvalue" << endl;
}
 
template<typename T>
void print(T&& t)
{
    cout << "rvalue" << endl;
}
 
template<typename T>
void TestForward(T && v)
{
    //print(v); //编译错误  不知道是哪个print
    print(std::forward<T>(v));
    print(std::move(v));
}
 
int main()
{
    TestForward(1);
    int x = 1;
    //TestForward(x); //使print(std::forward<T>(v));编译错误
    TestForward(std::forward<int>(x));

    print(x);
    cout << "afer std::move"<<std::endl;
    print(std::move(x));
    cout << "afer std::forward (std::move)"<<std::endl;
    print(std::forward<int>(std::move(x)));
    return 0;
}
