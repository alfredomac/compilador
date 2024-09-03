
/*
Programa: Compilador Macutunda
Autor: Alfredo Macutunda
Data de Início: 24/08/2024
Última modificação: 02/09/2024 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 500

FILE *arq;
char buffers[MAX_LINHA];
char lexema[MAX_LINHA];
unsigned short tipo = 0;
int linhAtual=0,colunaAtual=0;
int aux=0;
unsigned short estado,fim=0;


int abrirArquivo (char []);
void lerArquivo ();
unsigned short delimitadorLexema(unsigned short);
unsigned short analisadorLexico(unsigned short); 

void limparString (char *cadeia){
    for(unsigned short i=0;i<strlen(lexema);i++){
        cadeia[i]=' ';
    }
}

unsigned short delimitadorLexema(unsigned short Ccaracter){
    // Os delimitadores são: espaço (32), nova linha, ponto e virgula
    switch (Ccaracter){
    case 0:
    case 13: 
    case 10 : // Nova linha
    case 32 : // Espaço
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
        return -1; // Acusar erro na abertura do ficheiro
    }
}

unsigned short analisadorLexico(unsigned short Ccaracter){
    
    /* Essa função tem os seguintes retornos:
        1 - Continua  
        2 - Fim
        0 - Não reconheceu a palavra 
    */
    
    char caracter = Ccaracter;
    unsigned short int  c = 0 ; // Variável controlada ( 1 - Tudo bem ; 0 - Erro) ;

    if (estado==0){ // Determinar tipo de autômato. O 1º caracter quem determina o tipo é o caracter
        switch (Ccaracter){
        case 48 ... 57 :
            c = 1;
            tipo = 1;
        break;
        case 65 ... 90: // Letra maiuscula
        case 97 ... 122: // Letra minuscula
        case 95: // Undercore
            c=1;
            tipo=3;
        break;
        }

        if(c==1){
            estado++; // Passa para para o próximo estado
        }
    } else {
        switch (tipo) { // A partir do 2º é determinado pelo caracter anterior
        case 1: 
        case 2: //Número
            switch (estado){ // Estado de transição
            case 1:
                switch (Ccaracter){
                case 48 ... 57 :
                    c = 1;
                    fim = 1;
                break;
                case 46:
                    c=1;
                    estado++;
                    tipo = 2; //Número real
                break;
                }
            break;
            case 2:
                fim = 0;
                switch (Ccaracter){
                case 48 ... 57 : // Se for um número
                    c = 1;
                    estado ++;
                break;
                }
            break;
            case 3:
               fim = 1;
               switch (Ccaracter){
               case 48 ... 57 : // Se for um número
                    c = 1;
               break;
               }
            break;
            }
            break;
        case 3: //Identificador
            fim = 1;
            switch (Ccaracter){
            case 48 ... 57 : // Se for um número
            case 65 ... 90: // Se for letra maiusculas
            case 97 ... 122: // Se for letra minuscula
            case 95: // Undercore
                c=1;
            break;
            }
        break;
        }
    }
    //printf("%s - %d\n", lexema,tipo);
    if(c==1){
        lexema[aux] = caracter;  
        //printf("%s - %d\n", lexema,tipo);
        return 1;
    }else{
        if((delimitadorLexema(Ccaracter)==1) && (fim==1)){ //Terminar com sucesso
            //printf("%s - %d\n", lexema,tipo);
            return 2;
        }else{
            return 0;
        }
    }
}

void lerArquivo (){
    int pos, nLinha;
    char caracter;
    unsigned short Ccaracter;
    linhAtual = 1;
    if (abrirArquivo ("programa.txt") !=-1 ){
        //while(fgets(buffers,MAX_LINHA,arq) != NULL){
            fgets(buffers,MAX_LINHA,arq);
            colunaAtual=0;
            estado=0;
            //printf("Lexema: %s  - %d \n",buffers,strlen(buffers) -1);
            while(colunaAtual<=strlen(buffers) -1 ){
                caracter = buffers[colunaAtual];
                Ccaracter = caracter;
                //printf("%d Caracter : %d\n", colunaAtual+1, Ccaracter );
                switch (analisadorLexico(Ccaracter))  {
                case 2:
                    printf("Lexema: %s - %d\n",lexema,tipo);
                    limparString(lexema);
                    aux=0;
                    estado=0;
                    break;
                case 0:
                    printf("Erro na linha %d coluna %d \n", linhAtual, (colunaAtual - strlen(lexema) +1 ));   
                    colunaAtual = strlen(buffers); 
                    limparString(lexema);
                    estado=0;
                    break;
                default:
                    break;
                }
                colunaAtual++;
                aux++;
            }
            linhAtual++;
        //}        
    }
}

/*
void analisadorLexico(unsigned short Ccaracter,unsigned short tipo){
   

    printf("%s",lexema);
}
*/

int main(){
   lerArquivo();

}



