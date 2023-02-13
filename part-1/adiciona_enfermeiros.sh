#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98416 Nome: Bibiana Martins Farinha 
## Nome do Módulo: adiciona_enfermeiros.sh
## Descrição/Explicação do Módulo: abaixo descrito
##
##
###############################################################################

if [ "$#" -ne 4 ]; then #se o número de argumentos não for 4, dá erro e para o script
    echo "Erro: Síntaxe: $0 <número cédula profissional>:<nome>:<centro saúde associado>:<nº de vacinações efetuadas>:<disponibilidade>"
    exit 1

elif grep -qF -i -w "$3" enfermeiros.txt;then #verifica se o centro de saúde já existe dentro do ficheiro enfermeiros.txt, se existir dá erro
    echo "Erro: O Centro de Saúde introduzido já tem um enfermeiro registado" 
    exit 1 

elif grep -qF -i -w "$2" enfermeiros.txt;then #verifica se o número já existe dentro do ficheiro enfermeiros.txt, se existir dá erro
    echo "Erro: O Enfermeiro já esta inscrito noutro centro saúde"
    exit 1

fi

centroDeSaude=$3 #guarda o terceiro argumento numa variável
inicioCentroDeSaude="${centroDeSaude:0:2}" #guarda as primeiras duas letras do terceiro argumento
if [[ "$inicioCentroDeSaude" != "CS" ]] || [[ $3 =~ [0-9] ]];then #verifica se o centro de saúde está na forma que queremos, isto é, começa com CS, se não começar dá erro
    echo "Erro: O Centro de Saúde tem começar pelas iniciais CS e não deve conter números, $3 não verifica essa condição"

elif [[ $1 =~ [0-9] ]];then #verifica se o nome do enfermeiro
    echo "Erro: O nome do enfermeiro não deve conter números, $1 não verifica essa condição"

elif ! [[ $2 =~ ^[+-]?[0-9]+$ ]];then #verifica se o número do enfermeiro é um número inteiro, se não for dá erro
    echo "Erro: A cédula tem de ser um número, $2 não verifica essa condição"

elif [ $4 != 0 ] && [ $4 != 1 ];then #verifica se o argumento da disponibilidade é válido, isto é, se é 1 ou 0, se não for dá erro
    echo "Erro: a disponibilidade tem de ser representada ou 1-disponível- ou 0-indisponível-, $4 não verifica essa condição"
else
    echo $2:$1:$3:0:$4 >> enfermeiros.txt #caso não tenha entrado em nenhum if, é adicionado o enfermeiro ao ficheiro enfermeiros.txt
    cat enfermeiros.txt #mostra o documento enfermeiros.txt
fi
