#!/usr/bin/python3
def prn_obj(obj): 
    print('\n'.join(['%s:%s' % item for item in obj.__dict__.items()])) 
    print(obj.__class__)
    print(obj.__doc__)
    print(obj.__getattribute__)

class test_A:

    def __init__(self, val):
        self.a = val

    def printA(self):
        print("test_A print out: a=%d\n" % self.a)

    def oneMethod(self):      ##  任何时候， 类的方法都必须带 self 参数
        print("it's fathre-class methond")

class test_B(test_A):
    '''
    some words for class document/description
    '''
    def __init__(self, a, b):
        test_A.__init__(self, a)
        self.b = b
        self.__privateVar = 123        ### 这里的__privateVar 是私有变量，其实它已经被改名字了：_test_B__privateVar, 前面增加 _className

    def printB(self):
        print("test_B print out: a=%d, b=%d\n" % (self.a, self.b))

    def oneMethod(self):
        print("it's child-class B methond")

A = test_A(1)
A.printA()
B = test_B(1, 2)
B.printB()
prn_obj(B)
B.oneMethod()
super(test_B, B).oneMethod()   # 访问B的父类的方法
print("===============")
B.a = 100
B.b = 200
B.c = 300       # 凭空 新增了一个成员变量。 可以在类的使用过程随时新增 数据成员
B.__c = 400     # 新增 私有变量，可以吗？ No。 即使有 “__"开头的变量名，在使用时 新增的，也不能算是 私有变量。
B.__c = B.__c + 1  #   __c 依然可以被访问 ！！因为它不是私有变量
B.__privateVar = 1234    # 声明在  类里的 有2个下划线’__‘开头的变量, 注意 : !!!! 这里 __privateVar  依然是 公开变量，因为它是通过在外部新增的。
print("access private var: ", B.__privateVar)
#prn_obj(B)
print(B.__dict__)

'''
output:
{'a': 100, 'b': 200, '_test_B__privateVar': 123, 'c': 300, '__c': 401, '__privateVar': 1234}

从这里的打印结果看， 有2个 ’看起来像的‘ 私有变量：
_test_B__privateVar         # 这个才是真的 私有变量， __privateVar是在内部声明的，保存时，加了前缀 _className, i.e. _test_B
__privateVar                # 这个是假的的 私有变量，因为它是在外部新增的（通过赋值），所以内部保存是没有前缀_className，



'''