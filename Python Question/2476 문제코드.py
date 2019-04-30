a = int(input())
result = []
maxim = 0
for i in range(a):
    x,y,z = map(int,input().split())
    if x==y==z:
        result.append(10000+(x*1000))
    elif x==y:
        result.append(1000+x*100)
    elif x==z:
        result.append(1000+x*100)
    elif y==z:
        result.append(1000+y*100)
    else:
        a = max(x,y,z)
        result.append(a*100)
for i in result:
    if(maxim < i):
        maxim = i;
print(maxim)
