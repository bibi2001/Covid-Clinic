#!/bin/bash

###############################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos
##
## Aluno: Nº: 98416 Nome: Bibiana Martins Farinha 
## Nome do Módulo: agendamento.sh
## Descrição/Explicação do Módulo: abaixo descrito
##
##
###############################################################################

if [ "$#" -ne 0 ];then #verifica se o script recebe argumentos (se receber então dá um erro)
    echo "Erro: Este script não recebe argumentos."
    exit 1
fi

lengthEnf=$(cat enfermeiros.txt | wc -l) #numero de enfermeiros
lengthCid=$(cat cidadaos.txt | wc -l) #numero de cidadaos
auxA=0 #auxiliar para dizer se existem enfermeiros disponiveis
auxB=0 #auxiliar para dizer se existe alguma correspondencia entre os centros de saude disponiveis e as localidades dos cidadaos
data=$(date "+%Y-%m-%d") #data do formato desejado

#verifica se existe um ficheiro chamado agenda.txt (se existir apaga-o)
if [ -f "agenda.txt" ];then
    rm agenda.txt
fi

for i in $(seq 1 $lengthEnf); do #percorre os enfermeiros e vê a disponibilidade de cada um
    Disp=$(awk -F "[:]" '{print $5}' enfermeiros.txt | head -n $i | tail -1) #disponibilidade

    if [[ $Disp -eq 1 ]];then #se o enfermeiro em questão estiver disponível então...
        locEnf=$(awk -F "[:]" '{print $3}' enfermeiros.txt | sed 's/CS//g' | head -n $i | tail -1) #localidade do enfermeiro (sem o CS do centro de saude)
        nomeEnf=$(awk -F "[:]" '{print $2}' enfermeiros.txt | head -n $i | tail -1) #nome do enfermeiro
        numEnf=$(awk -F "[:]" '{print $1}' enfermeiros.txt | head -n $i | tail -1) #numero do enfermeiro
        ((auxA++)) #fica registado que houve um enfermeiro disponivel (pelo menos)

        for j in $(seq 1 $lengthCid); do #percorre os cidadãos e vê a localidade de cada um
            locCid=$(awk -F "[:]" '{print $4}' cidadaos.txt | head -n $j | tail -1) #localidade do cidadão

            if [ "$locCid" = "$locEnf" ];then #se a localidade do enfermeiro e do cidadão em questão forem iguais então...
                nomeCid=$(awk -F "[:]" '{print $2}' cidadaos.txt | head -n $j | tail -1) #nome do cidadão
                numCid=$(awk -F "[:]" '{print $1}' cidadaos.txt | head -n $j | tail -1) #numero do cidadão
                echo "$nomeEnf":$numEnf:"$nomeCid":$numCid:CS$locCid:$data >> agenda.txt #agenda como desejado no documento agenda.txt
                ((auxB++)) #fica registado que houve um agendamento (pelo menos)
            fi
        done
    fi
done

if [[ $auxA -eq 0 ]] ;then #se não houver nenhum enfermeiro disponível, mostra a seguinte mensagem
    echo "Não existem enfermeiros disponíveis."

elif [[ $auxB -eq 0 ]];then #se não houve nenhum agendamento, mostra a seguinte mensagem
    echo "Não há correspondência entre os enfermeiros disponíveis e as localidades dos cidadãos."
fi
