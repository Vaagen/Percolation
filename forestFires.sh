#!/bin/bash

for (( N=10; N<=100; N+=10 ))
do
	reps=$(bc <<< "$N*$N*2")
	fileName="outputFiles/N$N-reps$reps.txt"
  echo $name
  ./run $N $fileName $reps &
done
