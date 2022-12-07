#!/bin/bash -xe
set -xe

#git clone git@ssh.dev.azure.com:v3/Stenograph/NextGen/Dev_RisLib rislib
#git checkout master
#cd rislib

#globals:
PROJECT_DIR=$(pwd)
SHA="$(sudo git config --global --add safe.directory $PROJECT_DIR;sudo git rev-parse --verify --short HEAD)"

function parseArgs()
{
   for change in "$@"; do
      name="${change%%=*}"
      value="${change#*=}"
      eval $name="$value"
   done
}


function buildX86(){
	parseArgs $@
	cmake --preset prime-clang
	pushd out/build/prime-clang/
	ninja
	popd
}

function buildArm(){
	parseArgs $@
	cmake --preset prime-arm8
	pushd out/build/prime-arm8/
	ninja
	popd
}

function package(){
	parseArgs $@
	local workdir=$(mktemp -d)
	mkdir -p $workdir/rislib_installs/x86-build
	mkdir -p $workdir/rislib_installs/arm-build
	mkdir -p $workdir/rislib_installs/includes

	cp $PROJECT_DIR/RisLib/*.h $workdir/rislib_installs/includes/
	cp $PROJECT_DIR/out/build/prime-clang/RisLib/libRisLib.a $workdir/rislib_installs/x86-build/
	cp $PROJECT_DIR/out/build/prime-arm8/RisLib/libRisLib.a $workdir/rislib_installs/arm-build/

	pushd $workdir
	tar -cvJf rislib.$SHA.tar.xz rislib_installs
	popd
	
	echo "Package is built at $workdir/rislib.$SHA.tar.xz"
	if [ -d /datadisk/nextgen/out/ ]; then
	   sudo cp -f $workdir/rislib.$SHA.tar.xz /datadisk/nextgen/out/
	   echo "Package can be downloaded from https://10.57.3.4/artifacts/rislib.$SHA.tar.xz"
	fi
	if [ -d /home/$USER/Downloads ]; then
	   sudo cp -f $workdir/rislib.$SHA.tar.xz /home/$USER/Downloads/
	   echo "Package is availabled at /home/$USER/Downloads/rislib.$SHA.tar.xz"
	fi
}

function main(){
	parseArgs $@
	buildX86
	buildArm
	package
}

time main $@

