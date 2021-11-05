idx=["idel", '1', '2', '3', '4', '5']
labels = ['a', 'b', 'c', 'd', 'e', 'f']
cyc=[23, 0, 45, 4, 14, 29]

combo = zip(idx, labels, cyc)
print(idx, cyc, labels)
print(combo)                    # return object
print(list(combo))              # with list() to turn combo to a list from a object
print(list(combo))          # it's empty !! ??

#
# all are empty !!!   似乎用了一次list()转换combo后，就无法再用了。
#
# sorted_combo = sorted(combo)    # cannot sorted() a object
# print("sorted_combo = ", list(sorted_combo))
combo_list = list(combo)
print("combo_list = ", list(combo))
sorted_combo_list = sorted(combo_list)
print("sorted_combo_list = ", sorted_combo_list)

combo1_list = list(zip(idx, labels, cyc))
print("combo1_list = ", combo1_list)
sorted_combo1_list = sorted(combo1_list, key=lambda s: s[2])   # 按第3列的值排序
print("sorted_combo1_list = ", sorted_combo1_list)


#for i, j in zip(range(0, length, 2), range(length-1, 0, -2)):

length = len(sorted_combo1_list)
# 照着"大,小,大,小"的方式交叉排列
print("range(0, length, 2), range(length-1, 0, -2) = ", list(range(0, length, 2)), list(range(length-1, 0, -2)))
for i, j in zip(range(0, length, 2), range(length-1, 0, -2)):
    if j <= i:
        break
    sorted_combo1_list[i], sorted_combo1_list[j] = sorted_combo1_list[j], sorted_combo1_list[i]

print("adjusted: sorted_combo1_list = ", sorted_combo1_list)
unzipped_sorted = list(zip(*sorted_combo1_list))
print("adjusted: unzipped_sorted = ", unzipped_sorted)