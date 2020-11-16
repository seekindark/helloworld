#include <iostream>

using namespace std;
struct alignas(64) Tfa
{
    int data_a;
};

struct Tf
{
    int data;
};

alignas(128) char cacheline[127];


int main()
{
    char str1[1];
    char str2;
    char str3[5];
    //alignas (16) char str4[23];
    alignas (2)char str4[11];
   
    struct Tf tf; 
    struct Tfa tfa;
  
    cout << "&tf=" << &tf << endl;
    cout << "&tfa=" <<&tfa << endl;
    cout << "&cacheline=" << &cacheline << endl;

    cout << "&str1= " <<&str1 << endl;
    cout << "&str2= " <<(void *)&str2 << endl;
    cout << "&str3= " <<&str3 << endl;
    cout << "&str4= " <<&str4 << endl;
    return 0;
}