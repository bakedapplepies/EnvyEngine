cd ..
mkdir build
cd build
mkdir Release
cd ..
cmake -S . -B build/Release -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Release -DENVY_BUILD_TEST:BOOL=TRUE
cd build
ninja