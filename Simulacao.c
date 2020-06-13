#include "Simulacao.h"
#include "main.h"

pPessoaLocal criaLista(int nPessoas, pPessoa aux, pLocal espaco, int i, int *nPessoasLocal);
void infeta(int nPessoasLocal,int nPessoasTotal,pPessoaLocal listaPessoasLocal,int TaxaDisseminacao);
void recuperacao(pPessoaLocal listaPessoas, int nPessoas, float taxaimunidade);

void limpaListaPessoaLocal(pPessoaLocal lista){
   pPessoaLocal aux;

    while (lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

//CASO 1
int avancaIteracao(pLocal espaco, pPessoa listaPessoas, int nPessoas, int nLocal, float TaxaImunidade) {

    //percorrer todos os locais
    for (int i = 0; i < nLocal; ++i) {
        
        printf("\n*****************\n"
               "Local [%d]:",espaco[i].id);

        //a lista é refeita todas as iterações, pode haver novos doentes ou alguem foi transferido

        //lista das pessoas do local na posicao [i] do array dinamico ESPACO
        pPessoaLocal listaPessoasLocal = NULL;

        //para percorrer as pessoas
        pPessoa aux = listaPessoas;

        //numero de pessoas neste local
        int nPessoasLocal = 0;

        //percorrer todas as pessoas a ver quem é que pertence a este local
        /*
         * aux -> lista de todas as pessoas
         * i -> posicao no array espaco
         * nPessoasLocal -> ponteiro para a variável que irá guardar quantas pessoas foram adicionadas a lista
         */

        listaPessoasLocal = criaLista(nPessoas, aux, espaco, i, &nPessoasLocal);
        
        

        if (listaPessoasLocal == NULL)
            return -1;

        //lista está feita, siga fazer a iteração
        //TaxaDeDisseminacao vai ser o numero de pessoas que vai ser infetada
        int TaxaDeDisseminacao = (float) (0.05 * nPessoasLocal);

        if(nPessoasLocal!=0){
            if (nPessoasLocal-1>TaxaDeDisseminacao){
                infeta(nPessoasLocal,nPessoas,listaPessoasLocal,TaxaDeDisseminacao);
            } else{
                printf("\nO local tem apenas 0 ou 1 pessoa, impossivel infetar");
            }
            recuperacao(listaPessoasLocal, nPessoasLocal, TaxaImunidade);
        }
        limpaListaPessoaLocal(listaPessoasLocal);
    }

    //no fim de percorrer todos os locais, vamos acrescentar 1 dia aos dias infetados
    pPessoa aux = listaPessoas;
    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado=='D'){
            (aux->dias_infetado)++;
            printf("\nA pessoa com ID %s ficou mais um dia infetada",aux->id);
        }
        aux = aux->nextPessoa;
    }
}
//

//CASO 2
int adicionaDoente(pLocal espaco, pPessoa listaPessoas, int nLocais, int *nPessoas, int *capacidadeLocais) {

    char buffer[30];
    int localEscolhido;
    int flag = 0;

    pPessoa newPessoa = malloc(sizeof(Pessoa));

    newPessoa->estado = 'D';
    newPessoa->nextPessoa = NULL;

    //obter informacao do novo doente
    printf("\nInserir novo doente:");
    printf("\nEscreva o ID (max 30 char): ");

    scanf("%s", &buffer);
    strcpy(newPessoa->id, buffer);

    printf("\nDigite a idade do doente: ");
    scanf("%d", &newPessoa->idade);

    printf("\nHa quantos dias se encontra o doente infetado? ");
    scanf("%d", &newPessoa->dias_infetado);

    if (verificaDados(newPessoa,listaPessoas,*nPessoas) != 1) {
        return -1;
    }

    printf("\nInsira o local onde quer inserir o novo doente: ");
    scanf("%d", &localEscolhido);


    //check se o local escolhido existe
    for (int i = 0; i < nLocais; ++i) {
        if (espaco[i].id == localEscolhido) {

            //o local existe
            //temos que verificar se tem espaco
            if (capacidadeLocais[i] > 0) {

                //ponteiro para percorrer a lista das pessoas
                pPessoa aux = listaPessoas;

                //ir ate ao ultimo
                while (aux->nextPessoa != NULL)
                    aux = aux->nextPessoa;

                //inserir a nova pessoa no fim da lista
                aux->nextPessoa = newPessoa;

                //inserir o local na pessoa
                newPessoa->idLocal = localEscolhido;

                //calcular duracao maxima da infeção
                newPessoa->duracaoMaxInfecao = 5 + (int) (newPessoa->idade / 10);

                //calcular probabilidade de recuperar
                int prob = (1 / ((float) (newPessoa->idade))) * 100;
                newPessoa->probabilidadeRecuperacao = (float) prob / 100;

                newPessoa->nextPessoa=NULL;

                //inserimos com sucesso a pessoa, vamos tirar a 1 à capacidade do local
                (capacidadeLocais[i])--;

                //adiciona 1 ao numero de pessoas existentes
                (*nPessoas)++;

                printf("\nLi esta pessoa:");
                imprimePessoa(newPessoa);
                flag = 1;//para nao aparecer mensagem de local invalido
                break;
            } else {
                printf("\nO local esta cheio");
            }
        }
    }
    if (!flag) { // se o local for invalido a flag nunca passa a 1
        printf("\nLocal invalido\nA retornar ao menu...");
        return 0;
    }
}
//

//CASO 3
int estatisticas(int nLocais, int nPessoas, pPessoa listaPessoas, pLocal espaco) {

    //Percentagem de pessoas por local
    int pessoasEmCadaLocal[nLocais];

    for (int i = 0; i < nLocais; ++i) {
        pessoasEmCadaLocal[i] = 0;
    }

    pPessoa aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        for (int j = 0; j < nLocais; ++j) {
            if (aux->idLocal == espaco[j].id) {
                pessoasEmCadaLocal[j]++;
                break;
            }
        }
        aux = aux->nextPessoa;
    }

    float percentagens[nLocais];

    for (int i = 0; i < nLocais; ++i) {
        percentagens[i] = ((float) pessoasEmCadaLocal[i] / nPessoas) * 100;
    }

    printf("\nPercentagem de pessoas por local:");
    for (int i = 0; i < nLocais; ++i) {
        printf("\nO local %d tem %.2f %% das pessoas", espaco[i].id, percentagens[i]);
    }
    printf("\n\n*******************\n");
    //

    //Taxa de pessoas saudáveis
    //aux volta ao inicio

    aux = listaPessoas;

    float percentagemSaudaveis = 0;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado == 'S')
            percentagemSaudaveis++;
        aux = aux->nextPessoa;
    }
    percentagemSaudaveis = (percentagemSaudaveis / nPessoas) * 100;

    printf("\nPercentagem de pessoas saudaveis: %.2f %%", percentagemSaudaveis);
    //

    //Taxa de pessoas Infetadas
    float percentagemInfetados = 0;

    aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado == 'D')
            percentagemInfetados++;
        aux = aux->nextPessoa;
    }

    percentagemInfetados = (percentagemInfetados / nPessoas) * 100;

    printf("\nPercentagem de pessoas infetadas: %.2f %%", percentagemInfetados);
    //

    //Taxa de pessoas imunes
    float percentagemImunes = 0;

    aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado == 'I')
            percentagemImunes++;
        aux = aux->nextPessoa;
    }

    percentagemImunes = (percentagemImunes / nPessoas) * 100;

    printf("\nPercentagem de pessoas Imunes: %.2f %%", percentagemImunes);
    //

}
//

//CASO 4 - TRANSFERIR PESSOAS
int transferePessoas(int idOrigem,int idDestino,int N,int *capacidadeLocais,int nLocais,pLocal espaco,int nPessoasTotal,pPessoa listaTodasPessoas){

    /*
     * idOrigem - ID de onde se vai tirar as pessoas
     * idDestino - ID onde se vai por as pessoas
     * N - numero de pessoas a transferir
     * capacidadeLocais - Array com a capacidade atual de cada local
     * nLocais - numero de locais existentes
     * espaco - array com os locais
     * nPessoasTotal - numero total de pessoas existentes
     * listaTodasPessoas - lista ligadas das pessoas existentes
     */

    //se nao houver lugar para as pessoas não vale a pena tentar
    int posId = -1; //posId do destino

    //obter posicao onde esta o id
    for (int i = 0; i < nLocais; ++i) {
        if (espaco[i].id==idDestino)
            posId=i;
    }

    if (capacidadeLocais[posId]<N){
        printf("\nImpossivel transferir %d pessoas, nao existe espaco suficiente!\n"
               "Espaco disponivel no local: %d"
                ,N,capacidadeLocais[posId]);
    }
    else{//ha espaco, vamos verificar se tem ligacao
        int flag = 0;

        for (int i = 0; i < 3; ++i) {
            if (espaco[posId].liga[i]==idOrigem){
                flag=1; // foi encontrada a ligacao (ja se sabe que ha ligacao de volta devido a verificacao efetuada logo no inicio)
            }
        }
        if (flag){

            //vamos transferir as pessoas
            //obter lista das pessoas que pertecem ao local original

            //obter posicao no array ESPACO do IdOrigem
            int posIDOrigem = -1;

            for (int i = 0; i < nLocais; ++i) {
                if (espaco[i].id==idOrigem){
                    posIDOrigem=i;
                    break;
                }
            }
            pPessoaLocal listaPessoasLocal = NULL;

            int nPessoasLocal = 0; //nos ja sabemos este valor em capacidadeLocais[], mas para nao alterar a composicao
                                   //da funcao existente, criamos esta variavel

            listaPessoasLocal = criaLista(nPessoasTotal,listaTodasPessoas,espaco,posIDOrigem,&nPessoasLocal);

            if (nPessoasLocal<N){
                printf("\nO local de origem nao tem pessoas suficientes para serem transferidas");
                return -1;
            }
            if (listaPessoasLocal==NULL){
                printf("\nErro ao criar lista das pessoas");
                return -1;
            }
            
            //escolher N pessoas ao calhas
            pPessoa pessoasATransferir[N]; //array com as pessoas que tem que se alterar o id do local
            int posPessoasaTransferir[N]; //array com as posicoes de quem vai ser transferido

            //preencher array para evitar erros
            for (int l = 0; l < N; ++l) {
                posPessoasaTransferir[l]=-1;
            }
            for (int m = 0; m < N; ++m) {
                pessoasATransferir[m]=NULL;
            }

            printf("\nNumero de pessoas a transferir = %d",N);

            int npessoasPAT =0;

            for (int j = 0; j < N; ++j) {
                int posPessoaATransferir; //esta vai ser a pessoa escolhida

                int flagdo = 0; // passa a 0

                //escolher uma pessoa
                do {
                    posPessoaATransferir = intUniformRnd(0,nPessoasLocal-1); //escolhe uma pessoa entre 0 e nPessoasLocal
                    flagdo = 0;

                    for (int i = 0; i < npessoasPAT; ++i) {

                        //verifica se essa pessoa ja foi escolhida antes
                        if (posPessoasaTransferir[i]==posPessoaATransferir){ //se foi, flag = 1 o que implica voltar atras para escolher outra pessoas
                            flagdo=1;
                            break;
                        }
                    }
                }while (flagdo);

                //a pessoa ja foi escolhida, vamos adiciona-la ao array de pessoas a mexer e por a sua posicao no array

                pPessoaLocal aux = listaPessoasLocal;
                for (int i = 0; i < posPessoaATransferir; ++i) {
                    aux = aux->next;
                }

                pessoasATransferir[npessoasPAT]=aux->pessoa;
                posPessoasaTransferir[npessoasPAT]=posPessoaATransferir;
                npessoasPAT++;

            }

            //transferir as pessoas
            for (int k = 0; k < N; ++k) {
                pessoasATransferir[k]->idLocal=idDestino;
            }

            printf("\nJa transferi as pessoas");
            //vamos diminuir a capacidade do local
            capacidadeLocais[posId] -= N;
            capacidadeLocais[posIDOrigem] += N;
            printf("\nJa alterei a capacidade dos locais");
        } else{ //nao ha ligacao
            printf("\nErro: Os locais nao tem ligacao direta!");
            return -1;
        }
    }
    return 1;
}
//

//FUNÇÕES EXTRA
void infeta(int nPessoasLocal,int nPessoasTotal,pPessoaLocal listaPessoasLocal, int TaxaDisseminacao) {
    
    /*
     * O infeta vai agir uma vez por cada local
     */

    //Criar um array com todos
    pPessoa todosNoLocal[nPessoasLocal];

    int pessoasAdicionadas =0;
    int pessoasDoentes = 0;

    pPessoaLocal aux = listaPessoasLocal;

    for (int k = 0; k < nPessoasLocal; ++k) {
        todosNoLocal[k]=NULL;
    }

    for (int i = 0; i < nPessoasLocal; ++i) {
            todosNoLocal[pessoasAdicionadas]=aux->pessoa;
            pessoasAdicionadas++;
            if (aux->pessoa->estado=='D'){
                pessoasDoentes++;
            }
        aux = aux->next;
    }

    //criar um array com as pessoas doentes
    pPessoa listaPessoasDoentes[pessoasDoentes];
    int pessoasListaDoentes=0;

    for (int i = 0; i < nPessoasLocal; ++i) {
        if (todosNoLocal[i]->estado=='D'){
            listaPessoasDoentes[pessoasListaDoentes]=todosNoLocal[i];
            pessoasListaDoentes++;
        }
    }
    
   printf("\n");
   printf("\nExistem de momento %d pessoas doentes neste local",pessoasListaDoentes);
   printf("\nCada pessoa vai ter que infetar %d pessoas",TaxaDisseminacao);
   printf("\n*************************************");

    //para cada pessoas doente, vamos escolher pessoas para infetar
    for (int i = 0; i < pessoasListaDoentes; ++i) {

        //array com as pessoas que vai infetar
        //este array guarda o numero da posicao onde a pessoa a infetar está
        int pessoasAinfetar[TaxaDisseminacao];

        //vamos escolher <TaxaDisseminacao> pessoas a infetar
        for (int j = 0; j < TaxaDisseminacao; ++j) {
            int flag = 1;
            do {
                int pessoaEscolhida = intUniformRnd(0,nPessoasLocal-1);
                if (strcmp(listaPessoasDoentes[i]->id,todosNoLocal[pessoaEscolhida]->id)!=0){ //significa que nao escolheu ele proprio
                    pessoasAinfetar[j]=pessoaEscolhida;
                    flag = 0;
                }
            }while (flag);
        }

        //vamos infetar as pessoas
        for (int j = 0; j < TaxaDisseminacao; ++j) {
            if (todosNoLocal[pessoasAinfetar[j]]->estado=='S'){
                todosNoLocal[pessoasAinfetar[j]]->estado='D';
                todosNoLocal[pessoasAinfetar[j]]->dias_infetado=0;
                printf("\nEstou a infetar a pessoa com ID %s",todosNoLocal[pessoasAinfetar[j]]->id);
            } else{
                printf("\nA pessoa escolhida (com ID %s), nao foi infetada, pois o estado dele e':%c",todosNoLocal[pessoasAinfetar[j]]->id,todosNoLocal[pessoasAinfetar[j]]->estado);
            }
        }
    }
}

void imunidade(float TaxaImunidade, pPessoa p) {
    if (probEvento(TaxaImunidade)){
        p->estado = 'I';
        printf("\nA pessoa com id %s ficou imune ao virus!",p->id);
    }
}

void recuperacao(pPessoaLocal listaPessoas, int nPessoas, float taxaimunidade) {

    //ponteiro para percorrer as pessoas
    pPessoaLocal aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->pessoa->estado == 'D') {
            if (probEvento(aux->pessoa->probabilidadeRecuperacao)) {
                //ver se fica imune
                imunidade(taxaimunidade, aux->pessoa);
                //se nao ficar imune fica saudavel
                if (aux->pessoa->estado != 'I')
                    aux->pessoa->estado = 'S';
                printf("\nA pessoa com id %s ficou curada naturalmente!",aux->pessoa->id);
            }
        }
        aux = aux->next;
    }

    //agora verificar dias
    //vamos voltar ao inicio da lista
    aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->pessoa->estado == 'D')
            if (aux->pessoa->dias_infetado >= aux->pessoa->duracaoMaxInfecao) {
                    aux->pessoa->estado = 'S';
                    printf("\nA pessoa com id %s ficou curada passado o montante maximo de dias!",aux->pessoa->id);
            }
        aux = aux->next;
    }
}

pPessoaLocal criaLista(int nPessoas, pPessoa aux, pLocal espaco, int i, int *nPessoasLocal) {
    

    pPessoaLocal listaPessoasLocal = malloc(sizeof(PessoaLocal));
    
    if (listaPessoasLocal == NULL) {
        printf("\nErro ao alocar memoria");
        return NULL;
    } else {
        listaPessoasLocal->next = NULL;
        listaPessoasLocal->pessoa = NULL;
    }

    for (int j = 0; j < nPessoas; ++j) {

        //se o idLocal da pessoas for o mesmo id do local onde estamos
        if (aux->idLocal == espaco[i].id) {

            //se a lista estiver fazia, então a primeira pessoa é esta que estamos
            if (listaPessoasLocal->pessoa == NULL) {
                listaPessoasLocal->pessoa = aux;
                listaPessoasLocal->next = NULL;

                (*nPessoasLocal)++;

            } else {//Já existe alguem, vamos até lá

                //ponteiro que ajuda a percorrer a lista
                pPessoaLocal auxPL = listaPessoasLocal;

                while (auxPL->next != NULL)
                    auxPL = auxPL->next;

                //alocar memoria para a nova pessoaLocal
                pPessoaLocal temp = malloc(sizeof(pPessoaLocal));

                if (temp == NULL) {
                    printf("\nErro ao alocar memoria");
                    return NULL;
                }

                temp->pessoa = aux; //associar a pessoa
                temp->next = NULL;
                auxPL->next = temp; //por o ultimo da lista a apontar para o novo elemento

                (*nPessoasLocal)++;
            }
        }

        if (aux->nextPessoa != NULL)
            aux = aux->nextPessoa;
        else if (aux->nextPessoa == NULL) {
            return listaPessoasLocal;
        }
    }
    return listaPessoasLocal;
}
//