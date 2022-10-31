#!/bin/bash
find . -name "*.csv" -type f -delete
opp_scavetool x Experiment3RDPN1-#0.vec -o Exp3N1.csv
opp_scavetool x Experiment3RDPN2-#0.vec -o Exp3N2.csv
opp_scavetool x Experiment3RDPN10-#0.vec -o Exp3N10.csv
opp_scavetool x Experiment3RDPN40-#0.vec -o Exp3N40.csv
