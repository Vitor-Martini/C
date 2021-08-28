#include <stdio.h>
#include <stdlib.h>
#define NIL NULL

/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 08 para 07/05
Pesquisa Interna - Hashing
 */

 //Estrutura Célula
typedef struct Celula
{
    struct Celula *Anterior;
    struct Celula *Proximo;
    int Chave;

} TipoCelula;

//Estrutura Lista
typedef struct Lista
{
    TipoCelula *Primeiro;
    TipoCelula *Ultimo;
    int Tamanho;
} TipoLista;

//Estrutura Hash
typedef struct
{
    TipoLista *Lista;
    int maximo, qtdElementos;
} TipoHash;

//Método de Inserção
void Insercao(TipoLista *Lista, int Chave)
{
    //Aloca espaço para o novo registro
    TipoCelula* Novo = (TipoCelula*)malloc(sizeof(TipoCelula));
    Novo->Chave = Chave;

    //Caso a lista não seja vazia, ou caso seja
    if (Lista->Tamanho != 0)
    {
        Novo->Proximo = NIL;
        Novo->Anterior = Lista->Ultimo;
        Lista->Ultimo->Proximo = Novo;
        Lista->Ultimo = Novo;
    }
    else
    {
        Novo->Proximo = NIL;
        Novo->Anterior = NIL;
        Lista->Primeiro = Novo;
        Lista->Ultimo = Novo;
    }
    Lista->Tamanho++;
}

//Pesquisa na tabela hash
int Pesquisa(TipoHash *Hash, int Chave)
{
    int i = 0, Posicao;
    Posicao = Chave % Hash->maximo;

    //Começa na primeira posição da lista
    TipoCelula* Aux = Hash->Lista[Posicao].Primeiro;

    //Percorre todas as posições até encontrar
    for(i = 0; i < Hash->Lista[Posicao].Tamanho; i++)
    {
        if (Aux->Chave == Chave)
            return 1;
        else if (Aux->Proximo != NIL)
            Aux = Aux->Proximo;
    }

    return 0;
}

//Procedimento de remoção
void Remover(TipoHash *Hash, int Chave)
{
    int i = 0, Posicao;
    Posicao = Chave % Hash->maximo;

    TipoCelula* Aux = Hash->Lista[Posicao].Primeiro;

    //Primeiro pesquisa a chave na lista da tabela hash
    for(i = 0; i < Hash->Lista[Posicao].Tamanho; i++)
    {
        //Ao encontrar, remove de acordo com a situação
        if (Aux->Chave == Chave)
        {
            if (Aux->Anterior == NIL && Aux->Proximo == NIL)
            {
                Hash->Lista[Posicao].Primeiro = Aux;
                Hash->Lista[Posicao].Primeiro = Aux;
                Hash->Lista[Posicao].Tamanho = 0;
                return;
            }
            else if(Hash->Lista[Posicao].Primeiro->Chave == Aux->Chave)
            {
                Hash->Lista[Posicao].Primeiro = Aux->Proximo;
                Hash->Lista[Posicao].Primeiro->Anterior = NIL;
                Hash->Lista[Posicao].Tamanho--;
                return;
            }
            else if(Hash->Lista[Posicao].Ultimo->Chave == Aux->Chave)
            {
                Hash->Lista[Posicao].Ultimo = Aux->Anterior;
                Hash->Lista[Posicao].Ultimo->Proximo = NIL;
                Hash->Lista[Posicao].Tamanho--;
                return;
            }

            TipoCelula *Aux1, *Aux2;
            Aux1 = Aux->Anterior;
            Aux2 = Aux->Proximo;
            Aux1->Proximo = Aux2;
            Aux2->Anterior = Aux1;
            Hash->Lista[Posicao].Tamanho--;
            return;
        }
        else if (Aux->Proximo != NIL)
            Aux = Aux->Proximo;
    }



}

int main()
{
    int i = 0, j = 0, Chave, IndiceLista;
    TipoHash* Hash;

    //Inicializa Hash
    Hash = (TipoHash *) malloc(sizeof(Hash));
    scanf("%d", &Hash->maximo);
    scanf("%d", &Hash->qtdElementos);
    Hash->Lista = (TipoLista *) malloc((Hash->maximo) * sizeof(TipoLista));

    //Inicializa Listas
    for(i; i < Hash->maximo; i++)
    {
        Hash->Lista[i].Tamanho = 0;
        Hash->Lista[i].Primeiro = NIL;
        Hash->Lista[i].Ultimo = NIL;
    }

    //Insere os elementos
    for(i = 0; i < Hash->qtdElementos; i++)
    {
        scanf("%d", &Chave);
        IndiceLista = Chave % Hash->maximo;
        Insercao(&Hash->Lista[IndiceLista], Chave);
    }

    //Procedimento para o último dígito
    scanf("%d", &Chave);
    if (Pesquisa(Hash, Chave) == 0)
        Insercao(&Hash->Lista[Chave % Hash->maximo], Chave);
    else
        Remover(Hash, Chave);

    //Impressão
    for (i = 0; i < Hash->maximo; i++)
    {
        printf("[%d]", i);
        if (Hash->Lista[i].Tamanho > 0)
        {
            for (j = 0; j<Hash->Lista[i].Tamanho; j++)
            {
                if(Hash->Lista[i].Primeiro != NIL)
                {
                    printf(" %d", Hash->Lista[i].Primeiro->Chave);
                    Hash->Lista[i].Primeiro = Hash->Lista[i].Primeiro->Proximo;
                }
            }
        }
        printf("\n");
    }
}
