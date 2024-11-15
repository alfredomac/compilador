/*
Programa: Compilador Macutunda
Autor: Alfredo Macutunda
Data de Início: 24/08/2024
Última modificação: 07/11/2024 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 500
#define MAX_CARACTERES 9

// Estrutura para Token
typedef struct Token {
    char tValor[MAX_LINHA];
    unsigned short tTipo;
} token;


// Estrutura para ID
typedef struct ID id;
struct ID {
    token info;
    unsigned short int idTipo;
    id *prox;
};


FILE *arq;
token gerado;
token t; // Token a ser consumido

char buffers[MAX_LINHA];
id tabelaHash[MAX_CARACTERES]; // Tabela Hash
int linhAtual = 0,colunaAtual = 0;
unsigned short estado = 1,fim = 0,aux = 0;
unsigned short aberto = 0; // Modifica se o arquivo está aberto
unsigned short tipo = 0, tipoAnterior = 0;
unsigned short cSe = 0; // Conta o núemro de Se's
unsigned short cP = 0; // Conta o núemro de Parênteses


// Geral
void abrirArquivo ();
void fecharArquivo ();
void limparString (char*);

// Analisador Léxico
void lerLinha ();
void explorarLinha();
void transicao(unsigned short , unsigned short, unsigned short);
void erroTransicao();
void alfaNumerico(unsigned short);
token automato (unsigned short);
token consumir (); 
token proximoToken(); 


// Analisador Sintático
unsigned short int cmds();
unsigned short int cmd();
unsigned short int atribuicao();
unsigned short int atribuicaoIncremental();
unsigned short int declaracao();
unsigned short int le();
unsigned short int imprime();
unsigned short int se();
unsigned short int senao();
unsigned short int enquanto();
unsigned short int para();
unsigned short int expressao();
unsigned short int expressaoCondicional();
unsigned short int condicao();
unsigned short int termo();
unsigned short int fator();
void erroSintatico();

// Analisador Semântico
unsigned short int codigoHash(char *);
void adicionarTabelaHash(token, unsigned short int);
unsigned short int verTabelaHash(token);
unsigned short int tabelaVazia(unsigned short int);
id *criarId( token ident, unsigned short int);
void erroSemantico(char *);



/* --------------------------------------------- USO GERAL -----------------------------------------*/

void fecharArquivo( ){
    if(aberto == 1){
        aberto = 0;
        fclose(arq);
    }
}

void abrirArquivo( ){
    char nomeArquivo[] = "programa.txt";
    arq = fopen(nomeArquivo,"r");
    if(arq == NULL){
        fclose(arq);
    }else{
        aberto = 1;
    }
}

void limparString (char* cadeia){
    for(unsigned short i=0;i<strlen(cadeia);i++){
        cadeia[i] =' ';
    }
}

/* --------------------------------------------- USO GERAL -----------------------------------------*/

/* --------------------------------------------- ANALISADOR SEMÂNTICO -----------------------------------------*/

void erroSemantico(char* cadeia ){
    printf("Erro (Semântico) na Ln %d Col %d - %s ", linhAtual+1, colunaAtual-1, cadeia);
}

id *criarId( token ident, unsigned short t){
    id *p = (id *) malloc (sizeof(id));
    p->prox = NULL;
    p->info = ident;
    p->idTipo = t;
    return p;
}

unsigned short int tabelaVazia(unsigned short int indice){
    if(tabelaHash[indice].prox == NULL){
        return 1;
    }
    return 0;
}

unsigned short int codigoHash (char* cadeia){
    if(strlen(cadeia) > MAX_CARACTERES ){
        return 9;
    }
    return strlen(cadeia-1);
}

void adicionarTabelaHash(token ident, unsigned short int t){
    unsigned short int indice = codigoHash(ident.tValor);
    id *p ;
    id *pNovo;
    p = tabelaHash[indice].prox;
    pNovo = criarId(ident,t);
    tabelaHash[indice].prox = pNovo;
    if(tabelaVazia(indice)==0){
        pNovo->prox = p;
    }

}

unsigned short int verTabelaHash(token ident){
    id *p;
    unsigned short int indice = codigoHash(ident.tValor);
    if( tabelaVazia(indice) == 0 ){
        p = tabelaHash[indice].prox;
        while ( p != NULL ) {
            if( strcmp(ident.tValor, p->info.tValor) == 0  ){
                return  p->idTipo;
            }
            p = p->prox;
        }
    }
    return 0;
}


/* --------------------------------------------- ANALISADOR SEMÂNTICO -----------------------------------------*/



/* --------------------------------------------- ANALISADOR SINTÁTICO -----------------------------------------*/

void erroSintatico(){
    printf("Erro (sintático) na Ln %d Col %d ", linhAtual+1, colunaAtual-1);
}

unsigned short int cmds(){
    unsigned short int certo = 0;
    printf("CMDS\n");
    if(aberto == 1 ){
        if(fgets(buffers,MAX_LINHA,arq) != NULL){
            certo = cmd();  
            // Enquanto for atribuição ou nova linha
            while ( ((certo==9) || (certo==3)) && (certo!=10) && (certo!=0)){
                if(fgets(buffers,MAX_LINHA,arq) != NULL ){
                    certo = cmd();
                } else {
                    certo = 0;
                }
            }
        }

        if(certo == 10){
            while ( (certo!=11) && (certo!=0)){
                if(fgets(buffers,MAX_LINHA,arq) != NULL ){
                    certo = cmd();
                }else{
                    certo = 0;
                }
            }    
        }
    }
    printf("CMDS\n");
    return certo;
    
}

unsigned short int cmd(){
    unsigned short int e=0;
    /*
    Tipos: 
    02 - Atribuição
    03 - Declaração
    04 - Imprime 
    05 - Le
    06 - Para
    07 - Enquanto      
    08 - Se
    09 - Nova Linha
    10 - Abrir Bloco
    11 - Fechar Bloco
    12 - Senao
    */
    tipo = 0;
    tipoAnterior = 0;

    colunaAtual = 0;
    printf("CMD - ");
    t = proximoToken();
    printf("Olha %d ", t.tTipo);
    switch (t.tTipo){
    case 25:
        e = atribuicao();
        break;
    case 26:
    case 27:
    case 28:
    case 29:
        e = declaracao();
        break;
    case 30: 
        e = imprime();
        break;
    case 31:
        e = le();
        break;
    case 32:
        e = se();
        break;
    case 33:
        e = senao();
        break;
    case 34:
        e = enquanto();
        break;
    case 35:
        e = para();
        break;
    case  36: // \n
        printf("Nova Linha - ");
        e = 9;
        break;
    case 17: // {
        printf("Abrir Bloco - ");
        cP++;
        e = 10;
        break;      
    case 18: // }
        if(cP>0){
            printf("Fechar Bloco - ");
            cP--;
            e = 11;
        }
        break;
    default:
        erroSintatico();
        break;
    }
    if(e!=0){
        printf("Boas");
    }
    printf("\n");
    linhAtual++;
    return e;
    
}

unsigned short int atribuicao(){
    unsigned short int certo = 0; //certo
    unsigned short int tipoV, tipoE,  tAnterior ;
    
    printf("Atribuição - ");
    tipoV = verTabelaHash(t);
    if(tipoV != 0 ){
        t = proximoToken();
        switch (t.tTipo){
        case 8: // =
            t = proximoToken();
            tipoE = expressao();
            if( tipoV >= tipoE ){
                certo = 2;
            } else{
                erroSemantico("Tipos Incompatíveis");
                if(tipoE!=0){
                    certo = 2;
                }
            }
            break;
        case 4: // + 
        case 5: // -
            if(tipoV != 26 ){
                erroSemantico("Atribuicao Incremental e para inteiro");
            }

            tAnterior = t.tTipo; // Pegar o sinal anterior
            t = proximoToken();
            if(t.tTipo == tAnterior){ //Se for o mesmo sinal do anterior
                certo = 2;
            }            
            break;
        default:
            break;
        }

        if(certo == 0){
            erroSintatico();
        }
    }else{
        erroSemantico("Variável não declarada");
    }

    return certo;
}

unsigned short int atribuicaoIncremental(){
    unsigned short int tAnterior, certo = 0 ; 
    printf("Atribuição Incremental - ");
    if (t.tTipo == 25){ // id
        if(verTabelaHash(t) != 26){
            erroSemantico("Atribuicao Incremental e para inteiro");
        }
    
        t = proximoToken();
        switch (t.tTipo){
        case 8: // =
            t = proximoToken();
            if(t.tTipo == 25){ //Id
                t = proximoToken();
                if((t.tTipo == 4) || (t.tTipo ==5)){ // + ou -
                    t = proximoToken();
                    if( (t.tTipo == 25) || (t.tTipo==1) ){ // id ou inteiro
                        certo = 1;
                    }
                }
            }
            break;
        case 4: // + 
        case 5: // -
            tAnterior = t.tTipo; // Pegar o sinal anterior
            t = proximoToken();
            if(t.tTipo == tAnterior){ //Se for o mesmo sinal do anterior
                certo = 1;
            }
            break;
        default:
            break;
        }
    }

    if(certo == 0){
        erroSintatico();
    }
    return certo;
}

unsigned short int declaracao(){
    unsigned short int certo = 0 ; //certo
    unsigned short int tipo = t.tTipo;
    printf("Declaração - ");
    t = proximoToken();
    if( t.tTipo == 25 ){
        certo = 3;
        adicionarTabelaHash(t,tipo);
        t = proximoToken();
        while ( (t.tTipo == 21) && ( certo==3)  ){
            t = proximoToken();
            if(t.tTipo == 25){
                certo = 3;
                adicionarTabelaHash(t,tipo);
            } else {
                certo = 0;
            }

            if(certo==3){
                t = proximoToken();    
            }
        }
    }

    if(certo==0){
        erroSintatico();
    }
    return certo;
}

unsigned short int imprime(){
    unsigned short int certo = 0 ; 
    printf("Imprime - ");
    t = proximoToken();
    if(t.tTipo = 15){ // (
        t = proximoToken();
        if( expressao() != 0){
            certo = 1;
            while ( (t.tTipo == 21) && (certo==1)  ){
                certo = 0;
                t = proximoToken();
                if(expressao() == 1){
                    certo = 1;
                } 
            }
            if(certo == 1){
                certo = 0;
                if(t.tTipo==16){ // )
                    certo = 4;
                }
            }
        }
    }
    if(certo==0){
        erroSintatico();
    }
    return certo;
}

unsigned short int le(){
    unsigned short int certo = 0 ; //certo?
    printf("Le - ");
    t = proximoToken();
    if(t.tTipo = 15){
        t = proximoToken();
        if( t.tTipo == 25 ){
            certo = 1;
            t = proximoToken();
            while ( (t.tTipo == 21) && ( certo==1)  ){
                t = proximoToken();
                if(t.tTipo == 25){
                    certo = 1;
                } else {
                    certo = 0;
                }

                if(certo==1){
                    t = proximoToken();    
                }
            }
            if(certo == 1){
                certo = 0;
                t = proximoToken();
                if(t.tTipo==16){
                    certo = 5;
                }
            }
        }
    }

    if(certo==0){
        erroSintatico();
    }
    return certo;
}

unsigned short int para(){
    unsigned short int certo = 0;
    printf("Para - ");
    t =  proximoToken();
    if(t.tTipo == 15){ // (
        t = proximoToken();
        if(t.tTipo == 25){ // Pegar variável de atribuição
            if(atribuicao()==1){ // Atribuição
                if(t.tTipo == 21){ //,
                    if(condicao() == 1){ // condição
                        if(t.tTipo == 21){ //,
                            t = proximoToken();
                            if(atribuicaoIncremental() == 1){ // Atribuição Incremental
                                if(t.tTipo==16){ // )
                                    if(cmds() == 11){
                                        certo = 6;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if(certo == 0){
        erroSintatico();
    }
}


unsigned short int enquanto(){
    unsigned short int certo = 0;
    printf("Enquanto - ");
    t =  proximoToken();
    if(t.tTipo == 15){ // (
        if(condicao()==1){ // condição
            if(t.tTipo==16){ // )
                if(cmds() == 11){
                    certo = 7;
                }
            }
        }
    }

    if(certo == 0){
        erroSintatico();
    }
}


unsigned short int se(){
    unsigned short int certo = 0;
    printf("Se - ");
    t = proximoToken();
    if(t.tTipo == 15){ // (
        if(condicao()==1){ // Condição
            if(t.tTipo == 16){ // )
                printf("\n");
                if(cmds()==11){ // Bloco de Comandos       
                    certo = 0;  
                    cSe++;      
                    if(senao()==12){
                        certo = 8;
                    }
                    cSe--;
                }
            }
        }
    }

    if(certo==0){
        erroSintatico();
    }
    return certo;
}

unsigned short int senao(){
    unsigned short int certo=0;
    if(cSe>0){    
        printf("Senao - ");
        printf("\n");
        certo = cmds();
        if( (certo==11) ||  (certo==12) ){ // Bloco Senao
            certo = 12;
        }

        if(certo!=12){
            certo = 0;
            erroSintatico();
        }
    }
    return certo;
}


unsigned short int condicao(){
    unsigned short int certo = 0;
    printf("Condição - ");
    t = proximoToken();
    if(t.tTipo==24){ // nao
        t = proximoToken();
    }

    if(expressaoCondicional()==1){
        certo = 1;
        printf("<%s>", t.tValor);
        while (((t.tTipo==22) || (t.tTipo==23) ) && (certo == 1) ) {
            certo = 0;
            if(expressaoCondicional()==1){
                certo = 1;
            }
        }
        
    }
    return certo;
}

unsigned short int expressaoCondicional(){
    unsigned short int certo = 0;
    printf("Expressão Condicional - ");
    certo = expressao();
    printf("Operador %d", certo);
    if(certo!=0){
        certo = 0;
        switch (t.tTipo){
        case 9 ... 14: // Operador relacional
            t = proximoToken();
            if(expressao()!=0){
                certo = 1;
            }
            break;
        default:
            break;
        }
    }
    return certo;
}

unsigned short expressao(){
    printf("Expressão - ");
    if(t.tTipo==3){
        printf("String - ");
        tipoAnterior = 28; // Literal
        t = proximoToken();
    }else{
        tipo = termo();
        if(tipoAnterior!=27){
            tipoAnterior = tipo;
        }
        while( (t.tTipo==4) || (t.tTipo==5) ){
            printf("Adição - ");
            t = proximoToken();
            tipo = termo();
            if((tipo!=26) && (tipo!=27)){
                if(tipo == 27){
                    tipoAnterior = tipo;
                }
            }
        }
    }
    return tipoAnterior;
}

unsigned short int termo(){
    printf("Termo - ");
    tipo = fator(); // Pegar o tipo retornado
    if(tipoAnterior!=27){
        tipoAnterior = tipo;
    }
    while( (t.tTipo==6) || (t.tTipo==7) ){
        printf("Multiplicação - ");
        t = proximoToken();
        tipo = fator();
        if((tipo==26) || (tipo==27)){
            tipoAnterior = tipo;
            if(tipo == 27){
                tipoAnterior = tipo;
            }
        }
    }
    return tipoAnterior;
}

unsigned short int fator(){
    printf("Fator - ");
    unsigned short int certo=0;
    switch (t.tTipo){
    case 1: 
    case 2: 
        printf("Número - ");
        tipo = t.tTipo+25; // Tipos = {Inteiro: 26, Real: 27}, por isso somou-se com 25
        break;
    case 25:
        printf("Id - ");
        tipo = verTabelaHash(t) ; // Enviar o tipo da variável
        if(tipo==0){
            erroSemantico("Variavel nao declarada");
            tipo = 26;
        }
        break;
    case 15:
        printf("Parenteses - ");
        printf("Entrando - ");
        t = proximoToken();
        tipo = expressao();
        if(tipo != 0){
            if(t.tTipo == 16){
                printf("Saindo - "); 
            }else{
                tipo = 0;
            }
        }
        break;
    default:
        break;
    }

    t = proximoToken();
    return tipo;
}

/* --------------------------------------------- ANALISADOR SINTÁTICO -----------------------------------------*/



/* --------------------------------------------- ANALISADOR LÉXICO -----------------------------------------*/

void gerarIdentificador(){
    estado = 67;
    gerado.tTipo=25;
    fim=1;
}

void alfaNumerico(unsigned short Ccaracter){
    switch (Ccaracter){
        case 48 ... 57: //   [0-9]
        case 65 ... 90: //   [A-Z]
        case 97 ... 122: //  [a-z]
            gerarIdentificador();
            break;
        default:
            erroTransicao();
            break;
    }
}

void erroTransicao(){
    estado = 0;
}

void transicao(unsigned short est, unsigned short ger, unsigned short f){
    estado = est;
    gerado.tTipo = ger;
    fim = f;
}



void lerLinha (){
    token p;
    if(aberto == 1 ){
        while(fgets(buffers,MAX_LINHA,arq) != NULL){     
            /*
            if (cmds()==1){
                printf("Boas\n");
            }
    
            */
            explorarLinha();
            
            /*
            if(colunaAtual<=strlen(buffers)-1){
                p = consumir();
                printf("Aqui: <%s,%d>", p.tValor , p.tTipo);
            }
            */

            linhAtual++;
        }        
    }
    fclose(arq);
}


void explorarLinha(){
    token t; 
    unsigned short cCaracter;
    unsigned short cAux=0; //Guarda posição do erro
    char* cadVazio = "";
    colunaAtual=0;
    estado=1;
    aux = 0;
    while(colunaAtual<=strlen(buffers)-1 ){
        cCaracter= buffers[colunaAtual];
        t = automato(cCaracter);
        if(estado==0){
            printf("<%s,%d>", gerado.tValor , gerado.tTipo);
            t.tTipo = gerado.tTipo;
            strcpy(t.tValor,gerado.tValor);
            limparString(gerado.tValor);
            if(fim==1){
                aux = 0;
                estado = 1;
                t = automato(cCaracter);
            } else{
                estado==0; // Erro
                colunaAtual = strlen(buffers); // Sair do Loop
            }
        }
        cAux++;
        colunaAtual++;
    }
    if(fim==1){
        if(cCaracter!=10){
            printf("<%s,%d>", gerado.tValor, gerado.tTipo);
        }
    }else{
        printf("Erro na Ln %d Col %d\n",linhAtual, cAux-1);
    }
    printf("\n");
}


token consumir(){
    token t; 
    unsigned short cCaracter;
    unsigned short cAux=colunaAtual; //Guarda posição do erro
    estado = 1;
    aux = 0;
    while(colunaAtual<=strlen(buffers)-1){
        cCaracter= buffers[colunaAtual];
        t = automato(cCaracter);
        if(estado==0){
            t.tTipo = gerado.tTipo;
            strcpy(t.tValor,gerado.tValor);
            limparString(gerado.tValor);
            if(fim==1){
                estado = 1;
                return t; 
            } else{
                printf("Erro (Léxico) na Ln %d Col %d - ",linhAtual+1, cAux);
                t.tTipo = 0;
                return t;
            }
        }
        cAux++;
        colunaAtual++;
    }
    if(fim==1){
        if(cCaracter!=10){
            return gerado;
        }
    }else{
        gerado.tTipo = 0;
        printf("Aqui Erro (Léxico) na Ln %d Col %d\n",linhAtual+1, cAux);
        return gerado;
    }
}

token proximoToken(){
    // Consome espaços em brancos: (Espaço) e Tab (Tabulação)
    t = consumir();
    while ( ( (t.tTipo==19) || (t.tTipo==20) )){
        t = consumir();
    }
    return t;
}
    



token automato(unsigned short Ccaracter){
  
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
   21 - , (Vírgula)
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
   36 - Nova Linha
   */
    
    char caracter = Ccaracter;
    switch (estado) {
        case 1:
            aux = 0;
            fim = 1;
            /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 10: //(Nova Linha)
                transicao(0,36,fim);
                break;
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
            case 44: // , (Vírgula)
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
                erroTransicao();
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 31:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 37:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 38:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 116 : // t
                transicao(39,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 40:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 114 : // r
                transicao(41,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 42:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 108 : // l
                transicao(43,28,1);
                break;
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 43:
        /* ---------------- Caracteres do Estado -------------------*/    
            alfaNumerico(Ccaracter);
        case 44:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 109 : // m
                transicao(45,25,1);
                break;
            case 110: // n
                transicao(51,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/
        
        case 50:
        /* ---------------- Caracteres do Estado -------------------*/
           alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 51:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 116 : // t
                transicao(52,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 56:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 57:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 101 : // e
                transicao(58,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 60:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 61:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(62,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 64:
        /* ---------------- Caracteres do Estado -------------------*/
           alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 65:
        /* ---------------- Caracteres do Estado -------------------*/
           alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 66:
        /* ---------------- Caracteres do Estado -------------------*/
           alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 67:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 68:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 110 : // n
                transicao(69,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 75:
        /* ---------------- Caracteres do Estado -------------------*/
           alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 76:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 97 : // a
                transicao(77,25,1);
                break;
            default:
                alfaNumerico(Ccaracter);
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
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 78:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 79:
        /* ---------------- Caracteres do Estado -------------------*/
            switch (Ccaracter){
            case 117 : // u
                transicao(80,23,1);
                break;
            default:
                alfaNumerico(Ccaracter);
                break;
            }
        break;
        /* ---------------- Caracteres do Estado -------------------*/

        case 80:
        /* ---------------- Caracteres do Estado -------------------*/
            alfaNumerico(Ccaracter);
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

/* --------------------------------------------- ANALISADOR LÉXICO -----------------------------------------*/

int main(){
    unsigned short int certo;
    //lerLinha();
    
    abrirArquivo();
 
    if( (cmds()==11) || (cP==0) ){
        printf("Tudo bem\n");
    }else{
        printf("Erro: Não fechou o bloco\n");
    }

    fecharArquivo();
}
