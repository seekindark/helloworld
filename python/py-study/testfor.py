for i in range(5):
    print(i, end=" ")
else:
    print('\nexit the for, and i=', i)

for i in range(2, 20, 3):
    print(i, end=" ")

print('\n')

for i in range(20, 0, -3):
    print(i, end=" ")
    if(i <= 10): 
        break      # when break is called, the block of 'else' will not be executed.
else:
    print('\nexit the for, and i=', i)

def func():
    pass

