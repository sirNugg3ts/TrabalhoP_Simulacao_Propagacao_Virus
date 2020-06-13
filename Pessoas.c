#include "main.h"
#include "Pessoas.h"




pPessoa copiaLista(pPessoa listaOG){

    //ponteiro para percorrer a lista original
    pPessoa aux = listaOG;

    //nó inicial da nova lista
    pPessoa novaLista = malloc(sizeof(Pessoa));

    if (novaLista==NULL){
        return NULL;
    }

    //copia a primeira pessoa
    copiaPessoa(listaOG,novaLista);
    aux = aux->nextPessoa;

    pPessoa percorreNovaLista = novaLista;

    while (aux != NULL){
        pPessoa newPessoa = malloc(sizeof(Pessoa));

        if (newPessoa==NULL){
            printf("\nErro ao alocar memoria");
            return NULL;
        }

        copiaPessoa(aux,newPessoa);
        newPessoa->nextPessoa=NULL;

        percorreNovaLista->nextPessoa=newPessoa;
        percorreNovaLista = percorreNovaLista->nextPessoa;

        aux = aux->nextPessoa;

    }

    return novaLista;
}

void imprimePessoa(pPessoa pessoa){
    printf("\n*****\n"
    "ID:%s\n"
    "Idade:%d\n"
    "Estado:%c\n"
    "Probabilidade Recuperacao:%f\n"
    "Duracao Max Infecao:%d\n"
    "Local Atribuido:%d\n",
    pessoa->id,
    pessoa->idade,
    pessoa->estado,
    pessoa->probabilidadeRecuperacao
    ,pessoa->duracaoMaxInfecao
    ,pessoa->idLocal);
    
    if(pessoa->estado=='D'){
        printf("Dias infetado:%d",pessoa->dias_infetado);
    }
}

void limpaListaPessoas(pPessoa p){
    pPessoa aux;

    while (p != NULL){
        aux = p;
        p = p->nextPessoa;
        free(aux);
    }
}

void leTodaGente(pPessoa p){
    pPessoa aux=p;
    while (aux!=NULL){
        imprimePessoa(aux);
        aux = aux->nextPessoa;
    }
}

int verificaDados(pPessoa newPessoa,pPessoa lista,int nPessoas){
    
    pPessoa aux = lista;
    
    for(int i=0;i < nPessoas;i++){
        if(strcmp(newPessoa->id,aux->id)==0)
            return -1;
        else
            aux = aux->nextPessoa;
    }

    if (strlen(newPessoa->id) == 0) {
        printf("\nID inválido");
        return -1;
    }
    if (newPessoa->idade < 0) {
        printf("\nIdade negativa!");
        return -1;
    }
    if (newPessoa->estado != 'S' && newPessoa->estado != 'D' && newPessoa->estado != 'I') {
        printf("\nEstado inválido");
        return -1;
    }
    if (newPessoa->estado == 'D') {
        if (newPessoa->dias_infetado < 0) {
            printf("\nDias Infetado invalido");
            return -1;
        }
    }
    return 1;
}

int copiaPessoa(pPessoa origem,pPessoa destino){

    if((strcpy(destino->id,origem->id))==NULL)
        return -1;
    destino->idLocal = origem->idLocal;
    destino->dias_infetado = origem->dias_infetado;
    destino->estado = origem->estado;
    destino->idade = origem->idade;
    destino->probabilidadeRecuperacao = origem->probabilidadeRecuperacao;
    destino->duracaoMaxInfecao = origem->duracaoMaxInfecao;
    destino->nextPessoa=NULL;
}

int exportaListaPessoas(pPessoa listaPessoas){
    
    /*
     Informação necessária guardar:
     * ID
     * Idade
     * Estado
     * Se doente, dias infetado
     */
    
    printf("\nInsira o nome do ficheiro para guardar a lista das pessoas: ");
    
    char nomeFicheiro[50];
    
    scanf(" %s",&nomeFicheiro);
    
    FILE *ficheiro = fopen(nomeFicheiro,"wt");
    
    if(ficheiro == NULL){
        printf("\nErro ao criar ficheiro");
        return -1;
    }
    
    pPessoa aux = listaPessoas;
    
    while(aux!=NULL){
        fprintf(ficheiro,"%s %d %c",aux->id,aux->idade,aux->estado);
        
        if(aux->estado=='D')
            fprintf(ficheiro," %d\n",aux->dias_infetado);
        else
            fprintf(ficheiro,"\n");
        aux = aux->nextPessoa;
    }
    fclose(ficheiro);
}

pPessoa startupPessoas(int nLocal,pLocal listaLocais,int *nPessoas,int capacidadeLocais[]){

    pPessoa lista = NULL;

    char nomeFicheiro[100];
    char buffer[30];


    //atualizar capacidadeLocais[] com o valor inicial de capacidade
    for (int i = 0; i < nLocal; ++i) {
        capacidadeLocais[i] = listaLocais[i].capacidade;
    }

    //receber nome do ficheiro
    printf("\n\n\nInsira o nome do ficheiro das pessoas: ");
    scanf(" %s",&nomeFicheiro);

    
    //abrir ficheiro para leitura ficheiro
    FILE* ficheiro = fopen(nomeFicheiro, "rt");

    if (ficheiro == NULL) {
        printf("\nErro ao abrir ficheiro");
        return NULL;
    }

    while(fscanf(ficheiro,"%s",buffer)!=EOF) {

        pPessoa newPessoa = malloc(sizeof(Pessoa));

        if (newPessoa == NULL) {
            printf("\nErro ao alocar memoria");
            return NULL;
        }

        strcpy(newPessoa->id, buffer);  //copiar o nome para a Pessoas
        fscanf(ficheiro, "%d", &newPessoa->idade); //guardar idade
        fscanf(ficheiro, " %c", &newPessoa->estado); //guardar estado
        newPessoa->idLocal = -1; //ainda nao tem id atribuido, por isso -1
        newPessoa->nextPessoa = NULL;


        //se o estado for doente, o ficheiro tem que ter o nº de dias infetado
        if (newPessoa->estado == 'D') {
            fscanf(ficheiro, " %d", &newPessoa->dias_infetado);
        }

        if(verificaDados(newPessoa,lista,*nPessoas)==-1)
            printf("\nPessoa invalida, a ir para a proxima");
        else{
            newPessoa->idLocal=listaLocais[0].id;


        //atribuir um local a esta pessoa
        int flag = 0; //caso não exista mais espaco em lado nenhum, nao vale a pena tentar atribuir um local, esta pessoa sera ignorada

        for (int i = 0; i < nLocal; i++) {
            if (capacidadeLocais[i] > 0)
                flag = 1; //existe pelo menos um local com espaço
                break; //nao vale a pena procurar mais, existe ja pelo menos um local
        }

        if (flag == 1) {
        do { // vou estar em loop porque caso este escolha um numero invalido, voltamos a tentar

            int localAtribuido = intUniformRnd(0, nLocal - 1); //da posicao 0 à posicao n-1

            if (capacidadeLocais[localAtribuido] > 0) {
                newPessoa->idLocal = listaLocais[localAtribuido].id;
                capacidadeLocais[localAtribuido]--;
            }

        } while (newPessoa->idLocal == -1); //a pessoa continuará com o ID a -1 se não foi escolhido um local válido

    } else{
            printf("\nJá não existe espaco para novas pessoas!");
            return lista;
        }

        //duracao maxima da infecao
        newPessoa->duracaoMaxInfecao=5+(int)(newPessoa->idade/10);

        //probabilidade recuperacao
        int prob = (1/((float)(newPessoa->idade)))*100;;
        newPessoa->probabilidadeRecuperacao=(float)prob/100;

        (*nPessoas)++;

       if (lista==NULL){
           lista=newPessoa; //se a lista estiver vazia, esta pessoa é o primeiro nó
       }
       else{
           pPessoa aux=lista;
           while(aux->nextPessoa!=NULL)
               aux=aux->nextPessoa;
           aux->nextPessoa=newPessoa;
       }
        }  
    }
    leTodaGente(lista);
    return lista;
}

