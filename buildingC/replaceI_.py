
with open('replaceI_.txt', 'r') as f:

    lines = f.readlines()
    for line in lines:

        if 'I_' in line:
            line = line.replace('I_', 'F_')

        print(line, end='')