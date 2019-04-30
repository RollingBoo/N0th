count = int(input());

for i in range(count):
    num = input().split(' ');
    a = float(num[0]);
    for j in range(1,len(num)):
        if num[j] == '@':
            a*=3;
        elif num[j] == '%':
            a+=5;
        elif num[j] == '#':
            a-=7;
    print("%0.2f"%a);
