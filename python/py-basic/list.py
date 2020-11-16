

list =['abc', 1, 2.0, True]
print(list)
print(list[0])
print(list[0:-1])
print(list * 2)

print('----concatinate------------')
list2=[4]
list = list + list2
print (list)
print(list2)
list2 = list
print(list2)

print('----modify------------')
list[0:1]=['x', 5 , 6]


print (list)
#==============

print("======tuple-========")
tup=(1,2,3,4,'x')
print(tup)

tup2=('a','b','c')
tup=tup2
print(tup)

#tup[1:2] =(5,)

del tup2
#print(tup2)
tup2=('contain a list', list)
print("tup2=", tup2);

list=['contain a tuple', tup2]
print("list=", list)
