# since an optimizer is required for optimization we turn this on only if
# NLOPT was enabled for MAST
if(ENABLE_NLOPT OR ENABLE_SNOPT OR ENABLE_GCMMA)
    add_executable(structural_example_8
                   ${MAST_ROOT_DIR}/examples/fluid/meshing/cylinder.cpp
                   example_8.cpp)

    target_include_directories(structural_example_8 PRIVATE
                                ${CMAKE_CURRENT_LIST_DIR}
                                ${PROJECT_SOURCE_DIR}
                                ${PROJECT_SOURCE_DIR}/src)
    target_link_libraries(structural_example_8 mast)

    install(TARGETS structural_example_8
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/examples)
endif()
