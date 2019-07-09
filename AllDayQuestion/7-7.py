class exange:
    def __init__(self,number):
        self.num = []
        self.count = 0
        self.number = number
    def countnumber(self):
        for i in range(len(self.number)):
            for j in range(len(self.number)):
                if self.number[i] == -1:
                    break
                elif i==j:
                    continue
                else:
                    if self.number[i] == self.number[j]:
                        self.count+=1
                        self.number[j] = -1
            self.num.append(self.count)
            self.count = 0

    def printnum(self):
        for i in range(len(self.num)):
            if(self.num[i] > 0):
                print(self.number[i],i)

answer = input()[1:-1]
answer = answer.split(',')
a = exange(answer)
a.countnumber()
a.printnum()
