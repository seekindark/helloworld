# list as stack
stack = []
stack.append(1)
stack.append(2)
stack.append(5)
print(stack)
stack.pop()
print(stack)

# list as queue
from collections import deque
q = deque(['alice', 'bob', 'tom'])
print(q)  # deque(['alice', 'bob', 'tom'])
q.append('dock')
q.append('johon')
print(q)
q.pop()   # no arguments
print(q)
q.popleft()
print(q)
"""
output:
deque(['alice', 'bob', 'tom'])
deque(['alice', 'bob', 'tom', 'dock', 'johon'])
deque(['alice', 'bob', 'tom', 'dock'])
deque(['bob', 'tom', 'dock'])
"""

