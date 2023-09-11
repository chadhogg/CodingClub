# HouseOfCards.py
# Chad Hogg
# My solution to https:#open.kattis.com/problems/houseofcards
# Note: Can't easily solve this in C++ because it needs a BigNum implementation.

# Height | New Cards | Total Cards
#      1 |         2 |           2
#      2 |         5 |           7
#      3 |         8 |          15
#      4 |        11 |          26
#      5 |        14 |          40

# Recurrence relation for New Cards:
#  N(1) = 2
#  N(X) = N(X - 1) + 3
# solving attempt
#  N(1) = 2
#  N(2) = N(1) + 3
#  N(3) = N(2) + 3 = (N(1) + 3) + 3 = N(1) + 2 * 3
#  N(4) = N(3) + 3 = (N(1) + 2 * 3) + 3 = N(1) + 3 * 3
#  N(X) = N(1) + 3(X - 1)

# Recurrence relation for Total Cards
#  T(1) = 2
#  T(X) = T(X - 1) + N(X)
#  T(X) = T(X - 1) + 2 + 3(X - 1)
# solving attempt
#  T(1) = 2
#  T(2) = T(1) + 2 + 3(1)
#  T(3) = T(2) + 2 + 3(2) = (T(1) + 2 + 3) + 2 + 6
#  T(4) = T(3) + 2 + 3(3) = ((T(1) + 2 + 3) + 2 + 6) + 2 + 9
#  T(X) = T(1) + 2(X - 1) + 3(X(X-1)/2)

min_height = int(input())

while (True):
    cards = 2 + 2*(min_height - 1) + 3 * min_height * (min_height - 1) // 2
    if cards % 4 == 0:
        print(min_height)
        break
    min_height += 1
    