#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 05 para 28/03
Ordenação Interna - BucketSort
 */

 //Cria estrutura do balde
 // "chaves" é o vetor em si
 // "indice" armazena o último índice utilizado do vetor
struct balde
{
    int indice;
    float* chaves;
};

//Procedimento BubbleSort, usado para organizar os vetores dentro do Radix
void BubbleSort(int *vetor, int qtdElementos)
{
    int i, j, aux;
    //Para cada elemento do vetor (laço externo)
    for(i = 0; i < qtdElementos; i++)
    {
        //Compara o elemento com o próximo
        for(j = 0; j < qtdElementos; j++)
        {
            //Caso o próximo seja maior que o atual, troca
            if(vetor[j] > vetor[j+1])
            {
                aux = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = aux;
            }
        }
    }
}

//Procedimento BucketSort
void BucketSort(float *vetor, int qtdElementos, int qtdBaldes)
{
    //Declare Variáveis
    int i, j;
    struct balde bucket[qtdBaldes];

    //Inicializa baldes
    for (i = 0; i < qtdBaldes; i++)
    {
        bucket[i].indice = -1;
        bucket[i].chaves = (float*)malloc(sizeof(float) * qtdElementos);
    }

    //Preenche baldes com as entradas
    int indiceBalde, indiceVetorBalde;
    for(i = 0; i < qtdElementos + 1; i++)
    {
        //A regra para definição do balde é: valor * número de baldes
        //Caso o valor encontrado seja maior que o número de baldes, a chave será inserida no último balde
        indiceBalde = vetor[i] * qtdBaldes;
        if(indiceBalde >= qtdBaldes)
            indiceBalde = qtdBaldes - 1;

        //Soma +1 no indice
        bucket[indiceBalde].indice++;
        indiceVetorBalde = bucket[indiceBalde].indice;
        //Atribui o valor
        bucket[indiceBalde].chaves[indiceVetorBalde] = vetor[i];
    }

    //Printa baldes desordenados
    for(i = 0; i < qtdBaldes; i++)
    {
        if (bucket[i].indice > -1)
        {
            printf("%d-> ", i);
            for(j = -1; j < bucket[i].indice; j++)
            {
                printf("%7.3f", bucket[i].chaves[j+1]);
            }
            printf("\n");
        }
    }

    //Chama a função BubbleSort balde a balde
    for(i = 0; i < qtdBaldes; i++)
    {
        if (bucket[i].indice > -1)
        {
            //Chama o procedimento BubbleSort
            BubbleSort(bucket[i].chaves, bucket[i].indice);
        }
    }

    //Printa os baldes com os vetores ordenados
    printf("\n");
    for(i = 0; i < qtdBaldes; i++)
    {
        if (bucket[i].indice > -1)
        {
            printf("%d-> ", i);
            for(j = -1; j < bucket[i].indice; j++)
            {
                printf("%7.3f", bucket[i].chaves[j+1]);
            }
            printf("\n");
        }
    }
    printf("\n");

    //Printa o vetor ordenado
    for(i = 0; i < qtdBaldes; i++)
    {
        if (bucket[i].indice > -1)
        {
            for(j = -1; j < bucket[i].indice; j++)
            {
                printf("%7.3f", bucket[i].chaves[j+1]);
            }
        }
    }
}

int main()
{
    //Recebe as entradas
    int qtdElementos, i, qtdBaldes;

    //Recebe valor dos vetores
    for(i = 0; i < 2; i++)
    {
        if(i == 0)
            scanf("%d", &qtdElementos);
        else
            scanf("%d", &qtdBaldes);
    }

    //Cria os vetores
    float vetor[qtdElementos];

    //Recebe valor dos vetores
    for(i = 0; i < qtdElementos; i++)
        scanf("%f", &vetor[i]);

    //Chama o procedimento BucketSort
    BucketSort(vetor, qtdElementos - 1, qtdBaldes);
}
