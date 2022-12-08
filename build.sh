#!/bin/bash -e
set -e

#git clone git@ssh.dev.azure.com:v3/Stenograph/NextGen/Dev_RisLib rislib
#git checkout master
#cd rislib

#globals:
PROJECT_DIR=$(pwd)
SDK_DIR=/opt/usr_data/sdk
SHA="$(sudo git config --global --add safe.directory $PROJECT_DIR;sudo git rev-parse --verify --short HEAD)"

function parseArgs()
{
   for change in "$@"; do
      name="${change%%=*}"
      value="${change#*=}"
      eval $name="$value"
   done
}


function pushBuildDir(){
	local workdir=$(mktemp -d);
	pushd $workdir
}

function popBuildDir(){
	popd
}
function buildX86(){
	parseArgs $@
	SDK_DIR=${SDK_DIR} cmake --preset prime-clang $PROJECT_DIR
	pushd out/build/prime-clang/
	ninja
	popd
}

function buildArm(){
	parseArgs $@
	SDK_DIR=${SDK_DIR} cmake --preset prime-arm8 $PROJECT_DIR
	pushd out/build/prime-arm8/
	ninja
	popd
}

function stripArchive()
{
	local strip="${SDK_DIR}/sysroots/x86_64-fslcsdk-linux/usr/bin/aarch64-fslc-linux/aarch64-fslc-linux-strip --strip-debug --strip-unneeded -p"
	$strip *.a
}

function package(){
	parseArgs $@
	local workdir=rislib_installs
	mkdir -p $workdir/x86-build
	mkdir -p $workdir/arm-build
	mkdir -p $workdir/includes

	cp $PROJECT_DIR/RisLib/*.h $workdir/includes/
	cp out/build/prime-clang/RisLib/libRisLib.a $workdir/x86-build/
	cp out/build/prime-arm8/RisLib/libRisLib.a $workdir/arm-build/
	
	pushd $workdir/arm-build/
	stripArchive
	popd


	tar -cvJf rislib.$SHA.tar.xz $workdir
	
	echo "Package is built at $(pwd)/$workdir/rislib.$SHA.tar.xz"
	if [ -d /datadisk/nextgen/out/ ]; then
	   sudo cp -f rislib.$SHA.tar.xz /datadisk/nextgen/out/
	   echo "Package can be downloaded from https://10.57.3.4/artifacts/rislib.$SHA.tar.xz"
	fi
	if [ -d /home/$USER/Downloads ]; then
	   sudo cp -f rislib.$SHA.tar.xz /home/$USER/Downloads/
	   echo "Package is availabled at /home/$USER/Downloads/rislib.$SHA.tar.xz"
	fi
	sudo mkdir -p $PROJECT_DIR/out
	sudo cp -f rislib.$SHA.tar.xz $PROJECT_DIR/out
}

function main(){
	parseArgs $@
	pushBuildDir
	buildX86
	buildArm
	package
	popBuildDir
}

time main $@

