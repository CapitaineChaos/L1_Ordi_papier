

# Préparer la 2ème multiplication
def saut_1(end, w, x, y):
	y = x
	x = w
	end = 1
	saut_2(end, w, x, y)

# Affichage du résultat
def saut_out(x):
	print(x)
	exit(0)

# initialisation de la multiplication
def saut_2(end, w, x, y):
	z = 0
	saut_3(end, w, x, y, z)


# Multiplication z = y * x
def saut_3(end, w, x, y, z):
	if x == 0:
		saut_4(end, w, x, y, z)
	x = x - 1
	z = z + y
	saut_3(end, w, x, y, z)

# Multiplier une 2èeme fois ou afficher le résultat
def saut_4(end, w, x, y, z):
	x = z
	if end == 0:
		saut_1(end, w, x, y)
	else:
		saut_out(x)


w = int(input())
x = w
y = w
end = 0

saut_2(end, w, x, y)