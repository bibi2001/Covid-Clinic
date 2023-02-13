/******************************************************************************
 ** ISCTE-IUL: Trabalho prático de Sistemas Operativos
 **
 ** Aluno: Nº: 98416    Nome: Bibiana Martins Farinha
 ** Nome do Módulo: servidor.c
 ** Descrição/Explicação do Módulo: descrito abaixo
 **
 **
 ******************************************************************************/
#include "common.h"
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

Vaga vagas[NUM_VAGAS];
Enfermeiro *enfermeiros;
int num_enfermeiros; 
int index_vaga; //index da vaga que usamos
Cidadao aux_c; //cidadão que usaremos


//S1 guardar o pid do servidor no documento servidor.pid
void pid(){
    FILE *file; 
    file = fopen("servidor.pid", "w+"); //abrir o servidor.dat em modo de escrita

    if(access( "servidor.pid", W_OK )!=0){ //ver se o ficheiro servidor.pid disponibiliza escrita, se disponibilizar isto deveria dar 0 e não entrar no if
        erro("S1) Não consegui registar o servidor!");
        exit(1);  
    }

    fprintf(file,"%i\n",getpid()); //escrever o pid do servidor no ficheiro servidor.dat
    sucesso("S1) Escrevi no ficheiro FILE_PID_SERVIDOR o PID: %i",getpid());
    fclose(file); //fechar o ficheiro
}

//S2 guardar informaçoes do cidadão
void data_structure(){
    num_enfermeiros=0;
    if(access( "enfermeiros.dat", R_OK )!=0 ){ //ver se o enfermeiros.dat disponibiliza leitura
        erro("S2) Não consegui ler o ficheiro FILE_ENFERMEIROS!");
        exit(1);
    }

    FILE *file;
    file =fopen("enfermeiros.dat","r"); //abrir o enfermeiros.dat em modo de leitura
    Enfermeiro enfermeiro;
    while ( fread( &enfermeiro, sizeof(enfermeiro), 1, file ) > 0 ) //conta o numero de enfermeiros que estao no enfermeiros.dat
        num_enfermeiros++;
    fclose(file); //fechamos o ficheiro

    enfermeiros= malloc(num_enfermeiros*sizeof(Enfermeiro)); //definimos uma estrutura de memória dinâmica na memória (o vetor enfermeiros)
    sucesso("S2) Ficheiro FILE_ENFERMEIROS tem %i bytes, ou seja, %i enfermeiros", num_enfermeiros*sizeof(Enfermeiro),num_enfermeiros);

    file =fopen("enfermeiros.dat","r"); //abrir o enfermeiros.dat em modo de leitura
    
    int i =0; //auxiliar i para percorrer o vetor enfermeiros
    while ( fread( &enfermeiro, sizeof(enfermeiro), 1, file ) > 0 ){
        enfermeiros[i]=enfermeiro;
        i++; //incrementamos o i
        }
    fclose(file); //fechamos o ficheiro
}

//S3 iniciar todas as vagas a -1
void lista_vagas(){
    for(int i=0; i!=NUM_VAGAS; i++) //enquanto i for menor ao numero de vagas
        vagas[i].index_enfermeiro=-1; //o index do enfermeiro fica a -1
    sucesso("S3) Iniciei a lista de %i vagas",NUM_VAGAS);
}

//função auxiliar para o S5.2.2
int existe_vaga(){ //devolve o index da primeira vaga ou se não houverem vagas devolve -1
    for(int i=0; i!=NUM_VAGAS; i++){ //enquanto i for menor ao numero de vagas
        if(vagas[i].index_enfermeiro==-1) //se o index do enfermeiro for -1 entra no if e devolve o seu index no vetor das vagas
            return i;
    }
    return -1; //se não encontrar nenhuma vaga disponivel no vetor ou seja que o index do enfermeiro nao seja -1, devolve -1
}

void handler_S552(int sinal){
    int pid = wait(NULL); //guarda na variavel pid 

    int index_v, index_e; //auxiliares para guardar o index no vetor das vagas e no vetor dos enfermeiros
    for(int i=0; i!=NUM_VAGAS; i++)
        if(vagas[i].PID_filho==pid)
            index_v=i;
    
    index_e=vagas[index_v].index_enfermeiro; //guarda o index do enfermeiro da vaga em questão na variavel auxiliar
    vagas[index_v].index_enfermeiro=-1; //limpa a vaga ao por a -1
    sucesso("S5.5.3.1) Vaga %i que era do servidor dedicado %i libertada",index_v, pid);
    
    enfermeiros[index_e].disponibilidade=1; //mete a disponibilidade a 1 do enfermeiro
    sucesso("S5.5.3.2) Enfermeiro %i atualizado para disponível",index_e);
        
    enfermeiros[index_e].num_vac_dadas++; //aumenta o numero de vacinas dadas do enfermeiro
    sucesso("S5.5.3.3) Enfermeiro %i atualizado para %i vacinas dadas",index_e,enfermeiros[index_e].num_vac_dadas);
        
    FILE *file; 
    Enfermeiro enf; 
    file = fopen("enfermeiros.dat", "r+"); //abrir o ficheiro enfermeiros.dat no modo escrever+
    fseek(file, index_vaga*sizeof(Enfermeiro), SEEK_SET);
    fread( &enf, sizeof(Enfermeiro), 1, file);  
    enf.num_vac_dadas++; //incrementamos o valor das vacinas dadas do enfermeiro em questão
    fseek(file, -1*sizeof(Enfermeiro), SEEK_CUR );
    fwrite(&enf , sizeof(Enfermeiro), 1, file);
    fclose(file); //fechamos o ficheiro
    sucesso("S5.5.3.4) Ficheiro FILE_ENFERMEIROS %i atualizado para %i vacinas dadas",index_vaga, enfermeiros[index_vaga].num_vac_dadas );
    
    sucesso("S5.5.3.5) Retorna");
}

//S5.6.1 - handler do SIGTERM
void handler_S561(int sinal){
    sucesso("S5.6.1) SIGTERM recebido, servidor dedicado termina Cidadão");
    kill(aux_c.PID_cidadao,SIGTERM); //manda o sinal SIGTERM para o cidadão em questão
    exit(1);
}

//S5.6.2 
void send_S562(){
    sucesso("S5.6.2) Servidor dedicado inicia consulta de vacinação");
    kill(aux_c.PID_cidadao,SIGUSR1); //manda o sinal SIGUSR1 para o cidadão em questão
}

//S5.6.4
void send_S564(){
    sucesso("S5.6.4) Servidor dedicado termina consulta de vacinação");
    kill(aux_c.PID_cidadao,SIGUSR2); //manda o sinal SIGUSR2 para o cidadão em questão
}



//S5
void espera_pedido(){
    //-------------------------------------------------------------------------------------------------------------->S5.1
    int num_utente, idade, estado_vacinacao, PID_cidadao; 
    char localidade[100], nome[100], nr_telemovel[10];
    if( access( "pedidovacina.txt", F_OK )!=0 ){ //verifica se o ficheiro pedidovacina.txt existe
        erro("S5.1) Não foi possível abrir o ficheiro FILE_PEDIDO_VACINA");
        exit(1);
    }
    if(access( "pedidovacina.txt", R_OK )!=0 ){ //verifica se dá para ler o ficheiro pedidovacina.txt
        erro("S5.1) Não foi possível ler o ficheiro FILE_PEDIDO_VACINA");
        exit(1);
    }
    FILE *file;
    file =fopen("pedidovacina.txt","r"); //abrir o ficheiro pedidovacina.txt para guardar as informações do cidadão que está a fazer um pedido
    fscanf(file,"%i:%[^:]:%i:%[^:]:%[^:]:%i:%i",&num_utente,nome,&idade,localidade,nr_telemovel,&estado_vacinacao,&PID_cidadao); 
    fclose(file); //fecha o ficheiro
    printf("Chegou o cidadão com o pedido nº %i, com nº utente %i, para ser vacinado no Centro de Saúde CS%s\n", PID_cidadao, num_utente, localidade);
    sucesso("S5.1) Dados Cidadão: %i; %s; %i; %s; %s; 0", num_utente, nome, idade, localidade, nr_telemovel);

    //guardar as informações do cidadão numa variável cidadão para ser utilizado no futuro
    Cidadao c; //criamos um cidadão c
    c.num_utente = num_utente; //guardamos o número de utente do cidadão
    strcpy(c.nome,nome); //guardamos o nome do cidadão
    c.idade = idade; //guardamos a idade do cidadão
    strcpy(c.localidade,localidade); //guardamos a localidade do cidadão
    strcpy(c.nr_telemovel,nr_telemovel); //guardamos o número de telemóvel do cidadão
    c.estado_vacinacao=estado_vacinacao; //guardamos o estado de vacinação
    c.PID_cidadao=PID_cidadao; //guardamos o pid do cidadão

    //-------------------------------------------------------------------------------------------------------------->S5.2
    char CS_cidadao[102]="CS"; //guardamos as iniciais CS na variável cs
    strcat(CS_cidadao,localidade); //queremos juntar CS ao início da localidade
    
    int aux_index_enf=-1; //auxiliar para guardar o index do enfermeiro no vetor enfermeiros com o mesmo centro de saúde que o cidadão

    for(int i=0; i!=num_enfermeiros; i++){ //percorremos o vetor dos enfermeiros 
        int cmp= strcmp(CS_cidadao,enfermeiros[i].CS_enfermeiro);
        if(cmp == 0){ //comparamos os centros de saude, dá 0 quando são iguais
            aux_index_enf=i;
        }
    }

    if(aux_index_enf==-1){  //não foi encontrado o centro de saúde em comum
        erro("Não existe correspondência entre o centro de saúde do enfermeiro e do cidadão");
        return;
    }

    if(enfermeiros[aux_index_enf].disponibilidade==0){
        erro("S5.2.1) Enfermeiro %i indisponível para o pedido %i para o Centro de Saúde %s",aux_index_enf,c.PID_cidadao,enfermeiros[aux_index_enf].CS_enfermeiro);
        return;
    } //se não entrar no if mostra a seguinte mensagem e continua o programa:
    sucesso("S5.2.1) Enfermeiro %i disponível para o pedido %i para o Centro de Saúde %s",aux_index_enf,c.PID_cidadao,enfermeiros[aux_index_enf].CS_enfermeiro);  

    int index_vaga = existe_vaga();
    if(index_vaga==-1){
        erro("S5.2.2) Não há vaga para vacinação para o pedido %i",c.PID_cidadao);
        kill(c.PID_cidadao,SIGTERM); //se não houverem vagas, manda o sinal sigterm ao processo cidadão
        return;
    } //se não entrar no if mostra a seguinte mensagem e continua o programa:
    sucesso("S5.2.2) Há vaga para vacinação para o pedido %i",c.PID_cidadao);

    //-------------------------------------------------------------------------------------------------------------->S5.3 
    vagas[index_vaga].index_enfermeiro=aux_index_enf;
    vagas[index_vaga].cidadao=c; //c é o nome do cidadão anteriormente criado
    enfermeiros[aux_index_enf].disponibilidade=0;
    sucesso("S5.3) Vaga nº %i preenchida para o pedido %i",index_vaga,c.PID_cidadao);

    //-------------------------------------------------------------------------------------------------------------->S5.4 ao S5.6
    int pid=fork(); //S5.4 criamos o filho
    int pid_filho; //variável para guardar o pid do filho
    if( pid==-1 ){ //se o fork for menor que 0 o processo filho não foi criado corretamente
        erro("S5.4) Não foi possível criar o servidor dedicado");
        exit(1);
    
    }if( pid>0 ){ //executado pelo processo pai 
        pid_filho=pid; //o fork do processo pai é o pid do processo filho
        sucesso("S5.4) Servidor dedicado %i criado para o pedido %i",pid_filho,c.PID_cidadao);
        
        vagas[index_vaga].PID_filho=pid;
        sucesso("S5.5.1) Servidor dedicado %i na vaga %i",pid,index_vaga);

        signal(SIGCHLD,handler_S552);
        sucesso("S5.5.2) Servidor aguarda fim do servidor dedicado %i",pid);
        return;

    }if( pid==0 ){ //executado pelo processo filho

        aux_c=c;

        signal(SIGTERM,handler_S561); //trata e arma o sinal SIGTERM
        send_S562(); 
        
        sleep(TEMPO_CONSULTA); //espera para a consulta acabar
        sucesso("S5.6.3) Vacinação terminada para o cidadão com o pedido nº %i",aux_c.PID_cidadao);
        
        send_S564();
        exit(0);
    }
}

//S4 - handler do sinal SIGUSR1
void handler_S4(int sinal){
    espera_pedido(); //chama a função S5 quando o sinal SIGURS1 é enviado para o servidor
}

//S6 - handler do sinal SIGINT
void handler_S6(int sinal){
    sucesso("\nS6) Servidor terminado");
    for(int i=0; i!=NUM_VAGAS; i++)
        if(vagas[i].index_enfermeiro!=-1){
            kill(vagas[i].PID_filho,SIGTERM);
        }
    remove("servidor.pid");
    exit(0);
}

int main() {
    pid(); //guardar no ficheiro servidor.pid o pid do servidor
    data_structure(); //guarda as informações do cidadão que estão no pedidovacina.txt
    lista_vagas(); //inicializa os index dos enfermeiros todos a -1
    signal(SIGUSR1, handler_S4); //trata do sinal SIGUSR1
    sucesso("S4) Servidor espera pedidos");
    signal(SIGINT, handler_S6);  //trata do sinal SIGINT
    while(1)
        pause();
    return 0;
}
