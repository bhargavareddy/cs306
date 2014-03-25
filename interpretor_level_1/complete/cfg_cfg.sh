#!/bin/bash
make -f Makefile.cfglp
cd test_files_cfg

for i in $(ls *.cfg)
do
	cd ..
	cp test_files_cfg/${i} test_files_cfg_given/${i}
	echo $i
	./cfglp -lra -compile test_files_cfg/${i}
	./cfglp64 -lra -compile test_files_cfg_given/${i} 
	diff -Bb test_files_cfg/${i}.spim test_files_cfg_given/${i}.spim
	cd test_files_cfg
done

#for i in $(ls test_files/*.ecfg)
#do
#
#	./cfglp -d -tokens test_files/${i} > ${i}.output
#	./cfglp64 -d -tokens test_files/${i} > ${i}.output.given
#	diff ${i}.output ${i}.output.given

#done
