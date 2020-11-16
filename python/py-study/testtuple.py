

def showtuple(team):
    i = 0
    for item in team:
        # end= "" will not  generate '\n' automatically
        print("tuple[%d]=%s" % (i, item), end=" ")
        i += 1
    print("\n-------------------")


#
# the len of tuple cannot be changed !!!
#
team = ("alice", 'bob', 'colin')
print(team)
showtuple(team)

# repalce  impossible
# team[0] = 'last'
# print(team)

# t2 = (1,)  #只有1个元素的tuple， 后面需要一个 逗号 来避免运算符（）
# print(t2)

# tuple, list convert
team_list = list(team)
print(team_list)

team_tuple = tuple(team_list)
print(team_tuple)
