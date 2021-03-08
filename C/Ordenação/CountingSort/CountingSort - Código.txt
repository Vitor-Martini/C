#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 01 para 13/02
Ordenação Interna - HeapSort
*/

//Procedimento para organizar o vetor em ordem crescente
void countingSort(int *vetor, int qtdElementos)
{
    //Declare variáveis
    int i, Aux = 0, maiorValor = 0;

    //Busca a maior chave no vetor principal
    for(i = 0; i < qtdElementos; i++)
    {
        if(vetor[i] > maiorValor)
            maiorValor = vetor[i];
    }

    //Cria vetores auxiliares
    int vetorC[maiorValor], vetorB[qtdElementos];

    //Inicia vetor C
    for(i = 0; i <= maiorValor; i++)
        vetorC[i] = 0;

    //C recebe como chave quantas vezes as chaves de A se repetem
    for(i = 0; i < qtdElementos; i++)
        vetorC[vetor[i]]++;

    //Percorre C somando seus elementos
    for(i = 0; i <= maiorValor; i++)
    {
        Aux = Aux + vetorC[i];
        vetorC[i] = Aux;
    }

    int indiceB;


    //Percorre o vetor A, consultando em C a posição que deve ficar em B
    for(i = qtdElementos - 1; i >= 0; i--)
    {
        //O INDICE de B será a CHAVE de C no indice do valor da CHAVE de A - 1
        indiceB = vetorC[vetor[i]] - 1;
        //Vetor B no indice encontrado recebe o valor da chave em C
        vetorB[indiceB] = vetor[i];
        //Decrementa -1 na posição em C
        vetorC[vetor[i]]--;
    }

    //Printa vetor ordenado em B
    for(i = 0; i < qtdElementos; i++)
        printf("%d ", vetorB[i]);
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

    //Ordena o vetor
    countingSort(vetor, qtdElementos);
}
