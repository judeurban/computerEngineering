# code for calculating pi!

# pseudocode to calculate the ratio of pi:
# diameter = 10.0
# circumference = 3.18309886184
# pi = circumference / diameter

# r1 contains the circumference
# r2 contains the diameter
# r3 contains the result of pi

# load the status values into the registers
loadf(r1, 31.4159265359)         # circumference
loadf(r2, 0.0)                  # diameter

divf(r3, r1, r2)                 # calculate pi
console(r3)                      # print the value in r3 to the console
