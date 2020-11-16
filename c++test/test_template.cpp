//shape.h
class Shape {
    //int type;
};
class Circle : public Shape {
    //int radius;
};

//statck.h
template <class T> class Stack {
    public:
        Stack();
        ~Stack();
        void push(T t);
        T pop();
        bool isEmpty();
        template<class T2>  operator Stack<T2>(); //模板类里有模板函数！！
    private:
        T *m_pT;        
        int m_maxSize;
        int m_size;
};

template <class  T>  Stack<T>::Stack(){
   m_maxSize = 100;      
   m_size = 0;
   m_pT = new T[m_maxSize];
}
template <class T>  Stack<T>::~Stack() {
   delete [] m_pT ;
}
        
template <class T> void Stack<T>::push(T t) {
    m_size++;
    m_pT[m_size - 1] = t;
    
}
template <class T> T Stack<T>::pop() {
    T t = m_pT[m_size - 1];
    m_size--;
    return t;
}
template <class T> bool Stack<T>::isEmpty() {
    return m_size == 0;
}

template <class T> 
template <class T2>  Stack<T>::operator Stack<T2>() {
    Stack<T2> StackT2;
    for (int i = 0; i < m_size; i++) {
        StackT2.push((T2)m_pT[m_size - 1]);
    }
    return StackT2;
}

//main.cpp
#include <stdio.h>
//#include "stack.h"
//#include "shape.h"
int main() {
    Stack<Circle*> pcircleStack;
    Stack<Shape*> pshapeStack;
    pcircleStack.push(new Circle);
    pshapeStack = pcircleStack;
    return 0;
}