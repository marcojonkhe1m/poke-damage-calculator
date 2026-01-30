# Add the cpp files to executable
target_sources(
    ${PROJECT_NAME} PRIVATE 
    #    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    #    ${CMAKE_CURRENT_SOURCE_DIR}/pane.cpp
    #    ${CMAKE_CURRENT_SOURCE_DIR}/window_manager.cpp
    #    ${CMAKE_CURRENT_SOURCE_DIR}/file_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/linux_pokecalc.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/pokecalc.cpp
)

# Make include directory available
target_include_directories(
    ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

