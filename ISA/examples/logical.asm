# 0xFFFFFFFF = 0d4294967295

# specify binary print option
0b

loadf(r1, 3.14159)
console(r1)

loadf(r2, 2.7182)
console(r2)

xnor(r3, r1, r2)
console(r3)