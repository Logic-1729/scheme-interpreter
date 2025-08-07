#include "Def.hpp"

// 直接在定义时初始化，无需额外的初始化函数
std::map<std::string, ExprType> primitives = {
    {"*", E_MUL}, {"-", E_MINUS}, {"+", E_PLUS},
    {"<", E_LT}, {"<=", E_LE}, {"=", E_EQ}, {">=", E_GE}, {">", E_GT},
    {"void", E_VOID}, {"eq?", E_EQQ}, {"boolean?", E_BOOLQ},
    {"fixnum?", E_INTQ}, {"null?", E_NULLQ}, {"pair?", E_PAIRQ},
    {"procedure?", E_PROCQ}, {"symbol?", E_SYMBOLQ}, {"cons", E_CONS},
    {"not", E_NOT}, {"car", E_CAR}, {"cdr", E_CDR}, {"exit", E_EXIT}
};

std::map<std::string, ExprType> reserved_words = {
    {"let", E_LET}, {"lambda", E_LAMBDA}, {"letrec", E_LETREC},
    {"if", E_IF}, {"begin", E_BEGIN}, {"quote", E_QUOTE}
};
