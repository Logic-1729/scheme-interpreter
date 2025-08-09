from cyaron import *
import os
import argparse
import re

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


primitive_ii2i = ["+", "-", "*"]
primitive_ii2b = ["<", "<=", "=", ">=", ">"]
primitive_2v = ["void"]
primitive_exit = ["exit"]
primitive_aa2p = ["cons"]
primitive_p2a = ["car", "cdr"]
primitive_a2b = []
primitive_aa2b = []

def gen_fixnum(depth=0):
    return randint(-2147483648, 2147483648)
def is_in_range(integer):
    return integer <= 2147483647 and integer >= -2147483648

first_charset = "abcdefghijklmnopqrstuvwxyz" + "ABCDEFGHIJKLMNOPQRSTUVWXYZ" + "?!+-*/<=>:$%&_~"
aux_charset = first_charset + ".@"

def is_number(s):
    pattern = r"^[-+]?\d*\.?\d+$"
    return bool(re.match(pattern, s))

def gen_symbol_str():
    index = randint(0, 10)
    if index == 0:
        return choice(primitive_ii2i)
    elif index == 1:
        return choice(primitive_ii2b)
    elif index == 2:
        return choice(primitive_2v)
    elif index == 3:
        return choice(primitive_aa2p)
    elif index == 4:
        return choice(primitive_p2a)
    elif index == 5:
        return choice(primitive_exit)
    elif index == 6:
        pass
    elif index == 7:
        pass

    str = String.random(1, charset=first_charset) + String.random((0, 4), charset=aux_charset)
    while is_number(str):
        str = String.random(1, charset=first_charset) + String.random((0, 4), charset=aux_charset)
    return str
def gen_symbol(depth=0):
    str = gen_symbol_str()
    return f"(quote {str})"

def gen_null(depth=0):
    return "(quote ())"
def gen_void(depth=0):
    return "(void)"

def gen_integer_(depth=0):
    if depth > max_dep or randint(0, 9) == 0:
        integer = gen_fixnum()
        if task_id == 1:
            return str(integer), integer
        return choice([str(integer), f"(quote {str(integer)})"]), integer

    op = choice(primitive_ii2i)
    left_str, left_val = gen_integer_(depth + 1)
    right_str, right_val = gen_integer_(depth + 1)
    if op == "*":
        val = left_val * right_val
        if not is_in_range(val):
            return left_str, left_val
        else:
            return f"(* {left_str} {right_str})", val
    if op == "+":
        val = left_val + right_val
        if not is_in_range(val):
            return left_str, left_val
        else:
            return f"(+ {left_str} {right_str})", val
    if op == "-":
        val = left_val - right_val
        if not is_in_range(val):
            return left_str, left_val
        else:
            return f"(- {left_str} {right_str})", val
        
def gen_integer(depth=0):
    expr, val = gen_integer_(depth=depth)
    if task_id == 1:
        return expr
    wrap = randint(0, 29)
    if wrap == 0:
        return f"(car {gen_pair(depth + 1, left=expr)})"
    elif wrap == 1:
        return f"(car {gen_list(depth + 1, len=randint(1, max_dep // 2), first=expr)})"
    elif wrap == 2:
        return f"(cdr {gen_pair(depth + 1, right=expr)})"
    return expr
        
def gen_boolean(depth=0):
    wrap = randint(0, 19)
    if wrap == 0:
        return f"(car {gen_pair(depth + 1, left=gen_boolean(depth + 1))})"
    elif wrap == 1:
        return f"(car {gen_list(depth + 1, len=randint(1, max_dep // 2), first=gen_boolean(depth + 1))})"
    elif wrap == 2:
        return f"(cdr {gen_pair(depth + 1)})"
    index = randint(0, [0, 0, 1][task_id])
    if index == 0 or depth > max_dep:
        return choice(["#t", "(quote #t)", "#f", "(quote #f)"])
    elif index == 1:
        primitive = choice(primitive_ii2b)
        left = gen_integer(depth + 1)
        right = gen_integer(depth + 1)
        return f"({primitive} {left} {right})"
    elif index == 2:
        pass
    elif index == 3:
        pass
    elif index == 4:
        pass

def gen_list(depth=0, len=0, first=None):
    if len == 0:
        return gen_null()
    index = randint(0, 1)
    if first == None:
        first = gen_expr(depth + 1)
    else:
        index = 0
    if index == 0:
        return f"(cons {first} {gen_list(depth=depth, len=len - 1, first=gen_expr(depth + 1))})"
    else:
        expr = f"(quote {first}"
        for i in range(len - 1):
            expr += f" {gen_expr(depth + 1)}"
        expr += ")"
        return expr

def gen_pair(depth=0, left=None, right=None):
    index = randint(0, 1)
    if left == None:
        left = gen_expr(depth + 1)
    else:
        index = 0
    if right == None:
        right = gen_expr(depth + 1)
    else:
        index = 0
    if index == 0:
        return f"(cons {left} {right})"
    else:
        return f"(quote ({left} . {right}))"

def gen_if(depth=0, left=None, right=None):
    index = randint(0, 1)
    
        
def gen_expr(depth):
    if randint(0, 20) == 0:
        return gen_null()
    if randint(0, 20) == 0:
        return gen_void()
    index = randint(0, [0, 0, 3][task_id] if depth <= max_dep else 1)
    if index == 0:
        return gen_integer(depth)
    elif index == 1:
        return gen_boolean(depth)
    elif index == 2:
        return gen_pair(depth=depth)
    elif index == 3:
        return gen_symbol(depth=0)

def gen_RE_integer(depth=0):
    index = randint(0, [0, 0][task_id] if depth <= max_dep else 0)
    if index == 0:
        len = choice([0, 1, 3, 4, 5])
        if depth > max_dep:
            len = 0
        expr = "(" + String.random(1, charset="+-*")
        is_all_integer = randint(0, 1)
        for i in range(len):
            expr += f" {gen_RE_expr(depth + 1) if is_all_integer else gen_integer(depth=depth + 1)}"
        expr += ")"
        return expr

def gen_RE_exit(depth=0):
    len = randint(1, 3)
    expr = "(exit "
    for i in range(len):
        expr += gen_RE_expr(depth + 1) + " "
    expr += ")"
    return expr

def gen_RE_expr(depth=0):
    if randint(0, 100) == 0:
        return gen_RE_exit(depth + 1)
    index = randint(0, [0, 0][task_id] if depth <= max_dep else 0)
    if index == 0:
        return gen_RE_integer(depth)
    

test_data = IO(data_id=data_id, file_prefix="")

if is_RE == 0:
    for i in range(expr_num):
        test_data.input_writeln(gen_expr(depth=0))
    test_data.input_writeln("(exit)")
    print("Have generated the input data.")
else:
    for i in range(expr_num):
        test_data.input_writeln(gen_RE_expr(depth=0))
        test_data.output_writeln("RuntimeError")
    test_data.input_writeln("(exit)")
    print("Have generated RE data.")