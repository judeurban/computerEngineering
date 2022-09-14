# Currently Valid Operations

## Memory Operations

`loadf(r1, 3.14159)` -  Load a float into a register.

`loadi(r1, 54)` - Load an int into a register.

## maths

`addi(r0, r1, r2)` - Add the integer values inside registers 1 and 2, storing the value into register 3.

`divf(r1, r2, r3)` - Divide the value in register 1 by the value in register 2, storing the value into register 3.

`console(r1)` - Print the value inside register 1.

## Branching

`my_label:` - Create a new label called "my_label". The software enumerates each label and assigns an instruction pointer to it.

`jump(my_label)` - Jump to label "my_label". There is no conditional justification to the jump. It's just a jump.

`bne(r1, r2, my_label)` - Jump to the label "my_label" if the data inside the two registers is identical.


# Currently Inactive Operations (Not implemented)

## maths

addf
subi
subf
muli
mulf
divi
slt

not
and
nand
or
nor
xor

## branching
beq
