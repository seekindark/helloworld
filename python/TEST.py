#!/usr/bin/python3
def solution(X, Y, A):
    result = -1
    N = len(A)
    nX = 0
    nY = 0
    for i in range(N):
        if A[i] == X:
            nX += 1
        if A[i] == Y:
            nY += 1
        if nX == nY:
            result = i

    return result



