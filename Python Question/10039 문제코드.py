num = [];
total = 0;
for i in range(5):
    num.append(int(input()));
    if(num[i]<40):
        total += 40;
    else:
        total += num[i];
print(total//5);
