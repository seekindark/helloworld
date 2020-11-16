#
# define a function to print the items of the list
#


def showlist(team):
    i = 0

    for item in team:
        # end= "" will not  generate '\n' automatically
        print("list[%d]=%s" % (i, item), end=" ")
        i += 1
    print("\n-------------------")


team = ['alice', 'bob', 'tom']
print(team)
print("size = ", len(team))
print("team[0]=%s, [1]=%s, [2]=%s, || [-1]=%s, [-2]=%s, [-3]=%s" %
(team[0], team[1], team[2], team[-1], team[-2], team[-3]))
showlist(team)

# append
team.append(4)          # there is no type limited in a list !!
team.append("last-one")
showlist(team)

# insert
team.insert(0, 'insert0')
team.insert(-1, 'insert-1')
showlist(team)

# pop
team.pop()
team.pop(0)
team.pop(1)      # pop out the item of the given index
showlist(team)

# replace
team[2] = 'jack'  #直接赋值替换
team[3] = 'last'
showlist(team)

# list as one element of another list
team[-1] = team
print(team)        # ['alice', 'tom', 'jack', [...]]
showlist(team)     # list[0]=alice list[1]=tom list[2]=jack list[3]=['alice', 'tom', 'jack', [...]]
team[-1] = [1, 2, 3, 4]
print(team)
showlist(team)

print("team[-1][-1] = ", team[-1][-1])

"""
this is comment
this is comment
"""
print("""-----------------""")
team1, team2 = [1, 2, 3, 4], ['a', 'b', 'c', 'd']
team = team1 + team2
team1x = team1*3
print('team1={0}'.format(team))
print('team1x={0}'.format(team1x))
print('team[2:-1]=', team[2:-1])

print("""---------xx--------""")
print(team)
print(team[0::2]) # 每隔两步截取一个列表
print(team[0:4:2]) # 每隔两步截取一个列表
print(team[-1::-1])   # 如果步长为负数，那么就反向截取
print(team[-1::-2])   # 如果步长为负数，那么就反向截取
print("""---------xx end--------""")


if __name__ == '__main__':
    print("""---------xxx--------""")
    team = [1 ,2, 3]*2
    showlist(team)
    print("""---------xxx end--------""")
