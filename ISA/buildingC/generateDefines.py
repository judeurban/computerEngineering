
'''
#define ADD_I_S "addi"
#define ADD_I_V 0x0
'''

c_define = "#define"
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
            if R_TYPE in line:
                print(f'\n// == {R_TYPE} ==')
            elif J_TYPE in line:
                print(f'\n// == {J_TYPE} ==')
            elif I_TYPE in line:
                print(f'\n// == {I_TYPE} ==')
            elif L_TYPE in line:
                print(f'\n// == {L_TYPE} ==')

            continue

        print(f'{c_define} {line.upper()}_S "{line}"')
        print(f'{c_define} {line.upper()}_V {hex(i)}')
        i += 1


    print("\n// total operations:", hex(i), f'= Od{i}')
    print("// =========================================")
