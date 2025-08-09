import argparse
import os
import re

parser = argparse.ArgumentParser(description="Input data generator")

parser.add_argument('data_id', type=int, help="the index of the data")

args = parser.parse_args()

data_id = args.data_id

file = f"{data_id}.out"

os.system(f"scheme < {data_id}.in > tmp.out")

fread = open("tmp.out", "r")
fwrite = open(f"{data_id}.out", "w")

cnt = 0

def to_quote(input_str):
    index = input_str.find("'")
    while index != -1:
        right = index + 1
        if input_str[right] == '(':
            cnt = 1
            while cnt:
                right += 1
                if input_str[right] == '(':
                    cnt += 1
                elif input_str[right] == ')':
                    cnt -= 1
        else:
            while right < len(input_str) and input_str[right] != ")" and input_str[right] != " ":
                right += 1
        # print("=========================")
        # print(index)
        # print(input_str)
        # print(input_str[:index])
        # print(f"(quote {input_str[index + 1:right]})")
        # print(input_str[right:])
        input_str = input_str[:index] + f"(quote {input_str[index + 1:right]})" + input_str[right:]
        index = input_str.find("'")
        # print(input_str)
    input_str = re.sub(r'\\x2B;', '+', input_str)
    input_str = re.sub(r'\\x2E;', '.', input_str)
    input_str = re.sub(r'\\x2D;', '-', input_str)
    return input_str

result = ""
last_expr = ""

for eachline in fread:
    cnt += 1
    if cnt <= 3:
        continue
    tmp = eachline[:-1]
    if tmp[0] == '>':
        result += to_quote(last_expr)
        if cnt > 4:
            result += "\n"
        last_expr = ""
        tmp = tmp[2:]
        while len(tmp) > 1 and tmp[0] == '>' and tmp[1] == ' ':
            result += "#<void>\n"
            tmp = tmp[2:]
        last_expr += tmp
    else:
        last_expr += " "
        tmp = tmp.strip()
        last_expr += tmp

fwrite.write(result)

fread.close()
fwrite.close()

# os.system("rm -f tmp.out")

print("Have generated the output data.")