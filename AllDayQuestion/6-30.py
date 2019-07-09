class Node:
    def __init__(self,data):
        self.data = data;
        self.next = None;

class Linkedlist:
    def __init__(self):
        self.head = None
        self.tail = None
        self.count = 0
    def insert(self,Node):
        if self.head == None:
            if Node.data >=10:
                Node.data = 0
            self.head = Node
            self.tail = Node
        else:
            self.tail.next = Node
            self.tail = Node
    def println(self):
        current = self.head
        while(True):
            print(current.data)
            current = current.next;
            if(current == None):
                break;

li1 = Linkedlist()
node1 = Node(1)
node2 = Node(5)
node3 = Node(6)
li1.insert(node1)
li1.insert(node2)
li1.insert(node3)

li2 = Linkedlist()
node1 = Node(0)
node2 = Node(0)
node3 = Node(4)
li2.insert(node1)
li2.insert(node2)
li2.insert(node3)

lit = Linkedlist()
for i in range(3):
    node = (Node(li1.head.data + li2.head.data))
    li1.head = li1.head.next
    li2.head = li2.head.next
    lit.insert(node)
lit.println()
