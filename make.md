## The makefile

This document details the available makefile targets/variables.  

### Targets

#### debug

Rebuilds the project with debug symbols enables and optimisations set to their lowest.  

#### verif_valgrind

Rebuilds the project with `debug` and then executes the interpreter with valgrind memcheck on every test case, writing a log file for each in the folder `valgrind_resultats`.  

#### verif_asan

Rebuilds the project with `debug` and then executes the interpreter with ASan enabled on every test case, writing a log file for each in the folder `asan_resultats`.  

#### verif_ubsan

Rebuilds the project with `debug` and then executes the interpreter with UBSan enabled on every test case, writing a log file for each in the folder `ubsan_resultats`.  

#### create_fuzz_in

Creates a directory of test files for the fuzzer as valid input meaning all programs with calls to the built in function `lire`, which reads an input from the user, are ignored.  

#### fuzz

A target which runs [AFL++](https://github.com/aflplusplus/aflplusplus) on the interpreter.This is achieved by first rebuilding the project with the afl compiler and `create_fuzz_in` and then running the fuzzer on the interpreter. This target has two variables associated: `FUZZ_UBSAN`, `TIMEOUT`. See **variables** section for more details.  

### Variables

#### LEXEME

If defined, the lexema verifier (`test_lexeme`) is built.  

#### COMP

#### UBSAN

The project is built with UBSan enabled.  

#### ASAN

The project is built with ASan enabled.  

#### FUZZ_UBSAN

By default, the fuzzer is run with ASan enabled, if this variable is defined for the target `fuzz`, UBSan is used instead.  

#### TIMEOUT

Adds a rudimentary timeout (in seconds) to the fuzzer. This is done by running the fuzzer as a background process, while having the parent process sleep for `TIMEOUT` seconds and then killing the most recent background command which should be the fuzzer. The resulting TIMEOUT however isn't exact as it takes a few seconds for the fuzzing to start.  