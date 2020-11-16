team = list(range(5))

for item in team:
    if item == 3:
        continue
    print(item)

n = 0
while n < 5:
    print("team[%d]=%d" % (n, team[n]), end=' ')
    n = n+1
    if(n == 3):
        break
