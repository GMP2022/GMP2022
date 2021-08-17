#! /bin/bash

#Dataset Name
DATASETS="
audio
"
K_PARAS=(50)
C_PARAS=(0.5)

EXE="./build/mips.exe"

DIR="/home/wanwanvv/workspace/research/mips"

for(( i=0;i<${#DATASETS[@]};i++))
do
	echo "==============================${DATASETS[$i]}=============================="
	#Excute program
	${EXE} ${DATASETS[$i]} ${DIR} ${K_PARAS[$i]} ${C_PARAS[$i]}
	echo Done
done
