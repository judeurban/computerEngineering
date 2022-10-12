
'''
\hline
item 11 & item 12 & item 13 \\
\hline
item 21  & item 22  & item 23  \\
'''

with open('defines.txt', 'r') as f:
    
    i = 0
    lines = f.readlines()

    for line in lines:
        line = line.strip()

        if len(line) < 1:
            continue
        elif '#' in line:
            continue

        print('\hline')

        if line.endswith('i'):
            print(f'{line.upper()} & {hex(i)} & Performs integer {line} operation on two source registers and stores the value in the destination register.\\\\')
        elif line.endswith('f'):
            print(f'{line.upper()} & {hex(i)} & Performs floating point {line} operation on two source registers and stores the value in the destination register.\\\\')
        else:
            print(f'{line.upper()} & {hex(i)} & Performs {line} operation on two source registers and stores the value in the destination register.\\\\')

        i += 1
