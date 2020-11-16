
set=set('abcxyabcx')
print(set)

set={1,2,3,4}
print(set)

a={"abc", "tome", "xyz", "abc", "tom"}
print (a)

print("==============dict=============");

dict={}
print(dict)

dict[1]="1-course"
dict['two']="2-tool"
dict[3]=3

dict2 = {'red':0x0001, 'yellow':0x0002, 'green':0x0003, 'red':0xffff} #the last key red is duplicated with first one and thus removed automatically
print(dict)
print(dict2)
print(dict.keys(), dict.values())

dict = {'Taobao': 3, 'Runoob': 1, 'Google': 2}
print(dict)

dict3 = {x: x**2 for x in (2, 4, 6)} #特别的赋值方法
print(dict3)

print("==list={x for x in (1, 2, 3)}=========")
list=[x for x in (1, 2, 3)]
print(list)

tuple=(y for y in (1,2,3))
print(tuple)

#a=dict((1, 2))
a=str(234)
del set
b=set(a)
print(a)

