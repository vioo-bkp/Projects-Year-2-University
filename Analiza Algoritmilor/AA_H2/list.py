from time import time

def insertionort(l):
    for i in range(1, len(l)):
        token = l[i]
        count = i

        while count > 0 and l[count - 1] > token:
            l[count] = l[count - 1]
            count = count - 1

        l[count] = token

l1 = [21, 3, 5, 210, 129, 69]
l2 = [22, 541, 102, 43, 98]
lists = [l1, l2]
temp = []

begin_time = time()

for x in lists:
    insertionort(x)
    print (x)

duration = time() - begin_time

print('{0:.14f}'.format(duration))

