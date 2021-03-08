#include <stdio.h>
#include <stdlib.h>
/**<
Vitor Galioti Martini
RA: 135543
AED II Noturno - Regina
Judge 01 para 18/02
Ordenação Interna - MergeSort
 */
 
//Procedimento RefazHeap
void intercalaMerge(int *vetor, int inicio, int meio, int fim)
{
    int *vetorAux, tamanho, i, j, k = inicio;
 
    //Inicio refente ao vetor 1 e inicio referente ao vetor 2
    int p1 = inicio, p2 = meio + 1;
 
    //Fim refente ao vetor 1 e fim referente ao vetor 2
    int fim1 = 0, fim2 = 0;
 
    //Tamanho do vetor
    tamanho = fim - inicio + 1;
 
    //Aloca memória para o vetor auxiliar
    vetorAux = (int *) malloc (tamanho*sizeof(int));
 
        //Percorre os dois vetores
        for(i = 0; i < tamanho; i++)
        {
            //Caso nenhum deles tenha chegado ao fim
            if(fim1 == 0 && fim2 == 0)
            {
                //Verifica qual o maior valor entre os dois vetores e insere no vetor auxiliar
                if(vetor[p1] < vetor[p2])
                    vetorAux[i] = vetor[p1++];
                else
                    vetorAux[i] = vetor[p2++];
 
                //Verifica se foi percorrido todo o vetor (referente ao vetor 1 e 2 analisados)
                if(p1 > meio)
                    fim1 = 1;
                if(p2 > fim)
                    fim2 = 1;
            }
            else
            {
                //Caso um dos vetores chegou ao fim e o outro não, copia o que sobrar
                if(fim1 == 0)
                    vetorAux[i] = vetor[p1++];
                else
                    vetorAux[i] = vetor[p2++];
            }
        }
 
          //Copia do auxiliar para o original
        for(j = 0; j < tamanho; j++)
        {
            vetor[k] = vetorAux[j];
            k++;
        }
 
    //Libera memória
    free(vetorAux);
}
 
 
//Procedimento para organizar o vetor em ordem crescente
void ordenaMerge(int *vetor, int inicio, int fim, int qtdElementos)
{
    int meio, i;
    if(inicio < fim)
    {
        //Define o meio do vetor
        meio = (inicio + fim)/2;
 
        //Chama o procedimento de ordenar para o primeiro vetor
        ordenaMerge(vetor, inicio, meio, qtdElementos);
 
        //Chama o procedimento de ordenar para o segundo vetor
        ordenaMerge(vetor, meio+1, fim, qtdElementos);
 
        //Intercala resultados
        intercalaMerge(vetor, inicio, meio, fim);
 
        //Printa saida
        for(i = 0; i < qtdElementos; i++)
            printf("%d ", vetor[i]);
 
        printf("\n");
    }
}
 
int main()
{
    //Recebe as entradas
    int qtdElementos, i;
    scanf("%d", &qtdElementos);
 
    //Cria o vetor
    int vetor[qtdElementos];
 
    //Recebe valor do vetor
    for(i = 0; i < qtdElementos; i++)
        scanf("%d", &vetor[i]);
 
    //Chama o procedimento de ordenarMerge
    ordenaMerge(vetor, 0, qtdElementos-1, qtdElementos);
}