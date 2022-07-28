#! /usr/bin/env bash -e
cd $(dirname $0 )
pushd build
cmake ..
cmake --build .
if [[ $? != 0 ]];then
	echo "BUILD FAILED."
	exit -1
fi
cp -r ../resource/* ./
cp  ../src/texture* ./
cp  ../src/skyTexture* ./
cp  ../src/cubetexture* ./
./Demo
popd
