#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 01 para 13/02
Ordenação Interna - HeapSort
 */

void BubbleSort(int *vetor, int qtdElementos)
{
    int i, j, aux;

    for(i = 0; i < qtdElementos; i++)
    {
        for(j = 0; j < qtdElementos; j++)
        {
            if(vetor[j] > vetor[j+1])
            {
                aux = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = aux;
            }
        }
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

    //Chama o procedimento BubbleSort
    BubbleSort(vetor, qtdElementos - 1);

    //Printa saída
    for(i = 0; i < qtdElementos; i++)
        printf("%d ", vetor[i]);
}
