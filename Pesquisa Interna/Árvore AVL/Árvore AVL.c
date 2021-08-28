#include <stdio.h>
#include <stdlib.h>

/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 07 para 15/04
Pesquisa Interna - Árvore AVL
 */

//Define estrutura de um nó
struct NO
{
    int chave;
    int fb;
    struct NO *esq;
    struct NO *dir;
};
typedef struct NO* Arvore;

//Inicia árvore
Arvore* Inicializacao()
{
    Arvore* raiz = (Arvore*) malloc(sizeof(Arvore));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

//Função pare buscar a altura do nó
int Altura(struct NO* noAnalisado)
{
    //caso a árvore esteja vazia
    if (noAnalisado == NULL)
        return -1;
    else
    {
        //Verifica a altura esquerda e direita recursivamente, retorna a maior + 1
        int altura_esquerda = Altura(noAnalisado->esq);
        int altura_direita = Altura(noAnalisado->dir);
        if (altura_esquerda < altura_direita)
            return altura_direita + 1;
        else
            return altura_esquerda + 1;
    }
}

//Função para calcular o fator de balanceamento do nó
int FB(struct NO* no)
{
    //Retorna em módulo a diferença da altura da subarvore a esqueda - a altura da subárvore a direita
    return labs(Altura(no->esq) - Altura(no->dir));
}

//Procedimento para imprimir pré ordem
void ImpressaoPreOrdem(Arvore *no)
{
    //Enquanto o nó não for nulo, vai imprimir a chave
    if(*no != NULL)
    {
        printf("%d ",(*no)->chave);
        ImpressaoPreOrdem(&((*no)->esq));
        ImpressaoPreOrdem(&((*no)->dir));
    }
}

//Função para pesquisar um valor na árvore
int Pesquisa(Arvore *no, int valor)
{
    //Define uma variável auxiliar para o nó
    struct NO* aux = *no;

    //Enquanto o nó não for nulo
    while(aux != NULL)
    {
        //Se o valor for o procurado, retorna 1
        if(valor == aux->chave)
            return 1;

        //Se o valor for maior que o procurado
        if(valor > aux->chave)
            aux = aux->dir; //nó recebe nó a direita
        else
            aux = aux->esq; // nó recebe nó a esquerda
    }

    return 0; // caso não encontre o nó
}


//Procedimentos de Rotação:
//Left left
void LL(Arvore *A)
{
    //Define nó B
    struct NO *B;

    //B recebe a esqueda de A
    B = (*A)->esq;

    //Faz a troca
    (*A)->esq = B->dir;
    B->dir = *A;

    *A = B;
}

//Right Right
void RR(Arvore *A)
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

//Left Right
void LR(Arvore *A)
{
    //Define nós auxiliares
    struct NO *B;
    struct NO *C;

    //B recebe a esquerda de A e C a direita de B
    B = (*A)->esq;
    C = B->dir;

    //Faz a troca
    B->dir = C->esq;
    C->esq = B;
    (*A)->esq = C->dir;
    C->dir = (*A);
    (*A) = C;
}

//Right Left
void RL(Arvore *A)
{
    struct NO *B;
    struct NO *C;

    //B recebe a direita de A e C a esquerda de B
    B = (*A)->dir;
    C = B->esq;

    //Faz a troca
    B->esq = C->dir;
    C->dir = B;
    (*A)->dir = C->esq;
    C->esq = (*A);
    (*A) = C;
}

//Função de inserção
int Insercao(Arvore *no, int valor)
{
    //Verifica se o valor já não está na árvore
    if (Pesquisa(no, valor) == 1)
        return 0;

    int retornar;

    //Verifica se o nó analizado é folha
    if(*no == NULL)
    {
        struct NO *novo;
        novo = (struct NO*)malloc(sizeof(struct NO));
        if(novo == NULL)
            return 0;

        novo->chave = valor;
        novo->fb = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *no = novo;
        return 1;
    }

    //Se o valor for menor que o valor do nó analizado
    struct NO *aux = *no;
    if(valor < aux->chave)
    {
        //Insere a esquerda
        if((retornar = Insercao(&(aux->esq), valor)) == 1)
        {
            //Verifica balanceamento
            if(FB(aux) >= 2)
            {
                if(valor < (*no)->esq->chave )
                    LL(no);
                else
                    LR(no);
            }
        }
    }
    else //Se o valor for maior que o nó analisado
    {
        //Insere a direita
        if((retornar = Insercao(&(aux->dir), valor)) == 1)
        {
            //Verifica balanceamento
            if(FB(aux) >= 2)
            {
                if((*no)->dir->chave < valor)
                    RR(no);
                else
                    RL(no);
            }
        }
    }

    //Define o fator de balanceamento do novo nó
    aux->fb =  FB(aux);

    return retornar;
}

int Remocao(Arvore *no, int valor)
{
    //Verifica se o valor existe na árvore
    if (Pesquisa(no, valor) == 0)
        return 0;

    int retornar;

    //Se o valor for menor que o do nó analisado
    if(valor < (*no)->chave)
    {
        //Chama a função recursivamente até encontrar o valor exato
        if((retornar = Remocao(&(*no)->esq,valor)) == 1)
        {
            //Verifica balanceamento
            if(FB(*no) >= 2)
            {
                if(Altura((*no)->dir->esq) <= Altura((*no)->dir->dir))
                    RR(no);
                else
                    RL(no);
            }
        }
    }

    //Se o valor for maior que o do nó analisado
    if(valor > (*no)->chave)
    {
        //Chama a função recursivamente até encontrar o valor exato
        if((retornar = Remocao(&(*no)->dir, valor)) == 1)
        {
            //Verifica balanceamento
            if(FB(*no) >= 2)
            {
                if(Altura((*no)->esq->dir) <= Altura((*no)->esq->esq) )
                    LL(no);
                else
                    LR(no);
            }
        }
    }

    //Se o valor analizado for exatamente o do nó analisado
    if((*no)->chave == valor)
    {
        //Caso o só tenha apenas 1 ou nenhum filho
        if(((*no)->esq == NULL || (*no)->dir == NULL))
        {
            struct NO *Aux = (*no);
            if((*no)->esq != NULL)
                *no = (*no)->esq;
            else
                *no = (*no)->dir;
            free(Aux);
        }
        else //Caso o nó tenha dois filhos
        {
            //Define os filhos
            struct NO *filho1 = (*no)->dir;
            struct NO *filho2 = filho1->esq;
            //Busca o menor elemento a direita
            while(filho2 != NULL)
            {
                filho1 = filho2;
                filho2 = filho2->esq;
            }

            //Substitui
            (*no)->chave = filho1->chave;
            //Chama a função novamente
            Remocao(&(*no)->dir, (*no)->chave);
            //Verifica balanceamento
            if(FB(*no) >= 2)
            {
                if(Altura((*no)->esq->dir) <= Altura((*no)->esq->esq))
                    LL(no);
                else
                    LR(no);
            }
        }
        //Define o fator de balanceamento para o nó
        if (*no != NULL)
            (*no)->fb = FB(*no);

        return 1;
    }

    //Define o fator de balanceamento na volta de todas as recursões
    (*no)->fb = FB(*no);

    return retornar;
}

int main()
{
	//Estrutura auxiliar para receber os valores a serem removidos
     typedef struct
    {
        int *chave;
        int max;
    } Dicionario;
	
	//Inicia estrutura
    Dicionario* lista;
    lista = (Dicionario *) malloc(sizeof(Dicionario));
    lista->max = 10;
    lista->chave = (int *) malloc(10 * sizeof(int));

    //Define e inicializa árvore
    Arvore* arv;
    int retornar, i;
    arv = Inicializacao();

	//Recebe os valores a inserir e monta a árvore
	scanf("%d", &i);
    while(i != -1)
    {
        retornar = Insercao(arv,i);
        scanf("%d", &i);
    }

	//Recebe os valores a serem removidos e os armazena na estrutura auxiliar
    int aux = 0;
    scanf("%d", &i);
    while(i != -1)
    {
		//Caso a estrutura chegue no armazenamento máximo, o espaço será realocado
        if(aux == lista->max)
        {
            lista->max = lista->max * 2;
            lista->chave = (int *) realloc(lista->chave, lista->max * sizeof(int));
        }
		//Recebe o valor
        lista->chave[aux] = i;
		//Incrementa contador
        aux++;
		//Recebe o novo valor
        scanf("%d", &i);
    }

    //Printa pré ordem da árvore montada
    ImpressaoPreOrdem(arv);
    printf("\n");
    //Print altura
    printf("%d \n",Altura(*arv));

    //Remove os elementos armazenados no dicionario
    for(i = 0; i < aux; i++)
        Remocao(arv,lista->chave[i]);

    //Printa pré ordem da árvore com os elementos retirados
    ImpressaoPreOrdem(arv);
    printf("\n");

    //Printa altura
    printf("%d ",Altura(*arv));
}
