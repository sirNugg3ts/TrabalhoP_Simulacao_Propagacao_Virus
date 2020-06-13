#ifndef TRABALHOPCLION_TIMEMACHINE_H
#define TRABALHOPCLION_TIMEMACHINE_H

#include "Pessoas.h"
#include "Local.h"
#include "main.h"

struct timemachine{
    pPessoa iteracao1;
    pPessoa iteracao2;
    pPessoa iteracao3;
};

typedef struct timemachine TimeMachine, pTimeMachine;

pPessoa criaSnapshot(pPessoa listaPessoas);
int atualizaCapacidadeLocais(int capacidadeLocais[],int nLocais, pPessoa listaPessoas, pLocal espaco);


#endif //TRABALHOPCLION_TIMEMACHINE_H
