#include <iostream>

int main()
{
// 1） [] 不捕获任何变量值
//
// 2） [=]捕获外部作用域中所有变量，并作为副本在函数体中使用(按值捕获)
int a = 0;
auto f = [=] { return a; };
a+=1;
cout << f() << endl;       //输出 0

//3） [&]捕获外部作用域中所有变量，并作为引用在函数体中使用（按引用捕获）
int a = 0;
auto f = [&a] { return a; };
a+=1;
cout << f() <<endl;       //输出 1



}