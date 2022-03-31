set -e

# find script
SCRIPT_DIR=$(cd "$(dirname $0)" && pwd)
cd $SCRIPT_DIR

LLVM_PROJECT=$SCRIPT_DIR

ROOT=$SCRIPT_DIR/../..

if [ "$OUT_DIR" == "" ]; then
  OUT_DIR="$ROOT/out"
fi
rm -rf $OUT_DIR
mkdir -p "$OUT_DIR"
OUT_DIR="$(cd $OUT_DIR && pwd)"
export OUT_DIR

if [ "$DIST_DIR" == "" ]; then
  DIST_DIR="$OUT_DIR/dist"
fi
mkdir -p "$DIST_DIR"
export DIST_DIR

PREBUILTS=$ROOT/prebuilts
BIN_PATH=$PREBUILTS/build-tools/linux-x86/bin


# We need a host clang to build the stage1 clang to build the stage2 clang
# (build machines have a very old gcc by default)
HOST_SYSROOT=$PREBUILTS/clang/host/linux-x86/clang-r433403
HOST_CLANG_BIN=$HOST_SYSROOT/bin
#HOST_CLANG_BIN=/usr/bin/


# Add prebuilts (especially ninja) to PATH
#   (cmake won't run if ninja is not on PATH)
export PATH=$BIN_PATH:$PATH

CMAKE=$PREBUILTS/cmake/linux-x86/bin/cmake
NINJA=$BIN_PATH/ninja



function stage2Build() {
      STAGE2BUILD=$OUT_DIR/stage2build
      rm -rf $STAGE2BUILD
      mkdir -p $STAGE2BUILD
      pushd $STAGE2BUILD

      $CMAKE \
        $LLVM_PROJECT/llvm \
        -G Ninja \
        -DLLVM_ENABLE_ASSERTIONS=OFF \
        -DLLVM_ENABLE_TERMINFO=OFF \
        -DLLVM_INCLUDE_GO_TESTS=OFF \
        -DLLVM_ENABLE_Z3_SOLVER=OFF \
        -DLLVM_ENABLE_THREADS=ON \
        -DLLVM_OPTIMIZED_TABLEGEN=ON \
        -DLLVM_BUILD_LLVM_DYLIB=OFF \
        -DLLVM_LINK_LLVM_DYLIB=OFF \
        -DLLVM_ENABLE_LIBCXX=ON \
        -DLLVM_ENABLE_PROJECTS="clang;lld;compiler-rt" \
        -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=$DIST_DIR \
        -DCOMPILER_RT_BUILD_BUILTINS=ON \
        -DCLANG_ENABLE_BOOTSTRAP=On \
        -DCLANG_BOOTSTRAP_PASSTHROUGH="CMAKE_INSTALL_PREFIX;CMAKE_BUILD_TYPE" \
        -DCMAKE_C_COMPILER=$HOST_CLANG_BIN/clang \
        -DCMAKE_CXX_COMPILER=$HOST_CLANG_BIN/clang++ \
        -DCMAKE_EXE_LINKER_FLAGS="-static-libstdc++" \
        -DCMAKE_SHARED_LINKER_FLAGS="-static-libstdc++" \
        -DCMAKE_MODULE_LINKER_FLAGS="-static-libstdc++"

      $NINJA stage2-install

      popd
}

stage2Build

echo "Copying zip directly to distribution folder"
zip -r $DIST_DIR.zip $DIST_DIR
cp $DIST_DIR.zip $DIST_DIR
