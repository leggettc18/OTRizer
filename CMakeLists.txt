cmake_minimum_required(VERSION 3.24.0)
project(OTRizer VERSION 0.2)
add_library(OTRizer Resource.cpp)
add_library(OTRizer::OTRizer ALIAS OTRizer)
option(BUILD_SHARED_LIBS "Build shared library" OFF)
include(GenerateExportHeader)
generate_export_header(OTRizer
    EXPORT_MACRO_NAME OTRIZER_API
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/OTRizer/core/common.h
)
target_compile_features(OTRizer PUBLIC cxx_std_20)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(OTRizer PRIVATE
    "$<${gcc_like_cxx}:$<$<CONFIG:Debug>:-O0;-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
    "$<${msvc_like_cxx}:$<$<CONFIG:Debug>:/Od;/Wall;/Zi;>>"
)
target_include_directories(OTRizer
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(OTRizer PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

find_package(libultraship)
target_link_libraries(OTRizer PUBLIC libultraship::libultraship)

include(GNUInstallDirs)

install(TARGETS OTRizer
    EXPORT OTRizerTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/OTRizer
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT OTRizerTargets
    FILE OTRizerTargets.cmake
    NAMESPACE OTRizer::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OTRizer
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/OTRizerConfig.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/OTRizerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OTRizer
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/OTRizerConfigVersion.cmake
    VERSION ${OTRIZER_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/OTRizerConfig.cmake
        ${CMAKE_BINARY_DIR}/cmake/OTRizerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OTRizer
)

export(EXPORT OTRizerTargets
    FILE ${CMAKE_BINARY_DIR}/cmake/OTRizerTargets.cmake
    NAMESPACE OTRizer::
)
