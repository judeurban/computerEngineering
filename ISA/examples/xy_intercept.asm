# code for slope and intercept... y = mx + b

# specify hex print option
0x

# slope_m  = (yfinal - yinit) / (xfinal - xinit)

# define initial point (1, 1)
loadf(r1, 1.0)      # xinit
loadf(r2, 1.0)      # yinit

# define final point (2, 2)
loadf(r3, 3.25)      # xfinal
loadf(r4, 9.09)      # yfinal

# calculate slope m
subf(r5, r4, r2)    # (yfinal - yinit)
subf(r6, r3, r1)    # (xfinal - xinit)
divf(r7, r5, r6)    # m = (yfinal - yinit) / (xfinal - xinit)
console(r7)         # print slope

# calculate y-intercept b
# b = y - mx
mulf(r8, r7, r1)    # m*x
subf(r9, r2, r8)    # b = y - m*x
console(r9)         # print y-intercept