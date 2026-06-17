cmake_minimum_required(VERSION 3.16)

project(KorepetycjeApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- DODAJ TE TRZY LINIJKI ---
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
# -----------------------------

# Pobieramy ścieżkę instalacji GStreamera...
# ... reszta pliku bez zmian ...

# Pobieramy ścieżkę instalacji GStreamera ze zmiennej systemowej
file(TO_CMAKE_PATH "$ENV{GSTREAMER_1_0_ROOT_MINGW_X86_64}" GST_ROOT)
if(NOT GST_ROOT)
    # Ścieżka awaryjna, jeśli zmienna systemowa nie odświeżyła się w Qt Creatorze
    set(GST_ROOT "C:/gstreamer/1.0/mingw_x86_64")
endif()

# Wyszukiwanie podstawowych pakietów Qt (bez Multimedia)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        authmanager.cpp
        authmanager.h
        communicationtool.cpp
        communicationtool.h
)

add_executable(KorepetycjeApp ${PROJECT_SOURCES})

# --- KONFIGURACJA GSTREAMER ---
# Dołączamy foldery z nagłówkami (.h)
target_include_directories(KorepetycjeApp PRIVATE
    ${GST_ROOT}/include/gstreamer-1.0
    ${GST_ROOT}/include/glib-2.0
    ${GST_ROOT}/lib/glib-2.0/include
)

# Dołączamy folder z bibliotekami (.a)
target_link_directories(KorepetycjeApp PRIVATE ${GST_ROOT}/lib)

# Linkujemy konkretne, wymagane biblioteki GStreamera i GLib
target_link_libraries(KorepetycjeApp PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    gstreamer-1.0
    glib-2.0
    gobject-2.0
)