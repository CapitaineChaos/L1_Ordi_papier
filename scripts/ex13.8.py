x = 0
y = int(input())
while y == 255:
  x = x + y
  y = int(input())
print(x % 256)