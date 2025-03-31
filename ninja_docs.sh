mkdir build
cd build
cmake .. -G "Ninja" -DDOXYGEN_ACTIVE=ON
ninja doxygen-docs