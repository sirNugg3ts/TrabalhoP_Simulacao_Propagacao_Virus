#include "reportCreator.h"
#include "main.h"

int estatisticasParaFicheiro(FILE **ficheiro,int nLocais, int nPessoas, pPessoa listaPessoas, pLocal espaco){
    
    //copia da funcao original, mas adaptada para escrever num ficheiro

    if (*ficheiro==NULL){
        printf("\nErro ao escrever estatisticas para o ficheiro");
        return -1;
    }

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

    fprintf(*ficheiro,"\nPercentagem de pessoas por local:");
    for (int i = 0; i < nLocais; ++i) {
        fprintf(*ficheiro,"\nO local %d tem %.2f %% das pessoas", espaco[i].id, percentagens[i]);
    }
    fprintf(*ficheiro,"\n\n*******************\n");

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

    fprintf(*ficheiro,"\nPercentagem de pessoas saudaveis: %.2f %%", percentagemSaudaveis);

    
    //Taxa de pessoas Infetadas
    float percentagemInfetados = 0;

    aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado == 'D')
            percentagemInfetados++;
        aux = aux->nextPessoa;
    }

    percentagemInfetados = (percentagemInfetados / nPessoas) * 100;

    fprintf(*ficheiro,"\nPercentagem de pessoas infetadas: %.2f %%", percentagemInfetados);

    //Taxa de pessoas imunes
    float percentagemImunes = 0;

    aux = listaPessoas;

    for (int i = 0; i < nPessoas; ++i) {
        if (aux->estado == 'I')
            percentagemImunes++;
        aux = aux->nextPessoa;
    }

    percentagemImunes = (percentagemImunes / nPessoas) * 100;

    fprintf(*ficheiro,"\nPercentagem de pessoas Imunes: %.2f %%", percentagemImunes);
    fprintf(*ficheiro,"\n");
}

void imprimePessoaParaFicheiro(FILE **ficheiro,pPessoa pessoa){
    fprintf(*ficheiro,"\n*****\n"
           "ID:%s\n"
           "Idade:%d\n"
           "Estado:%c\n"
           "Probabilidade Recuperacao:%f\n"
           "Duracao Max Infecao:%d\n"
           "Local Atribuido:%d\n",pessoa->id,pessoa->idade,pessoa->estado,pessoa->probabilidadeRecuperacao
            ,pessoa->duracaoMaxInfecao
            ,pessoa->idLocal);
    if(pessoa->estado=='D'){
        fprintf(*ficheiro,"Dias infetado:%d",pessoa->dias_infetado);
    }
}

void imprimeListaLocaisParaFicheiro(FILE **ficheiro,pLocal espaco,int nLocais) {
    for (int i = 0; i < nLocais; i++) {
        fprintf(*ficheiro, "\nLocal%d:"
                          "\nId:%d"
                          "\nCapacidade:%d\n"
                          "Liga[0]:%d"
                          "\nLiga[1]:%d"
                          "\nLiga[2]:%d\n", i, espaco[i].id, espaco[i].capacidade, espaco[i].liga[0], espaco[i].liga[1],
                espaco[i].liga[2]);
    }
}

int criaReport(int nPessoasInicial,int nPessoasFinal,int nLocais,pPessoa listaPessoasInicial,pPessoa listaPessoasFinal,pLocal espaco){

    /*
     * O ficheiro report.txt irá ter as estatísticas (percentagem de pessoas saudaveis, infetadas e imunes) e de seguida
     * uma lista impressa das pessoas
     */

    FILE* ficheiro = fopen("report.txt","w");

    if (ficheiro == NULL){
        printf("\nErro ao criar o ficheiro report.txt");
        return -1;
    }

    fprintf(ficheiro,"Numero de pessoas no inicio da simulacao: %d",nPessoasInicial);
    fprintf(ficheiro,"\n\nNumero de pessoas no final da simulacao: %d",nPessoasFinal);

    fprintf(ficheiro,"\n\nEstatisticas:\n\nInicio da Simulacao: ");

    estatisticasParaFicheiro(&ficheiro,nLocais,nPessoasInicial,listaPessoasInicial,espaco);

    fprintf(ficheiro,"\n\nFinal da Simulacao: ");

    estatisticasParaFicheiro(&ficheiro,nLocais,nPessoasFinal,listaPessoasFinal,espaco);

    fprintf(ficheiro,"\n\nLista Locais");

    imprimeListaLocaisParaFicheiro(&ficheiro,espaco,nLocais);

    fprintf(ficheiro,"\n\nLista inicial de Pessoas: ");

    pPessoa aux = listaPessoasInicial;

    while (aux!=NULL){
        imprimePessoaParaFicheiro(&ficheiro,aux);
        aux = aux->nextPessoa;
    }

    fprintf(ficheiro,"\n\nLista Final de Pessoas: ");

    aux = listaPessoasFinal;

    while (aux!=NULL){
        imprimePessoaParaFicheiro(&ficheiro,aux);
        aux=aux->nextPessoa;
    }

    fprintf(ficheiro,"\n\nFIM RELATORIO\n**************************************");

    fclose(ficheiro);

    return 1;
}