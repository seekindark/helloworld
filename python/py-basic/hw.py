#! /usr/bin/python3

#first comment
#中午注释
print('Hello, world')
print ('hello', "world", sep=',')  #works like this

'''
more comments line 4
more comments line 5
'''

"""
fasfa
asfasfsa
fasfaf
"""
print   ('lllll hello workd')

if False:
    print (True)
else:
    print (False)
print ('end----')


# bytes
print("b=",bytes.fromhex('2eff'))

# line slicing === one sentences written on multiple lines
print (bytes([98,99,100,101,0xff, \
              1,2]), \
             "fasfa")

print("中国",u'伟大')

paragraph = """fafa
    line 2"""
print(paragraph)

#input ("\n\n enter to exit:\n");

import sys; x='runoob'; sys.stdout.write(x+'\n')
print("=========import sys module=========")
for i in sys.argv:
    print(i)
print('\npython path is:', sys.path)

t=help(sys)
#tlen=len(t)
print(t)
     
       
