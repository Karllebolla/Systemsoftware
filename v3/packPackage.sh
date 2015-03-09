#!/bin/sh

path_src=$1
path_dest=$2

if [ $# -ne 2 ]
then
echo "$0 <absolute path src> <absolute path destination>: <absolute path src> is the path to the src files\
										<absolute path destination> path to buildroot/dl"
return 1
fi

#if ! [-d ${pathToCopy}]
#then
#echo "$1 is no directory"
#return 1
#fi

cd ${path_src}

for VZ in *
do
if [ -d ${VZ} ]
then
tar -cf ${VZ}.tar ${VZ}

fi
done

cd ${path_dest}
cp ${path_src}/*.tar .
