a = int(input());
i=2;
while(True):
    if(int(a)<=1):
        break;
    if(int(a)%int(i) == 0):
        print(i);
        a = int(a)//int(i);
        i=2;
    else:
        i+=1;
