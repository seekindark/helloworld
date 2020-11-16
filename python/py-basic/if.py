#filename if.py

num = 123

guess = int(input('Entern a number : '))
if guess == num:
    print("Congratuations ! You're right !")
elif guess < num:
    print("Wrong, Your guess is less than target!")
else:
    print("Wrong, Your guess is more than target!")
print("======If over ! ========\n\n")

loop = True;
while loop:
    guess = int(input('Entern a number : '))
    if guess == num:
        print("Congratuations ! You're right !")
        loop=False;
    elif guess < num:
        print("Wrong, Your guess is less than target!")
    else:
        print("Wrong, Your guess is more than target!")
else:
    print("====While over =========\n\n\n")


for i in range(1, 4, 1):
    print(i)
    #break
else:
    print("========for over in range========\n\n\n")

for i in [1, 2, 3,4,5]:
    print(i)
else:
    print("========for over in [123345]========\n\n\n")
