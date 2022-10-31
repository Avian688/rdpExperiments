#!/bin/bash
find . -name "*.csv" -type f -delete
opp_scavetool x Experiment2RDPN1-#0.vec -o Exp2N1.csv
opp_scavetool x Experiment2RDPN2-#0.vec -o Exp2N2.csv
opp_scavetool x Experiment2RDPN10-#0.vec -o Exp2N10.csv
opp_scavetool x Experiment2RDPN40-#0.vec -o Exp2N40.csv
