/*
Programa: Compilador Macutunda
Autor: Alfredo Macutunda
Data de Início: 24/08/2024
Última modificação: 03/10/2024 
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
unsigned short estado=1,fim=0,aux=0;


int abrirArquivo (char []);
void lerLinha ();
unsigned short delimitador(unsigned short);
token lexico (unsigned short); 

void limparString (char* cadeia){
    for(unsigned short i=0;i<strlen(cadeia);i++){
        cadeia[i] =' ';
    }
}

void gerarIdentificador(){
    estado = 67;
    gerado.tTipo=25;
    fim=1;
}

void alfaNumerico(unsigned short Ccaracter){
    switch (Ccaracter){
        case 48 ... 57: //   [0-9]
        case 65 ... 90: //   [A-Z]
        case 97 ... 104: //  [a-h]
        case 106 ... 122: // [j-z]
            gerarIdentificador();
            break;
        default:
            estado=0;
        break;
    }
}

void transicao(unsigned short est, unsigned short ger, unsigned short f){
    estado = est;
    gerado.tTipo = ger;
    fim = f;

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
    if (abrirArquivo ("programa.txt") !=-1 ){
        //while(fgets(buffers,MAX_LINHA,arq) != NULL){
            fgets(buffers,MAX_LINHA,arq);   
            aux = 0;
            estado = 1; 
            limparString(gerado.tValor);
            linhAtual++;
        //}        
    }
}

void lerCaracter(){
    token t;
    unsigned short cCaracter;
    unsigned short cAux=0; //Guarda posição do erro
    char* cadVazio = "";
    colunaAtual=0;
    estado=1;
    lerLinha();
    while(colunaAtual<=strlen(buffers)-1 ){
        cCaracter= buffers[colunaAtual];
        t = lexico(cCaracter);
        if(estado==0){
            printf("<%s,%d> ", gerado.tValor, gerado.tTipo);
            limparString(gerado.tValor);
            //strcpy(gerado.tValor,cadVazio);
            if(fim==1){
                aux = 0;
                estado = 1;
                t = lexico(cCaracter);
            } else{
                estado==0; // Erro
                colunaAtual = strlen(buffers); // Sair do Loop
            }
        }
        cAux++;
        colunaAtual++;
    }
    if(fim==1){
    //if(estado!=0){
        printf("<%s,%d> ", gerado.tValor, gerado.tTipo);
    }else{
        printf("Erro na Ln %d Col %d",linhAtual, cAux-1);
    }
    printf("\n");
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
   33 - senao
   34 - enquanto
   35 - para
   */
    
    char caracter = Ccaracter;
    switch (estado) {
        case 1:
            aux = 0;
            fim = 1;
            /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57 : // [0-9]
                transicao(2,1,fim);
                break;
            case 34: // "
                transicao(5,3,fim);
                fim = 0;
                break; 
            case 43: // +
                transicao(8,4,fim);
                break;
            case 45: // -
                transicao(9,5,fim);
                break;
            case 42: // *
                transicao(10,6,fim);
                break;
            case 47: // / 
                transicao(11,7,fim);
                break;
            case 61: // = 
                transicao(12,8,fim);
                break;
            case 33: // !
                fim = 0;
                transicao(2,1,fim);
                break;
            case 62: // >
                transicao(16,11,fim);
                break;
            case 60: // <
                transicao(18,13,fim);
                break;
            case 40: // (
                transicao(20,15,fim);
                break;
            case 41: // )
                transicao(20,16,fim);
                break;
            case 123: // {
                transicao(22,17,fim);
                break;
            case 125: // }
                transicao(23,18,fim);
                break;
            case 32: // (Espaço em Branco)
                transicao(24,19,fim);
                break;
            case 9: // (Tabulação)
                transicao(25,20,fim);
                break;
            case 10: // (Nova Linha)
                transicao(26,21,fim);
                break;
            case 101: // e
                transicao(68,22,fim);
                break;
            case 111: // o
                transicao(79,25,fim);
                break;
            case 110: // n
                transicao(76,25,fim);
                break;
            case 105: // i
                transicao(44,25,fim);
                break;
            case 114: // r
                transicao(57,25,fim);
                break;
            case 108: // l
                transicao(32,25,fim);
                break;
            case 115: // s
                transicao(27,25,fim);
                break;
            case 112: // p
                transicao(61,25,fim);
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
                transicao(66,25,fim);
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
                transicao(2,1,1);
                break;
            case 46: // .
                transicao(3,2,0);
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
                transicao(4,2,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 4:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57 : // [0-9]
                transicao(4,2,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 5:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 32 ... 33: 
            case 35 ... 126: // Todos caracteres excepto os de controle
                transicao(6,3,0);
                break;
            case 34: // "
                transicao(7,3,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 6:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 32 ... 33: 
            case 35 ... 126: // Todos caracteres excepto os de controle
                transicao(6,3,0);
                break;
            case 34: // "
                transicao(7,3,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 12:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 61 : // =
                transicao(13,9,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 14:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 61 : // =
                transicao(15,10,1);
                fim=1;
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 16:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 61 : // =
                transicao(17,12,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 18:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 61 : // =
                transicao(19,14,1);
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 27:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(28,32,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 122: // [f-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 28:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 110 : // n
                transicao(29,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 109: //  [a-m]
            case 111 ... 122: // [o-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 29:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(30,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 122: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 30:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 111 : // o
                transicao(31,33,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 110: //  [a-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 31:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 32:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101: // e
                transicao(65,31,1);
                break;
            case 105: // i
                transicao(38,25,1);
                break;
            case 111 : // o
                transicao(33,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 104: // [f-h]
            case 106 ... 110: // [j-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 33:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 103 : // g
                transicao(34,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 102: //  [a-f]
            case 104 ... 122: // [h-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 34:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 105 : // i
                transicao(35,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 104: //  [a-h]
            case 106 ... 122: // [j-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 35:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 99 : // c
                transicao(36,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 98: //   [a-b]
            case 100 ... 122: // [d-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 36:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 111 : // o
                transicao(37,29,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 110: //  [a-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 37:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 38:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 116 : // t
                transicao(39,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 115: //  [a-s]
            case 117 ... 122: // [u-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 39:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(40,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 122: // [f-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 40:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 114 : // r
                transicao(30,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 113: //  [a-q]
            case 115 ... 122: // [s-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 41:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(42,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 122: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 42:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 108 : // l
                transicao(43,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 107: //  [a-k]
            case 109 ... 122: // [m-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 43:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 44:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 109 : // m
                transicao(45,25,1);
                break;
            case 110: // n
                transicao(51,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 108: //  [a-l]
            case 111 ... 122: // [o-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 45:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 112 : // p
                transicao(46,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 111: //  [a-o]
            case 113 ... 122: // [q-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 46:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 114 : // r
                transicao(47,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 113: //  [a-q]
            case 115 ... 122: // [s-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 47:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 105 : // i
                transicao(48,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 104: //  [a-h]
            case 106 ... 122: // [j-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 48:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 109 : // m
                transicao(49,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 108: //  [a-l]
            case 110 ... 122: // [n-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 49:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(50,30,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 122: // [f-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 50:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 51:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 116 : // t
                transicao(52,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 115: //  [a-s]
            case 117 ... 122: // [u-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 52:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(53,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 122: // [f-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 53:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 105 : // i
                transicao(54,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 104: //  [a-h]
            case 106 ... 122: // [j-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 54:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 114 : // r
                transicao(55,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 113: //  [a-q]
            case 115 ... 122: // [s-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 55:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 111 : // o
                transicao(56,26,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 110: //  [a-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 56:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 57:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(58,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 100: //  [a-d]
            case 102 ... 122: // [f-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 58:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(59,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 122: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 59:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 108 : // l
                transicao(56,27,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 107: //  [a-k]
            case 109 ... 122: // [m-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 60:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-q]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 61:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(62,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 113: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 62:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 114 : // r
                transicao(63,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 113: //  [a-q]
            case 115 ... 122: // [s-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 63:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(62,35,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 113: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 64:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 65:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 66:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-q]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 67:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 68:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 110 : // n
                transicao(62,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 109: //  [a-m]
            case 111 ... 122: // [o-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

         case 69:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 113 : // q
                transicao(70,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 112: //  [a-p]
            case 114 ... 122: // [r-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

         case 70:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 117 : // u
                transicao(71,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 116: //  [a-t]
            case 118 ... 122: // [v-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 71:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(72,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 122: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 72:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 110 : // n
                transicao(73,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 109: //  [a-m]
            case 111 ... 122: // [o-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 73:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 116 : // t
                transicao(74,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 115: //  [a-s]
            case 117 ... 122: // [u-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 74:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 111 : // o
                transicao(75,34,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 110: //  [a-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 75:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 76:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(77,25,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 98 ... 122: //  [b-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 77:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 111 : // o
                transicao(78,24,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 110: //  [a-n]
            case 112 ... 122: // [p-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 78:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 79:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 117 : // u
                transicao(80,23,1);
                break;
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 116: //  [a-t]
            case 118 ... 122: // [v-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 80:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 48 ... 57: //   [0-9]
            case 65 ... 90: //   [A-Z]
            case 97 ... 122: //  [a-z]
                gerarIdentificador();
                break;
            default:
                estado=0;
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/



    
    default:
        estado = 0;
        break;
    }

    if(estado!=0){
        gerado.tValor[aux] = caracter; 
        aux++;
    }

    return gerado;

}





int main(){
   lerCaracter();

}



