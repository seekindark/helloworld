name = ''
count = 0
while name != 'exit':
    count += 1
    name = input("please input your name:")
    if name == 'cq':
        print("You are a genius, %s" % name)
    elif name == 'exit':
        print('Bye!')
    else:
        print("{0} is a SB-{1}".format(name, count), '!')

    if count >= 10:
        print("I guess you want to exit the program. please input 'exit'")
