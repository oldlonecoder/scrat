
# scrat :  { Scripting Runtime Application Terminology }

---

My personal c++ not modern way to experiment coding a machineless scripting interpreter.

# List of main component and classes:
---
- <h5>scrat::Alu</h5> Arithmetic Logical Unit or Accumulator, component of IONode.
---
 - <h5>scrat::Ion</h5> Instruction Object Node.
---
- <h5>scrat::Stack</h5> Scoped Ion List".
---
- <h5>scrat::TokenData</h5> Token data struct: type, source-location, operations, and mnemonic informations".
---
- <h5>scrat::Types</h5> Types and enum declarations for the Ion env: scrat::Type; scrat::Mnemonic; scrat::Lexem; scrat::Distance.
---
- <h5>scrat::Amu</h5> Sibbling Script Module Unit within the Interpreter's Stack/(global)scope.
---
- <h5>scrat::Variable</h5> scrat Ion<-Variables holding primitive types and reference to runtime data and functions.
--- 
- <h5>scrat::Lexer</h5>  The scrat lexical analyser and pre-arithmetic syntax parser; TokenData stream producer.
---
- <h5>scrat::Compiler</h5> The scrat Ion Ast producer.
--- 
- <h5>scrat::Lang::Grammar</h5>  Testing, Exploring, Expirementing  Scrat (language) Grammar in a unique and particular non-std way.
---
- <h5>scrat::Interpreter</h5>  The scrat Interpreter ...
- ...
