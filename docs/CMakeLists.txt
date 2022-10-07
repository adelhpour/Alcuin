##   Sphinx
find_package(Python COMPONENTS Interpreter)
if (NOT ${Python_FOUND})
    message(STATUS "Python cannot be found")
endif ()

get_filename_component(PYTHON_ROOT ${Python_EXECUTABLE} DIRECTORY)
find_program(SPHINX_EXECUTABLE
        NAMES sphinx-build sphinx-build.exe
        PATHS
        "${PYTHON_ROOT}/Scripts/sphinx-build.exe"
        "${PYTHON_ROOT}/Scripts/sphinx-build"
        REQUIRED
        COMMENT
        "absolute path to the sphinx-build executable. If you do not have sphinx-build
        you will need to install it. In your conda environment do: $conda activate <env>;
        pip install breathe sphinx sphinxcontrib-bibtex sphinx-tabs sphinx_rtd_theme; which sphinx-build. Then copy the value of which sphinx-build into
        -DSPHINX_EXECUTABLE")

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE} COMMAND 
                        ${SPHINX_EXECUTABLE} -b html
                        ${SPHINX_SOURCE} ${SPHINX_BUILD}
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/conf.py
                        COMMENT "Generating documentation with Sphinx")

# Nice named target so we can run the job easily
add_custom_target(Sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT doc_extra)