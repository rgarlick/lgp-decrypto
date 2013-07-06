project(lgp-decrypto)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -pthread")

add_executable(lgp-decrypto src/individual.cpp src/cipher.cpp src/gp.cpp src/machine.cpp src/population.cpp)

install(TARGETS lgp-decrypto RUNTIME DESTINATION bin)

add_subdirectory(src)
