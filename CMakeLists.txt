cmake_minimum_required(VERSION 3.15)

project(oop)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configure required libraries ...
if(UNIX) # Darwing or Linux

    find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
    find_package(Threads REQUIRED)

    if(APPLE)
        # Prefer a homebrew version of OpenSSL over the one in /usr/lib
        file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@3)
        # Prefer the latest (make the latest one first)
        list(REVERSE OPENSSL_ROOT_DIR)

        find_package(OpenSSL 3.0.2 REQUIRED)
        set(OPENSSL_VERSION "3.0.2")
    else()
        find_package(OpenSSL 3.0.2 REQUIRED)
        set(OPENSSL_VERSION "3.0.2")
    endif()

elseif(WIN32) # Windows systems including Win64.
    message(FATAL_ERROR "-- Windows is not supported for now.")
else()
    message(FATAL_ERROR "-- Unsupported platform sorry! :( ")
endif()

# Configure compiler options ...
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

    message("-- configuring clang options")
    set(CMAKE_CXX_FLAGS "-arch x86_64 -std=c++11 -stdlib=libc++ -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations")

elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

    message("-- configuring gcc options")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -DBOOST_LOG_DYN_LINK")

endif()

# Project construction ...

# headers search paths ...
set(CPPRESTSDK_INCLUDE_DIR "./libs/cpprestsdk/Release/include")
set(POSTGRES_INCLUDE_DIR "./libs/libpqxx/include")

set(PROJECT_INCLUDE_DIR "./headers/")
set(HEADER_SEARCH_PATHS ${CPPRESTSDK_INCLUDE_DIR} ${POSTGRES_INCLUDE_DIR} ${PROJECT_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

# library search paths ...
if(APPLE)
    set(OPENSSL_LIBS "/usr/local/Cellar/openssl@3/${OPENSSL_VERSION}/lib/libssl.3.dylib;/usr/local/Cellar/openssl@3/${OPENSSL_VERSION}/lib/libcrypto.3.dylib")
    set(CPPRESTSDK_LIBRARY "${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.release/Binaries/libcpprest.a")
    set(POSTGRES_LIBRARY "${PROJECT_SOURCE_DIR}/libs/libpqxx/build.release/src/libpqxx.a")
    set(ZIP_LIBRARY "/usr/local/Cellar/zlib/1.2.11/lib/libz.dylib")

    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${CPPRESTSDK_LIBRARY} ${ZIP_LIBRARY} ${POSTGRES_LIBRARY})
else()
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    set(CPPRESTSDK_LIBRARY "${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.release/Binaries/libcpprest.a")
    set(POSTGRES_LIBRARY "${PROJECT_SOURCE_DIR}/libs/libpqxx/build.release/src/libpqxx.a")

    set(LIBRARIES_SEARCH_PATHS ${CPPRESTSDK_LIBRARY} ${OPENSSL_LIBS} ${Boost_LIBRARIES} ${POSTGRES_LIBRARY})
endif()

include_directories(${HEADER_SEARCH_PATHS})
# source files ...
add_executable(${PROJECT_NAME} main.cpp core/headers/Produs.h core/sources/Produs.cpp core/headers/Angajat.h core/sources/Angajat.cpp core/headers/Tranzactie.h core/sources/Tranzactie.cpp core/headers/Depozit.h core/sources/Depozit.cpp service/headers/HTTPListener.h infra/headers/Utilities.h service/sources/HTTPListener.cpp infra/headers/SQLRepository.h infra/sources/SQLRepository.cpp service/sources/AngajatController.cpp service/headers/AngajatController.h core/headers/Controller.h service/sources/WebService.cpp service/headers/WebService.h)

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} "-framework Security")
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
    target_link_libraries(${PROJECT_NAME} ${PQXX_LIB})
    target_link_libraries(${PROJECT_NAME} ${PQ_LIB})
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-W1, -F/Library/Frameworks")
else()
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_SEARCH_PATHS})
    target_link_libraries(${PROJECT_NAME} ${PQXX_LIB})
    target_link_libraries(${PROJECT_NAME} ${PQ_LIB})
endif()

###############################################################################

# custom compiler flags
message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
        target_link_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
    endif()
endif()

###############################################################################

# sanitizers
# should disable them when releasing executables
if(APPLE)
    # if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    # first check Apple since Apple is also a kind of Unix
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
    # endif()
elseif(UNIX)
    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        # we cannot mix sanitizers with Valgrind
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "12.")
            # check leaks on Linux since macOS does not support the leaks sanitizer yet
            # leaks sanitizer is enabled by default on Linux, so we do not need to enable it explicitly
            target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
            target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=memory,undefined -fsanitize-memory-track-origins)
            target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=memory,undefined -fsanitize-memory-track-origins -Wl,-rpath,tools/llvm-project/build/lib)
        endif()
    endif()
endif()

###############################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION bin)