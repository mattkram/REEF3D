#!/bin/sh

# make install -j${CPU_COUNT}

make

mkdir -p "${PREFIX}/bin"
cp bin/REEF3D "${PREFIX}/bin"
chmod 755 "${PREFIX}/bin/reef3d"