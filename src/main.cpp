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
    // read - evaluation - print loop with define grouping
    Assoc global_env = empty();
    std::vector<std::pair<std::string, Expr>> pending_defines;
    
    while (1){
        #ifndef ONLINE_JUDGE
            std::cout << "scm> ";
        #endif
        Syntax stx = readSyntax(std::cin); // read
        try{
            Expr expr = stx->parse(global_env); // parse
            
            // 检查是否是 define 表达式
            Define* define_expr = dynamic_cast<Define*>(expr.get());
            if (define_expr != nullptr) {
                // 收集 define 表达式
                pending_defines.push_back({define_expr->var, define_expr->e});
                // 不立即求值，继续读取下一个表达式
                continue;
            } else {
                // 不是 define 表达式
                // 如果有待处理的 define，先批量处理它们
                if (!pending_defines.empty()) {
                    evaluateDefineGroup(pending_defines, global_env);
                    pending_defines.clear();
                }
                
                // 处理当前的非 define 表达式
                Value val = expr->eval(global_env);
                if (val->v_type == V_TERMINATE)
                    break;
                
                // 检查是否是 set!, set-car!, set-cdr! 表达式 - 这些不显示返回值
                Set* set_expr = dynamic_cast<Set*>(expr.get());
                
                // 检查是否是 set-car!, set-cdr!, display 的函数调用
                Apply* apply_expr = dynamic_cast<Apply*>(expr.get());
                bool is_void_returning_func = false;
                if (apply_expr != nullptr) {
                    Var* var_expr = dynamic_cast<Var*>(apply_expr->rator.get());
                    if (var_expr != nullptr) {
                        if (var_expr->x == "set-car!" || var_expr->x == "set-cdr!" || var_expr->x == "display") {
                            is_void_returning_func = true;
                        }
                    }
                }
                
                if (set_expr != nullptr || is_void_returning_func) {
                    // set!, set-car!, set-cdr!, display 表达式不显示返回值，只显示空行
                    puts("");
                } else {
                    // 其他表达式显示结果
                    val->show(std::cout);
                    puts("");
                }
            }
        }
        catch (const RuntimeError &RE){
            // 如果出错，清空待处理的 define
            pending_defines.clear();
            std::cout << "RuntimeError";
            puts("");
        }
    }
    
    // 如果程序结束时还有待处理的 define，处理它们
    if (!pending_defines.empty()) {
        try {
            evaluateDefineGroup(pending_defines, global_env);
        } catch (const RuntimeError &RE) {
            std::cout << "RuntimeError in final defines";
            puts("");
        }
    }
}


int main(int argc, char *argv[]) {
    REPL();
    return 0;
}
