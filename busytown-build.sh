# This is a descendant of the output discovered by running:
#
# $ git clone https://github.com/JetBrains/kotlin.git
# $ cd kotlin/kotlin-native-tools/llvm_builder
#
# $ export PREBUILTS=$SRC/kotlin-native-toolchain/prebuilts
#
# $ python3 package \
#     --dry-run \
#     --ninja=$PREBUILTS/build-tools/linux-x86/bin/ninja \
#     --cmake=$PREBUILTS/cmake/linux-x86/bin/cmake

LLVM_PROJECT=$(pwd)
ROOT=$LLVM_PROJECT/../..

OUT=$ROOT/out
mkdir -p $OUT

PREBUILTS=$ROOT/prebuilts
export PATH=$PATH:$PREBUILTS/build-tools/linux-x86/bin

CMAKE=$PREBUILTS/cmake/linux-x86/bin/cmake
CMAKE_COMMON_OPTS= \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_ASSERTIONS=OFF \
    -DLLVM_ENABLE_TERMINFO=OFF \
    -DLLVM_INCLUDE_GO_TESTS=OFF \
    -DLLVM_ENABLE_Z3_SOLVER=OFF \
    -DCOMPILER_RT_BUILD_BUILTINS=ON \
    -DLLVM_ENABLE_THREADS=ON \
    -DLLVM_OPTIMIZED_TABLEGEN=ON \
    '-DLLVM_ENABLE_PROJECTS=clang;lld;libcxx;libcxxabi;compiler-rt' \
    -DLLVM_BUILD_LLVM_DYLIB=OFF \
    -DLLVM_LINK_LLVM_DYLIB=OFF

### Stage 1 build

STAGE1=$OUT/stage1
mkdir -p $STAGE1

STAGE1BUILD=$OUT/stage1build
mkdir -p $STAGE1BUILD
pushd $STAGE1BUILD

$CMAKE \
    -DCMAKE_INSTALL_PREFIX=$STAGE1 \
    -DLLVM_TARGETS_TO_BUILD=Native \
    $CMAKE_COMMON_OPTS \
    $LLVM_PROJECT/llvm

ninja install

popd

### Stage 2 build (bootstrapped)

DIST=$OUT/llvm_distribution

STAGE2BUILD=$OUT/stage2build
mkdir -p $STAGE2BUILD
pushd $STAGE2BUILD

$CMAKE \
    -DCMAKE_INSTALL_PREFIX=$DIST \
    -DCMAKE_C_COMPILER=$STAGE1/bin/clang \
    -DCMAKE_CXX_COMPILER=$STAGE1/bin/clang++ \
    -DCMAKE_LINKER=$STAGE1/bin/ld.lld \
    -DCMAKE_AR=$STAGE1/bin/llvm-ar \
    $CMAKE_COMMON_OPTS \
    $LLVM_PROJECT/llvm

ninja install

popd
    
