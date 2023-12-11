#!/bin/bash
for d in @day_list@
do
    echo === DAY $d ===
    ./day$d < ../data/day$d.txt
    echo ==============
    echo
done

