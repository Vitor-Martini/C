#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define TAM_MEM_VIRT 100
#define TAM_MEM_FIS 50
#define CLOCKS 200

// Estrutura da memória física
typedef struct MemoriaFisica
{   
    int id_pagina[TAM_MEM_FIS]; // Armazena os ID's das páginas
    int espaco_ocupado; 
    int ultimo_id_requisitado;
    int ultimo_id_removido;
    int page_miss;
} TipoMemoriaFisica;

// Estrutura da página
typedef struct Pagina
{    
    int ID;   
    int referenciado;
    int frequencia[8];
    int frequencia_decimal;
} TipoPagina;

// Estrturua da memória virtual, isto é, um agrupamento de páginas
typedef struct MemoriaVirtual
{
    TipoPagina *Pagina;
    int tamanho;
} TipoMemoriaVirtual;

TipoMemoriaFisica *iniciar_memoria_fisica()
{
    // Aloca espaço da memória
    TipoMemoriaFisica * MemFisica;
    MemFisica = (TipoMemoriaFisica *) malloc(sizeof(TipoMemoriaFisica));

    // Preenche os id's das páginas com -1, ou seja, não há página alocada
    int i;
    for (i = 0; i < TAM_MEM_FIS; ++i)
        MemFisica->id_pagina[i] = -1; 
        
    MemFisica->ultimo_id_removido = -1;
    MemFisica->ultimo_id_requisitado = -1;
    MemFisica->espaco_ocupado = 0;
    MemFisica->page_miss = 0;
    return MemFisica;
}

TipoMemoriaVirtual *iniciar_memoria_virtual()
{
    TipoMemoriaVirtual * MemVirtual;
    MemVirtual = (TipoMemoriaVirtual *) malloc(sizeof(TipoMemoriaVirtual));
    MemVirtual->tamanho = TAM_MEM_VIRT;
    MemVirtual->Pagina = (TipoPagina *) malloc(MemVirtual->tamanho * sizeof(TipoPagina));

    // Inicia as páginas, todas com frequencia 0
    int i, j;
    for (i = 0; i < TAM_MEM_VIRT; ++i)
    {       
        MemVirtual->Pagina[i].ID = i; 
        MemVirtual->Pagina[i].referenciado = 0;  
        MemVirtual->Pagina[i].frequencia_decimal = 0;  
        for (j = 0; j < 8; ++j){
            MemVirtual->Pagina[i].frequencia[j] = 0; 
        }
    }
   
    return MemVirtual;
}

void adicionar_pagina_memoria(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual, int ID_requisicao)
{            
    int i;
    // Procura os espaços vazios na memória e aloca essa página
    for (i = 0; i < TAM_MEM_FIS; ++i){
        if(MemFisica->id_pagina[i] == -1){
            MemFisica->id_pagina[i] = ID_requisicao;            
            MemVirtual->Pagina[ID_requisicao].referenciado = 1;            
            i = TAM_MEM_FIS;
        }                
    }
    MemFisica->espaco_ocupado++;
}

// Converte a frequencia da página para decimal
int converter_binario_decimal(TipoPagina Pagina){
    int i, decimal = 0;

    for(i = 0; i <= 7; i++) 
        decimal += Pagina.frequencia[i] * pow(2, abs(i - 7));
    
    return decimal;
}

int pesquisar_pagina_menos_frequente(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual)
{    
    int i, retorno, menor, candidato_menor;
    menor = -1;

    // Dentre as páginas já alocadas em memória, procura qual é a menos frequente
    for (i = 0; i < TAM_MEM_FIS; ++i){
        if(MemFisica->id_pagina[i] != -1){
            if(menor == -1){
                menor = MemVirtual->Pagina[MemFisica->id_pagina[i]].frequencia_decimal;
                retorno = i;
            }
            if(menor > MemVirtual->Pagina[MemFisica->id_pagina[i]].frequencia_decimal){
                menor = MemVirtual->Pagina[MemFisica->id_pagina[i]].frequencia_decimal;
                retorno = i;
            }     
        }      
    }

    return retorno;
}

void page_miss(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual, int ID_requisicao)
{       
    int ID_pagina_remover;

    // Remove a página menos frequente da memória
    ID_pagina_remover = pesquisar_pagina_menos_frequente(MemFisica, MemVirtual);
    MemFisica->page_miss++;
    MemFisica->espaco_ocupado--;
    MemFisica->ultimo_id_removido = MemVirtual->Pagina[MemFisica->id_pagina[ID_pagina_remover]].ID;    
    MemVirtual->Pagina[MemFisica->id_pagina[ID_pagina_remover]].referenciado = 0;  
    MemFisica->id_pagina[ID_pagina_remover] = -1;  

    // Adiciona a nova página requisitado
    adicionar_pagina_memoria(MemFisica, MemVirtual, ID_requisicao);
}

void atualizar_frequencia(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual, int ID_requisicao)
{          
    int i, j;  
    // Percorre todas as páginas
    for(i = 0; i < MemVirtual->tamanho; ++i){

        for (j = 7; j >= 1; --j) // Atualiza a frequencia n com o valor de n-1
            MemVirtual->Pagina[i].frequencia[j] = MemVirtual->Pagina[i].frequencia[j-1];

        // Atualiza o primeiro índice da frequencia
        if(MemVirtual->Pagina[i].ID == ID_requisicao)
            MemVirtual->Pagina[i].frequencia[0] = 1;
        else
            MemVirtual->Pagina[i].frequencia[0] = 0; 

        // Guarda o valor em decimal
        MemVirtual->Pagina[i].frequencia_decimal = converter_binario_decimal(MemVirtual->Pagina[i]);      
    }   
}

void requisitar_pagina_memoria(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual)
{            
    int ID_requisicao, i, j; 

    // Sorteia uma página para ser utilizada
    ID_requisicao = rand() % TAM_MEM_VIRT;   
    MemFisica->ultimo_id_requisitado = ID_requisicao;
    MemFisica->ultimo_id_removido = -1;

    // Caso a página ainda não foi referenciada
    if(MemVirtual->Pagina[ID_requisicao].referenciado == 0){        
        if(MemFisica->espaco_ocupado < TAM_MEM_FIS) // Caso exista espaço na memória
            adicionar_pagina_memoria(MemFisica, MemVirtual, ID_requisicao);
        else // Caso contrário, ocorre um page miss
            page_miss(MemFisica, MemVirtual, ID_requisicao);
    }

    // Atualiza a frequencia
    atualizar_frequencia(MemFisica, MemVirtual, ID_requisicao);
}

void print(TipoMemoriaFisica * MemFisica, TipoMemoriaVirtual * MemVirtual)
{
    int i, j;

    // Informações da memória física
    printf("MEMORIA FISICA:\n");
    printf("    ID SOLICITADO: %d \n", MemFisica->ultimo_id_requisitado);   
    if(MemFisica->ultimo_id_removido != -1)
        printf("    PAGE MISS: REMOVIDA A PAGINA %d \n", MemFisica->ultimo_id_removido);

    printf("    PAGINAS: ");
    for (i = 0; i < TAM_MEM_FIS; ++i)
        printf("%d ", MemFisica->id_pagina[i]);
    printf("\n\n");   

    // Informações da memória virtual
    printf("MEMORIA VIRTUAL:\n");
    for (i = 0; i < TAM_MEM_VIRT; ++i){
        printf("    ID PAGINA: %d   REFERENCIADA: %d   FREQUENCIA: ", MemVirtual->Pagina[i].ID, MemVirtual->Pagina[i].referenciado);
        for (j = 0; j < 8; ++j)
            printf("%d ", MemVirtual->Pagina[i].frequencia[j]);    
         printf("-> %d \n", MemVirtual->Pagina[i].frequencia_decimal);
    }  

    printf("\nQUANTIDADE PAGE MISS: %d\n", MemFisica->page_miss);
}

int main()
{
    srand(time(NULL)); 
    int i;

    // Inicia estruturas
    TipoMemoriaFisica* MemFisica = iniciar_memoria_fisica();
    TipoMemoriaVirtual* MemVirtual = iniciar_memoria_virtual();

    // Relógio
    for (i = 1; i <= CLOCKS; ++i){
        printf("=================================================================================\n");     
        printf("TICK: %d \n\n", i);              
        requisitar_pagina_memoria(MemFisica, MemVirtual);
        print(MemFisica, MemVirtual);     
    }        
}
