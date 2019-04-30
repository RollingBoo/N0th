a = int(input());
b = int(input());
print(a*(b%10));
c = b//10;
print(a*(c%10));
c = c//10;
print(a*(c%10));
print(a*b);
