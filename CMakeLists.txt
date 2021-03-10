cmake_minimum_required(VERSION 3.1)


#set project name
project(urano)

#create a src variable with a link to all cpp files to compile
set(SRC
    src/main.cpp
    src/file_browser.cpp
    src/file_reader.cpp
    src/utils.cpp
    src/terminal_emulator.cpp
)

#Sourceview packages
#find_package(gtksourceview-4 REQUIRED)

#GTK packages
find_package(PkgConfig REQUIRED)
IF		(PKG_CONFIG_FOUND)
	message(STATUS "Pkg config found")
endif 	(PKG_CONFIG_FOUND )

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


add_executable(urano ${SRC})

#link .h files
target_include_directories(urano
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/glade
        ${PROJECT_SOURCE_DIR}/css
        ${PROJECT_SOURCE_DIR}/assets
)

#link libs
    target_link_libraries(urano ${GTK3_LIBRARIES})
    target_link_libraries(urano ${VTE_LIBRARIES})
    target_link_libraries(urano ${SOURCE_VIEW_LIBRARIES})
