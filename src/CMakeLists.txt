cmake_minimum_required(VERSION 3.1)

project(server)

#set_target_properties(server PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME}
        "main.cpp"
        "generic_server.h"
)

target_link_libraries(${PROJECT_NAME}
                        libboost_program_options.a
                        -lboost_system
                        -lboost_thread
                        -lpthread
)
