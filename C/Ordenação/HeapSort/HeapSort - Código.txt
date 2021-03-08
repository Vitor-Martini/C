#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 01 para 13/02
Ordenação Interna - HeapSort
 */
 
//Procedimento RefazHeap
void refazHeap(int *vetor, int qtdElementos, int pai)
{
    //Define o valor dos dois filhos  do nó analisado
    //Atribui valor somente caso exista registro
    int filho1;
    int filho2;
 
    if((2*pai+1) < qtdElementos)
        filho1 = vetor[2*pai+1];
    else
        filho1 = 0;
 
    if(2*pai+2 < qtdElementos)
        filho2 = vetor[2*pai+2];
    else
        filho2 = 0;
 
    //Caso um dos filhos seja menor que o pai
    if(vetor[pai] < filho1 || vetor[pai] < filho2)
    {
        //Compara qual o maior entre os filhos
        //Troca o valor, verifica se esses filhos tem filhos,
        //Se tiver, repete a função para eles
        if(filho1 > filho2)
        {
            vetor[2*pai+1] = vetor[pai];
            vetor[pai] = filho1;
            refazHeap(vetor, qtdElementos, 2*pai+1);
        }
        else
        {
            vetor[2*pai+2] = vetor[pai];
            vetor[pai] = filho2;
            refazHeap(vetor, qtdElementos, 2*pai+2);
        }
    }
}
 
//Procedimento para construir o heap
void constroiHeap(int *vetor, int qtdElementos)
{
    int i;
 
    //Aplica o método refaz para cada elemento do vetor
    for(i = ((qtdElementos-1)/2); i >= 0; i--)
        refazHeap(vetor, qtdElementos, i);
}
 
//Procedimento para organizar o vetor em ordem crescente
void ordenaHeap(int *vetor, int qtdElementos)
{
    //Constroi o heap
    int aux, i, k = qtdElementos;
 
    //Procedimento para organizar o vetor em ordem crescente
    while(qtdElementos > 1)
    {
        aux = vetor[qtdElementos-1];
        vetor[qtdElementos-1] = vetor[0];
        vetor[0] = aux;
        qtdElementos--;
        refazHeap(vetor, qtdElementos, 0);
 
        for(i = 0; i < k; i++)
            printf("%d ", vetor[i]);
        printf("\n");
    }
}
 
int main()
{
    //Recebe as entradas
    int qtdElementos, i;
    scanf("%d", &qtdElementos);
 
    //Cria os vetores
    int vetor[qtdElementos];
 
    //Recebe valor dos vetores
    for(i = 0; i < qtdElementos; i++)
        scanf("%d", &vetor[i]);
 
    //Constroi Heap
    constroiHeap(vetor, qtdElementos);
    for(i = 0; i < qtdElementos; i++)
        printf("%d ", vetor[i]);
    printf("\n");
 
    //Ordena o vetor
    ordenaHeap(vetor, qtdElementos);
}