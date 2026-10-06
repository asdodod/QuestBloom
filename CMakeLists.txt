cmake_minimum_required(VERSION 3.22)

if(NOT DEFINED CMAKE_ANDROID_NDK)
    if(DEFINED ENV{ANDROID_NDK_HOME})
        set(CMAKE_ANDROID_NDK "$ENV{ANDROID_NDK_HOME}")
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/ndkpath.txt")
        file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/ndkpath.txt" CMAKE_ANDROID_NDK)
    else()
        message(FATAL_ERROR "Set ANDROID_NDK_HOME or pass -DCMAKE_ANDROID_NDK=path")
    endif()
endif()
file(TO_CMAKE_PATH "${CMAKE_ANDROID_NDK}" CMAKE_ANDROID_NDK)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_ANDROID_NDK}/build/cmake/android.toolchain.cmake")
set(ANDROID_ABI arm64-v8a)
set(ANDROID_PLATFORM 24)
set(ANDROID_STL c++_static)
set(ANDROID_USE_LEGACY_TOOLCHAIN_FILE OFF)
project(questbloom VERSION 0.1.3 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern" CACHE PATH "QPM dependencies directory")
if(NOT EXISTS "${EXTERN_DIR}/includes/bs-cordl/include")
    message(FATAL_ERROR "Run qpm restore first, or pass -DEXTERN_DIR=existing/dependencies")
endif()

add_library(questbloom SHARED src/main.cpp src/Config.cpp src/SettingsView.cpp src/logging_bridge.cpp)
file(GLOB inline_hook_sources CONFIGURE_DEPENDS
    "${EXTERN_DIR}/includes/beatsaber-hook/shared/inline-hook/*.c"
    "${EXTERN_DIR}/includes/beatsaber-hook/shared/inline-hook/*.cpp")
target_sources(questbloom PRIVATE ${inline_hook_sources})
target_compile_definitions(questbloom PRIVATE MOD_ID="questbloom" VERSION="${PROJECT_VERSION}"
    QUEST UNITY_2021 NEED_UNSAFE_CSHARP HAS_CODEGEN FMT_HEADER_ONLY)
target_compile_options(questbloom PRIVATE -frtti -fexceptions -fvisibility=hidden -fdeclspec)
# Cordl headers use offsetof on managed layouts and compiler extensions.
target_compile_options(questbloom PRIVATE -Wno-invalid-offsetof -Wno-extra-qualification)
target_include_directories(questbloom PRIVATE include "${EXTERN_DIR}/includes"
    "${EXTERN_DIR}/includes/bs-cordl/include")
target_include_directories(questbloom SYSTEM PRIVATE
    "${EXTERN_DIR}/includes/libil2cpp/il2cpp/libil2cpp"
    "${EXTERN_DIR}/includes/libil2cpp/il2cpp/external/baselib/Include"
    "${EXTERN_DIR}/includes/libil2cpp/il2cpp/external/baselib/Platforms/Android/Include"
    "${EXTERN_DIR}/includes/paper2_scotland2/shared/utfcpp/source"
    "${EXTERN_DIR}/includes/fmt/fmt/include")
target_link_directories(questbloom PRIVATE "${EXTERN_DIR}/libs")
target_link_libraries(questbloom PRIVATE beatsaber-hook custom-types bsml sl2 log)
target_link_options(questbloom PRIVATE -Wl,--no-undefined)
add_custom_command(TARGET questbloom POST_BUILD
    COMMAND "${CMAKE_STRIP}" --strip-debug "$<TARGET_FILE:questbloom>"
    VERBATIM)
