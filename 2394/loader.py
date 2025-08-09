import argparse

parser = argparse.ArgumentParser(description="Input data generator")

parser.add_argument('task_id', type=int, help="the index of the subtask")
parser.add_argument('data_id', type=int, help="the index of the data")
parser.add_argument('expr_num', type=int, help="the number of expression")
parser.add_argument('max_dep', type=int, help="the max depth of the expression")
parser.add_argument('is_RE', type=int, help="whether generating RE cases or not")

args = parser.parse_args()

task_id = args.task_id
data_id = args.data_id
expr_num = args.expr_num
max_dep = args.max_dep
is_RE = args.is_RE

import os

if is_RE:
    os.system(f"python3 ./input_generator.py {task_id} {data_id} {expr_num} {max_dep} 1")
else:
    os.system(f"python3 ./input_generator.py {task_id} {data_id} {expr_num} {max_dep} 0")
    os.system(f"python3 ./output_generator.py {data_id}")