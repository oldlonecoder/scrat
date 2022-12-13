cmake_minimum_required(VERSION 3.16..3.25 FATAL_ERROR)
# Project name
project(scrat VERSION 0.0.1 LANGUAGES CXX)


include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

message("source dir: " "${CMAKE_CURRENT_SOURCE_DIR}")

add_library(
        ${PROJECT_NAME}
        SHARED
        cxx/encodings/textattr.cxx
        cxx/geometry.cxx
        cxx/object.cxx
        cxx/rem.cxx
        cxx/stracc.cxx
        cxx/text.cxx
)


target_compile_definitions(${PROJECT_NAME} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME} EXPORT_FILE_NAME ${${PROJECT_NAME}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
			)
ENDIF()

target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        )

target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS} sqlite3)


install(DIRECTORY
        include/${PROJECT_NAME}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}"
)

install(TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

add_subdirectory(test)