t = input().split(' ');
count = int(input());

hour = int(t[0]);
time = int(t[1])+count;

a = time//60;
time %= 60;
hour +=a;
hour%=24;
print("%d %d"%(hour,time));
