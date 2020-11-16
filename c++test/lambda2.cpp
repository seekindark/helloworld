#include <iostream>

//当出现任一默认捕获符时，都能隐式捕获当前对象（*this）。
//当它被隐式捕获时，始终被以引用捕获，即使默认捕获符是 = 也是如此。
//当默认捕获符为 = 时，*this 的隐式捕获被弃用。 (C++20 起)
//=================================================

//1.  当默认捕获符是 & 时，后继的简单捕获符必须不以 & 开始。
struct S1 { void f(int i); };
void S1::f(int i)
{
    []{};           // OK, don't catpure any variables.
    [&]{};          // OK：默认以引用捕获所有外部变量
    [&, i]{};       // OK：以引用捕获，但 i 以值捕获
    //[&, &i] {};     // 错误：以引用捕获为默认时的以引用捕获
    [&, this] {};   // OK：等价于 [&]
    [&, this, i]{}; // OK：等价于 [&, i]
    [&, *this, i]{}; // OK in C++17; Error in C++14 and backward
}



//2.  当默认捕获符是 = 时，后继的简单捕获符必须以 & 开始，或者为 *this
struct S2 { void f(int i); };
void S2::f(int i)
{
    [=]{};          // OK：默认以复制捕获 all external variables
    //[=, i]{};    // redundant with by-copy =, 
    [=, &i]{};      // OK：以复制捕获，但 i 以引用捕获
    [=, *this]{};   // C++17 前：错误：无效语法
                    // C++17 起：OK：以复制捕获外围的 S1
   // [=, this] {};   // C++20 前：错误：= 为默认时的 this
                    // C++20 起：OK：同 [=]
}

// 3. 任何捕获符只可以出现一次：
struct S3 { void f(int i); };
void S3::f(int i)
{
    //[i, i] {};        // 错误：i 重复   //already captured ‘i’ in lambda expression
    //[this, *this] {}; // 错误："this" 重复 (C++17)
}

// 4. 使用简单的捕获 然后组合 （不能同时使用默认的= 或 &）
struct S4{ 
    void f(int i, int y);
    
    int s4_member;
    };
void S4::f(int i, int y)
{
    int a;
    char b;
    [i, y]{};
    [&i, y]{};
    [i, &y](){};
    [a, &b, i, &y, this](int mm, int nn)
        {return mm+nn+i+y+a+b + s4_member;};
    /*
    对非静态数据成员‘S4::s4_member’的无效使用 , 缺少捕获 this
    [a, &b, i, &y](int mm, int nn)
        {return mm+nn+i+y+a+b + s4_member;};
    */
}

// 5. 达作用域（reaching scope）定义为直至并包含其最内层的外围函数
//  （及其形参）的外围作用域的集合
//  带有初始化器的捕获符，其行为如同它声明并显示捕获一个以类型 auto 声明
// 的变量，该变量的声明区是 lambda 表达式体（即它不在其初始化器的作用域中）
// 下面的例子中， y 是lambda表达式的一部分，所以不能被捕获！！
int x = 4;
auto y = [&r = x, x = x + 1]()->int  //拖尾返回类型 int
    {
        r += 2;
        return x * x;
    }(); // 更新 ::x 为 6,  并初始化 y 为 25。


// 6. 隐式捕获 this or *this
struct S6{ 
    void f(int i, int y);
    
    int s6_member;
    };    
void S6::f(int i, int y)
{
    [=](){s6_member += 1;};
    //[](){s6_member += 1;};  // 错误
    [&](){s6_member += 1;};
    //[](){this->s6_member += 1;};// 错误
    
}

// 7. ODR体式 使用变量或this ，c++ 14 起支持
void f(int, const int (&)[2] = {}) {} // #1  ；const int (&)[2] 是const整形数组含2个元素，默认为空 
void f(const int&, const int (&)[1]) {} // #2
void test()
{
    const int x = 17;
    auto g0 = [](auto a) { f(x); }; // ok：调用 #1，不捕获 x
    auto g1 = [=](auto a) { f(x); }; // C++14 中不捕获 x，C++17 中捕获 x
                                     // 捕获能被优化掉
    auto g2 = [=](auto a) {
            int selector[sizeof(a) == 1 ? 1 : 2] = {};
            f(x, selector); // ok：此为待决表达式，故 x 被捕获
    };
    auto g3 = [=](auto a) {
      typeid(a + x);  // 捕获 x，不管 a + x 是否为不求值操作数
    };
}

// 8. 若 lambda 体 ODR 式使用了以复制捕获的实体，则它访问的是闭包类型的成员。
//    若它未 ODR 式使用该实体，则访问是到原对象的
void f(const int*);
void g()
{
    const int N = 10;
    [=]{ 
        int arr[N]; // 非 ODR 式使用：指代 g 的 const int N
        //f(&N); //Error ?? don;t know why??   ODR 式使用：导致 N 被（以复制）捕获
               // &N 是闭包对象的成员 N 的地址，而非 g 中的 N
    }();
}
// 若 lambda ODR 式使用了以引用捕获的引用，则它使用原引用所指代的对象，
// 而非被捕获的引用自身：
#include <iostream> 
auto make_function(int& x) {
  return [&]{ std::cout << x << '\n'; };
}
 void test1() {
  int i = 3;
  auto f = make_function(i); // f 中对 x 的使用直接绑定到 i, //生成一个函数
  i = 5;
  f(); // OK；打印 5
}

//9. 在 lambda 体内，在任何具有自动存储期的变量上使用的任何 decltype，
//   都如同将它捕获并 ODR 式使用，
//   尽管 decltype 自身不是 ODR 式使用且不实际发生捕获：
void f3() {
    float x, &r = x;
    [=]
    { // x 与 r 不被捕获（出现于 decltype 的操作数中并不是 ODR 式使用）
        decltype(x) y1; // y1 拥有 float 类型
        decltype((x)) y2 = y1; // y2 拥有 float const& 类型，因为此 lambda
                               // 非 mutable 且 x 是左值
        decltype(r) r1 = y1;   // r1 拥有 float& 类型（不考虑变换）
        //decltype((r)) r2 = y2; // r2 拥有 float const& 类型
                     //  Error: binding reference of type ‘float&’ to ‘const float’ discards qualifiers

    };
}

// 10. Lambda 嵌套
struct s10 {
  double ohseven = .007;
  auto f() { // 以下两个 lambda 的最接近外围函数
    return [this] { // 以引用捕获外围的 s10
      return [*this] { // 以复制捕获外围的 s10  (C++17)
          return ohseven;   // OK
       };
     } /*()*/;    // why ?? 这里加了（）还时能编译通过！！
  }
  auto g() {
     return []{ // 无捕获
         //return [*this]{};  // 错误：*this 未被外层 lambda 表达式所捕获
      }();
   }
};

//11. 捕获的范围讨论，
// ODR 式使用了 this 或任何具有自动存储期的变量，则它必须被该 lambda 表达式所捕获。 ？？？
void f11(int i)
{
    std::cout << "f11()" <<std::endl;
    int const N = 20;
    auto m1 = [=] {
            std::cout <<"f11(), N=" << N << std::endl;
            int const M = 30;
            auto m2 = [i] {
            int x[N][M]; // N 与 M 未被 ODR 式使用 
                            // （它们未被捕获是 OK 的）
            x[0][0] = i; // i 被 m2 显式捕获
                            // 并被 m1 隐式捕获
            std::cout << "sizeof(x[N][M])=" << sizeof(x) <<std::endl;
            };

            m2();  // only m2 is called, its body is running
            
    };
    
    m1();  //only m1 is called, its body is running

    struct s11 // f1() 中的局部类
    {
        int f;
        void work(int n) // 非静态成员函数
        {
            int m = n * n;
            int j = 40;
            auto m3 = [this, m] {
                auto m4 = [&/*,j*/] { // 错误：j 未被 m3 所捕获
                        //int x = n; // 错误：n 被 m4 隐式捕获 
                                    // 但未被 m3 所捕获
                        int xx=0;
                        xx += m;    // ok：m 被 m4 捕获, because m is caputred by m3
                                    // 且为 m3 显式捕获
                        //x += i;    // 错误：i 在可达作用域之外
                                    // （该作用域终于 work()）
                        xx += f;    // ok：this 被 m4 隐式捕获, because this is captured by m3 explicitly
                                    // 且被 m3 显式捕获
                };
            };
        }
    };
}

//12. 以不带有初始化器的 捕获符 不能捕获类成员
#define use(a, b) (a+b)
class S12 {
  int x = 0;
  void f() {
    int i = 0;
//  auto l1 = [i, x]{ use(i, x); };    // 错误：x 非变量
    auto l2 = [i, x=x]{ use(i, x); };  // OK，复制捕获
    i = 1; x = 1; 
    l2(); // 调用 use(0,0)
    auto l3 = [i, &x=x]{ use(i, x); }; // OK，引用捕获
    i = 2; x = 2; 
    l3(); // 调用 use(1,2)
  }
};

// 13. 当 lambda 用隐式的以 复制捕获  捕获某个成员时，它并不产生该成员变量的副本：
//     对成员变量 m 的使用被处理成表达式 (*this).m，而 *this 始终被隐式以引用捕获：
#define use(a, b) (a+b)
class S13 {
  int x = 0;
  void f() {
    int i = 0;
    auto l1 = [=]{ use(i, x); }; // 捕获 i 的副本和 this 指针的副本
    i = 1; x = 1; l1(); // 调用 use(0,1)，如同 i 以复制而 x 以引用捕获
    auto l2 = [i, this]{ use(i, x); }; // 同上，令之为显式捕获
    i = 2; x = 2; l2(); // 调用 use(1,2)，如同 i 以复制而 x 以引用捕获
    auto l3 = [&]{ use(i, x); }; // 以引用捕获 i，并捕获 this 指针的副本
    i = 3; x = 2; l3(); // 调用 use(3,2)，如同 i 与 x 均以引用捕获

    auto l4 = [i, *this]{ use(i, x); }; // 制造 *this 的副本，包含 x 的副本
    i = 4; x = 4; l4(); // 调用 use(3,2)，如同 i 与 x 均以复制捕获
  }
};


//=========================
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>
 
int main()
{
    std::vector<int> c = {1, 2, 3, 4, 5, 0, 7};
    int x = 5;
    //c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; }), c.end());
    
    std::cout << "std::vector<int> c = {1, 2, 3, 4, 5, 0, 7};" << std::endl;
    // std::remove_if(first, last, policy)  
    //从范围 [first, last) 移除所有满足特定判别标准的元素，并返回范围新结尾的尾后迭代器。
    //把满足policy的值移到左侧，返回范围新结尾的迭代器，容器的物理长度不变；
    //通常是随后调用 容器的erase（start, end），擦除剩余范围，减小容器的物理长度
    auto start_it = std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; });
    std::cout << "After std::remove_if, c: ";
    std::for_each(c.begin(), c.end(), [](int i){ std::cout << i << ' '; });
    std::cout << std::endl;

    c.erase(start_it, c.end());
    std::cout << "Then after c.erase(), c: ";
    std::for_each(c.begin(), c.end(), [](int i){ std::cout << i << ' '; });
    std::cout << '\n';
 
    // 闭包的类型不能被指名，但可用 auto 提及
    // C++14 起，lambda 能拥有自身的默认实参
    auto func1 = [](int i = 6) { return i + 4; };
    std::cout << "func1: " << func1() << '\n';
 
    // 与所有可调用对象相同，闭包能可以被捕获到 std::function 之中
    // （这可能带来不必要的开销）
    std::function<int(int)> func2 = [](int i) { return i + 4; };
    std::cout << "func2: " << func2(6) << '\n';
}

/*
int main()
{
    f11(11);
    return 0;
}
*/
