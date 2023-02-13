/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº: 98416      Nome: Bibiana Martins Farinha
 ** Nome do Módulo: cidadao.c
 ** Descrição/Explicação do Módulo: descrito abaixo.
 **
 **
 ******************************************************************************/
#include "common.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int PID_servidor;
int aux_while=1; //auxiliar para ver se o ficheiro pedidovacina.txt existe

Cidadao novo_cidadao(){ 
    
    //C1
    int idade, num_utente;
    char nome[100], localidade[100], nr_telemovel[10];

    printf("Insira o seu nome:\n"); 
    my_fgets(nome,100,stdin); //guardar o nome na variável nome

    printf("Insira o seu número de utente:\n");
    scanf("%d",&num_utente); //guardar o número de utente na variável num_utente
    
    printf("Insira a sua idade:\n"); 
    scanf("%d",&idade); //guardar a idade na variável idade

    printf("Insira a sua localidade:\n");
    my_fgets(localidade,100,stdin); //guardar a localidade na variável localidade
    
    printf("Insira o seu número de telemóvel:\n");
    my_fgets(nr_telemovel,10,stdin); //guardar o numero de telemovel na variável nr_telemovel

    sucesso("C1) Dados Cidadão: %d; %s; %d; %s; %s; 0",num_utente,nome,idade,localidade,nr_telemovel); //reproduz a mensagem pedida no C1
    
    Cidadao c; //criamos um cidadão c
    c.num_utente = num_utente; //guardamos o número de utente do cidadão
    strcpy(c.nome,nome); //guardamos o nome do cidadão
    c.idade = idade; //guardamos a idade do cidadão
    strcpy(c.localidade,localidade); //guardamos a localidade do cidadão
    strcpy(c.nr_telemovel,nr_telemovel); //guardamos o número de telemóvel do cidadão

    c.estado_vacinacao = 0; //guardamos o estado de vacinação como 0
    c.PID_cidadao =getpid(); //guardamos o pid do cidadão, achamos o pid com a função getpid()
    sucesso("C2) PID Cidadão: %d", c.PID_cidadao); //reproduz a mensagem pedida no C2
    
    return c;

}

//C10 - handler do SIGALRM
void handler_C10(int sinal){
    
    if(access( "pedidovacina.txt", F_OK )==0){ //se o file ainda existir:
        erro("C3) Não é possível iniciar o processo de vacinação neste momento");
    }else{ //se já não existir, o auxiliar vai mudar de 1 para 0 e sai do loop while
        aux_while=0;
    }
}

//C3 C4
void novo_pedido(){
    Cidadao c = novo_cidadao(); //pedimos um cidadão novo e guardamos no cidadão c
    
    handler_C10(0);

    signal(SIGALRM,handler_C10);
    while(aux_while==1){ //se o aux_while for 1 entra no while
        alarm(5); //espera durante 5 segundos
        pause();
    }

    FILE *file;
    file = fopen("pedidovacina.txt", "w+"); //abre o ficheiro em modo de leitura
    sucesso("C3) Ficheiro FILE_PEDIDO_VACINA pode ser criado");

    fprintf(file,"%d:%s:%d:%s:%s:%d:%d\n",c.num_utente,c.nome,c.idade,c.localidade, c.nr_telemovel,c.estado_vacinacao,c.PID_cidadao); //imprime no ficheiro as informaões
    if (file==NULL){ //se o ficheiro nao existir
        fclose; //fecha o ficheiro
        erro("C4) Não é possível criar o ficheiro FILE_PEDIDO_VACINA");
        exit(1); //acaba o processo
    }
    fclose(file);
    sucesso("C4) Ficheiro FILE_PEDIDO_VACINA criado e preenchido");
}

//C5 - handler do SIGINT (CTRL-C)
void handler_C5(int sinal){
    remove("pedidovacina.txt"); //apaga o ficheiro pedidovacina
    sucesso("\nC5) O cidadão cancelou a vacinação, o pedido nº %i foi cancelado",getpid());
    kill(PID_servidor,SIGTERM); //manda um sinal SIGTERM para o servidor
    exit(0);
}

//C6 - envia o SIGUSR1 para o servidor
void send_C6(){
    FILE * file;
    file = fopen("servidor.pid","r"); //abrimos o servidor.pid para ler o pid do servidor que está lá guardado
    if(file==NULL){ //se o ficheiro não existir dá erro e acaba o programa cidadãos
        erro("C6) Não existe ficheiro FILE_PID_SERVIDOR!");
        fclose(file); //fechamos o ficheiro
        exit(1);
    }
    fscanf(file,"%i",&PID_servidor); //guarda o pid do servidor na variavel PID_servidor
    sucesso("C6) Sinal enviado ao Servidor: %i",PID_servidor);
    fclose(file); //fecha o ficheiro
    kill(PID_servidor,SIGUSR1); //manda o sinal SIGURSR1 para o servidor usando a variavel anteriormente criada (PID_servidor)
}

//C7 - handler do SIGUSR1 
void handler_C7(int sinal){
    remove("pedidovacina.txt"); //apaga o ficheiro pedidovacina.txt para poder-se começar uma vacinação nova
    sucesso("C7) Vacinação do cidadão com o pedido nº %i em curso",getpid());
}

//C8 - handler do SIGUSR2 (quando a consulta é concluida)
void handler_C8(int sinal){
    sucesso("C8) Vacinação do cidadão com o pedido nº %i concluída.",getpid());
    exit(0);
}

//C9 - handler do SIGTERM (sinal enviado do servidor)
void handler_C9(int sinal){
    sucesso("C9) Não é possível vacinar o cidadão no pedido nº %i.",getpid());
    remove("pedidovacina.txt"); //apaga o ficheiro pedido de vacina
    exit(0);
}


int main(){
    
    novo_pedido(); //cria um pedido novo
    send_C6(); //manda o sinal que pergunta se pode ou não ser vacinado
    signal(SIGINT,handler_C5); //trata de quando acontece o CTRL-C
    signal(SIGUSR1,handler_C7); //trata do sinal de quando existe enfermeiro
    signal(SIGUSR2,handler_C8); //trata do sinal de quando a vacinação é concluída
    signal(SIGTERM,handler_C9); //trata do sinal de quando a vacinação não foi possível
    while(1)
        pause();
    return 0;
}
