
'''
#define ADD_I_S "addi"
#define ADD_I_V 0x0
'''

R_TYPE = 'R-TYPE'
J_TYPE = 'J-TYPE'
I_TYPE = 'I-TYPE'
L_TYPE = 'L-TYPE'

with open('defines.txt', 'r') as f:
    
    i = 0

    lines = f.readlines()

    print("// =========================================")
    print("// OPCODES\n")

    for line in lines:
        line = line.strip()

        if len(line) < 1:
            continue
        elif '#' in line:
            continue

        print(f'void {line.upper()}(uint32_t*, uint32_t*, uint32_t*);')
        # i += 1


    print("\n// total operations:", hex(i), f'= Od{i}')
    print("// =========================================")
