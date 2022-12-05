#!/bin/sh

# if [[ "$target_platform" == "osx-64" ]]; then
#   TOOLS_DIR=$(dirname $($FC --print-libgcc-file-name))
#   if [[ ! -f "$TOOLS_DIR/ld" ]]; then
#     ln -sf $LD $TOOLS_DIR/ld
#     ln -sf $LD $BUILD_PREFIX/bin/ld
#   fi
# fi

# if [[ "$target_platform" == linux-* || "$target_platform" == "osx-arm64"  ]]
# then
#   # Workaround for https://github.com/conda-forge/scalapack-feedstock/pull/30#issuecomment-1061196317
#   # As of March 2022, on macOS arm64 gfortran 9 is still used
#   export FFLAGS="${FFLAGS} -fallow-argument-mismatch"
#   export OMPI_FCFLAGS=${FFLAGS}
# fi



# if [[ "$CONDA_BUILD_CROSS_COMPILATION" == "1" ]]; then
#   # This is only used by open-mpi's mpicc
#   # ignored in other cases
#   export OMPI_CC=$CC
#   export OMPI_CXX=$CXX
#   export OMPI_FC=$FC
#   export OPAL_PREFIX=$PREFIX
#   export EXTRA_CMAKE="-DCDEFS=Add_ "
# fi

# # As mpi libraries are not correctly linked in CMake scripts, use mpi wrappers for the compilers
# export CC=mpicc
# export CXX=mpic++
# export FC=mpifort

# mkdir build && cd build
# cmake ${CMAKE_ARGS} \
#     $EXTRA_CMAKE \
#     -DBLAS_LIBRARIES="blas" \
#     -DLAPACK_LIBRARIES="lapack" \
#     -DBUILD_SHARED_LIBS=ON \
#     -DCMAKE_INSTALL_PREFIX="$PREFIX" \
#     -DCMAKE_PREFIX_PATH="$PREFIX" \
#     -DCMAKE_BUILD_TYPE=Release \
#     .. || (cat CMakeFiles/CMakeOutput.log && cat CMakeFiles/CMakeError.log && exit 1)

# make install -j${CPU_COUNT}

make

mkdir -p "${PREFIX}/bin"
cp bin/REEF3D "${PREFIX}/bin"
chmod 755 "${PREFIX}/bin/reef3d"