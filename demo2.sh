#!/bin/sh

./windows/grid/dbscan -algo 3 -n 7291 -d 256 -r 60 -k 14 -ds "data/grid/uspshw.txt" -rf "data/output/result.txt"

./windows/grid/dbscan -algo 1 -n 7291 -d 256 -r 60 -k 14 -ds "data/grid/uspshw.txt" -rf "data/output/result.txt"

./windows/metric/dbscan1 "data/metric/uspshw.txt" "data/output/result.txt" 7291 256 60 14 10 40 10

./windows/metric/dbscan2 "data/metric/uspshw.txt" "data/output/result.txt" 7291 256 60 14 10 40 10