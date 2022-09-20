import enum


c_define = "#define"
R_TYPE = 'R-TYPE'
J_TYPE = 'J-TYPE'
I_TYPE = 'I-TYPE'
L_TYPE = 'L-TYPE'

'''
return type, (ptr, name[size]) (param1, param2, param3)
int (*p[4]) (int x, int y);
'''

buffer = []

with open('defines.txt', 'r') as f:
    
    i = 0

    lines = f.readlines()

    print("// =========================================")
    print("// ARRAY OF FUNCTION POINTERS\n")

    # count the number of lines
    for line in lines:

        line = line.strip()
        if len(line) < 1:
            continue
        elif '#' in line or ':' in line:
            continue
        elif 'jump' in line:
            break

        buffer.append(line)

        # print(f'{c_define} {line.upper()}_S "{line}"')
        # print(f'{c_define} {line.upper()}_V {hex(i)}')
        i += 1

    print(f"#define NUM_RTYPE_OPERATIONS {i}")
    print(f"void (*RTypeFunctionPtrs[NUM_RTYPE_OPERATIONS]) (uint32_t* rd, uint32_t* r1, uint32_t* r2);")

    for j, op in enumerate(buffer):
        op_value = f'{op.upper()}_V'
        print(f'RTypeFunctionPtrs[{op_value}] = {op.upper()}; /* address of {op.upper()}() */')
        