if [ $# != 1 ] || [ $1 != armeabi-v7a -a $1 != arm64-v8a -a $1 != x86_64 -a $1 != x86 ]; then
echo "argcount = $#"
echo "USAGE: $0 ANDROID_ABI"
exit 1;
fi
TMP_ANDROID_ABI=$1

TOP_DIR=$(cd $(dirname $0);pwd)
echo $TOP_DIR

if [ ! $ANDROID_NDK_HOME ]; then
export ANDROID_NDK_HOME="/Users/qiaopingliu/Library/Android/sdk/ndk/21.3.6528147"
else
echo "ANDROID_NDK_HOME:"$ANDROID_NDK_HOME
fi

rm -rf $TOP_DIR/bin/android/${TMP_ANDROID_ABI}
rm -rf $TOP_DIR/build

if [ ! -d "$TOP_DIR/build" ]; then
  mkdir -p $TOP_DIR/build
fi

cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake -DANDROID_NATIVE_API_LEVEL=19  -DANDROID_ABI=${TMP_ANDROID_ABI} -DCMAKE_INSTALL_PREFIX=$TOP_DIR/bin/android/${TMP_ANDROID_ABI} -B$TOP_DIR/build -H$TOP_DIR
cd $TOP_DIR/build
cmake --build .
cmake --install .