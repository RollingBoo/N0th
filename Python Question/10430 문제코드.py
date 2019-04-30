data = input().split(' ');
A = int(data[0]);
B = int(data[1]);
C = int(data[2]);
answer = (A+B)%C
print(answer);
answer = ((A%C)+(B%C))%C
print(answer);
answer = (A*B)%C
print(answer);
answer = ((A%C)*(B%C))%C
print(answer);
