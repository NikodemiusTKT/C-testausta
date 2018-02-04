#!/bin/bash
export LC_NUMERIC="en_US.UTF-8"
# input="input2.txt"
# while IFS= read -r var1 var2
# do
# done < "$input"
sh ./rtuote.sh $2
echo "1" > input.txt 
lines=$(wc -l tuotetiedosto.txt | awk '{print $1}')
for((i=0;i<$1;i++))
do
    echo $(( $RANDOM % $lines+1 )) >> input.txt
done
echo $(($lines+1))  >> input.txt
echo "0" >> input.txt
# START=$(date +%s.%N)
./ohjelma < input.txt
# END=$(date +%s.%N)
# dt=$(echo "$END - $START" | bc)
# dd=$(echo "$dt/86400" | bc)
# dt2=$(echo "$dt-86400*$dd" | bc)
# dh=$(echo "$dt2/3600" | bc)
# dt3=$(echo "$dt2-3600*$dh" | bc)
# dm=$(echo "$dt3/60" | bc)
# ds=$(echo "$dt3-60*$dm" | bc)
echo -e "\nTESTI $1 $2:" >> time.txt
echo -e "====================">> time.txt
{ time ./ohjelma < input.txt 2> sleep.stderr ; } 2>> time.txt
printf "Total runtime: %0.2d %0.6f\n" $dm $ds >> time.txt
echo -e "====================">> time.txt
