class myClass():
    def __init__(self, a, b):
        self.a = a
        self.b = b

    x = 100
    y = 'asbc'

    def test(self):
        print('x=', self.x, 'y=', self.y, 'a=', self.a, 'b=', self.b)
        return self.x + 1


c = myClass(-200, 'hell world-c')
d = myClass(200, 'hello world-d')
print(c.__init__)
print(c.__class__)
c.x += 1
print(c.x, c.y)
print(d.x, d.y)
c.test()
d.test()

print('===========================')


class person:
    name = ''
    age = 0
    __weight = 0    # 私有成员变量

    def __init__(self, n, a, w, g):
        self.name = n
        self.age = a
        self.__weight = w
        self.__gender = g

    def speak(self):
        print("{0} say: I am {1}, age is {2} and weight is {3}".format(self.name, self.__gender, self.age, self.__weight))


p = person('Jack', 20, 200, 'boy')
p.speak()
print('self.name = ', p.name)
try:
    print('self.__gender = ', p.__weight)
except AttributeError:
    print("Not allowed to externally call a private member '__weight' of the calss ")
else:
    pass
