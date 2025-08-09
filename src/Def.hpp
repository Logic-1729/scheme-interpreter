#ifndef DEF_HPP
#define DEF_HPP

/**
 * @file Def.hpp
 * @brief Core definitions and enumerations for the Scheme interpreter
 * @author luke36
 * 
 * This file contains essential type definitions, enumerations, and forward
 * declarations used throughout the Scheme interpreter implementation.
 */

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <map>

// Forward declarations
struct Syntax;
struct Expr;
struct Value;
struct AssocList;
struct Assoc;

/**
 * @brief Expression types enumeration
 * 
 * Defines all possible expression types that can be parsed and evaluated
 * in the Scheme interpreter.
 */
enum ExprType {
    // Control structures
    E_LET,              ///< Let binding expression
    E_LAMBDA,           ///< Lambda (function) expression
    E_APPLY,            ///< Function application
    E_LETREC,           ///< Recursive let binding
    E_IF,               ///< Conditional expression
    E_BEGIN,            ///< Sequential execution
    E_COND,             ///< Multi-way conditional
    E_AND,              ///< Logical AND
    E_OR,               ///< Logical OR
    
    // Basic types and literals
    E_VAR,              ///< Variable reference
    E_FIXNUM,           ///< Integer literal
    E_STRING,           ///< String literal
    E_TRUE,             ///< Boolean true literal
    E_FALSE,            ///< Boolean false literal
    E_QUOTE,            ///< Quote expression
    E_VOID,             ///< Void value
    
    // Arithmetic operations
    E_PLUS,             ///< Addition
    E_MINUS,            ///< Subtraction
    E_MUL,              ///< Multiplication
    E_DIV,              ///< Division
    E_QUOTIENT,         ///< Integer division
    E_MODULO,           ///< Modulo operation
    E_EXPT,             ///< Exponentiation
    
    // Comparison operations
    E_LT,               ///< Less than
    E_LE,               ///< Less than or equal
    E_EQ,               ///< Equal
    E_GE,               ///< Greater than or equal
    E_GT,               ///< Greater than
    
    // List operations
    E_CONS,             ///< Cons (pair construction)
    E_CAR,              ///< Get first element of pair
    E_CDR,              ///< Get second element of pair
    E_LIST,             ///< Create list
    E_SETCAR,           ///< Set first element of pair
    E_SETCDR,           ///< Set second element of pair
    
    // Type predicates
    E_EQQ,              ///< eq? predicate
    E_BOOLQ,            ///< boolean? predicate
    E_INTQ,             ///< integer? predicate (also number?)
    E_NULLQ,            ///< null? predicate
    E_PAIRQ,            ///< pair? predicate
    E_PROCQ,            ///< procedure? predicate
    E_SYMBOLQ,          ///< symbol? predicate
    E_LISTQ,            ///< list? predicate
    E_NUMBERQ,          ///< number? predicate
    E_STRINGQ,          ///< string? predicate
    
    // Other operations
    E_NOT,              ///< Logical NOT
    E_DEFINE,           ///< Variable/function definition
    E_SET,              ///< Variable assignment
    E_DISPLAY,          ///< Display output
    E_EXIT              ///< Exit interpreter
};

/**
 * @brief Value types enumeration
 * 
 * Defines all possible value types that can be represented and manipulated
 * in the Scheme interpreter runtime.
 */
enum ValueType {
    V_INT,              ///< Integer value
    V_RATIONAL,         ///< Rational number (currently unused)
    V_BOOL,             ///< Boolean value
    V_SYM,              ///< Symbol value
    V_NULL,             ///< Null/empty list
    V_STRING,           ///< String value
    V_PAIR,             ///< Pair/cons cell
    V_PROC,             ///< Procedure/function
    V_VOID,             ///< Void value
    V_PRIMITIVE,        ///< Built-in primitive function
    V_TERMINATE         ///< Termination signal
};

#endif // DEF_HPP
