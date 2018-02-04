#!/bin/bash
tyypit=("muovipullo" "lasipullo" "tölkki" "kori")
koot=("1.0" "0.5" "0.35" "0.33")
pantit=("0.10" "0.15" "0.20" "0.40" "2.20" "3.20" "4.20" "4.20" "4.60" "5.60" "6.20" "9.0")
RANDOM=$$$(date +%s)

cp /dev/null > tuotetiedosto.txt
for((i=0;i<$1;i++))
do
    tyyppi=${tyypit[$RANDOM % ${#tyypit[@]} ]}
    koko=${koot[$RANDOM % ${#koot[@]} ]}
    pantti=${pantit[$RANDOM % ${#pantit[@]} ]}
    echo "$tyyppi $koko $pantti" >> tuotetiedosto.txt
done
