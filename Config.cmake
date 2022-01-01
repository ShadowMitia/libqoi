# Enable address sanitizer.
set_option(ASAN OFF)

# Enable Cpp Check, if available.
set_option(CPP_CHECK ON)

# Enable Clang tidy, if available.
set_option(CLANG_TIDY ON)

# Enable Include-what-you-use, if available.
set_option(INCLUDE_WHAT_YOU_USE ON)

# Enable Ccache, if available.
set_option(CCACHE ON)

# Enable google Gtest testing platform.
set_option(GTEST_DOWNLOAD_IF_MISSING OFF)

# Enable Catch2 testing platform.
set_option(CATCH2_DOWNLOAD_IF_MISSING OFF)

# Enable gcovr for code coverage (not compatible MSVC), if available.
set_option(GCOVR ON)

# Enable lcov for code coverage (not compatible MSVC), if available.
set_option(LCOV ON)

# Use Conan package manager
set_option(CONAN_VERBOSE OFF)

# Use vcpkg package manager
set_option(VCPKG_VERBOSE OFF)
set_option_path(VCPKG_LOCATION "~/vcpkg/")
set_option(VCPKG_LOCAL_DOWNLOAD OFF) # will download in ./build/ directory
