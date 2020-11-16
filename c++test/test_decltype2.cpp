#include <iostream>

int main()
{
    int i = 4;
    int arr[5] = { 0 };
    int *ptr = arr;
    struct S{ double d; }s ;
    void Overloaded(int);
    void Overloaded(char);//重载的函数
    int && RvalRef();
    const bool Func(int);
    int x = 10000;
    const int y_const_value = 2000;

    //规则一：推导为其类型
    decltype (arr) var1 = {1};  //int[] 标记符表达式; 整形数组 var1初始化为{1}
    
    int *test = &i;
    decltype (ptr) var2 = test;//int *  标记符表达式
    var2=&i;
    var2=var1;
 
    decltype(s.d) var3;//doubel 成员访问表达式
 
    //decltype(Overloaded) var4;//重载函数。编译错误。
 
    //规则二：将亡值。推导为类型的右值引用。
    decltype (RvalRef()) var5 =1000; //var5被初始化 引用，必须赋给它常量，否则编译出错


    //规则三：左值，推导为类型的引用。
    decltype ((i))var6 = x;     //int&, (i) 不同于 i， 这里是整形引用，必须赋值，可以是 i或是x 
    var6 = i;
    var6 = 3000;

    decltype (true ? i : i) var7 = i; //int&  条件表达式返回左值。
    var7 = x;
    var7 = y_const_value;

    decltype (++i) var8 = i; //int&  ++i返回i的左值。
 
    decltype(arr[5]) var9 = i;//int&    []操作返回左值
 
    decltype(*ptr)var10 = i;//int&     *操作返回左值
 
    decltype("hello")var11 = "12345"; //const char(&)[6]  字符串字面常量为左值，且为const左值。 
                                      //赋给的值 只能是宽度为6（len=5)的常量字符串
 
    //规则四：以上都不是，则推导为本类型
 
    decltype(1) var12;//const int
 
    decltype(Func(1)) var13=true;//const bool
 
    decltype(i++) var14 = i;//int   i++返回右值

    return 0;
}