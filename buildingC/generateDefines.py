
'''
#define ADD_I_S "addi"
#define ADD_I_V 0x0
'''

c_define = "#define"

with open('defines.txt', 'r') as f:
    
    i = 0

    lines = f.readlines()
    for line in lines:
        line = line.strip()
        print(f'{c_define} {line.upper()}_S "{line}"')
        print(f'{c_define} {line.upper()}_V {hex(i)}')
        i += 1