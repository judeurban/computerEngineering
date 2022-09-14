# code for calculating pi!

# pseudocode to calculate the ratio of pi:
# diameter = 5.0
# circumference = 15.7079632679
# pi = circumference / diameter

# r1 contains the circumference
# r2 contains the diameter
# r3 contains the result of pi

# load the status values into the registers
loadf(r1, 15.7079632679)         # circumference
loadf(r2, 5.0)                   # diameter

divf(r3, r1, r2)                 # calculate pi

label:
    console(r3)                      # print the value in r3 to the console
    jump(label)

# this outputs 0x40490fda which, converted to IEEE-754 
# floating point algorithm is 3.14159