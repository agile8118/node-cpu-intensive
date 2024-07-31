#!/bin/sh

# This script will download and combile GMP (GNU Multiple Precision Arithmetic Library) from source code.

# Set GMP version and download URL
GMP_VERSION="6.3.0"
GMP_URL="https://gmplib.org/download/gmp/gmp-${GMP_VERSION}.tar.xz"

# Set directory where GMP will be downloaded and extracted
DEPS_DIR="deps"
GMP_DIR="${DEPS_DIR}/GMP"

# Create deps directory if it doesn't exist
mkdir -p ${DEPS_DIR}

# Download GMP source code
curl -L ${GMP_URL} -o ${DEPS_DIR}/gmp-${GMP_VERSION}.tar.xz

# Extract GMP source code to the target directory
mkdir -p ${GMP_DIR}
tar -xvf ${DEPS_DIR}/gmp-${GMP_VERSION}.tar.xz -C ${GMP_DIR} --strip-components=1

# Remove tarball
rm ${DEPS_DIR}/gmp-${GMP_VERSION}.tar.xz

echo "GMP ${GMP_VERSION} has been downloaded and extracted to ${GMP_DIR}"

# Configure GMP
cd ${GMP_DIR}
./configure --enable-cxx --disable-shared
make
make check

echo "GMP setup is complete."