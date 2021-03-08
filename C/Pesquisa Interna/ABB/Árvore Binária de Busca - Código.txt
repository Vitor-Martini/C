#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Pesquisa Interna - Árvore Binária de Busca
*/

typedef struct Registro{
    int chave;
} Registro;

typedef struct No * Apontador;

typedef struct No{
    Registro reg;
    struct No *esq, *dir;
} No;

typedef No * Apontador;

void Pesquisa(Registro *x, Apontador *p){

    if(*p == NULL){
        printf("ERRO: Registro nao esta presente na arvore \n");
        return;
    }
    if(x -> chave < (*p)->reg.chave){
        Pesquisa(x, &(*p)->esq);
        return;
    }
    if(x -> chave > (*p)->reg.chave)
        Pesquisa(x, &(*p)->dir);
    else
        *x = (*p) -> reg;

}

void Inicializa(Apontador *Dicionario){

    *Dicionario = NULL;

}

void Insere(Registro x, Apontador *p){

    if(*p == NULL){
        *p = (Apontador) malloc(sizeof(No));
        (*p) ->reg = x;
        (*p) -> esq = NULL;
        (*p) -> dir = NULL;
        return;
  }
    if(x.chave < (*p)->reg.chave)
    {
        Insere(x, &(*p)->esq);
        return;
    }
    if(x.chave > (*p)->reg.chave)
       Insere(x, &(*p)->dir);
    else
        printf("ERRO: Esse registro ja existe na arvore");

}

void Antecessor(Apontador q, Apontador *r){

    if((*r) -> dir != NULL){
        Antecessor(q, &(*r) -> dir);
        return;
    }
    q -> reg = (*r) -> reg;
    q = *r;
    *r = (*r) -> esq;
    free(q);

}

void Retira(Registro x, Apontador *p){

    Apontador Aux;
    if(*p == NULL){
        printf("ERRO: Registro nao esta na arvore");
        return;
    }

    if(x.chave < (*p) -> reg.chave) { Retira(x, &(*p) -> esq); return; }
    if(x.chave > (*p) -> reg.chave) { Retira(x, &(*p) -> dir); return; }

    if((*p) -> dir == NULL){
        Aux = *p;
        *p = (*p) -> esq;
        free(Aux);
        return;
    }
    if((*p) -> esq != NULL){
        Antecessor(*p, &(*p) -> esq);
        return;
    }
    Aux = *p;
    *p = (*p) -> dir;
    free(Aux);

}

void EmOrdem(Apontador p){

    if(p == NULL) return;
    EmOrdem(p -> esq);
    printf("%d ", p->reg.chave);
    EmOrdem(p -> dir);

}

int main(){

    No* Arvore; Registro x;
    char c, op;
    Inicializa(&Arvore);

    printf("Operacao I - Inserir. Formato: I 1 I 2 I 3 I 4\n");
    printf("Operacao R - Retirar. Formato: R 1 R 2 R 3 R 4\n");
    printf("Operacao P - Imprimir. Formato: I 1 R 1 I 4 P\n");
    printf("Exemplo: I 1 I 2 I 3 I 4 I 5 R 2 R 3 P\n");
    printf("\n");

    while(c != '\n')
    {
        scanf("%c%c", &op, &c);
        if((op == 'I' || op == 'R' || op == 'P'))
        {
            if(op == 'I')
            {
                scanf("%d%c", &x.chave);
                Insere(x, &Arvore);
            }
            if(op == 'R')
            {
               scanf("%d%c", &x.chave);
               Retira(x, &Arvore);
            }
            if(op == 'P')
            {
               printf("\nArvore em ordem: ");
               EmOrdem(Arvore);
            }
        }
        else
        {
            printf("opcao invalida");
            return;
        }
    }

}

