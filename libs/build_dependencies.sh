#!/bin/bash
# Credits:
#   https://github.com/ivanmejiarocha/micro-service/blob/master/libs/build_dependencies.sh
#   https://www.tutorialspoint.com/postgresql/postgresql_c_cpp.htm

RESTSDK_VERSION="v2.10.6"
PQXX_VERSION="7.7.0"
DEFAULT_LIB_DIRECTORY_PATH="."
OPENSSL_ROOT_DIR="/usr/local/opt/openssl"

libDir=${1:-$DEFAULT_LIB_DIRECTORY_PATH}

install_cpprestsdk(){
	restsdkDir="$libDir/cpprestsdk"
	restsdkBuildDir="$restsdkDir/build.release"

   if [ -d "$restsdkDir" ]; then
      rm -rf "$restsdkDir"
   fi

	git clone https://github.com/Microsoft/cpprestsdk.git "$restsdkDir"
	(cd $restsdkDir && git submodule update --init && git checkout tags/$RESTSDK_VERSION -b $RESTSDK_VERSION)
	mkdir "$restsdkBuildDir"
	if [[ "$OSTYPE" == "linux-gnu" ]]; then
		export CXX=g++-4.9
	fi
	(cd "$restsdkBuildDir" && cmake ../Release -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF)
	(cd "$restsdkBuildDir" && make)
}

install_postgresql(){
  postgresqlDir="$libDir/libpqxx"
  postgresqlBuildDir="$postgresqlDir/build.release"

  git clone https://github.com/jtv/libpqxx "$postgresqlDir"
  (cd "$postgresqlDir" && git submodule update --init && git checkout tags/$PQXX_VERSION -b $PQXX_VERSION)
  mkdir "$postgresqlBuildDir"
  (cd "$postgresqlBuildDir" && cmake ../ -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF)
  #(cd $postgresqlDir && ./configure && make && make install) || exit
  (cd "$postgresqlBuildDir" && make)
}

#mkdir -p "$libDir"
#install_cpprestsdk
install_postgresql