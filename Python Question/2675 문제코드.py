count = int(input());

for i in range(count):
    num = input().split(' ');
    a = int(num[0]);
    text = num[1];
    for j in range(len(text)):
        for x in range(a):
            print(text[j],end='');
    print();
