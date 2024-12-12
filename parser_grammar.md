# Parser Grammar Documentation

## Basic Program Structure

```
program → block
block   → { stmts }
stmts   → stmt stmts | ε
```

## Statement Rules

```
stmt → id = expr ;
     | if ( bool ) stmt
     | if ( bool ) stmt else stmt
     | while ( bool ) stmt
     | do stmt while ( bool )
     | break
     | block
```

## Boolean Expression Rules

```
bool → expr < expr
     | expr <= expr
     | expr > expr
     | expr >= expr
     | expr
```

## Expression Rules

```
expr → expr + term
     | expr - term
     | term
```

## Term and Factor Rules

```
term   → term * factor
       | term / factor
       | factor

factor → ( expr )
       | id
       | num
```

## Example Derivation Process

Source:

```
{
     i = 2;
     while (i <= 100){
          sum = sum + i;
          i = i + 2;
     }
}
```

Process:

```
program ⇒ block
       ⇒ { stmts }
       ⇒ { stmt stmts }
       ⇒ { id = expr ; stmts }
       ⇒ { id = num; stmts }
       ⇒ { id = num; stmt stmts }
       ⇒ { id = num; while (bool) stmt stmts }
       ⇒ { id = num; while (expr <= expr) stmt stmts }
       ⇒ { id = num; while (id <= expr) stmt stmts }
       ⇒ { id = num; while (id <= num) stmt stmts }
       ⇒ { id = num; while (id <= num) block stmts }
       ⇒ { id = num; while (id <= num) { stmts } stmts }
```

Note: This grammar defines a simple programming language with basic control structures including assignment, conditional statements, and loops. The language supports arithmetic expressions with standard operator precedence through its hierarchical structure of expressions, terms, and factors.
