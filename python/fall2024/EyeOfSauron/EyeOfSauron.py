# My solution to https://open.kattis.com/problems/eyeofsauron from August 29, 2024

line = input()
if len(line) % 2 == 1:
    print('fix')
elif line[len(line) // 2 - 1] != '(' or line[len(line) // 2] != ')':
    print('fix')
else:
    print('correct')