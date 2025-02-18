mkdir ./build/
cd ./build/
cmake .. -G "Visual Studio 17 2022"
cmake --build build -- /m
cmake --build . --config Debug
cmake --build . --config Release