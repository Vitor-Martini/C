#include <stdio.h>
#include <stdlib.h>
#define NIL -1
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 06 para 01/04
Pesquisa - Busca Sequencial
 */

 //Define a estrutura do item do vetor, a chave
typedef struct
{
    int Chave;
} TipoItem;

//Define a estrutura do dicionario. N é a quantidade de chaves alocadas, max é o número máximo permitido
typedef struct
{
    TipoItem *Item;
    int n, max;
} TipoDicionario;

//Função para iniciar o dicionário
TipoDicionario *Inicia_Dicionario()
{
    TipoDicionario *Dicionario;
    Dicionario = (TipoDicionario *) malloc(sizeof(TipoDicionario));
    Dicionario->n = 0;
    Dicionario->max = 10;
    Dicionario->Item = (TipoItem *) malloc(Dicionario->max * sizeof(TipoItem));
    return Dicionario;
}

//Função para pesquisar um valor no dicionário
int Pesquisa(TipoDicionario *Dicionario, int chave)
{
    int i;
    //Percorre até o último item inserido e compara chave a chave
    for(i = 0; i < Dicionario->n; i++)
    {
        if(Dicionario->Item[i].Chave == chave)
            return i;
    }
    return NIL;
}

//Função para inserir no dicionario
int Insere(TipoDicionario *Dicionario, int chave)
{
    //Verifica se já existe registro
    if(Pesquisa(Dicionario, chave) == NIL)
    {
        //Caso o número de registro tenha chegado no limite, realoca a memória
        if(Dicionario->n+1 == Dicionario->max)
        {
            Dicionario->max = Dicionario->max * 2;
            Dicionario->Item = (TipoItem *) realloc(Dicionario->Item, Dicionario->max * sizeof(TipoItem));
        }

        //0 últimp registro recebe a chave
        Dicionario->Item[Dicionario->n].Chave = chave;
        Dicionario->n++;
        return 1;
    }
    else
        return 0;
}

//Função para remover um valor do dicionário
int Remove(TipoDicionario *Dicionario, int chave)
{
    //Pesquisa qual é o índice da chave informada
    int indice = Pesquisa(Dicionario, chave);
    //Caso a chave exista:
    if(indice != NIL)
    {
        //Diminui uma unidade em N
        Dicionario->n--;
        //Variáveis auxiliares
        int *VetorAux, i, aux = 0;

        //VetorAux é alocado da mesma forma de Dicionario->Item
        VetorAux = (TipoItem *) malloc(Dicionario->max * sizeof(TipoItem));
        //A chave pesquisada recebe NIL
        Dicionario->Item[indice].Chave = NIL;

        //Vetor auxiliar recebe o vetor principal sem a chave
        for(i = 0; i < Dicionario->n; i++)
        {
            if(Dicionario->Item[i].Chave == NIL)
                aux = 1;

            if(aux == 0)
                VetorAux[i] = Dicionario->Item[i].Chave;
            else
                VetorAux[i] = Dicionario->Item[i+1].Chave;
        }

        //Libera espaço do vetor principal e esse recebe o vetor auxiliar
        free (Dicionario->Item);
        Dicionario->Item = VetorAux;
        free(VetorAux);

        //Realoca memória caso esteja sendo pouco usada
        if(Dicionario->n < (Dicionario->max * (1/3)))
        {
            Dicionario->max = Dicionario->max / 2;
            Dicionario->Item = (TipoItem *) realloc(Dicionario->Item, Dicionario->max * sizeof(TipoItem));
        }

        return 1;
    }
    else
        return 0;
}

int main()
{
    int i, valor;
    TipoDicionario* Dicionario;
    Dicionario = Inicia_Dicionario();

    //Duas estradas:
    for(i = 0; i < 2; i++)
    {
        scanf("%d", &valor);
        while(valor >= 0)
        {
            //Para a primeira entrada, só irá ser feita pesquisa e inserção
            if(i == 0)
                Insere(Dicionario, valor);
                else //Para a segunda pode ser feita todos as funções
                {
                    if (Insere(Dicionario, valor) == 0)
                        Remove(Dicionario, valor);
                }
            scanf("%d", &valor);
        }
    }

    //Printa saída
    for(i = 0; i < Dicionario->n; i++)
        printf("%d ", Dicionario->Item[i].Chave);

}
