#!/bin/bash

if [ $# -lt 4 ]; then
	echo "Please provide the following parameters:"
	echo "	1) top folder to run experiments on; should contain a sub-folder for each benchmark"
	echo "	2) runs per benchmark"
	echo "	3) parallel threads; 1 for single threading"
	echo "	4) time out for each run, e.g., 1m"
	echo ""
	exit
fi

bench_folder=$1
runs=$2
threads=$3
timeout=$4

work_dir=`pwd`

for folder in `ls $bench_folder/* -d`
do
	cd $folder

	for (( r = 1; r <= $runs; r++ ))
	do
		## run attack, grep only those lines with successfully generated keys
		echo "Run $r in folder $folder ..."

		timeout $timeout $work_dir/3D_F2F_Proximity_Attack top.v bottom.v wrapper.v mappings.file cells.inputs cells.outputs out_$r.v $threads | tee -a $r.log

		echo "Done"
	done

	cd $work_dir
done
