#!bin/bash

export PATH=$PATH:"./x86-windows/debug/lib:./x86-windows/debug/bin:./win32_build/Debug:./x86-windows/lib:./x86-windows/bin:./win32_build/Release"

echo "Set path to cmake !!!"

CMAKE=/c/Qt/Tools/CMake_64/bin/cmake.exe

rm -rf win32_build

echo "compile Debug Windows 32"
${CMAKE} -DCMAKE_BUILD_TYPE=Debug -S ./  -A Win32 -B win32_build ; ${CMAKE} --build win32_build --config Debug

echo "compile Release Windows 32"
${CMAKE} -DCMAKE_BUILD_TYPE=Release -S ./ -A Win32 -B win32_build ; ${CMAKE} --build win32_build --config Release
