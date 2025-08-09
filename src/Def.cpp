/**
 * @file Def.cpp
 * @brief Implementation of primitive functions and reserved words mappings
 * @author luke36
 * 
 * This file defines the mapping tables that associate Scheme function names
 * and special forms with their corresponding internal expression types.
 */

#include "Def.hpp"

/**
 * @brief Mapping of primitive function names to expression types
 * 
 * This map contains all built-in functions that can be called in Scheme.
 * These are functions that have direct implementations in the interpreter
 * and can be used in function application contexts.
 * 
 * Categories:
 * - Arithmetic: +, -, *, /, quotient, modulo, expt
 * - Comparison: <, <=, =, >=, >
 * - Type predicates: eq?, boolean?, number?, null?, pair?, procedure?, symbol?, list?, string?
 * - List operations: cons, car, cdr, list, set-car!, set-cdr!
 * - Logic: not
 * - I/O: display
 * - Control: void, exit
 */
std::map<std::string, ExprType> primitives = {
    // Arithmetic operations
    {"+",        E_PLUS},
    {"-",        E_MINUS},
    {"*",        E_MUL},
    {"/",        E_DIV},
    {"quotient", E_QUOTIENT},
    {"modulo",   E_MODULO},
    {"expt",     E_EXPT},
    
    // Comparison operations
    {"<",        E_LT},
    {"<=",       E_LE},
    {"=",        E_EQ},
    {">=",       E_GE},
    {">",        E_GT},
    
    // Type predicates
    {"eq?",        E_EQQ},
    {"boolean?",   E_BOOLQ},
    {"number?",    E_INTQ},      // Note: Also handles integer? in some contexts
    {"null?",      E_NULLQ},
    {"pair?",      E_PAIRQ},
    {"procedure?", E_PROCQ},
    {"symbol?",    E_SYMBOLQ},
    {"list?",      E_LISTQ},
    {"string?",    E_STRINGQ},
    
    // List operations
    {"cons",      E_CONS},
    {"car",       E_CAR},
    {"cdr",       E_CDR},
    {"list",      E_LIST},
    {"set-car!",  E_SETCAR},
    {"set-cdr!",  E_SETCDR},
    
    // Logic operations
    {"not",       E_NOT},
    
    // I/O operations
    {"display",   E_DISPLAY},
    
    // Special values and control
    {"void",      E_VOID},
    {"exit",      E_EXIT}
};

/**
 * @brief Mapping of reserved words (special forms) to expression types
 * 
 * This map contains Scheme special forms that have special syntax and
 * evaluation rules. These cannot be used as regular function names and
 * have special parsing and evaluation semantics.
 * 
 * Categories:
 * - Binding: let, letrec, define
 * - Control flow: if, begin, cond, and, or
 * - Functions: lambda
 * - Data: quote
 * - Assignment: set!
 */
std::map<std::string, ExprType> reserved_words = {
    // Binding constructs
    {"let",     E_LET},
    {"letrec",  E_LETREC},
    {"define",  E_DEFINE},
    
    // Control flow
    {"if",      E_IF},
    {"begin",   E_BEGIN},
    {"cond",    E_COND},
    {"and",     E_AND},
    {"or",      E_OR},
    
    // Function definition
    {"lambda",  E_LAMBDA},
    
    // Data construction
    {"quote",   E_QUOTE},
    
    // Assignment
    {"set!",    E_SET}
};
