#include "Local.h"
#include "main.h"

int verificaSeIDExiste(int idAverificar, pLocal espaco, int nLocaisNoEspaco);
int verificaLigacoes(pLocal espaco, int nLocais);
void imprimeListaLocais(pLocal espaco, int n);

pLocal startupLocal(int *tam) {

    /*
     tam - Representa nLocal em main
     */

    char nomeFicheiro[100];
    pLocal espaco = NULL;

    //receber nome do ficheiro
    printf("\nInsira o nome do ficheiro dos locais: ");
    scanf("%s", nomeFicheiro);

    //abrir ficheiro binário para leitura
    FILE* ficheiro = fopen(nomeFicheiro, "rb");

    if (ficheiro == NULL) {
        printf("\nErro ao abrir ficheiro dos locais");
        return NULL;
    }


    //alocar memoria para o espaco de locais (apenas com espaço para 1 local por enquanto)
    espaco = malloc(sizeof (Local));
    if (espaco == NULL) {
        printf("\nErro ao alocar memoria para os locais");
        return NULL;
    }


    //se este local nao tiver nada, é porque o ficheiro está vazio
    Local newLocal;
    if (fread(&newLocal, sizeof (Local), 1, ficheiro) == 1) {
        espaco[*tam] = newLocal;
        (*tam)++;
    } else {
        printf("\nFicheiro sem informacao");
        return NULL;
    }

    //ler o resto dos locais
    while (fread(&newLocal, sizeof (Local), 1, ficheiro) == 1) {

        //realocar espaço para adicionar este novo local
        pLocal newEspaco = realloc(espaco, sizeof (Local)*(*tam + 1));

        if (newEspaco == NULL) {
            printf("\nErro ao alocar memoria para os locais");
            return NULL;
        } else {
            espaco = newEspaco;
            espaco[*tam] = newLocal;
            (*tam)++;
        }

        //verificar se o ID é positivo
        if (newLocal.id<-1 || newLocal.id == 0) {
            printf("\nFicheiro corrupto: ID inválido no local %d", (*tam));
            return NULL;
        }

        //verificar se o ID é único
        for (int i = (*tam) - 2; i > 0; i--) {
            if (newLocal.id == espaco[i].id) {
                printf("\nFicheiro corrupto: ID duplicado no local %d", (*tam));
                return NULL;
            }
        }

        //verificar se o local se tenta ligar a ele proprio
        if (newLocal.liga[0] == newLocal.id || newLocal.liga[1] == newLocal.id || newLocal.liga[2] == newLocal.id) {
            printf("Ficheiro Corrupto, o Local com ID %d tenta se ligar a ele próprio!", newLocal.id);
            return NULL;
        }

    }

    //verificar se as ligacoes estão corrretas
    if (verificaLigacoes(espaco, (*tam)) == -1)
        return NULL;

    //fechar ficheiro
    fclose(ficheiro);

    return espaco;
}

int verificaSeIDExiste(int idAverificar, pLocal espaco, int nLocaisNoEspaco) {

    for (int i = 0; i < nLocaisNoEspaco; i++) {
        if (espaco[i].id == idAverificar)
            return i;

    }
    return -1;
}

int verificaLigacoes(pLocal espaco, int nLocais) {

    for (int i = 0; i < nLocais; i++) {
        for (int j = 0; j < 3; j++) {
            if (espaco[i].liga[j] != -1) {
                int pos = verificaSeIDExiste(espaco[i].liga[j], espaco, nLocais);
                if (pos == -1) {
                    printf("\nFicheiro corrupto, o local com ID %d tenta se ligar a local com ID %d mas este nao existe", espaco[i].id, espaco[i].liga[j]);
                    return -1;
                } else {
                    int flag = 0;
                    int x = 0;
                    do {
                        if (espaco[pos].liga[x] == espaco[i].id) {
                            flag = 1;
                        } else
                            x++;
                    } while (flag == 0 && x < 3);
                    if (flag == 0) {
                        printf("\nFicheiro corrupto: o local %d tenta se ligar a %d mas este não tem a ligacao de volta", espaco[i].id, espaco[i].liga[j]);
                        return -1;
                    }
                }
            }
        }
    }


    printf("\n\nCheck completo: os locais estao corretos:");
    imprimeListaLocais(espaco, nLocais);

    return 1;
}

void imprimeListaLocais(pLocal espaco, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nLocal%d:"
                "\nId:%d"
                "\nCapacidade:%d\n"
                "Liga[0]:%d"
                "\nLiga[1]:%d"
                "\nLiga[2]:%d\n", i, espaco[i].id, espaco[i].capacidade, espaco[i].liga[0], espaco[i].liga[1], espaco[i].liga[2]);
    }

}