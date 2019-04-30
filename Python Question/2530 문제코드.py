t = input().split(' ');
count = int(input());

hour = int(t[0]);
time = int(t[1]);
sec = int(t[2]);

sec+= count
time += (sec//60);
hour += (time//60);
sec %= 60;
time %= 60;
hour%=24;
print("%d %d %d"%(hour,time,sec));
