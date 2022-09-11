# code for calculating pi!

# pseudocode to calculate the ratio of pi:
# diameter = 10.0
# circumference = 3.18309886184
# pi = circumference / diameter c=2pir > pi=c/R

# r1 is the circumference
# r2 is the diameter
# r3 is the result of pi

# load the status values into the registers
loadf(r1, 3.18309886184)                  # circumference
loadf(r2, 10.0)                           # diameter

div(r3, r1, r2)                 # calculate pi
console(r3)                     # print the value in r3 to the console

