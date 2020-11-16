//能在实现移动赋值运算符和移动构造函数时使用此函数：
struct S
{
  int* p;
  int n;
 
  S(S&& other)
    :p{std::exchange(other.p, nullptr)}
    ,n{std::exchange(other.n, 0)}
  {}
 
  S& operator=(S&& other) {
    p = std::exchange(other.p, nullptr); // 移动 p ，同时留 nullptr 于 other.p 中
    n = std::exchange(other.n, 0); // 移动 n ，同时留零于 other.n 中
    return *this;
  }
};

