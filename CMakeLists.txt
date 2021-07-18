cmake_minimum_required(VERSION 3.1)

#set project name
project(urano)

#create a src variable with a link to all cpp files to compile
set(SRC
    src/main.c
    src/file_browser.c
    src/file_reader.c
    src/utils.c
    src/project_handler.c
    src/terminal_emulator.c
)

if(WIN32)
    message(STATUS "Building for WIN32")
endif(WIN32)

find_package(PkgConfig REQUIRED)
if(PKG_CONFIG_FOUND)
message(STATUS "Pkg config found")
endif 	(PKG_CONFIG_FOUND )

#GTK packages
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

pkg_check_modules(VTE REQUIRED vte-2.91)

include_directories(${VTE_INCLUDE_DIRS})
link_directories(${VTE_LIBRARY_DIRS})
add_definitions(${VTE_CFLAGS_OTHER})

pkg_check_modules(SOURCE_VIEW REQUIRED gtksourceview-4)

include_directories(${SOURCE_VIEW_INCLUDE_DIRS})
link_directories(${SOURCE_VIEW_LIBRARY_DIRS})
add_definitions(${SOURCE_VIEW_CFLAGS_OTHER})


add_executable(${PROJECT_NAME} ${SRC})

#link .h files
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/glade
        ${PROJECT_SOURCE_DIR}/css
        ${PROJECT_SOURCE_DIR}/assets
)

#link libs
    target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${VTE_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} ${SOURCE_VIEW_LIBRARIES})
