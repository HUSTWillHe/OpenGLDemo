#! /usr/bin/env bash -e
cd $(dirname $0 )
pushd build
cmake ..
cmake --build .
if [[ $? != 0 ]];then
	echo "BUILD FAILED."
	exit -1
fi
cp ../resource/* ./
./Demo $1 $2
popd
