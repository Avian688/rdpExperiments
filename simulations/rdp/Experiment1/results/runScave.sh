#!/bin/bash
find . -name "*.csv" -type f -delete
opp_scavetool x Experiment1RDPN1-#0.vec -o Exp1N1.csv
opp_scavetool x Experiment1RDPN2-#0.vec -o Exp1N2.csv
opp_scavetool x Experiment1RDPN10-#0.vec -o Exp1N410.csv
opp_scavetool x Experiment1RDPN40-#0.vec -o Exp1N40.csv

python3 plotCwnd.py Exp1N1.csv Exp1N2.csv Exp1N10.csv Exp1N40.csv
