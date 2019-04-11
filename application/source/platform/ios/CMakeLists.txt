cmake_minimum_required (VERSION 3.2)
project (example-ios C CXX)

macro(ADD_SYSTEMFRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname} ${fwname})
    if( FRAMEWORK_${fwname} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(FATAL_ERROR "Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_SYSTEMFRAMEWORK)


# Includes
include_directories(${example-ios_SOURCE_DIR})

set(RESOURCES
  Assets.xcassets
  Base.lproj/LaunchScreen.storyboard
  Base.lproj/Main.storyboard
)
set_source_files_properties(${RESOURCES} 
PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Source files
set(SOURCES
  Info.plist
  AppDelegate.m
  main.m
  ViewController.m
  MainView.m
)

# Headers
set(HEADERS
  AppDelegate.h
  ViewController.h
  MainView.h
)

# Library
#if(BUILD_SHARED)
#  add_library (example SHARED ${SOURCES} ${HEADERS})
#  message(STATUS "Building shared version...")
#else()
#  add_library (example STATIC ${SOURCES} ${HEADERS})
#  message(STATUS "Building static version...")
#endif()

add_executable (example MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${RESOURCES})
set_property (TARGET example APPEND_STRING PROPERTY LINK_FLAGS "-ObjC")
ADD_SYSTEMFRAMEWORK(UIKit example)

set_target_properties(example PROPERTIES
            RESOURCE Assets.xcassets
            # MACOSX_BUNDLE_INFO_PLIST Info.plist
            # XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS ${CMAKE_CURRENT_SOURCE_DIR}/TimeOfSilensIOS/TimeOfSilens/TimeOfSilens.entitlements
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.alex.app"
            # Code signing
            # XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer"
            # XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "8744AVB4AD"
            # XCODE_ATTRIBUTE_PROVISIONING_PROFILE_SPECIFIER titanworldPushDevelopment

            XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] "YES")


        set(APP_BUNDLE_IDENTIFIER "com.alex.app")
        # set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.alex.app")
        set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
        set(CMAKE_XCODE_ATTRIBUTE_ENABLE_BITCODE "NO")


# Debug symbols set in XCode project
set_xcode_property(example GCC_GENERATE_DEBUGGING_SYMBOLS YES "All")

# Installation
#set(CMAKE_INSTALL_PREFIX "${example-ios_SOURCE_DIR}/../example-app/example-lib")
##install (TARGETS example DESTINATION lib)
#install (FILES ${HEADERS} DESTINATION include)
