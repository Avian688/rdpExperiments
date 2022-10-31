#!/bin/bash
find . -name "*.csv" -type f -delete
opp_scavetool x Experiment4RDPN1-#0.vec -o Exp1N1.csv
opp_scavetool x Experiment4RDPN2-#0.vec -o Exp1N2.csv
opp_scavetool x Experiment4RDPN10-#0.vec -o Exp1N10.csv
opp_scavetool x Experiment4RDPN40-#0.vec -o Exp1N40.csv

python3 plotCwnd.py Exp4N1.csv Exp4N2.csv Exp4N10.csv Exp4N40.csv
