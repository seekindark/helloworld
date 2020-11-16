
def prn_obj(obj): 
    print('\n'.join(['%s:%s' % item for item in obj.__dict__.items()])) 


class test_A:

    def __init__(self, val):
        self.a = val

    def printA(self):
        print("test_A print out: a=%d\n" % self.a)


class test_B(test_A):
    
    def __init__(self, a, b):
        test_A.__init__(self, a)
        self.b = b

    def printB(self):
        print("test_B print out: a=%d, b=%d\n" % (self.a, self.b))

A = test_A(1)
A.printA()
B = test_B(1, 2)
B.printB()
prn_obj(B)