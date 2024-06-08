#!/bin/bash

# Run the assembler on each test file and compare output to expected results
for test_file in tests/*.asm; do
    ./bin/assembler "$test_file" > "${test_file%.asm}.out"
    diff "${test_file%.asm}.out" "${test_file%.asm}.expected" > /dev/null
    if [ $? -eq 0 ]; then
        echo "Test $test_file passed"
    else
        echo "Test $test_file failed"
    fi
done

