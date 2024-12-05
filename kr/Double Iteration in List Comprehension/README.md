>>> Номер 1: срезы <<<
  
  варик 1
def decode(d):
    tmp = str(d).split(sep='&')
    a, b = int(tmp[0]), int(tmp[1])
    ans = tmp[2][a::2][:b]
    return ans
a = '3&6&   п р и в е т      '
b = '5&7&     м и р '
print(decode(a))
print(decode(b))

  варик 2
def decode(d):
    tmp = str(d).split(sep='%')
    a, b = int(tmp[0]), int(tmp[2])
    ans = tmp[1][a::-3][:b][::-1]
    return ans
a = '19%ммммПммРммИммВммЕммТммм%6'
b = '6%МппИппРппппппп%5'
print(decode(a))
print(decode(b))



>>> Номер 2: словари <<<
  
  варик 1
d = {}

def add(key, value):
    d[key] = d.get(key, []) + [value]

a = str(input())
while a != '':
    for x in a.split():
        add(x[-1].upper(), x)
    a = str(input())

for i in sorted(d.keys()):
    print(f'{i} - {' '.join(d.pop(i))}')




>>> Номер 3: двойные списочные выражения <<<

n = [1, 2, 3, 4, 5, 6]

  варик 1
сортируем на [четные, нечетные]
l = [x for y in [0, 1] for x in n if x % 2 == y]

  варик 2
сортируем на [>3, <=3]
l = [x for y in [True, False] for x in n if (x > 3) == y]



>>> Номер 4: итераторы <<<

  варик 1 + 2
from itertools import product
a = [['1', '2', '1'], ['4', '5'], ['8', '9'], ['3', '0']]
ans = [''.join(x) for x in list(product(*a))]
print(*sorted(set(ans)), sep='\n')
