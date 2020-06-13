#include "timemachine.h"

pPessoa criaSnapshot(pPessoa listaPessoas) {
    pPessoa novaLista = NULL;

    pPessoa aux = listaPessoas;
    
    novaLista = copiaLista(listaPessoas);
}

int atualizaCapacidadeLocais(int capacidadeLocais[],int nLocais, pPessoa listaPessoas, pLocal espaco){
    
    for(int i=0;i<nLocais;i++){
        capacidadeLocais[i]=espaco[i].capacidade;
    }
    
    pPessoa aux = listaPessoas;
    
    for(int i=0;i<nLocais;i++){
        aux=listaPessoas;
        
        while(aux !=NULL){
            if(aux->idLocal == espaco[i].id){
                capacidadeLocais[i]--;
            }
            aux = aux->nextPessoa;
        }
    }
}

