#include <stdio.h>
#include <stdlib.h>
 
#define RUBRO 1
#define NEGRO 0
 
//Definido: 1 para rubro, 0 para negro
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 08 para 29/04
Pesquisa Interna - Árvore Rubro-Negra
 */
 
 
//Estrutura No
typedef struct NO
{
    int chave;
    struct NO *dir;
    struct NO *esq;
    int cor;
} NO;
 
 
//Estrutura árvore, guarda a raiz
typedef struct
{
    NO *root;
} Arvore;
 
 
//Função para retornar a altura negra
int AlturaNegra(NO *no)
{
    //Inicia esquerda e direita
    int esq = 0, dir = 0;
 
    //Se a esquerda do nó for nulo, inicia recursão a esquerda, o mesmod vlor para a direita
    if (no->esq != NULL)
        esq = AlturaNegra(no->esq);
    if (no->dir != NULL)
        dir = AlturaNegra(no->dir);
 
    //Ao fim da recursão, verifica qual dos caminhos é o maior
    if (esq > dir)
    {
        if (no->cor == NEGRO)
            return esq + 1;
        else
            return esq;
    }
    else
    {
        if (no->cor == NEGRO)
            return dir + 1;
        else
            return dir;
    }
}
 
 
//Função para verificar se a árvore satisfaz as condições de rubro negra
int ArvoreARN (NO *no)
{
    //Se a árvore for vazia
    if (no == NULL)
        return 1;
    else
    {
        //Verifica se os filhos são negros, e o pai é rubro
        if ((no->dir->cor == NEGRO || no->dir == NULL) && (no->esq->cor == NEGRO || no->esq == NULL) && (no->cor == RUBRO))
            return 1;
 
        //Verifica a altura negra dos dois fihos
        if (AlturaNegra(no->esq) == AlturaNegra(no->dir))
            return 1;
    }
    return 0;
}
 
 
//Procedimento para executar uma coloração em um dado nó
void TrocaCores(NO *no)
{
    //Troca a cor
    if(no->cor == NEGRO)
        no->cor = RUBRO;
    else
        no->cor = NEGRO;
 
    //Se tiver filho esquerdo, troca a cor
    if (no->esq != NULL)
    {
        if(no->esq->cor == NEGRO)
            no->esq->cor = RUBRO;
        else
            no->esq->cor = NEGRO;
    }
 
    //Se tiver filho direito, troca a cor
    if (no->dir != NULL)
    {
        if(no->dir->cor == NEGRO)
            no->dir->cor = RUBRO;
        else
            no->dir->cor = NEGRO;
    }
 
    return;
}
 
 
//Procedimentos de Rotação:
//Left left
void LL(NO **A)
{
    //Define nó B
    struct NO *B;
 
    //B recebe a esqueda de A
    B = (*A)->esq;
 
    //Faz a troca
    (*A)->esq = B->dir;
    B->dir = (*A);
 
    (*A) = B;
}
 
 
//Right Right
void RR(NO **A)
{
    //Define nó B
    struct NO *B;
 
    //B recebe a direita de A
    B = (*A)->dir;
 
    //Realiza a troca
    (*A)->dir = B->esq;
    B->esq = (*A);
 
    (*A) = B;
}
 
 
void RotacaoEsquerda(NO **A, NO **B, NO **C)
{
    //A -> Analisado
    //C -> Avô
 
 
    //Verifica se o tio é rubro
    if ((*C)->dir != NULL && (*C)->dir->cor == RUBRO)
    {
        TrocaCores(*C);
        return;
    }
 
    //Caso contrário
    if ((*A)->dir == (*B))
        RR(A);
 
    //A recebe a cor de C e vice-versa
    if((*A)->cor == NEGRO)
        (*A)->cor = RUBRO;
    else
        (*A)->cor = NEGRO;
 
    if((*C)->cor == NEGRO)
        (*C)->cor = RUBRO;
    else
        (*C)->cor = NEGRO;
 
    if (*C != NULL)
        LL(C);
}
 
 
void RotacaoDireita(NO **A, NO **B, NO **C)
{
    //Verifica se o tio é rubro
    if ((*C)->esq != NULL && (*C)->esq->cor == RUBRO)
    {
        TrocaCores(*C);
        return;
    }
 
    //Caso contrário
    if ((*B)==(*A)->esq)
        LL(A);
 
    //A recebe a cor de C e vice-versa
    if((*A)->cor == NEGRO)
        (*A)->cor = RUBRO;
    else
        (*A)->cor = NEGRO;
 
    if((*C)->cor == NEGRO)
        (*C)->cor = RUBRO;
    else
        (*C)->cor = NEGRO;
    if (*C != NULL)
        RR(C);
}
 
 
//Procedimento para verificar o nó e efetuar o seu balanceamento
void BalancaNo (NO **A, NO **C, NO **B)
{
    //A -> analisado
    //B -> Pai
    //C -> Avô
 
    if (C != NULL)
    {
        //pai é rubro
        if ((*A)->cor == RUBRO && (*B)->cor == RUBRO)
        {
            int x  = (*A)->chave;
 
            //Se A for filho esquerdo, caso contrario
            if ((*C)->esq !=NULL && x == (*C)->esq->chave)
                RotacaoEsquerda(A, B, C);
            else if ((*C)->dir !=NULL && x == (*C)->dir->chave)
                RotacaoDireita(A, B, C);
        }
    }
}
 
//Procedimento para imprimir
void Imprime(NO *no )
{
    if (no != NULL)
    {
        printf("(");
        if (no->cor == RUBRO)
            printf("R%d",no->chave);
        else
            printf("N%d",no->chave);
 
        Imprime(no->esq);
        Imprime(no->dir);
        printf(")");
    }
    else
        printf("()");
}
 
 
//Procedimento para inserir
//A -> Nó a inserir
//chave -> informação
//C -> Pai, usado na recursão
void Inserir(NO **A, int chave, NO **C)
{
    if (*A == NULL)
    {
        //Cria o novo nó
        (*A) = (NO*)malloc(sizeof(NO));
        (*A)->chave = chave;
        (*A)->cor = RUBRO;
        (*A)->esq = NULL;
        (*A)->dir = NULL;
        return;
    }
    else
    {
        if (chave < (*A)->chave)
        {
            //Se o nó a inserir for menor que o nó analisado
            Inserir(&(*A)->esq, chave, A);
            BalancaNo(A, C, &(*A)->esq);
            return;
        }
        else if (chave > (*A)->chave)
        {
            //Se o nó a inserir for maior que o nó analisado
            Inserir(&(*A)->dir, chave, A);
            BalancaNo(A, C, &(*A)->dir);
            return;
        }
    }
}
 
 
main()
{
    int qtdElementos, i, chave;
    //Inicia árvore
    Arvore *arvoreRN = (Arvore*)malloc(sizeof(Arvore));
    arvoreRN->root = NULL;
 
    //Recebe as entradas
    scanf("%d", &qtdElementos);
 
    for (i = 0; i < qtdElementos; i++)
    {
        //Recebe valor
        scanf("%d", &chave);
 
        //Inserir a partir da raiz
        Inserir(&arvoreRN->root, chave, NULL);
 
        //Torna a raiz sempre negra
        arvoreRN->root->cor = 0;
    }
 
    //Caso a árvore seja Rubro-Negra, printa
    if(1 == ArvoreARN(arvoreRN->root))
    {
        //Printa a altura negra
        printf("%d",AlturaNegra(arvoreRN->root));
        printf("\n");
        //Printa a árvore
        Imprime(arvoreRN->root);
    }
}