
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//VARIAVEIS GLOBAIS
int contador=0;                             //CONTADOR PARA PEGAR OS NUMEROS
int pontos=0;                               //CONTADOR DE PONTOS
int mart=0;                                 //NUMERO DE MARTELOS
int pointmart=1;                            //CONTROLADOR DE PONTOS PARA PEGAR UM MARTELO
int mtrz[11][5];                            //MATRIZ DO JOGO (EXPLICACAO NO ARQUIVO EXTRAS)
int altr[11];                               //ALTURA
int numeros[9999];                          //ARRAY PARA COLOCAR OS NUMEROS DO ARQUIVO
int numt=0;                                 //VARIAVEL PARA COLOCAR A POSICAO DO ULTIMO NUMERO DO ARQUIVO
typedef struct{                             //STRUCT CONTENDO NOME DO PLAYER E PONTOS
    int pontuacao;
    char nome[20];
} strct;
strct player;

//DECLARANDO TODAS AS FUNCOES ANTES PARA PODER USAR QUALQUER UMA A QUALQUER MOMENTO
void CriarRank();
void ScanInicial();
int matriz(int y, int x, int c);
int altura(int x);
void gravidade();
void scan(int y, int x);
void estrutura();
void martelo();
void GameLoop();
void RankOrdem(strct aux[], int n);
void Rank(int c);
void Instrucoes();
void Config();
void Menu();
void Ganhou();
void Perdeu();



void CriarRank(){                           //FUNCAO QUE CRIA OU ZERA O RANK
    FILE* arq;
    arq = fopen("rank.bin", "wb");

    fclose(arq);
}


void ScanInicial(){                         //FUNCAO QUE ANALISA O ARQUIVO

    int aux;
    int i=0;
    int ret;

    FILE* num;
    num = fopen("numeros.txt", "r+");

    while(fscanf(num, "%d", &aux) != EOF){

        numeros[i]=aux;                     //COLOCA CADA NUMERO EM UMA POSICAO DO ARRAY

        i++;
    }

    numt=i;

    fclose(num);

}


int ScanNumeros(int cont){                  //FUNCAO QUE PEGA O NUMERO ATUAL

    int ret;
    ret=numeros[cont];

    return ret;

}


int matriz(int y, int x, int c){            //FUNCAO QUE FAZ O CONTROLE DA MATRIZ

    //c=-1  CRIA A MATRIZ OU ZERA ELA INTEIRA  ||  c=1  ADICIONA UM NUMERO NOVO NAS COORDENADAS  ||  c=0  NAO FAZ NADA

    if(c==-1){

        contador=0;
        pontos=0;

        ScanInicial();

        for(int i=0; i<11; i++){
            for(int j=0; j<5; j++){
                mtrz[i][j]=0;
            }
        }
    }else if(c==1){

        mtrz[y][x]+=ScanNumeros(contador);

    }    else if(c==0){

        mtrz[y][x]=mtrz[y][x];

    }

    return mtrz[y][x];

}


int altura(int x){                          //FUNCAO QUE FAZ O CONTROLE DA ALTURA


    for(int i=0; i<11; i++){

        if(mtrz[i][x]==0){

            altr[x]=i;

        }

    }

    return altr[x];
}


void gravidade(){                           //FUNCAO QUE FAZ OS BLOCOS FLUTUANTES CAIREM

    for(int i=10; i>=0; i--){               //ANALISA TODOS OS BLOCOS DE BAIXO PARA CIMA
        for(int j=4; j>=0; j--){

            if(mtrz[i][j]!=0 && mtrz[i+1][j]==0 && i!=10){      //SE O BLOCO DE BAIXO FOR 0 DERRUBA O DE CIMA

                mtrz[i+1][j]=mtrz[i][j];
                mtrz[i][j]=0;

                scan(i+1, j);               //ANALISA SE ONDE O BLOCO CAIU DA PRA FAZER COMBINACOES
                gravidade();                //FAZ A RECURSIVIDADE PARA CASO AINDA TENHAM BLOCOS FLUTUANTES
            }


        }
    }



}


void scan(int y, int x){                    //FUNCAO QUE ANALISA SE TEM ALGUM CASO DE COMBINACAO

    //RECEBE A COORDENADA DO BLOCO QUE VAI ANALISAR         obs: os prints comentados sao para debug
    //TODOS OS IFS TEM A MESMA ESTRUTURA: ANALISA SE A COORDENADA ATUAL SE ENQUADRA EM ALGUM CASO, FAZ A RECURSIVIDADE PARA SE UM NOVO CASO TIVER ACONTECIDO, ALTERA VALOR DA COORDENADA DA MATRIZ, ADICIONA OS PONTOS, CHAMA A GRAVIDADE PARA SE TIVER UM BLOCO FLUTUANTE

    /*
    atual= mtrz[y][x];
    cima=mtrz[y-1][x];
    baixo=mtrz[y+1][x];
    dir= mtrz[y][x+1];
    esq=mtrz[y][x-1];
    */

    if(y!=10 && mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y+1][x] && (mtrz[y][x+1]==mtrz[y][x] || mtrz[y][x-1]==mtrz[y][x])){

        //SE O DE BAIXO FOR IGUAL E O DA ESQUERDA OU DIREITA FOREM IGUAIS

        if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x+1] && mtrz[y][x]==mtrz[y][x-1]){
            //SE O DE BAIXO E DA DIREITA E ESQUERDA FOREM IGUAIS
            //printf("entrou if 1\n");
            mtrz[y][x]=mtrz[y][x+1]*4;
            mtrz[y][x+1]=0;
            mtrz[y][x-1]=0;
        } else if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x+1] && x!=4){
            //SE O DE BAIXO E DA DIREITA FOREM IGUAIS
            //printf("entrou if 2\n");
            mtrz[y][x]+=mtrz[y][x+1];
            mtrz[y][x+1]=0;
        } else if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x-1]){
            //SE O DE BAIXO E DA ESQUERDA FOREM IGUAIS
            //printf("entrou if 3\n");
            mtrz[y][x]+=mtrz[y][x-1];
            mtrz[y][x-1]=0;
        }

        mtrz[y+1][x]=mtrz[y][x]*2;
        mtrz[y][x]=0;
        pontos+=mtrz[y+1][x];

        scan(y+1, x);

        gravidade();


    }


    if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x+1] && mtrz[y][x]==mtrz[y][x-1] && x!=4 && x!=0){
        //SE O DA ESQUERDA E DIREITA FOREM IGUAL
        //printf("entrou if 1\n");

        mtrz[y][x]=mtrz[y][x+1]*4;
        mtrz[y][x+1]=0;
        mtrz[y][x-1]=0;
        pontos+=mtrz[y][x];

        scan(y, x);

        gravidade();

    }

    

    if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y+1][x] && y!=10){
        //SE APENAS O DE BAIXO FOR IGUAL
        //printf("entrou if 4\n");

        mtrz[y+1][x]+=mtrz[y][x];
        mtrz[y][x]=0;
        pontos+=mtrz[y+1][x];

        scan((y+1), x); 

        gravidade();

    }

    if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x+1] && x!=4){
        //SE APENAS O DA DIREITA FOR IGUAL
        //printf("entrou if 5\n");

        mtrz[y][x]+=mtrz[y][x+1];
        mtrz[y][x+1]=0;
        pontos+=mtrz[y][x];

        scan(y, x);

        gravidade();


    }

    if(mtrz[y][x]!=0 && mtrz[y][x]==mtrz[y][x-1] && x!=0){
        //SE APENAS O DA ESQUERDA FOR IGUAL
        //printf("entrou if 6\n");

        mtrz[y][x]+=mtrz[y][x-1];
        mtrz[y][x-1]=0;
        pontos+=mtrz[y][x];

        scan(y, x);

        gravidade();


    }


}


void estrutura(){                           //FUNCAO QUE ATUALIZA A ESTRUTURA DA MATRIZ

    system("cls");

    if(mart<3 && pontos>=(10000*pointmart)){ //ADICIONA 1 MARTELO A CADA QUANTIDADE DE PONTOS, MAXIMO DE MATERLOS EH 3
        mart++;
        pointmart++;
    }

    printf("\t\t +------+ +--+\n");
    printf("\t\t |%6d| |%2d|\n", ScanNumeros(contador), ScanNumeros(contador+1));
    printf("\t\t +------+ +--+\n");


    for(int i=1; i<11; i++){
        printf("+----+   ");
        
        if(i==2 || i==3 || i==4 || i==5){
            printf("+----+----+----+----+----+\t\t+--------+\n");
        }else{
            printf("+----+----+----+----+----+\n");
        }
        printf("| %2d |   ", i);
        printf("|");
        for(int j=0; j<5; j++){

            if(matriz(i, j, 0)==0){
                printf("    |");
            }else{
                printf("%4d|", matriz(i, j, 0));
            }
        }

        if(i==2){
            printf("\t\t|%8d|  PONTUACAO", pontos);
        } else if(i==4){
            printf("\t\t|%8d|  MARTELOS", mart);
        }
        
        printf("\n");
    }
    printf("+----+   ");
    printf("+----+----+----+----+----+\n\n");

    printf("         +----+----+----+----+----+\n");
    printf("         |  1 |  2 |  3 |  4 |  5 |\n");
    printf("         +----+----+----+----+----+\n");


    printf("Em qual coluna deseja inserir o numero? ");

}


void martelo(){                             //FUNCAO QUE FAZ O CONTROLE DOS "MARTELOS"

    //OS MATERLOS SAO ADICIONADOS A CADA CERTA QUANTIDADE DE PONTOS, ESSA PARTE E FEITO NA FUNCAO estrutura

    for(int i=0; i<5; i++){                 //ZERA LINHA INVISIVEL DE CIMA (explicacao no arquivo EXTRAS)
        mtrz[0][i]=0;
    }

    int x, y;
    printf("\nDigite a linha e coluna que voce deseja eliminar respectivamente: ");
    scanf("%d %d", &y, &x);                 //PEGA A LINHA E COLUNA QUE DESEJA APAGAR
    mtrz[y][x-1]=0;                         //APAGA
    mart--;                                 //DIMINUI UM MARTELO
    gravidade();                            //CHAMA A FUNCAO DE QUEDA/GRAVIDADE
    estrutura();                            //CHAMA A FUNCAO DE ATUALIZAR A ESTRUTURA

}


void GameLoop(){                            //FUNCAO QUE LE ONDE O JOGADOR QUE COLOCAR O BLOCO

    int tcl;

    //TODOS OS 5 CASOS PRINCIPAIS TEM A MESMA ESTRUTURA QUE EU IREI EXPLICAR APENAS NO PRIMEIRO CASO
    //O CASO 7 E 8 SAO CHEATS
    
    while(scanf("%d", &tcl) != EOF){

        switch (tcl){

        case 1:
            int t1=altura(tcl-1);           //COLOCA A ALTURA EM UMA VARIAVEL APENAS PARA CONTROLE

            matriz(t1, tcl-1, 1);           //CHAMA A FUNCAO MATRIZ QUE ALTERA O VALOR DA COORDENADA NA MATRIZ

            scan(t1, tcl-1);                //CHAMA A FUNCAO DE SCAN PAVA VER SE TEM ALGUMA COMBINACAO A SER FEITA

            contador++;                     //ADICIONA 1 NO CONTADOR

            estrutura();                    //CHAMA A FUNCAO ESTRUTURA PARA ATUALIZAR A ESTRUTURA

            if(matriz(0, tcl-1, 0)!=0){     //VE SE APOS ESSE MOVIMENTO O JOGADOR PERDEU
                Perdeu();
            }

            if(contador==numt){             //VE SE APOS O MOVIMENTO O JOGADOR GANHOU
                Ganhou();
            }

            break;
        
        case 2:

            int t2=altura(tcl-1);

            matriz(t2, tcl-1, 1);

            scan(t2, tcl-1);

            contador++;

            estrutura();

            if(matriz(0, tcl-1, 0)!=0){
                Perdeu();
            }

            if(contador==numt){
                Ganhou();
            }

            break;

        case 3:

            int t3=altura(tcl-1);

            matriz(t3, tcl-1, 1);

            scan(t3, tcl-1);

            contador++;

            estrutura();

            if(matriz(0, tcl-1, 0)!=0){
                Perdeu();
            }

            if(contador==numt){
                Ganhou();
            }

            break;

        case 4:

            int t4=altura(tcl-1);

            matriz(t4, tcl-1, 1);

            scan(t4, tcl-1);

            contador++;

            estrutura();

            if(matriz(0, tcl-1, 0)!=0){
                Perdeu();
            }

            if(contador==numt){
                Ganhou();
            }

            break;

        case 5:

            int t5=altura(tcl-1);

            matriz(t5, tcl-1, 1);

            scan(t5, tcl-1);

            contador++;

            estrutura();

            if(matriz(0, tcl-1, 0)!=0){
                Perdeu();
            }

            if(contador==numt){
                Ganhou();
            }

            break;

        case 7:                             //CHEAT PARA ZERAR A MATRIZ
            
            matriz(0, 0, -1);
            estrutura();

            break;


        case 8:                             //CHEAT PARA USAR 1 MARTELO

            mart=1;
            martelo();
            estrutura();
            break;

        }


    }


}


void RankOrdem(strct aux[], int n){         //FUNCAO QUE FAZ A ORDENACAO DO RANK EM ORDEM DECRESCENTE

    int ok=1;
    for(int i=0; i<n && ok!=0; i++){        //BUBBLE SORT COM O ARRAY COM AS INFORMACOES DO RANK
        ok=0;
        for(int j=0; j<n; j++){

            if(aux[i].pontuacao<aux[j].pontuacao && i<j){
                strct aj;

                aj=aux[i];
                aux[i]=aux[j];
                aux[j]=aj;
                ok=1;
    
            }

        }
    }


}


void Rank(int c){                           //FUNCAO QUE ESCREVE E LE O RANK

    //C=1 ESCREVE NO RANK   ||  C=0 LE O RANK

    if(c==1){

        player.pontuacao=pontos;            //COLOCA A POTUACAO FINAL NO STRUCT

        FILE *arq;                          //ABRE O ARQUIVO E COLOCA A PONTUACAO NO FINAL DELE
        arq = fopen("rank.bin","r+b");      
        fseek(arq, 0, SEEK_END);            
        fwrite(&player, sizeof(strct), 1, arq);
        fclose(arq);

    } else if(c==0){

        FILE *arq;                          //ABRE O ARQUIVO PRA LEITURA
        arq = fopen("rank.bin","rb");
        strct aux[1000];
        strct aux2;

        int n=0;

        while(fread(&aux2, sizeof(strct), 1, arq) > 0){

            aux[n]=aux2;                    //PEGA A INFORMACAO DO RANK E COLOCA NO ARRAY aux[1000]
            n++;                            //TAMANHO DO ARRAY

        }

        RankOrdem(aux, n);                  //CHAMA A FUNCAO DE ORDENACAO MANDANDO O ARRAY E O TAMANHO

        printf("RANK:\n\n");

        for(int i=0; i<n; i++){

            printf("%d) %s - %d\n\n", i+1, aux[i].nome, aux[i].pontuacao);          //PRINTA AS INFORMACOES ORDENADAS

        }
        
        fclose(arq);

    }


}


void Instrucoes(){                          //FUNCAO QUE MOSTRAS AS INSTRUCOES

    system("cls");

    printf("INSTRUCOES\n\n");

    printf("OBJETIVO DO JOGO:\n");
    printf("- O objetivo e combinar numeros identicos para formar numeros maiores e acumular pontos.\n\n");

    printf("INICIANDO O JOGO:\n");
    printf("- Ao iniciar o jogo, o jogador deve inserir seu nome.\n");
    printf("- O jogo exibe uma matriz de 11 linhas por 5 colunas, representando o tabuleiro.");
    printf("- No topo da tela, dois numeros aleatorios sao mostrados. Um deles sera inserido na coluna escolhida pelo jogador.\n\n");

    printf("COMO JOGAR:\n");
    printf("- O jogador deve escolher uma das cinco colunas digitando um numero de 1 a 5.\n");
    printf("- O numero selecionado caira na posicao mais baixa disponivel dessa coluna.\n");
    printf("- Se um numero cair sobre outro numero igual, eles serao combinados e dobrados (exemplo: 2 + 2 = 4).\n");
    printf("- Quando uma combinacao acontece, os numeros superiores podem cair e formar novas combinacoes.\n\n");

    printf("PONTUACAO:\n");
    printf("- A pontuacao aumenta com a soma dos valores combinados.\n");
    printf("- Apos o termino do jogo, a pontuacao do jogador e registrada.\n");
    printf("- O ranking pode ser acessado no menu principal.\n\n");

    printf("FIM DE JOGO:\n");
    printf("- O jogo termina quando nao ha mais numeros para serem colocados. (VITORIA)\n");
    printf("- OU\n");
    printf("- O jogo termina quando uma nova peca nao pode ser inserida porque a coluna esta cheia. (DERROTA)\n\n");

    printf("Digite ENTER para voltar ao MENU: ");
    char c;
    getchar();
    scanf("%c", &c);
    Menu();

}


void Config(){                              //FUNCAO QUE FAZ O CONTROLE DAS CONFIGURACOES

    printf("CONFIGURACOES\n\n");

    printf("1 - Zerar o Rank\n");
    printf("2 - Modo de Dificuldade\n");
    printf("3 - Voltar ao Menu\n\n");

    int c;
    scanf("%d", &c);                        //VE A TECLA PRESSIONADA
    switch (c)                              //1-ZERA O RANK || 2-DIFICULDADE || 3-VOLTA PRO MENU 
    {
    case 1:
        
        CriarRank();                        //CHAMA A FUNCAO PARA CRIAR UM ARQUIVO DE RANK NOVO E SUBSTITUIR O ANTIGO
        
        system("cls");
        printf("RANK ZERADO\n\n");
        Config();

        break;
    
    case 2:
        
        system("cls");
        printf("Em desenvolvimento...\n");
        Config();

        break;

    case 3:

        Menu();

        break;
    }



}


void Menu(){                                //FUNCAO QUE FAZ O CONTROLE DO MENU
    
    system("cls");

    printf("1 - Start\n");                 
    printf("2 - Configuracoes\n");          
    printf("3 - Instrucoes\n");             
    printf("4 - Rank\n");
    printf("5 - Sair\n");

    int n;
    
    printf("\nDigite a opcao desejada: ");
    scanf("%d", &n);                        //DA SCANF NA TECLA DIGITADA E CHAMA OUTRAS FUNCOES

    switch (n)                              //1-COMECA O JOGO || 2-ABRE AS CONFIGURACOES || 3-ABRE AS INSTRUCOES || 4-ABRE O RANK || 5-ENCERRA O JOGO
    {
    case 1:                                 //START
        
        pontos=0;
        contador=0;
        printf("Digite seu nome: ");
        getchar();
        scanf("%s", player.nome);           //SALVA O NICKNAME DO PLAYER
        ScanInicial();
        matriz(0, 0, -1);                   //CRIA A MATRIZ COM TODOS OS ELEMENTOS ZERADOS
        estrutura();                       //CHAMA A ESTRUTURA DO JOGO
        GameLoop();                         //INCIA O GAMELOOP
        
        break;
    

    case 2:                                 //CONFIGURACOES

        system("cls");
        Config();                           //FUNCAO CONFIG

        break;


    case 3:                                 //INSTRUCOES

        Instrucoes();                       //FUNCAO INSTRUCOES

        break;


    case 4:                                 //RANK
        system("cls");
        Rank(0);                            //CHAMA O RANK NA OPCAO DE LEITURA
        char v;
        printf("Pressione ENTER para voltar: ");
        getchar();
        scanf("%c", &v);
        Menu();
        
        break;

    
    case 5:                                 //SAIR
        
        system("cls");
        break;

    }
    
}


void Ganhou(){                              //FUNCAO DE VITORIA

    Rank(1);                                //COLOCA OS PONTOS NO RANK

    pontos=0;

    char c;

    printf("\nPARABENS VOCE GANHOU!!!!!!!!!!\n");
    printf("Deseja jogar novamente?(S/N): ");
    getchar();
    scanf("%c", &c);

    if(c=='N' || c=='n'){

        system("cls");
        Menu();

    } else if(c=='S' || c=='s'){

        system("cls");
        matriz(0, 0, -1);
        estrutura();
        GameLoop();

    }

}


void Perdeu(){                              //FUNCAO DE DERROTA

    if(mart!=0){                            //SE O JOGADOR TIVER MARTELOS DA MAIS UMA CHANCE

        martelo();

        for(int i=0; i<5; i++){
            mtrz[0][i]=0;
        }

        GameLoop();

    }

    Rank(1);                                //COLOCA OS PONTOS NO RANK

    pontos=0;

    char c;

    printf("\nQUE PENA! VOCE PERDEU!!!!!!!!!!\n");
    printf("Deseja reiniciar?(S/N): ");
    getchar();
    scanf("%c", &c);

    if(c=='N' || c=='n'){

        system("cls");
        Menu();

    } else if(c=='S' || c=='s'){

        system("cls");
        matriz(0, 0, -1);
        estrutura();
        GameLoop();

    }

}


int main(){    

    FILE *arq;
    if ((arq = fopen("rank.bin","rb")) == NULL){
        CriarRank();                         //SE O ARQUIVO DE RANK NAO EXISTIR CRIA UM
    }
    fclose(arq);

    system("cls");
    
    printf("Bem vindo(a) ao Jogo DROP MERGE de APC!\n\n");
    char c;
    printf("Pressione ENTER para continuar: ", &c);
    scanf("%c", &c);

    Menu();


    return 0;
}