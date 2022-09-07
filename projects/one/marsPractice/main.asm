# hello world program

## Data declaration section
.data

## String to be printed:
out_string: .asciiz "\nHello, World!\n"

## Assembly language instructions go in text segment
.text

## Start of code section
main:

# system call code for printing string = 4
li $v0, 4

# load address of string to be printed into $a0
la $a0, out_string

syscall # call operating system to perform operation
# specified in $v0

# syscall takes its arguments from $a0, $a1, ...
li $v0, 10 # terminate program
syscall