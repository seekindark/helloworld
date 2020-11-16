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
        print("{0} say: I am {1}, {2} years old and {3} kg".format(self.name, self.__gender, self.age, self.__weight))

    def get_weight(self):
        return self.__weight

    def get_gender(self):
        return self.__gender


class student(person):
    def __init__(self, name, age, weight, gender, grade):
        person.__init__(self, name, age, weight, gender)
        self.grade = grade

    def speak(self):
        # 父类的 私有 属性和方法 都不能被 继承，所以无法使用
        # print("{0} say: I am {1}, {2} years old and {3} kg, I am a student".format(self.name, self.__gender, self.age, self.__weight))
        print(
            "{0} say: I am {1} years old, grade {2} and I am a student at grade {3}, gender = {4}, weight = {5}kg"
            .format(self.name, self.age, self.grade, self.grade, self.get_gender(), self.get_weight()))


s = student('Tom', 10, 40, 'boy', 3)
s.speak()
print("=======EOL========")
