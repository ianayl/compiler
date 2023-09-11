# Grammar

```ebnf
Expr ::= Add
Add  ::= Mul Addp
Addp ::= '+' Mul Addp
     |   '-' Mul Addp
     |   ε
Mul  ::= Exp Mulp
Mulp ::= '*' Exp Mulp
     |   '/' Exp Mulp
     |   ε
Exp  ::= Term Expp
Expp ::= '^' Term Expp
     |   ε
Term ::= ( Expr )
     |   '-' Term
     |   <id>
     |   <num_lit>
```
