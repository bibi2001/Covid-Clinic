#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98416 Nome: Bibiana Martins Farinha
## Nome do Módulo: stats.sh
## Descrição/Explicação do Módulo: abaixo descrito
##
##
###############################################################################

if [ "$1" = "cidadaos" ] && [ $# -eq 2 ]; then #vê se o primeiro argumento é "cidadaos" e se tem dois argumentos

    if [[ $2 =~ [a-zA-Z]+ ]] && ! [[ $2 =~ [0-9] ]];then #vê se o segundo argumento é um string (caso seja um string mostra o número de cidadãos na localidade)
        numCid=$( cat cidadaos.txt | awk -F "[:]" '{print $4}' | grep -i -w -o "$2" | wc -l )
        echo "O número de cidadãos registados em $2 é $numCid"

    else #caso o segundo argumento não seja um string dá o seguinte erro 
        echo "Erro: a localidade não pode conter números"
    fi

elif [ "$1" = "registados" ] && [ $# -eq 1 ]; then #verifica se o primeiro argumento é "registados" e se é o único argumento
    echo "Lista contendo o Nome e Número de utente dos cidadãos com mais de 60 anos 
registados na plataforma, ordenados do mais velho para o mais novo:"
    cat cidadaos.txt | awk -F "[:]" '$3>60 {print $3 ":" $2 ":" $1 }' | sort -nr | awk -F "[:]" '{print $2 ":" $3}'

elif [ "$1" = "enfermeiros" ] && [ $# -eq 1 ]; then #verifica se o primeiro argumento é "enfermeiros" e se é o único argumento
    echo "Lista com o Nome dos enfermeiros assinalados como disponíveis na plataforma:"
    cat enfermeiros.txt | awk -F "[:]" '$5==1 {print $2}'

else #caso não entre nos ifs, dá um erro
    echo "Erro: Síntaxe: Comando inválido. Comandos possíveis: enfermeiros ou registados ou cidadaos <localidade>"
fi
