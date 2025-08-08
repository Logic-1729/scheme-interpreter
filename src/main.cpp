#include "Def.hpp"
#include "syntax.hpp"
#include "expr.hpp"
#include "value.hpp"
#include "RE.hpp"
#include <sstream>
#include <iostream>
#include <map>


extern std::map<std::string, ExprType> primitives;
extern std::map<std::string, ExprType> reserved_words;

void REPL(){
    // read - evaluation - print loop
    Assoc global_env = empty();
    while (1){
        #ifndef ONLINE_JUDGE
            std::cout << "scm> ";
        #endif
        Syntax stx = readSyntax(std::cin); // read
        try{
            Expr expr = stx->parse(global_env); // parse
            // stx->show(std::cout); // syntax print
            Value val = expr->eval(global_env);  // 注意：global_env 会被 define 修改
            if (val->v_type == V_TERMINATE)
                break;
            // 检查是否是显式的void调用或其他应该显示的情况
            bool should_display = true;
            if (val->v_type == V_VOID) {
                // 如果是Define表达式产生的void，不显示
                if (dynamic_cast<Define*>(expr.get()) != nullptr) {
                    should_display = false;
                }
            }
            if (should_display) {
                val->show(std::cout); // value print
                puts(""); // 换行
            }
        }
        catch (const RuntimeError &RE){
            // std::cout << RE.message();
            std::cout << "RuntimeError";
            puts(""); // 错误信息后仍然需要换行
        }
    }
}


int main(int argc, char *argv[]) {
    REPL();
    return 0;
}
