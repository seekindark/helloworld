
import testmain2


print("before main()---")

def main():
    print("main body: www.runoob.com: __name__={0}".format(__name__))
    testmain2.testmain2()



print("after main()---__name__={0}".format(__name__))

if __name__ == '__main__':
    print(dir(testmain2))
    testmain2.main()
    main()

    print("xxxxxxxx")

print("last line of the file")

