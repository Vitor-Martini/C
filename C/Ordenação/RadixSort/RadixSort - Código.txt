#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 04 para 27/03/
Ordenação Interna - RadixSort
*/

//Procedimento CountingSort
void countingSort(int *vetor, int qtdElementos, int expoente)
{
    //Vetor B -> Auxiliar que recebe o vator ordenado
    //Vetor C -> Apenas 10 indices fixos pois recebe apenas UM ALGORISMO de cada chave de entrada (0 - 9)
    int i, vetorB[qtdElementos], vetorC[10], Aux = 0;

    //Inicia vetor C
    for(i = 0; i < 10; i++)
        vetorC[i] = 0;

    //C recebe como chave quantas vezes as chaves de A se repetem
    //(vetor[i]/expoente)%10 -> indica qual casa decimal está se analisando no vetor A
    for (i = 0; i < qtdElementos; i++)
        vetorC[(vetor[i]/expoente)%10 ]++;

    //Percorre C somando seus elementos
    for(i = 0; i < 10; i++)
    {
        Aux = Aux + vetorC[i];
        vetorC[i] = Aux;
    }

    //Percorre o vetor A, consultando em C a posição que deve ficar em B
    int indiceB;
    for (i = qtdElementos - 1; i >= 0; i--)
    {
        //O INDICE de B será a CHAVE de C no indice do valor da CHAVE de A - 1
        indiceB = vetorC[ (vetor[i]/expoente)%10 ] - 1;
        //Vetor B no indice encontrado recebe o valor da chave em C
        vetorB[indiceB] = vetor[i];
        //Decrementa -1 na posição em C
        vetorC[(vetor[i]/expoente)%10]--;
    }

    //Transfere o valor ordenado de B para A
    for (i = 0; i < qtdElementos; i++)
        vetor[i] = vetorB[i];
}

//Procedimento RadixSort
void RadixSort(int *vetor, int qtdElementos)
{
    int maiorValor = 0, i, expoente = 1, qtdDigitos = 0;

    //Busca a maior chave no vetor principal para usar como base
    for(i = 0; i < qtdElementos; i++)
    {
        if(vetor[i] > maiorValor)
            maiorValor = vetor[i];
    }

    //Busca quantos digitos tem o maior valor
    while(maiorValor != 0)
    {
        qtdDigitos++;
        maiorValor /= 10;
    }

    // Chama o método counting para CADA DIGITO
    for (qtdDigitos; qtdDigitos > 0; qtdDigitos--)
    {
        countingSort(vetor, qtdElementos, expoente);
        for(i = 0; i < qtdElementos; i++)
            printf("%d ", vetor[i]);
        printf("\n");
        expoente *= 10;
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

    //Chama procedimento RadixSort
    RadixSort(vetor, qtdElementos);
}
