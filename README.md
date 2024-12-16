# scheme-interpreter
第一次大版本修改在2024.12.13, first upload in 2024.12.16
--completing subtask 1-4, and simple letrec
which wrong the following testpoint: 82 100 102 103 106 109 110 112 113 114 115 extratest 7

第二次大版本修改是发现quote写的一坨答辩，以下例子过不了一点：

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
    
    但这个发现太简单了
