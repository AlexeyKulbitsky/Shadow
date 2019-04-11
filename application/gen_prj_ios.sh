mkdir "Xcode_proj_IOS"
cd "Xcode_proj_IOS"
cmake .. -G Xcode -DCMAKE_TOOLCHAIN_FILE=source/platform/ios/ios.toolchain.cmake -DPLATFORM=OS64