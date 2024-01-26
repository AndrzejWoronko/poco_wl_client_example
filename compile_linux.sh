#!bin/bash

rm -rf linux_build

echo "compile Debug"
cmake -DCMAKE_BUILD_TYPE=Debug -S ./ -B linux_build ; cmake --build linux_build --config Debug

echo "compile Release"
cmake -DCMAKE_BUILD_TYPE=Release -S ./ -B linux_build ; cmake --build linux_build --config Release
