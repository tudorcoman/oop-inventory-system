#!/bin/bash
set -e

# Credits:
#   https://github.com/ivanmejiarocha/micro-service/blob/master/libs/build_dependencies.sh
#   https://www.tutorialspoint.com/postgresql/postgresql_c_cpp.htm

# WIP: https://gist.github.com/Shauren/5c28f646bf7a28b470a8
# https://github.com/boostorg/type_erasure/issues/16
# meh: https://gist.github.com/zrsmithson/0b72e0cb58d0cb946fc48b5c88511da8

BOOST_MV="1"
BOOST_mV="78"
BOOST_pV="0"
BOOST_DOT_VER=$BOOST_MV.$BOOST_mV.$BOOST_pV
BOOST_VER=$BOOST_MV\_$BOOST_mV\_$BOOST_pV
RESTSDK_VERSION="v2.10.18"
PQXX_VERSION="7.7.3"
DEFAULT_LIB_DIRECTORY_PATH=$(pwd) #"."
#OPENSSL_ROOT_DIR="/usr/local/opt/openssl"
#MAKE_COMMAND=mingw32-make

libDir=${1:-$DEFAULT_LIB_DIRECTORY_PATH}

install_boost() {
  #if [ -d boost_1_78_0 ]; then
  #    rm -rf boost_1_78_0
  #fi
  curl -L -O https://boostorg.jfrog.io/artifactory/main/release/$BOOST_DOT_VER/source/boost_$BOOST_VER.tar.gz
  tar -xzf boost_$BOOST_VER.tar.gz
  rm -rf boost_1_78_0.tar.gz
  #./b2 --with-system --with-date_time --with-regex variant=release link=shared toolset=gcc address-model=64
   #./b2 --with-system --with-date_time --with-regex variant=release link=shared toolset=gcc address-model=64 architecture=x64 runtime-link=shared install --prefix=install2 -d0
  if [ $BOOST_TOOLSET == "msvc" ];
  then
    (cd boost_$BOOST_VER && \
      ./bootstrap.bat --with-toolset=$BOOST_TOOLSET && \
      ./b2 $BOOST_LIBS toolset=$BOOST_TOOLSET variant=release link=$BOOST_LINK address-model=64 runtime-link=$BOOST_LINK install --prefix=install_dir -d0)
  else
    (cd boost_$BOOST_VER && \
      ./bootstrap.sh --with-toolset=$BOOST_TOOLSET && \
      ./b2 $BOOST_LIBS toolset=$BOOST_TOOLSET variant=release link=$BOOST_LINK address-model=64 runtime-link=$BOOST_LINK install --prefix=install_dir -d0)
  fi
}

# install_openssl() {
#     opensslDir=$libDir/openssl
#     opensslBuildDir=$opensslDir/build

#     git clone --depth=200 https://github.com/openssl/openssl $opensslDir || true
#     (cd $opensslDir && (git checkout -b openssl-3.0 || git checkout openssl-3.0) && git submodule update --init --depth=20)

#     mkdir -p $opensslBuildDir
#     (cd $opensslDir && ./Configure --prefix=build --openssldir=ssl '-Wl,-rpath,$(LIBRPATH)' && $MAKE_COMMAND)
# }

install_cpprestsdk(){
	restsdkDir="$libDir/cpprestsdk"
	restsdkBuildDir="$restsdkDir/build.release"

   #if [ -d "$restsdkDir" ]; then
   #   rm -rf "$restsdkDir"
   #fi

	git clone --depth=20 https://github.com/Microsoft/cpprestsdk.git "$restsdkDir"
	(cd $restsdkDir && (git checkout tags/$RESTSDK_VERSION -b $RESTSDK_VERSION || git checkout tags/$RESTSDK_VERSION) && git submodule update --init)
	mkdir -p "$restsdkBuildDir"
	#if [[ "$OSTYPE" == "linux-gnu" ]]; then
	#	export CXX=g++-4.9
	#fi
  # -DBoost_NO_WARN_NEW_VERSIONS=1 \
	(cd "$restsdkBuildDir" && \
    cmake ../Release \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
      -DWERROR=OFF \
      -DBUILD_TESTS=OFF \
      -DBUILD_SAMPLES=OFF \
      -DCMAKE_INSTALL_PREFIX=install_dir \
      -DBOOST_ROOT=$libDir/boost_1_78_0/install_dir \
      -DBoost_USE_STATIC_LIBS=$BOOST_USE_STATIC \
      -DBoost_USE_STATIC_RUNTIME=$BOOST_USE_STATIC && \
    cmake --build . -j$(nproc) && \
    cmake --install . --prefix install_dir) || exit
	#(cd "$restsdkBuildDir" && make)
}

install_postgresql(){
  postgresqlDir="$libDir/libpqxx"
  postgresqlBuildDir="$postgresqlDir/build.release"

  #if [ -d "$postgresqlDir" ]; then
  #  rm -rf "$postgresqlDir"
  #fi

  git clone --depth=20 https://github.com/jtv/libpqxx "$postgresqlDir"
  (cd "$postgresqlDir" && git checkout tags/$PQXX_VERSION -b $PQXX_VERSION || git checkout tags/$PQXX_VERSION)
  mkdir -p "$postgresqlBuildDir"
  (cd "$postgresqlBuildDir" && \
    cmake ../ \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=$BUILD_SHARED_LIBS \
      -DCMAKE_INSTALL_PREFIX=install_dir && \
    cmake --build . -j$(nproc) && \
    cmake --install . --prefix install_dir) || exit
  #(cd "$postgresqlBuildDir" && make)
}

mkdir -p "$libDir"
install_boost
# install_openssl
install_cpprestsdk
install_postgresql
