#!/bin/bash
#cherche pour des chaines qui indique un succes lors du test Valgrind
for res in *.txt; do
	echo $res
	echo
	grep "All heap blocks were freed -- no leaks are possible" $res
	echo
	grep "ERROR SUMMARY" $res
	echo
done

