option(GCOVR "use gcovr for code coverage (not compatible MSVC), if available" OFF)

if (COVERAGE_FLAGS AND GCOVR AND (NOT MSVC))
    find_program(GCOVR_EXEC "gcovr")
    if(GCOVR_EXEC)
        message(STATUS "Using gcovr")
        include(${PROJECT_SOURCE_DIR}/cmake/tools/test_helper.cmake)
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory ./gcovr/ WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
        string(REPLACE " " "%20" url_root_path "${CMAKE_BINARY_DIR}")
        if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|Intel")
            add_compile_options(-fprofile-arcs -ftest-coverage)
            add_link_options(-fprofile-arcs -ftest-coverage)
        endif()
        add_custom_command(TARGET run_coverage COMMAND echo && echo "-- Gcovr:" && "${GCOVR_EXEC}" -r "${PROJECT_SOURCE_DIR}" --exclude "${PROJECT_SOURCE_DIR}/external/*" "${CMAKE_BINARY_DIR}" && "${GCOVR_EXEC}" -r "${PROJECT_SOURCE_DIR}" --exclude "${PROJECT_SOURCE_DIR}/external/*" "${CMAKE_BINARY_DIR}" --html --html-details -o "${CMAKE_BINARY_DIR}/gcovr/index.html" && echo "HTML Summary: file://${url_root_path}/gcovr/index.html")
    else()
        find_program(HAS_PIP "pip3")
        if(NOT HAS_PIP)
            find_program(HAS_PYTHON "python3")
            find_program(HAS_APT "apt")
            if(HAS_APT)
                if(NOT HAS_PYTHON)
                    message(WARNING "Requested gcovr, but it was not found. Pip and python not found, you can install them with gcovr using:\nsudo apt install python3 python3-pip\npip3 install gcovr\n")
                else()
                    message(WARNING "Requested gcovr, but it was not found. Python found, but pip not found, you can install it with gcovr using:\nsudo apt install python3-pip\npip3 install gcovr\n")
                endif()
            else()
                if(NOT HAS_PYTHON)
                    message(WARNING "Requested gcovr, but it was not found. Pip and python not found. You will need python:\nhttps://www.python.org/downloads/\n You can then install pip with gcovr using:\ncurl -so https://bootstrap.pypa.io/get-pip.py && python get-pip.py && pip3 install gcovr\n")
                else()
                    message(WARNING "Requested gcovr, but it was not found. Python found, but pip not found, you can install it with gcovr using:\ncurl -so https://bootstrap.pypa.io/get-pip.py && python get-pip.py && pip3 install gcovr\n")
                endif()
            endif()
        else()
            message(WARNING "Requested gcovr, but it was not found. Pip and python found, you can install gcovr using:\npip3 install gcovr\n")
        endif()
    endif()
endif()
