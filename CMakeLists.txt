project(lgp-decrypto)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -pthread -O3")

add_executable(lgp-decrypto src/zkd/files.cpp src/zkd/zodiac.cpp src/zkd/solve.cpp src/zkd/z340.cpp src/zkd/strarray.cpp src/zkd/message.cpp src/zkd/map.cpp src/zkd/main.cpp src/individual.cpp src/cipher.cpp src/gp.cpp src/machine.cpp src/population.cpp)

install(TARGETS lgp-decrypto RUNTIME DESTINATION bin)
add_subdirectory(src/zkd)