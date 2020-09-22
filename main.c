#include "main.h"
#include "Local.h"
#include "Pessoas.h"
#include "utils.h"
#include "Simulacao.h"
#include "timemachine.h"
#include "reportCreator.h"

#define TaxaImunidade 0.2


int main() {

    setbuf(stdout,0); //evitar bug com a consola

    //bla bla bla alteracoes


    

    //iniciar random numbers
    initRandom();
    
    //variaveis necessárias para locais
    int nLocais=0; //saber a quantidade de locais existentes
    pLocal espaco=NULL; //vetor dinâmico com os locais

    //variaveis necessárias para pessoas
    pPessoa listaPessoas=NULL; //lista ligada das pessoas
    int nPessoas = 0;
    
    //maquina do tempo
    
    pTimeMachine timemachine;
    int times = 0;
    
    timemachine.iteracao1=NULL;
    timemachine.iteracao2=NULL;
    timemachine.iteracao3=NULL;
    
    
    //Fase 1 - Preparação
    printf("\n\n********************");
    printf("\nA PREPARAR LOCAIS\n");
    printf("********************\n");
    espaco=startupLocal(&nLocais);
    if(espaco==NULL)
        return -1;

    /*
     * capacidadeLocais[] é preenchida 1 vez ao criar a lista de pessoas
     * deve ser atualizada sempre que se efetuam alterações à capacidade
     * 
     * só é criada depois do startupLocais para ter o tamanho certo
     * também é atualizada ao chamar
     */

    int capacidadeLocais[nLocais];
    
    

    printf("\n\n********************");
    printf("\nA PREPARAR PESSOAS\n");
    printf("********************\n");

    listaPessoas=startupPessoas(nLocais,espaco,&nPessoas,capacidadeLocais);
    if (listaPessoas==NULL)
        return -2;

    
    //cópia da lista das pessoas para o relatorio
    int nPessoasInicial = nPessoas;
    pPessoa listaOGPessoas = copiaLista(listaPessoas);



    //Fase 2 - Simulação
    int flag = 1;
    while(flag){
        int opcao=0;

        printf("\n\nEscolha uma opcao:");
        printf("\n1 - Avancar uma iteracao");
        printf("\n2 - Adicionar Doente");
        printf("\n3 - Estatisticas");
        printf("\n4 - Transferir pessoas");
        printf("\n5 - Anular Iteracoes");
        printf("\n6 - Sair");
        printf("\nOpcao: ");

        do{
            scanf("%d",&opcao);
            if (opcao<0 || opcao>6)
                printf("\nOpcao invalida");
            if (opcao == 6)
                flag = 0;

        }while(opcao<0 || opcao>6);

        switch (opcao) {
            case 1:{ //Avançar uma iteração
                printf("\nOpcao 1 escolhida");
                
                if(times==0)
                    timemachine.iteracao1 = criaSnapshot(listaPessoas);
                else if (times==1)
                    timemachine.iteracao2 = criaSnapshot(listaPessoas);
                else if (times==2)
                    timemachine.iteracao3 = criaSnapshot(listaPessoas);
                else{
                    limpaListaPessoas(timemachine.iteracao1);
                    timemachine.iteracao1=timemachine.iteracao2;
                    timemachine.iteracao2=timemachine.iteracao3;
                    timemachine.iteracao3=criaSnapshot(listaPessoas);
                }
                    
                
                avancaIteracao(espaco,listaPessoas,nPessoas,nLocais,TaxaImunidade);
                if(times!=3)
                    times++;
                break;
            }
            case 2: //Adicionar um doente
                printf("\nOpcao 2 escolhida");
                adicionaDoente(espaco,listaPessoas,nLocais,&nPessoas,capacidadeLocais);
                break;
            case 3: //Apresenta Estatísticas
                printf("\nOpcao 3 escolhida");
                estatisticas(nLocais,nPessoas,listaPessoas,espaco);
                break;
            case 4:{//Transferir Pessoas
                int idorigem,iddestino,N;

                //obter informacao
                printf("\nId Origem: ");
                scanf("%d",&idorigem);
                printf("\nId destino");
                scanf("%d",&iddestino);
                printf("\nQuantas pessoas: ");
                scanf("%d",&N);

                transferePessoas(idorigem,iddestino,N,capacidadeLocais,nLocais,espaco,nPessoas,listaPessoas);
                break;
            }
            case 5:{
                switch(times){
                    case 0:
                        printf("\nAinda nao pode voltar atras");
                        break;
                    case 1:{
                        int opcao2;
                        
                        printf("\nEscolha uma opcao:\n"
                                "0 - Sair\n"
                                "1 - Voltar 1 iteracao atras\n"
                                "Escolha uma opcao: ");
                        
                        scanf(" %d",&opcao2);
                        
                        if(opcao2==1){
                            limpaListaPessoas(listaPessoas);
                            listaPessoas=copiaLista(timemachine.iteracao1);
                            limpaListaPessoas(timemachine.iteracao1);
                            atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                            times--;
                        }else if(opcao2!=0)
                            printf("\nOpcao invalida");
                        break;
                    }
                    case 2:{
                        int opcao2;
                        
                         printf("\nEscolha uma opcao:\n"
                                "0 - Sair\n"
                                "1 - Voltar 1 iteracao atras\n"
                                "2 - Voltar 2 iteracoes atras\n"
                                "Escolha uma opcao: ");
                         scanf(" %d",&opcao2);
                         
                         if(opcao2==1){
                             limpaListaPessoas(listaPessoas);
                             listaPessoas=copiaLista(timemachine.iteracao2);
                             limpaListaPessoas(timemachine.iteracao2);
                             atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                             times--;
                         }else if(opcao2==2){
                             limpaListaPessoas(listaPessoas);
                             limpaListaPessoas(timemachine.iteracao2);
                             listaPessoas=copiaLista(timemachine.iteracao1);
                             limpaListaPessoas(timemachine.iteracao1);
                             atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                             times -= 2;
                         }
                         else if(opcao2!=0)
                             printf("\nOpcao Invalida");
                         break;
                    }
                    case 3:{
                        int opcao2;
                        
                        printf("\nEscolha uma opcao:\n"
                                "0 - Sair\n"
                                "1 - Voltar 1 iteracao atras\n"
                                "2 - Voltar 2 iteracoes atras\n"
                                "3 - Voltar 3 iteracoes atras\n"
                                "Escolha uma opcao: ");
                         scanf(" %d",&opcao2);
                         
                          if(opcao2==1){
                             limpaListaPessoas(listaPessoas);
                             listaPessoas=copiaLista(timemachine.iteracao3);
                             limpaListaPessoas(timemachine.iteracao3);
                             atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                             times--;
                         }else if(opcao2==2){
                             limpaListaPessoas(listaPessoas);
                             limpaListaPessoas(timemachine.iteracao3);
                             listaPessoas=copiaLista(timemachine.iteracao2);
                             limpaListaPessoas(timemachine.iteracao2);
                             atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                             times -=2;
                         }else if(opcao2==3){
                             limpaListaPessoas(listaPessoas);
                             limpaListaPessoas(timemachine.iteracao3);
                             limpaListaPessoas(timemachine.iteracao2);
                             listaPessoas = copiaLista(timemachine.iteracao1);
                             limpaListaPessoas(timemachine.iteracao1);
                             atualizaCapacidadeLocais(capacidadeLocais,nLocais,listaPessoas,espaco);
                             times -=3;
                         }
                         else if(opcao2!=0)
                             printf("\nOpcao invalida");
                        break;
                    }
                }
                break;
            }
            case 6: //SAIR
                criaReport(nPessoasInicial,nPessoas,nLocais,listaOGPessoas,listaPessoas,espaco); //Criação ficheiro report.txt
                exportaListaPessoas(listaPessoas); //criacao do ficheiro da lista das pessoas
                free(espaco);//libertar array dinamico
                limpaListaPessoas(listaPessoas);//libertar lista ligada das pessoas
               
                //limpar maquina do tempo
                limpaListaPessoas(timemachine.iteracao1);
                limpaListaPessoas(timemachine.iteracao2);
                limpaListaPessoas(timemachine.iteracao3);
                
                //limpar listaOGPessoas, visto já ter servido o seu propósito em criaReport()
                limpaListaPessoas(listaOGPessoas);
                return (EXIT_SUCCESS);
        }
    }
}