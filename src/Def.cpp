#include "Def.hpp"

std :: map<std :: string, ExprType> primitives;
std :: map<std :: string, ExprType> reserved_words;

void initPrimitives()
{
    // primitives stores all procedures in library, mapping them to ExprTypes
    primitives["*"] = E_MUL;
    primitives["-"] = E_MINUS;
    primitives["+"] = E_PLUS;
    primitives["<"] = E_LT;
    primitives["<="] = E_LE;
    primitives["="] = E_EQ;
    primitives[">="] = E_GE;
    primitives[">"] = E_GT;
    primitives["void"] = E_VOID;
    primitives["eq?"] = E_EQQ;
    primitives["boolean?"] = E_BOOLQ;
    primitives["fixnum?"] = E_INTQ;
    primitives["null?"] = E_NULLQ;
    primitives["pair?"] = E_PAIRQ;
    primitives["procedure?"] = E_PROCQ;
    primitives["symbol?"] = E_SYMBOLQ;
    primitives["cons"] = E_CONS;
    primitives["not"] = E_NOT;
    primitives["car"] = E_CAR;
    primitives["cdr"] = E_CDR;
    primitives["exit"] = E_EXIT;
}

void initReservedWords()
{
    // reserved_words stores all reserved words, mapping them to bools
    reserved_words["let"] = E_LET;
    reserved_words["lambda"] = E_LAMBDA;
    reserved_words["letrec"] = E_LETREC;
    reserved_words["if"] = E_IF;
    reserved_words["begin"] = E_BEGIN;
    reserved_words["quote"] = E_QUOTE;
}
