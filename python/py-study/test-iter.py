import sys
list = [1, 2, 3, 4, 5]
it = iter(list)
print(it)       # just print out the pointer of obj. it
while True:
    try:
        print(next(it), end=" ")
    except StopIteration:
        break
        #sys.exit()
print("\n")


# define my iter for a class
class myNumbers:

    def __iter__(self):
        self.a = 10
        return self

    def __next__(self):
        if self.a <= 15:
            x = self.a
            self.a += 1
            return x
        else:
            raise StopIteration


mynum = myNumbers()
it = iter(mynum)
while True:
    try:
        a = next(it)
        print(a)
    except StopIteration:
        print("exit by exception: StopIteration")
        sys.exit()
