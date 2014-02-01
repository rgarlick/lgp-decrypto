project(lgp-decrypto)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Weffc++ -pthread -O2")

add_executable(lgp-decrypto src/individual.cpp src/cipher.cpp src/gp.cpp src/machine.cpp src/population.cpp)
target_link_libraries(lgp-decrypto ${CMAKE_SOURCE_DIR}/libsolver.a)

install(TARGETS lgp-decrypto RUNTIME DESTINATION bin)
