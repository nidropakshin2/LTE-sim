#! usr/bin/bash

echo -n "Enter the number of users (max 17): "
read nUE
echo -e "Generating Rlc Stats from minimal LTE scenario... \n"

./ns3 run $"scratch/ltemin.cc --nUE=$nUE"
echo 
awk 'BEGIN {
        print "Throughputs per user:"
        print "IMSI\tUL (Mbps)\tDL (Mbps)"
        flag = 0
    }
    {
        if (NR != 1) {
            if (FNR == 1) 
            {
                flag = 1
                next
            }
            
            IMSI = $4
            SimTime = $2
            if (flag)
                sumUL[IMSI] += $10
            else 
                sumDL[IMSI] += $10
        }
    }
    END {
        for (id in sumUL) 
        {
            print id "\t" (sumUL[id] * 8 / SimTime) / 1000000 "\t\t" (sumDL[id] * 8 / SimTime) / 1000000
        }
    }' UlRlcStats.txt DlRlcStats.txt