

print("filename = %s" %__file__)

def testmain2():
    print("funciton body of testmain2(), __name__==%s" % __name__)

def main():
    # test the module testmain2
    print(">>> start to test module: testmain2")
    testmain2()
    print("<<< stop to test module: testmain2")    


def testmain2_method(a, b):
    pass

if __name__ == "__main__":
    main()
else:
    print("module", __name__, "is imported!")

