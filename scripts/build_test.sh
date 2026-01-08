mkdir build
cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Release -DENVY_BUILD_TEST:BOOL=TRUE
cd build
ninja