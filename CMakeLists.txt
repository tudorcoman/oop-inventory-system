cmake_minimum_required(VERSION 3.15)
project(oop)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Boost_USE_MULTITHREADED TRUE)

find_package(Boost REQUIRED COMPONENTS system thread log program_options chrono)
find_package(Threads REQUIRED)

option(GITHUB_ACTIONS "Running on GitHub Actions" OFF)

if(UNIX) # Darwin or Linux
    if(APPLE AND NOT GITHUB_ACTIONS)
        # Prefer a homebrew version of OpenSSL over the one in /usr/lib
        file(GLOB OPENSSL_ROOT_DIR /usr/local/Cellar/openssl@3)
        # Prefer the latest (make the latest one first)
        list(REVERSE OPENSSL_ROOT_DIR)
    endif()
endif()

find_package(OpenSSL)
find_package(PostgreSQL)


add_executable(${PROJECT_NAME} main.cpp core/headers/Produs.h core/sources/Produs.cpp core/headers/Angajat.h core/sources/Angajat.cpp core/headers/Tranzactie.h core/sources/Tranzactie.cpp core/headers/Depozit.h core/sources/Depozit.cpp service/headers/HTTPListener.h infra/headers/Utilities.h service/sources/HTTPListener.cpp infra/headers/SQLRepository.h infra/sources/SQLRepository.cpp service/sources/AngajatController.cpp service/headers/AngajatController.h core/headers/Controller.h service/sources/WebService.cpp service/headers/WebService.h core/headers/CrudRepository.h infra/sources/AngajatRepository.cpp infra/headers/AngajatRepository.h core/sources/TableField.cpp core/headers/TableField.h infra/sources/ProdusRepository.cpp infra/headers/ProdusRepository.h service/sources/ProdusController.cpp service/headers/ProdusController.h core/headers/JsonEntity.h core/sources/Repository.tpp core/headers/Repository.h infra/sources/DepoziteRepository.cpp infra/headers/DepoziteRepository.h service/sources/DepozitController.cpp service/headers/DepozitController.h infra/sources/TranzactieRepository.cpp infra/headers/TranzactieRepository.h service/sources/TranzactieController.cpp service/headers/TranzactieController.h core/headers/Exceptions.h core/headers/ObjectFactory.h)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("-- configuring clang options")
    target_compile_options(${PROJECT_NAME} PRIVATE -DBOOST_LOG_DYN_LINK -Wno-deprecated-declarations)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("-- configuring gcc options")
    target_compile_options(${PROJECT_NAME} PRIVATE -DBOOST_LOG_DYN_LINK)
endif()

set(CPPRESTSDK_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/cpprestsdk/Release/include CACHE PATH "CPP Rest SDK include dir")
#set(PQXX_INCLUDE_DIR_gen ${PROJECT_BINARY_DIR}/libpqxx/include CACHE PATH "PQXX include dir (generated)")
set(PQXX_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/libpqxx/include CACHE PATH "PQXX include dir")


if(UNIX)
    SET(BUILD_SHARED_LIBS OFF)
else()
    SET(BUILD_SHARED_LIBS ON)
endif()
set(SKIP_BUILD_TEST ON)
add_subdirectory(libs/libpqxx)

set(HEADER_SEARCH_PATHS ${CPPRESTSDK_INCLUDE_DIR} ${PQXX_INCLUDE_DIR_gen} ${PQXX_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})

set(CPPRESTSDK_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.release/Binaries CACHE PATH "CPP Rest SDK lib dir")
#set(PQXX_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/libpqxx/build.release/src CACHE PATH "PQXX lib dir")

if(APPLE)
    if(NOT GITHUB_ACTIONS)
        set(OPENSSL_LIBS "/usr/local/Cellar/openssl@3/${OPENSSL_VERSION}/lib/libssl.3.dylib;/usr/local/Cellar/openssl@3/${OPENSSL_VERSION}/lib/libcrypto.3.dylib")
    else()
        set(OPENSSL_LIBS ${OPENSSL_LIBRARIES})
    endif()
    #set(CPPRESTSDK_LIBRARY "${PROJECT_SOURCE_DIR}/libs/cpprestsdk/build.release/Binaries/") #libcpprest.a")
    #set(POSTGRES_LIBRARY "${PROJECT_SOURCE_DIR}/libs/libpqxx/build.release/src/") #libpqxx.a")
    find_package(ZLIB)
    #set(ZIP_LIBRARY_PATH "/usr/local/Cellar/zlib/1.2.11/lib/" CACHE PATH "Zlib library path")

    set(LIBRARIES_SEARCH_PATHS ${Boost_LIBRARY_DIRS} ${CPPRESTSDK_LIBRARY} ${ZIP_LIBRARY_PATH} ${POSTGRES_LIBRARY})
else()
    set(OPENSSL_LIBS "${OPENSSL_LIBRARIES}")
    #set(CPPRESTSDK_LIBRARY "/usr/lib/build.release/Binaries/libcpprest.so")
    #set(POSTGRES_LIBRARY "${PROJECT_SOURCE_DIR}/libs/libpqxx/build.release/src/libpqxx.a")

    set(LIBRARIES_SEARCH_PATHS ${OPENSSL_LIBS} ${Boost_LIBRARY_DIRS})
endif()



#include(FetchContent)
#
#FetchContent_Declare(
#        SomeLib
#        GIT_REPOSITORY https://github.com/<SomeLib>/<SomeLib>.git
#        GIT_TAG        <some_git_hash> # <which tag/branch @ <some_date>>
#)
#
#FetchContent_MakeAvailable(SomeLib)

message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libstdc++)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        target_compile_options(${PROJECT_NAME} PRIVATE -stdlib=libc++)
    endif()
endif()

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
        # we might use this sanitizer on Clang 10 on Linux
        # we cannot mix sanitizers with Valgrind
        # target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
        # target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "12.")
            # check leaks on Linux since macOS does not support the leaks sanitizer yet
            # leaks sanitizer seems is enabled by default on Linux (according to docs),
            # so we do not enable it explicitly
            target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
            target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
        else()
            target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=memory,undefined)
            target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=memory,undefined)
        endif()
    endif()
endif()

# use system so clang-tidy does not report warnings from these folders
#target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${<SomeLib>_SOURCE_DIR}/include)
#target_link_directories(${PROJECT_NAME} PRIVATE ${<SomeLib>_BINARY_DIR}/lib)
#target_link_libraries(${PROJECT_NAME} <SomeLib>)

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${HEADER_SEARCH_PATHS})
target_link_directories(${PROJECT_NAME} PRIVATE ${Boost_LIBRARY_DIRS} ${CPPRESTSDK_LIB_DIR} ${PQXX_LIB_DIR})

if (APPLE)
    #target_link_directories(${PROJECT_NAME} PRIVATE ${ZIP_LIBRARY})
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework Security")
    #target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES_SEARCH_PATHS})
    if(GITHUB_ACTIONS)
        target_link_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_SEARCH_PATHS})
    else()
    endif()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBS} cpprest pqxx PostgreSQL::PostgreSQL ZLIB::ZLIB)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-W1, -F/Library/Frameworks")
elseif(UNIX)
    #target_link_directories(${PROJECT_NAME} PRIVATE ${CPPRESTSDK_LIB_DIR} ${PQXX_LIB_DIR} ${LIBRARIES_SEARCH_PATHS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBS} cpprest pqxx Threads::Threads PostgreSQL::PostgreSQL)
else()
    # link pqxx dynamically on windows
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBS} cpprest pqxx PostgreSQL::PostgreSQL)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)