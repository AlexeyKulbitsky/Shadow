mkdir "build"
cd "build"
cmake ../../.. -G Xcode -DCMAKE_TOOLCHAIN_FILE=source/platform/ios/ios.toolchain.cmake -DPLATFORM=OS64