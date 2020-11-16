#include <iostream>
 
struct Foo {
    int   i;
    float f;
    char  c;
};
 
struct Empty {};
struct Empty2 {
    virtual ~Empty2(){} //虚函数指针，8
};
struct alignas(16)Empty3 {
    virtual ~Empty3(){}
};
struct alignas(64) Empty64 {};
 
int main()
{
    std::cout << "Alignment of"  "\n"
        "- char             : " << alignof(char)    << "\n"
        "- pointer          : " << alignof(int*)    << "\n"
        "- class Foo        : " << alignof(Foo)     << "\n"
        "- empty class      : " << alignof(Empty)   << "\n"
        "- empty2 class      : " << alignof(Empty2)   << "\n"
         "- empty3 class      : " << alignof(Empty3)   << "\n"
        "- alignas(64) Empty: " << alignof(Empty64) << "\n";
}