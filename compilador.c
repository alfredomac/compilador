/*
Programa: Compilador Macutunda
Autor: Alfredo Macutunda
Data de Início: 24/08/2024
Última modificação: 12/09/2024 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 500

typedef struct Token {
    char tValor[MAX_LINHA];
    unsigned short tTipo;
} token;

FILE *arq;
token gerado;

char buffers[MAX_LINHA];
int linhAtual=0,colunaAtual=0;
int aux=0;
unsigned short estado=1,fim=0;


int abrirArquivo (char []);
void lerLinha ();
unsigned short delimitador(unsigned short);
token lexico (unsigned short); 

void limparString (char *cadeia){
    for(unsigned short i=0;i<strlen(gerado.tValor);i++){
        cadeia[i]=' ';
    }
}

unsigned short delimitador(unsigned short Ccaracter){
    // Os delimitadores são: espaço (32), nova linha, ponto e virgula
    switch (Ccaracter){
    case 0:
    case 13: 
    case 10 : // Nova linha
    case 32 : // Espaço
    case 61 : // Igual

        return 1;
        break;
    default:
        break;
    }


}

int abrirArquivo( char nomeArquivo[] ){
    arq = fopen(nomeArquivo,"r");
    if(arq == NULL){
        fclose(arq);
        return -1; // Erro na abertura do ficheiro
    }
}

void lerLinha (){
    linhAtual = 1;
    if (abrirArquivo ("programa.txt") !=-1 ){
        //while(fgets(buffers,MAX_LINHA,arq) != NULL){
            fgets(buffers,MAX_LINHA,arq);            
            linhAtual++;
        //}        
    }
}

void lerCaracter(){
    token t;
    unsigned short caracter;
    colunaAtual=0;
    estado=1;
    lerLinha();
    while(colunaAtual<=strlen(buffers) -1 ){
        caracter= buffers[colunaAtual];
        t = lexico(caracter);
        printf("%s - %d ", t.tValor, t.tTipo);
        colunaAtual++;
    }
}



token lexico(unsigned short Ccaracter){
    
    /* Essa função tem os seguintes retornos:
        1 - Continua  
        2 - Fim
        0 - Não reconheceu a palavra 
    */
   /*
   Tipos: 
   01 - Inteiro
   02 - Real
   03 - String
   04 - Op. Ari + 
   05 - Op. Ari -
   06 - Op. Ari *
   07 - Op. Ari /      
   08 - Op. Atr =
   09 - Op. Rel ==
   10 - Op. Rel != 
   11 - Op. Rel >
   12 - Op. Rel >=
   13 - Op. Rel <
   14 - Op. Rel <=
   15 - (
   16 - )
   17 - {
   18 - }
   19 - Espaço em branco
   20 - Tabulação
   21 - Nova Linha
   22 - Op. Log e
   23 - Op. Log ou
   24 - Op. Log nao
   25 - Identificador
   26 - inteiro
   27 - real
   28 - literal
   29 - logico
   30 - imprime
   31 - le
   32 - se
   33 - enquanto
   34 - para
   */
    
    char caracter = Ccaracter;
    
    switch (estado) {
        case 1:
            aux = 0;
            /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57 : // [0-9]
                estado = 2;
                gerado.tTipo = 1;
                break;
            case 34: // "
                estado = 5;
                gerado.tTipo = 3;
                break; 
            case 43: // +
                estado = 8;
                gerado.tTipo = 4;
                break;
            case 45: // -
                estado = 9;
                gerado.tTipo = 5;
                break;
            case 42: // *
                estado = 10;
                gerado.tTipo = 6;
                break;
            case 47: // / 
                estado = 11;
                gerado.tTipo = 7;
                break;
            case 61: // = 
                estado = 12;
                gerado.tTipo = 8;
                break;
            case 33: // !
                estado = 14;
                gerado.tTipo = 10;
                break;
            case 62: // >
                estado = 16;
                gerado.tTipo = 11;
                break;
            case 60: // <
                estado = 18;
                gerado.tTipo = 13;
                break;
            case 40: // (
                estado = 20;
                gerado.tTipo = 15;
                break;
            case 41: // )
                estado = 21;
                gerado.tTipo = 16;
                break;
            case 123: // {
                estado = 22;
                gerado.tTipo = 17;
                break;
            case 125: // }
                estado = 23;
                gerado.tTipo = 18;
                break;
            case 32: // (Espaço em Branco)
                estado = 24;
                gerado.tTipo = 19;
                break;
            case 9: // (Tabulação)
                estado = 25;
                gerado.tTipo = 20;
                break;
            case 10: // (Nova Linha)
                estado = 26;
                gerado.tTipo = 21;
                break;
            case 101: // e
                estado = 68;
                gerado.tTipo = 22;
                break;
            case 111: // o
                estado = 79;
                gerado.tTipo = 23;
                break;
            case 110: // n
                estado = 76;
                gerado.tTipo = 24;
                break;
            case 105: // i
                estado = 44;
                gerado.tTipo = 26;
                break;
            case 114: // r
                estado = 57;
                gerado.tTipo = 27;
                break;
            case 108: // l
                estado = 32;
                gerado.tTipo = 28;
                break;
            case 115: // s
                estado = 44;
                gerado.tTipo = 32;
                break;
            case 112: // p
                estado = 61;
                gerado.tTipo = 34;
                break;

            //Demais Caracteres e undercore (_)
            case 95: // _ (UnderCore)
            case 65 ... 90: // Letras Maiúsculas
            case 97 ... 100: // a-d
            case 102 ... 104: // f-g
            case 106 ... 107: // j-k
            case 109: // m
            case 113: // q
            case 116 ... 122: // t-z
                estado = 66;
                gerado.tTipo = 25;
                break;
            default:
                estado = 0;
                break;
            }
            

        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 2:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57 : // [0-9]
                estado = 2;
                gerado.tTipo = 1;
                break;
            case 46: // .
                estado = 3;
                gerado.tTipo = 3;
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

         case 3:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57 : // [0-9]
                estado = 2;
                gerado.tTipo = 3;
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/



    
    default:
        break;
    }

    if(gerado.tTipo!=0){
        gerado.tValor[aux] = caracter; 
    }

    return gerado;

}





int main(){
   lerCaracter();

}



