# Documentação

## 1. Introdução
Este documento espelha a documentação do compilador MACUTUNDA, um projeto da disciplina de compilador. O compilador será constituído por 3 partes: analisador léxico, analisador sintático e analisador semântico.  
- Analisador léxico: transforma os lexemas (palavras da linguagem original em token);
- Analisador sintático: analisa a estrutura das expressões;
- Analisador semântico: analisa o sentido e a compatibilidade das expressões.  
  
O compilador será feita e traduzido para linguagem C.

## 2. Analisador Léxico
### 2.1 RF
1. Ler caracter a partir de um ficheiro de texto
2. Transformar os lexemas em tokens.

### 2.2 Expressões regulares

As expressões regulares foram usadas para expressar a regra de formação de caracteres.
- Número inteiro: [0-9]+
- Número real: [0-9]+[.][0-9]+
- Identificador: [a-z A-Z _] [a-z A-Z  0-9 _]*
- Operador Aritméticos: {“+”, “-”,”*”,”/”,”=”}
- Operador Comparação: {“==”,”!=”}
- Palavras chave: {“se”, “enquanto”,”inteiro”,”real”, “caracter”}

**OBS:** Delimitador de lexemas: espaço (32 na tabela) e nova linha (10)

### 2.3 Autômato finito
Os autômatos permitem expressar as expressões regulares e levar para mais próxima da implementação. Eles serão implementos a partir de uma tabela que fará correspondência entre os símbolos das classes envolvidas. 