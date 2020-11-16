# 推导式
s = {1, 2, 3, 4}
y = {x*2 for x in s}  # {8, 2, 4, 6}， set is out of order
print(y)
y = {(i, x) for x in s for i in range(len(s))}

i = 0
y = set()
for x in s:
    y.add((i, x))
    i += 1
print(y)