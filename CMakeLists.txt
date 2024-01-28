cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
## Project name

project(scrat VERSION 0.1.0 LANGUAGES CXX)

message("Entering ${PROJECT_NAME} project configuration:")

include(GNUInstallDirs)

SET( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(TargetName ${PROJECT_NAME})

SET(FRAMEWORK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
    ${TargetName}
    SHARED
    include/${TargetName}/exports.scrat.h               #global framework-wide macros definitions and dll export and import macros for MSVC.
    include/${TargetName}/rt/Components.scrat.h impl/rt/Components.scrat.cxx
    include/scrat/rt/TToken.h impl/rt/TToken.cxx
    include/scrat/Compiler/Lang/TokenTable.h impl/Compiler/Lang/TokenTable.cxx impl/Compiler/Lang/Tokens.Scrat.Table.cxx
    impl/Compiler/TLexer.cxx
    include/scrat/Compiler/TLexer.h

    #include/${TargetName}/Alu.h                        impl/Alu.cxx
    #include/${TargetName}/Token.scrat.h                impl/Token.scrat.cxx
    #include/${TargetName}/Compiler/Lexer.h             impl/Compiler/Lexer.scrat.cxx
    #include/${TargetName}/xio.h                        impl/xio.cxx
    #include/${TargetName}/Compiler/Lang/Grammar.h      impl/Compiler/Lang/Grammar.scrat.cxx
    #include/${TargetName}/Stack.h                      impl/Stack.scrat.cxx
    #include/${TargetName}/Runtime/Variable.h           impl/Runtime/Variable.scrat.cxx
    #include/${TargetName}/Runtime/Interpreter.h        impl/Runtime/Interpreter.scrat.cxx
    #include/${TargetName}/Compiler/Compiler.h          impl/Compiler/Compiler.scrat.cxx
    #include/${TargetName}/Runtime/Amu.h                impl/Runtime/Amu.scrat.cxx
    #include/${TargetName}/Styles/ColorScheme.h         impl/Styles/ColorScheme.scrat.cxx
    #impl/Compiler/Lang/CCKWAmu.scrat.cxx

    # ...Will grow...
)


target_compile_definitions(${TargetName} PUBLIC "${TargetName}_DEBUG=$<CONFIG:Debug>")
target_compile_features(${TargetName} PUBLIC cxx_std_20)

include(GenerateExportHeader)
generate_export_header(${TargetName} EXPORT_FILE_NAME ${${TargetName}_export})

IF( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json" )
    EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            )
ENDIF()

target_include_directories(${TargetName} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        $<INSTALL_INTERFACE:include/${TargetName}>
        )

target_link_libraries(${TargetName} ${CMAKE_DL_LIBS} AppBook) # and normally logbook depends on chrtools


install(DIRECTORY
        include/${TargetName}/
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${TargetName}"
)

install(TARGETS ${TargetName}
        EXPORT ${TargetName}Targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
)

add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
#add_subdirectory(Tests)
