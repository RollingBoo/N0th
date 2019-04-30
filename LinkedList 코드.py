class Node:
    def __init__(self,data):
        self.data = data;
        self.next = None;

class LinkedList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.count = 0
    def insert(self, a):
        if self.head == None:
            self.head = a
            self.tail = a
        else:
            self.tail.next = a
            self.tail = a
    def println(self):
        current = self.head
        while(True):
            print(current.data)
            current = current.next;
            if(current == None):
                break;

li = LinkedList()
node1 = Node(1)
node2 = Node(2)
node3 = Node(3)
li.insert(node1)
li.insert(node2)
li.insert(node3)

li.println()
