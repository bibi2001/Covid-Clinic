#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98416 Nome: Bibiana Martins Farinha
## Nome do Módulo: lista_cidadaos.sh
## Descrição/Explicação do Módulo: 
##
##
###############################################################################

if [ "$#" -ne 0 ];then #verifica se o script recebeu argumentos (se receber dá erro)
    echo "Erro: Este script não recebe argumentos."
    exit 1
fi

#muda a sequencia de caracteres " |" para "-"; recebe o ":-" como separadores de colunas; itera desde 10000 até ao fim das linhas do documento;
#faz print das colunas escolhidas e tranfere tudo para o cidadaos.txt
cat listagem.txt | sed "s/ |/-/g" |awk -F "[:-]" -v aux=2021 'BEGIN{i=10000} {print ++i ":" $2 ":" aux-$6 ":" $8 ":" $10 ":" 0}' > cidadaos.txt 
#mostra o documento cidadaos.txt
cat cidadaos.txt
