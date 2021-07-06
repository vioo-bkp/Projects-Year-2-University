import sys
from itertools import product


def before(list):
    new_list = []
    for ele in list:
        if ele == "^":
            new_list.append(')')
            new_list.append('^')
            new_list.append('(')
        else:
            new_list.append(ele)
    return new_list


def transform_list_to_string(lista):
    list_1 = ""
    for ele in lista:
        list_1 += ele
    return list_1


negate = '~'
disjunction = 'V'
conjunction = '^'
k = int(sys.stdin.readline().rstrip())  # dimensiune de acoperire
n = int(sys.stdin.readline().rstrip())  # nr noduri
edge_count = int(sys.stdin.readline().rstrip())  # nr muchii
edges = []  # noduri ce formeaza muchii

for i in range(edge_count):
    string_value = sys.stdin.readline().rstrip()
    crt_edge = string_value.split(" ")
    edges.append(crt_edge)

i = list(range(1, k+1))
v = list(range(1, n+1))

# Creare LISTA X = [V, i]

X = list(list(zip(list(map(str, v)), element))for element in product(list(map(str, i)), repeat=len(v)))

final_X = []
for i in X:
    final_X += i

prim_i = list(range(1, n))
j = list(range(2, n+1))

res = sorted(prim_i + j)
# print(res)
res1 = list(map(str, res))
# print(res1)
for ele in range(len(res1)):
    res1[ele] = negate + res1[ele]
print()

z = 1
while z < len(res1):
    res1.insert(z, 'V')
    z = z + 1 + 1
for index in range(3, len(res1), 4):
    res1[index] = conjunction

res1.insert(0, '(')
res1.append(')')

SUPREM_F1 = before(res1)
FINAL_F1 = transform_list_to_string(SUPREM_F1)

temp = []
for i in edges:
    temp += i

# print(temp)
temp2 = temp[::2]
# print(temp2)  # --> U
temp1 = [item[1] for item in edges]
# print(temp1)  # --> V

prim_i = list(map(str, prim_i))
# print(prim_i)  # --> I

# Creare [u, i]
u_i = list(list(zip(temp2, element))
           for element in product(prim_i, repeat=len(temp2)))

final_u = []
for i in u_i:
    final_u += i

# Creare [v, i]
v_i = list(list(zip(temp1, element))
           for element in product(prim_i, repeat=len(temp1)))

final_v = []
for i in v_i:
    final_v += i

RES_U = list(set(final_X) & set(final_u))
RES_V = list(set(final_X) & set(final_v))

FINAL_U = [item for t in RES_U for item in t] 
FINAL_V = [item for t in RES_V for item in t]

z = 1
while z < len(FINAL_U):
    FINAL_U.insert(z, 'V')
    z = z + 1 + 1

for index in range(3, len(FINAL_U), 4):
    FINAL_U[index] = conjunction

FINAL_U.insert(0, '(')
FINAL_U.append(')')

SUPREM_U = before(FINAL_U)

z = 1
while z < len(FINAL_V):
    FINAL_V.insert(z, 'V')
    z = z + 1 + 1

for index in range(3, len(FINAL_V), 4):
    FINAL_V[index] = conjunction
# print(FINAL_V)

FINAL_V.insert(0, '(')
FINAL_V.append(')')

# print(FINAL_V)
SUPREM_V = before(FINAL_V)
# print(SUPREM_V)

FINAL_F2 = transform_list_to_string(SUPREM_U) + '^' + transform_list_to_string(SUPREM_V)
# print(FINAL_F2)

F = FINAL_F1 + '^' + FINAL_F2
print(F)
