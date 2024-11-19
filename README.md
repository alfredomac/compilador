# Documentação

## 1. Introdução
Este documento espelha a documentação do compilador MACUTUNDA, um projeto individual da disciplina de compilador. O compilador será constituído por 3 partes: analisador léxico, analisador sintático e analisador semântico.  
- Analisador léxico: transforma as palavras em tokens;
- Analisador sintático: analisa a estrutura das expressões;
- Analisador semântico: analisa o sentido e a coerência das expressões.  
  
O compilador será feita e traduzido para linguagem C.

## 2. Analisador Léxico

O analisadr léxico é a parte do complicador que reconhece as palavras da linguagem, que por sua vez são designados tokens.

### 2.1 RF
1. Transformar as palavras em tokens.

### 2.2 Expressões regulares

De caracter a caracter, as expressões regulares expressam as regreas para formação de tokens. As palavras reconhecidas pela linguagem são: 
- Número inteiro: [0-9]+
- Número real: [0-9]+[.][0-9]+
- Identificador: [a-z A-Z _] [a-z A-Z  0-9 _]*
- Operadores Aritméticos: {“+”, “-”,”*”,”/”,”=”}
- Operadores Comparação: {“==”,”!=”,">",">=","<","<="}
- Operadores lógicos: {"nao","ou","e"}
- Palavras chave: {“se”,“enquanto”,”inteiro”,”real”, “literal”, "logico","le", "para","imprime"}

### 2.3 Autômato determinístico finito
Os autômatos permitem representar as expressões regulares e levar para mais próxima da implementação. 

### 2.4 Tabela de Transição

A tabela que transição espelha, cada caracter e transição de estados detalhadamente, que se torna ainda mais próximo da implementação.

## 3. Analisador Sintático

Analisador sintático é responsável pela forma são de frases (conjunto de palavra). Está sendo usado o analisador sintático recursivo.

### 3.1 RF
1. Reconhecer as frases

### 3.2 Gramática

A gramática, permite o estabelecimento das possíveis frases (estrutura) na linguagem. Está sendo usado analisador sintático recursivo, e eis a gramática:
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

## 4. Analisador Semântico
Analisador semântico é reponsável por denotar a coerência dentro da frases.

### 4.1 RF
1. Verificar a coerência das frases escritas.

### 4.2 Regras Semânticas

Atribuição  
- {id:inteiro, e:inteiro| id = e : inteiro}
- {id:real, e:inteiro| id = e : real}
- {id:real, e:real| id = e : real}
- {id:literal, e:literal| id = e : literal}

Atribuição Incrementral
- {id:inteiro| id++ : inteiro}
- {id:inteiro| id-- : inteiro}

Soma  
- {e1:inteiro, e2:inteiro| e1 + e2 : inteiro}
- {e1:real, e2:inteiro| e1 + e2 : real}
- {e1:inteiro, e2:real| e1 + e2 : real}
- {e1:real, e2:real| e1 + e2 : real}

Subtração  
- {e1:inteiro, e2:inteiro| e1 - e2 : inteiro}
- {e1:real, e2:inteiro| e1 - e2 : real}
- {e1:inteiro, e2:real| e1 - e2 : real}
- {e1:real, e2:real| e1 - e2 : real}

Multiplicação  
- {e1:inteiro, e2:inteiro| e1 * e2 : inteiro}
- {e1:real, e2:inteiro| e1 * e2 : real}
- {e1:inteiro, e2:real| e1 * e2 : real}
- {e1:real, e2:real| e1 * e2 : real}

Divisão  
- {e1:inteiro, e2:inteiro| e1 / e2 : real}
- {e1:real, e2:inteiro| e1 + e2 : real}
- {e1:inteiro, e2:real| e1 + e2 : real}
- {e1:real, e2:real| e1 + e2 : real}



### 4.3 Tabela Hash

Atribuição de valor incompatíveis é um caso do analisador semântico.

As variáveis terão no máximo 10 caracteres.
O código Hash é o número de caracteres. A tabela está sendo implementada atráves de uma vetor de 10 posições e uma lista candadeada para caso de um conjunto de ID's  terem o mesmo código Hash (número de caracteres).
