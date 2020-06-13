#ifndef TRABALHOPCLION_SIMULACAO_H
#define TRABALHOPCLION_SIMULACAO_H

//INCLUDE aqui para poder fazer o struct
#include "Local.h"
#include "Pessoas.h"

typedef struct pessoaLocal PessoaLocal, *pPessoaLocal;
struct pessoaLocal{
    pPessoa pessoa;
    pPessoaLocal next;
};
int transferePessoas(int idOrigem,int idDestino,int N,int *capacidadeLocais,int nLocais,pLocal espaco,int nPessoasTotal,pPessoa listaTodasPessoas);
int estatisticas(int nLocais,int nPessoas,pPessoa listaPessoas,Local espaco[]);
int avancaIteracao(pLocal espaco, pPessoa listaPessoas, int nPessoas, int nLocal,float TaxaImunidade);
int adicionaDoente(pLocal espaco,pPessoa listaPessoas,int nLocais,int *nPessoas, int *capacidadeLocais);
#endif //TRABALHOPCLION_SIMULACAO_H
