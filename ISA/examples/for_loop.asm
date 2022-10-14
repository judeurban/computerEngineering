# code for making a for-loop!

# specify hex print option
0x

# r1 contains the maximum iterator
# r2 contains the iterator
# r3 contains the incremetor
# r4 contains the running sum

# pseudocode:
# load an iterator into a register
# create a label
# running_sum += iterator
# iterator -= 1
# if iterator != r0, then branch to label
# if iterator == 0, code will exit as normal

loadi(r1, 5)        # maximum iterator limit
loadi(r2, 1)        # iterator
loadi(r3, 1)        # incremetor (++), a constant one
loadi(r4, 0)        # running sum

# increment up to 0xa
first_for_loop:

    addi(r4, r4, r2)                    # running_sum += iterator
    addi(r2, r2, r3)                    # iterator = iterator + 1
    console(r4)
    bne(r1, r2, first_for_loop)         # if iterator != max_iterator_limit, go back to the label "first_for_loop"

# increment down back to 0x0
second_for_loop:

    subi(r2, r2, r3)                    # iterator = iterator - 1
    subi(r4, r4, r2)                    # running_sum -= iterator
    console(r4)
    bne(r2, r0, second_for_loop)