# instructions:
1) load() - load from memory
2) store() - store to memory
3) add() - adding values from two registers
4) sub() - subtracting values from two registers
4) mul() - multiplying values from two registers
5) div() - dividing a value by another value from two registers
6) jmp() - jump to a statement
7) beq() - branch to a statement given that two values are equal

# loading words from memory
`load()`

# storing words
`store(r0, 0)`

# maths

`add(r0, r1, r2)` - add the values inside registers 1 and 2, storing the value into register 3

`sub(r0, r1, r2)` - subctract the values inside registers 1 and 2, storing the value into register 3

`mul(r1, r2, r3)` - multiply the values inside registers 1 and 2, storing the value into register 3

`div(r1, r2, r3)` - divide the value in register 1 by the value in register 2, storing the value into register 3

`console(r1)` - print the value inside register 1