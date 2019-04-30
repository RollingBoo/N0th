count = int(input());

for i in range(count):
    num = input().split(' ');
    num1 = int(num[0]);
    num2 = int(num[1]);
    print("Case #%d: %d + %d = %d" %(i+1,num1,num2,num1+num2));
