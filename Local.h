#ifndef LOCAL_H
#define LOCAL_H

typedef struct sala{
    int id; // id numérico do local
    int capacidade; // capacidade máxima
    int liga[3]; // id das ligações (-1 nos casos não usados)
}Local,*pLocal;

pLocal startupLocal(int *tam);
void imprimeListaLocais(pLocal espaco,int n);

#endif /* LOCAL_H */
