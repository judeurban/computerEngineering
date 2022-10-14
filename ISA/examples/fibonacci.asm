# fibonacci!

# specify hex print option
0x

loadi(r1, 10)    # r1 contains the maximum iterator
loadi(r2, 0)     # r2 contains F-2
loadi(r3, 1)     # r3 contains F-1
loadi(r4, 0)     # r4 contains the running_sum
loadi(r5, 0)     # iterator
loadi(r6, 1)     # +=1

begin:
    addi(r4, r3, r2)    # add F-2 and F-1
    addi(r2, r3, r0)    # propogate the value from F-1 to F-2
    addi(r3, r4, r0)    # propogate the value from F to F-1
    addi(r5, r5, r6)    # iterator += 1
    bne(r1, r5, begin)  # go to "begin"

console(r4)