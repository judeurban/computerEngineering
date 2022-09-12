
'''
#define ADD_I_S "addi"
#define ADD_I_V 0x0
'''

c_define = "#define"
R_TYPE = 'R-TYPE'
J_TYPE = 'J-TYPE'
I_TYPE = 'I-TYPE'

with open('defines.txt', 'r') as f:
    
    i = 0

    lines = f.readlines()

    print("// OPCODES")
    print("// =========================================\n")

    for line in lines:
        line = line.strip()

        if len(line) < 2:
            continue
        elif '#' in line:
            if R_TYPE in line:
                print(f'\n// == {R_TYPE} ==')
            elif J_TYPE in line:
                print(f'\n// == {J_TYPE} ==')
            elif I_TYPE in line:
                print(f'\n// == {I_TYPE} ==')

            continue

        print(f'{c_define} {line.upper()}_S "{line}"')
        print(f'{c_define} {line.upper()}_V {hex(i)}')
        i += 1


    print("\n// =========================================")

    print("// total instructions:", hex(i), f'= Od{i}')