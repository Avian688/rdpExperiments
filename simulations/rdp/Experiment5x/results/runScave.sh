#!/bin/bash
find . -name "*.csv" -type f -delete
opp_scavetool x Experiment5xRDPN1-#0.vec -o Exp5xN1.csv
#opp_scavetool x Experiment5xRDPN2-#0.vec -o Exp5xN2.csv
#opp_scavetool x Experiment5xRDPN10-#0.vec -o Exp5xN10.csv
#opp_scavetool x Experiment5xRDPN40-#0.vec -o Exp5xN40.csv

#python3 plotCwnd.py Exp4N1.csv Exp4N2.csv Exp4N10.csv Exp4N40.csv
