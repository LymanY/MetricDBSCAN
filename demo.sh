#!/bin/sh

./windows/grid/dbscan -algo 3 -n 20000 -d 1500 -r 6 -k 20 -ds "data/grid/t2_1500.txt" -rf "data/output/result.txt"

./windows/grid/dbscan -algo 1 -n 20000 -d 1500 -r 6 -k 20 -ds "data/grid/t2_1500.txt" -rf "data/output/result.txt"

./windows/metric/dbscan1 "data/metric/t2_1500.txt" "data/output/result.txt" 20000 1500 6 20 10 50 110

./windows/metric/dbscan2 "data/metric/t2_1500.txt" "data/output/result.txt" 20000 1500 6 20 10 50 110