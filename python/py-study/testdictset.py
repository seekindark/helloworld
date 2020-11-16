dict = {1: 2, "a": "word"}
dict[2] = 3
dict[1] = 4
dict['abc'] = 'ddd'
print(dict)
dict.pop(1)
print(dict)

print(dict.get('a'))
print(dict.get(100))
dict.update({100: 200})
print(dict)
print(len(dict))
print("------", dict.pop('a'))
dict['a'] = 'asafsafaf'
print(dict.popitem())
print(dict)
s = set(dict)
print(s)

s = set((1, 2, 3, 4))
s.add(7)
s.remove(1)
s.add(10000)
# print('-----------'); print(s); print(dict); print("--------------")

# 集合set的交集，差集，并集
a = {1, 2, 3, 4, 5}
b = {2, 4, 6}
print(a - b)     # a 和 b 的差集 , a里去掉 a,b的交集, {1, 3, 5}
print(b - a)     # b 里去掉 a,b的交集, {6}
print(a | b)     # a 和 b 的并集
print(a & b)     # a 和 b 的交集 {2, 4}
print(a ^ b)     # a 和 b 中不同时存在的元素 {1, 3, 5 ,6}
