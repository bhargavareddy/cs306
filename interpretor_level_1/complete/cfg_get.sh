#!/bin/bash
make -f Makefile.cfglp
cd test_files

for i in $(ls *.c)
do
	cd ..
	make FILE=$i -f Makefile.cfg > /dev/null
	cp test_files/${i}s306.cfg test_files_given/${i}s306.cfg 
	echo $i
	./cfglp -icode test_files/${i}s306.cfg
	./cfglp64 -icode test_files_given/${i}s306.cfg 
	diff -Bb test_files/${i}s306.cfg.spim test_files_given/${i}s306.cfg.spim
	cd test_files
done

#for i in $(ls test_files/*.ecfg)
#do
#
#	./cfglp -d -tokens test_files/${i} > ${i}.output
#	./cfglp64 -d -tokens test_files/${i} > ${i}.output.given
#	diff ${i}.output ${i}.output.given

#done
