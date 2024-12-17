# scheme-interpreter
第一次大版本写完再2024.12.13,但是懒得传
update time:2024-12-16 18:18:40
自认为能过subtask1-4，上届的测试点没过的是：82 100 102 103 106 109 110 112 113 114 115 extratest 7

第二次大版本修改是发现quote写的一坨答辩，以下例子过不了一点：
update time:2024-12-16 22:01:13

    ( quote ( quote ( 1 2 ) ))
    (car (quote ( (1 . 2 ) 3 . 4)))
    (cdr (quote ( (1 . 2 ) 3 . 4)))
    ( quote ( quote () 1 2 . ( 4 . 2 )))
    (car ( quote ( quote () 1 2 . ( 4 . 2 ))))
    (cdr ( quote ( quote () 1 2 . ( 4 . 2 ))))
    
我本来写得是

    if (dynamic_cast<TrueSyntax*>(s.get())) return BooleanV(true);
    else if (dynamic_cast<FalseSyntax*>(s.get())) return BooleanV(false);
    else if (dynamic_cast<Number*>(s.get())) return IntegerV(dynamic_cast<Number*>(s.get())->n);
    else if (dynamic_cast<Identifier*>(s.get())) return SymbolV(dynamic_cast<Identifier*>(s.get())->s);
    else if (dynamic_cast<List*>(s.get())) {
        std::vector<Syntax> stxs_got = dynamic_cast<List*>(s.get())->stxs;
        if (stxs_got.size() == 0) return NullV();
        Value now_pair = NullV();
        for (int i = stxs_got.size() - 1; i >= 0; i--) {
            now_pair = Value(new Pair(stxs_got[i]->parse(e)->eval(e), now_pair));
        }
        return now_pair;
    }
    else throw(RuntimeError("Unknown quoted typename"));
    
但这个发现太简单了，于是GG

第三次大版本更新是发现eq?有两种情况没特判GG，，以下例子过不了一点：
update time:2024-12-16 23:50:25

    (quote ( if 1 ))
    (null? (cdr (cdr (quote (1 2)))))
    (eq? (void) (void))
    (eq? (quote ()) (quote ()))

第四次大版本更新是发现我总算看懂了wxngg要表达的意思，primitive全部解释成绑定了环境的变量，而且支持将其解读成保留字，以下例子过不了一点：
update time:2024-12-17 11:32:12

    (let ([+ 1]) +)
    ((if #t + -) 1 2)
    (let ([+ -]) (+ 2 1))

现在过不了的测试点是：82 100 102 103 106 109 110 112 113 114 115 extratest 7
    
