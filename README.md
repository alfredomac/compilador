# Documentação

## 1. Introdução
Este documento espelha a documentação do compilador MACUTUNDA, um projeto da disciplina de compilador. O compilador será constituído por 3 partes: analisador léxico, analisador sintático e analisador semântico.  
- Analisador léxico: transforma os gerado.tValors (palavras da linguagem original em token);
- Analisador sintático: analisa a estrutura das expressões;
- Analisador semântico: analisa o sentido e a compatibilidade das expressões.  
  
O compilador será feita e traduzido para linguagem C.

## 2. Analisador Léxico
### 2.1 RF
1. Ler caracter a partir de um ficheiro de texto
2. Transformar as palavras em tokens.

### 2.2 Expressões regulares

As expressões regulares foram usadas para expressar a regra de formação de caracteres.
- Número inteiro: [0-9]+
- Número real: [0-9]+[.][0-9]+
- Identificador: [a-z A-Z _] [a-z A-Z  0-9 _]*
- Operadores Aritméticos: {“+”, “-”,”*”,”/”,”=”}
- Operadores Comparação: {“==”,”!=”,">",">=","<","<="}
- Operadores lógicos: {"nao","ou","e"}
- Palavras chave: {“se”,“enquanto”,”inteiro”,”real”, “literal”, "logico","le", "para"}

### 2.3 Autômato determinístico finito
Os autômatos permitem expressar as expressões regulares e levar para mais próxima da implementação. Eles serão implementos a partir de uma tabela que fará correspondência entre os símbolos das classes envolvidas.

## 3. Analisador Sintático
### 3.1 RF
1. Reconhecer as frases escritas.

### 3
.2 Gramática

As expressões regulares foram usadas para expressar a regra de formação de caracteres.
- S -> CMDS   
- CMDS -> CMD {CMD}  
- CMD -> se (CONDICAO) '{' CMDS '}' [senao'{' CMDS '}']    
    | enquanto(CONDICAO)'{' CMDS '}'  
    | para(ATRIBUICAO,CONDICAO,ATRINCREMENTAL) '{' CMDS '}'  
    | le(id{,id})  
    | imprime(EXPRESSAO{,EXPRESSAO})  
    | ATRIBUICAO  
    | DECLARACAO  
- DECLARACAO -> real | inteiro | logico | literal id{,id}  
- ATRIBUICAO -> id = expressao  
- CONDICAO -> [nao] EXPCONDICIONAL {e | ou EXPCONDICIONAL}  
- EXPCONDICIONAL -> EXPRESSAO >| >= | < | <= | == | !=  EXPRESSAO  
- EXPRESSAO -> string| TERMO {+ | - TERMO}    
- TERMO -> FATOR {* | / FATOR}   
- FATOR -> '(' EXPRESSAO ')' | numero | id   

