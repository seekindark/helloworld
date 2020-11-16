def testGlobal(var):
    print("before global: x = ", x)
    global x
    x=2
    print("after global: x = ", x)

x=100
print("before testGlobal, x=100")
testGlobal(x)
print("after global: x = ", x)
