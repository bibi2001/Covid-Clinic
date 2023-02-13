#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98416 Nome: Bibiana Martins Farinha 
## Nome do Módulo: menu.sh
## Descrição/Explicação do Módulo: abaixo descrito
##
##
###############################################################################

aux=0 #auxiliar para o menu só sair quando queremos
while [[ aux -eq 0 ]] #verifica se o auxiliar é igual a 0, se for então mostra o menu e continua o script, se for diferente, sai do while
do
    #menu
    echo "Menu: Escolha uma das seguintes opções:
1. Listar cidadãos
2. Adicionar enfermeiro
3. Stats
4. Agendar vacinação
0. Sair"

    read var1 #lê a primeira variável que é pedida
    echo ""

    if [ -z "$var1" ] || ! [[ $var1 =~ ^[+-]?[0-9]+$ ]]; then #Se a var1 não for um número, dá erro
        echo "Erro: A opção $var1 não é válida
        "

    elif [ $var1 -eq 0 ];then #Se a var1 for 0, escolheram sair do script
        echo "Escolheu a opção de sair do menu."
        ((aux++)) #a auxiliar deixa de ser 0, então o script não vai mais conseguir entrar no while

    elif [ $var1 -eq 1 ];then #Se a var1 for 1, escolheram listar os cidadãos, então o devido script é chamado
        echo "Escolheu a opção de listar os cidadãos."
        ./lista_cidadaos.sh
        echo ""

    elif [ $var1 -eq 2 ];then #Se a var1 for 2, escolheram adicionar um enfermeiro
        read -p "Insira o nome do enfermeiro:" nome #pedimos o nome 
        read -p "Insira o número do enfermeiro:" numero #pedimos o numero
        read -p "Insira o centro de saúde do enfermeiro (tem de começar por CS):" CS #pedimos o devido centro de saúde
        read -p "Insira a disponibilidade do enfermeiro (1-disponível, 0-indisponível):" disponibilidade #pedimos a disponibilidade
        echo ""
        ./adiciona_enfermeiros.sh "$nome" $numero "$CS" $disponibilidade #é invocado o script de adicionar enfermeiros com as variaveis recolhidas
        echo ""

    elif [ $var1 -eq 3 ];then #Se a var1 for 3, escolheram a opção Stats
        auxStats=0 #auxiliar para saber quando sair do while (sai do while quando é incrementada)
        while [ $auxStats -eq 0 ] #enquanto a auxiliar for 0, o menu dos Stats vai continuar a aparecer
        do
        echo "Escolheu a opção de mostrar as estatísticas, escolha uma das seguintes opções:
1. Número de cidadãos em certa localidade
2. Lista dos cidadãos com mais de 60 anos registados na plataforma
3. Lista dos enfermeiros disponíveis na plataforma
0. Voltar ao menu principal"
        read var2 #lê qual das opções 
        echo ""

        if [ -z "$var2" ] || ! [[ $var2 =~ ^[+-]?[0-9]+$ ]]; then #Se a var2 não for um número, dá erro
            echo "Erro: A opção $var2 não é válida
            "
        elif [ $var2 -eq 1 ];then #se a var2 for 1, pede outra variável, a da localidade
            echo "Qual é a localidade que pretende procurar?"
            read var3
            ./stats.sh cidadaos $var3 #é invocado o script dos cidadaos com a localidade dada

        elif [ $var2 -eq 2 ];then #se a var2 for 2, é invocado o script dos registados
            ./stats.sh registados

        elif [ $var2 -eq 3 ];then #se a var2 for 3, é invocado o script dos enfermeiros
            ./stats.sh enfermeiros

        elif [ $var2 -eq 0 ];then #se a var2 for 0, volta ao menu principal
            ((auxStats++)) #incrementa o auxiliar para o while parar
        else #caso não seja nenhum dos valores, dá um erro
            echo "Erro: A opção $var2 não é válida."
        fi
        echo ""
        done #saimos do while e retomamos ao menu principal quando o auxStats é diferente de zero

    elif [ $var1 -eq 4 ];then #Se a váriavel for 4, escolheram a opção agendar vacinação
        echo "Escolheu a opção de agendar vacinações."
        ./agendamento.sh
        echo ""

     else #Se não for nenhumas das opções anteriores, dá erro
        echo "Erro: A opção $var1 não é válida
        "

    fi
done
