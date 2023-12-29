cmake_minimum_required(VERSION 3.12)
Project(NetworkExample)

set(CMAKE_CXX_STANDARD 17) #setting standard to 17 

#Set path to the asio library
set(ASIO_INCLUDE_DIR "lib/asio-1.28.0/include") #required the header lib to be in located here

# #adding src files
# add_executable(${PROJECT_NAME} src/main.cpp )

link_directories(src/include)

# #include asio lib
# target_include_directories(${PROJECT_NAME} PRIVATE 
# ${ASIO_INCLUDE_DIR}
# src/include
# )

## Compiler flags
# target_compile_options(${PROJECT_NAME} PRIVATE ...)

## Linker flags
# target_link_libraries(${PROJECT_NAME} PRIVATE src/include)

#cmake will not rebuild if new file is added
file(GLOB SOURCE_FILES "src/*.cpp")

foreach( SOURCE_PATH ${SOURCE_FILES})

get_filename_component(EXECUTABLE_NAME ${SOURCE_PATH} NAME_WE)

add_executable(${EXECUTABLE_NAME} ${SOURCE_PATH})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ASIO_INCLUDE_DIR} src/include)

endforeach(SOURCE_PATH ${SOURCE_FILES})



