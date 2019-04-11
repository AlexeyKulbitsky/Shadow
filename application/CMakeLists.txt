cmake_minimum_required(VERSION 3.4.1)

project (Application)

# Add engine
add_subdirectory(../engine "${CMAKE_CURRENT_BINARY_DIR}/engine_build")
include_directories(../engine/source)

set(APPLICATION_PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/source)
set(APPLICATION_PROJECT_NAME Application)

if(WIN32)
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
elseif(APPLE)
  SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
endif()

include_directories( ${EDITOR_PROJECT_INCLUDE_DIR} )

set(APPLICATION_SRC
  source/App.cpp
)

set(APPLICATION_INCLUDE
  source/App.h
)

if(WIN32)
  set(APPLICATION_PLATFORM_SRC
    source/platform/win/main.cpp
  )
elseif(ANDROID)
  set(APPLICATION_PLATFORM_SRC
    source/platform/android/ShadowJNI.cpp
  )
elseif(APPLE)
  if(IOS)
    set(APPLICATION_PLATFORM_SRC
      source/platform/ios/AppDelegate.m
      source/platform/ios/main.m
      source/platform/ios/ViewController.m
      source/platform/ios/MainView.m

      source/platform/ios/AppDelegate.h
      source/platform/ios/ViewController.h
      source/platform/ios/MainView.h
    )
  elseif(MACOSX)
    set(APPLICATION_PLATFORM_SRC
      source/platform/macos/main.cpp
    )
  endif()
  
endif()

set(APPLICATION_PROJECT_SOURCE_FILES
    ${APPLICATION_INCLUDE}
    ${APPLICATION_SRC}
    ${APPLICATION_PLATFORM_SRC}
)

include_directories(source)

# Group files according to files organization
foreach(FILE ${APPLICATION_PROJECT_SOURCE_FILES}) 
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    string(REPLACE "/" "\\" GROUP "${GROUP}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()

if(WIN32)
    add_executable(${APPLICATION_PROJECT_NAME} ${APPLICATION_PROJECT_SOURCE_FILES})
elseif(APPLE)
  if(IOS)
    
    set(IOS_RESOURCE_FILES
      source/platform/ios/Assets.xcassets
      source/platform/ios/Base.lproj/LaunchScreen.storyboard
      source/platform/ios/Base.lproj/Main.storyboard
    )
    set_source_files_properties(${IOS_RESOURCE_FILES} 
      PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    add_executable(${APPLICATION_PROJECT_NAME} MACOSX_BUNDLE ${APPLICATION_PROJECT_SOURCE_FILES} ${IOS_RESOURCE_FILES})
    set_target_properties(${APPLICATION_PROJECT_NAME} PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/source/platform/ios/Info.plist)

  elseif(MACOSX)
    add_executable(${APPLICATION_PROJECT_NAME} MACOSX_BUNDLE ${APPLICATION_PROJECT_SOURCE_FILES})
  endif()
    
elseif(ANDROID)
    add_library(${APPLICATION_PROJECT_NAME} SHARED ${APPLICATION_PROJECT_SOURCE_FILES})
endif()

set_target_properties(${APPLICATION_PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

target_link_libraries(${APPLICATION_PROJECT_NAME} Shadow)

if (WIN32$$$)
add_custom_command(TARGET ${APPLICATION_PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E 
                    copy ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/Shadow.dll ${PROJECT_SOURCE_DIR}/bin/$<CONFIGURATION>)
endif()
