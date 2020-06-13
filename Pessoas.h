#ifndef PESSOAS_H
#define PESSOAS_H

#include "Local.h"

typedef struct pessoa Pessoa, *pPessoa;

struct pessoa{
    char id[30];
    int idade;
    char estado;
    int dias_infetado;
    int idLocal;
    pPessoa nextPessoa;
    float probabilidadeRecuperacao;
    int duracaoMaxInfecao;
};

pPessoa startupPessoas(int nLocal,pLocal listaLocais, int *nPessoas,int capacidadeLocais[]);
void limpaListaPessoas(pPessoa p);
void imprimePessoa(pPessoa pessoa);
int verificaDados(pPessoa newPessoa,pPessoa local,int nPessoas);
void leTodaGente(pPessoa p);
int copiaPessoa(pPessoa origem,pPessoa destino);
pPessoa copiaLista(pPessoa listaOG);
int exportaListaPessoas(pPessoa listaPessoas);

#endif /* PESSOAS_H */

