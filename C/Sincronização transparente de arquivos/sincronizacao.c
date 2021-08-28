#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <pthread.h>

// Estrutura dos arquivos
typedef struct Arquivo
{    
    int analisado;
    char nome[255];
    char caminho[4000];
    long int modificacao;
} TipoArquivo;

// Estrutura dos diretórios
typedef struct Diretorio
{
    TipoArquivo * Arquivo;
    int tamanho; 
    char caminho[4000];
} TipoDiretorio;

// Inicia diretório
TipoDiretorio *iniciar_diretorio(char caminhoRaiz[4000])
{    
    struct dirent **dados;
    struct stat status = {0};
    int n, i;
    char caminhoAux[4000], caminhoArquivo[4000];

    // Verifica os arquivos daquele diretório
    n = scandir(caminhoRaiz, &dados, NULL, alphasort);

    // Caso o diretório não exista, o cria
    if(n == -1){
        mkdir(caminhoRaiz, S_IRWXU);
        printf("CRIADO: %s\n", caminhoRaiz);
        n = scandir(caminhoRaiz, &dados, NULL, alphasort);
    }

    // Formata o caminho do arquivo e o armazena na variável auxiliar
    strcpy(caminhoAux, caminhoRaiz);
    strcat(caminhoAux, "/"); 
    strcpy(caminhoArquivo, caminhoAux);

    // Aloca espaço na memória e insere o arquivo na struct
    TipoDiretorio * Diretorio;
    Diretorio = (TipoDiretorio *) malloc(sizeof(TipoDiretorio));
    Diretorio->tamanho = n - 2;
    strcpy(Diretorio->caminho, caminhoAux);
    Diretorio->Arquivo = (TipoArquivo*) malloc(Diretorio->tamanho * sizeof(TipoArquivo));

    // Percorre os arquivos daquele diretorio
    while(n--){     

        if(n == 1)
            break;

        // Formata nome do arquivo
        strcat(caminhoArquivo, dados[n]->d_name); 
        stat(caminhoArquivo, &status); 

        // Armaxena na struct
        strcpy(Diretorio->Arquivo[n - 2].caminho, caminhoArquivo);
        strcpy(Diretorio->Arquivo[n - 2].nome, dados[n]->d_name);
        Diretorio->Arquivo[n - 2].analisado = 0;        
        Diretorio->Arquivo[n - 2].modificacao = status.st_mtime;

        // Reseta o caminho raiz
        strcpy(caminhoArquivo, caminhoAux); 
        free(dados[n]);

    }

    free(dados);
  
    return Diretorio;
}

// Exluir arquivo
void excluir_arquivo(char caminho[4000]){

    struct stat statusRaiz;
    stat(caminho, &statusRaiz); 

    // Case o arquivo a ser excluído é um diretorio
    if(S_ISREG(statusRaiz.st_mode) == 0){ 
        struct stat status;
        struct dirent **dados;
        char novoArquivo[4000];
        strcpy(novoArquivo, caminho);
        int n = scandir(novoArquivo, &dados, NULL, alphasort);
        
        // Percorre os arquivos dentro do diretório
        while(n--){     

            if(n == 1)
                break;

            // Formata o nome do arquivo
            strcat(novoArquivo, "/");
            strcat(novoArquivo, dados[n]->d_name);
            stat(novoArquivo, &status); 

            // Chama recursivamente o procedimento de excluir
            excluir_arquivo(novoArquivo);

            // Reseta o caminho raiz
            strcpy(novoArquivo, caminho); 
            free(dados[n]);
        }  
    } 
    
    // Exclui o arquivo
    if (remove(caminho) == 0) 
        printf("EXCLUÍDO: %s\n", caminho);    
}

void criar_arquivo(char nome[4000], char caminhoSource[4000], char caminhoBackup[4000]){

    // Variáveis locais
    struct stat statusRaiz;
    struct utimbuf  timerBuff; 
    char novoArquivo[4000];
    unsigned char buffer[4096]; 
    int fonte, destino, err, n;
    
    // Recebe os status do caminho original
    stat(caminhoSource, &statusRaiz);

    // Formata os caminhos utilizados
    strcpy(novoArquivo, caminhoBackup); 
    strcat(novoArquivo, nome); 

    // Caso o arquivo a inserir não seja um diretório, faz uma réplica do arquivo original na pasta de backup
    if(S_ISREG(statusRaiz.st_mode) != 0){ // se não é pasta

        fonte = open(caminhoSource, O_RDONLY); 
        destino = open(novoArquivo, O_WRONLY | O_CREAT | O_EXCL, 0666);

        while (1) {
            err = read(fonte, buffer, 4000);
            n = err;
            if (n == 0) break;
            err = write(destino, buffer, n);
        }  

       if(err != -1)
            printf("CRIADO: %s.\n", novoArquivo);

        close(fonte);
        close(destino);
        
    } 

    // Seta st_mtime do arquivo do backup igual ao do arquivo original
	timerBuff.actime  = statusRaiz.st_atime;
	timerBuff.modtime = statusRaiz.st_mtime;
	utime(novoArquivo, &timerBuff);
}

// Executa a sincronização
void sincronizacao(char caminhoSource[4000], char caminhoBackup[4000]){
    
    // Inicia as estruturas com os diretórios 
    TipoDiretorio * Source = iniciar_diretorio(caminhoSource);
    TipoDiretorio * Backup = iniciar_diretorio(caminhoBackup);   
   
    int i, j, encontrado;

    // Percorre o diretório source
    for(i = 0; i < Source->tamanho; i++){

        // Variáveis locais
        struct stat statusRaiz;
        stat(Source->Arquivo[i].caminho, &statusRaiz); 
        encontrado = 0;
        
        // Caso o arquivo dentro do diretorio source seja uma pasta, vai chamar a função de sincronização recursivamente
        if(S_ISREG(statusRaiz.st_mode) == 0){ 
            char caminhoBackupAux[4000];
            strcpy(caminhoBackupAux, caminhoBackup);
            strcat(caminhoBackupAux, "/");
            strcat(caminhoBackupAux, Source->Arquivo[i].nome);

            // Percorre a pasta de backup
            for(j = 0; j < Backup->tamanho; j++){

                // Verifica se foi encontrado o arquivo atual na pasta de backup
                if(strcmp(Source->Arquivo[i].nome, Backup->Arquivo[j].nome) == 0){
                    encontrado = 1;
                    Backup->Arquivo[j].analisado = 1;
                }
            }

            // Executa a sincronização novamente
            sincronizacao(Source->Arquivo[i].caminho, caminhoBackupAux);
        } 
        else{

            // Caso o arquivo da pasta source não seja um diretório, percorre a pasta de backup
            for(j = 0; j < Backup->tamanho; j++){

                // Case o arquivo existe nas duas pastas
                if(strcmp(Source->Arquivo[i].nome, Backup->Arquivo[j].nome) == 0){

                    //Seta variáveis
                    encontrado = 1;
                    Backup->Arquivo[j].analisado = 1;

                    //Caso data de modificacao deles é diferente, então exclue o antigo e lança novamente atualizando-o
                    if(Source->Arquivo[i].modificacao != Backup->Arquivo[j].modificacao){                        
                        excluir_arquivo(Backup->Arquivo[j].caminho);
                        criar_arquivo(Source->Arquivo[i].nome, Source->Arquivo[i].caminho, Backup->caminho);
                    }
                }
            }
        }

        // Caso o arquivo existe apenas na pasta source
        if(encontrado == 0)
            criar_arquivo(Source->Arquivo[i].nome, Source->Arquivo[i].caminho, Backup->caminho);
        
    }
    
    // Percorre a pasta de backup verificando se o arquivo existe somente nela, se sim, o exclui
    for(j = 0; j < Backup->tamanho; j++){
        if(Backup->Arquivo[j].analisado == 0)    
            excluir_arquivo(Backup->Arquivo[j].caminho);        
    }
    
    free(Source);
    free(Backup);

}

// Método que executa a sincronização
void * rotina(void * caminhosParametro){

    // Variáveis locais
    char *caminhos = (char *)caminhosParametro;
    char caminhoSource[4000], caminhoBackup[4000];
	int i = 0;

    // Splita os diretórios
	char *split = strtok(caminhosParametro, "|");
	while(split != NULL)
	{
        if(i == 0)
           strcpy(caminhoSource, split);
        else
            strcpy(caminhoBackup, split);
        
		split = strtok(NULL, "|");
        i++;
	}

    // Chama a função de sincronização
    sincronizacao(caminhoSource, caminhoBackup);
}

int main()
{
    // Variáveis locais
    char caminhoSource[4000];
    char caminhoBackup[4000];

    // Recebe a entrada do usuário
    printf("Entre com o caminho original: ");
    scanf("%s", caminhoSource);   
    printf("Entre com o backup: ");
    scanf("%s", caminhoBackup); 
    printf("\n");
    
    // Concatena os diretórios que o usuário informou
    strcat(caminhoSource, "|");
    strcat(caminhoSource, caminhoBackup);
    
    // Cria thread
    pthread_t threadSincronizacao;
    
    // Laço infinito que chama a thread de execução
    while (1){
        pthread_create(&threadSincronizacao, NULL, rotina, &caminhoSource);
        pthread_join(threadSincronizacao, NULL);
        sleep(1);
    }
}
