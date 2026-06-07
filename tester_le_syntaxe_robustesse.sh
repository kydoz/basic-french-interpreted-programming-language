#!/bin/bash
set -e
#execute tous les test (aussi on fait des test avec Valgrind, les résultats sont stockées dans valgrind_resultats)


cd Tests/testes_robustesse
echo Testes robustesse
echo
for test in *; do
        echo $test
        cat $test
        echo
        #./test_syntaxe $test
        name="${test%.*}"
        valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=../../valgrind_resultats/"$name"_valgrind.txt \
         ../../test_syntaxe $test
        echo
done


