cmake_minimum_required(VERSION 3.0)

project(hawk LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()
include(Specifications.txt)
find_package(Qt5 COMPONENTS Core Quick LinguistTools Network REQUIRED)

set(TS_FILES hawk_en_US.ts)
set(QRC_FILES
    qml.qrc)

aux_source_directory(. ROOT_SRCS)
aux_source_directory(Core CORE_SRCS)

if(ANDROID)
#    add_library(hawk SHARED
#      main.cpp
#      qml.qrc
#      ${TS_FILES}
#    )
    add_library(hawk
      ${ROOT_SRCS}
      ${CORE_SRCS}
      ${TS_FILES}
      ${QRC_FILES}
    )
else()
#    add_executable(hawk
#      main.cpp
#      qml.qrc
#      ${TS_FILES}
#    )
    add_executable(hawk
      ${ROOT_SRCS}
      ${CORE_SRCS}
      ${TS_FILES}
      ${QRC_FILES}
    )
endif()

target_compile_definitions(hawk
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(hawk
  PRIVATE Qt5::Core Qt5::Quick)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
