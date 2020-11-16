class myError(Exception):
    def __init__(self, value):
        self.value = value

    def __str__(self):
        return repr(self.value)


try:
    raise myError('2/0 is illegal!')
except myError as e:        # 赋值 异常 给 e
    print('My exception occured: ', e.value)
    # print('My exception occured 2: ', myError.value)
