#include <iostream>
using namespace std;
class Test
{
public:
    Test(int x=0){ m_value = x;}
    Test &operator ++();    //前增量
    Test operator ++(int);//后增量
    operator int();
    print(){cout << "m_value="<< m_value << endl;}
private:
    int m_value;
};

Test &Test::operator ++()
{
    m_value ++;     //先增量
    return *this;   //返回当前对象
}

Test Test::operator ++(int)
{
    Test tmp(*this);    //创建临时对象
    m_value ++;         //再增量
    return tmp;        //返回临时对象
}

//没有返回类型，因为类型名就代表了它的返回类型
Test::operator int()
{
    return m_value;
}


int main()
{
    Test a1(1);
    Test a2(2);
    Test a3;
    a3 = a1+a2;

    a3.print();
    a3++.print();
    a3.print();
    (++a3).print();
    a3.print();
}