cmake_minimum_required(VERSION 3.2)
project(sonic)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(EXEC_NAME ${PROJECT_NAME} )
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" "${PROJECT_SOURCE_DIR}/cmake")
set(INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
set(INCLUDE_DIR ${Jsoncpp_INCLUDE_DIRS})

#CADA UNO TIENE SU CARIABLE, PARA AGREGAR CLASES. NO TOCAR LAS VARIABLES DE LOS DEMAS!!!! OJITO EH!
set(JESS_FILES modelo/clases/src/srpits.cpp)
set(NICO_FILES modelo/clases/src/Capa.cpp modelo/clases/headers/Capa.h modelo/clases/headers/Personaje.h modelo/clases/src/Personaje.cpp modelo/clases/src/JuegoCliente.cpp modelo/clases/headers/JuegoCliente.h modelo/clases/src/JuegoServidor.cpp modelo/clases/headers/JuegoServidor.h modelo/clases/headers/ParserClienteServidor.h modelo/clases/src/ParserClienteServidor.cpp)
set(DAMI_FILES modelo/clases/src/Parser.cpp modelo/clases/headers/Parser.h)
set(MARIAN_FILES modelo/clases/src/Log.cpp modelo/clases/headers/Log.h modelo/clases/headers/Dibujable.h modelo/clases/src/Dibujable.cpp)
set (AURORE_FILES modelo/clases/src/Enemigo.cpp modelo/clases/headers/Enemigo.h modelo/clases/src/Cangrejo.cpp modelo/clases/headers/Cangrejo.h modelo/clases/src/Pece.cpp modelo/clases/headers/Pece.h modelo/clases/src/Mosca.cpp modelo/clases/headers/Mosca.h modelo/clases/src/Anillo.cpp modelo/clases/headers/Anillo.h modelo/clases/src/Piedra.cpp modelo/clases/headers/Piedra.h modelo/clases/src/Pinche.cpp modelo/clases/headers/Pinche.h modelo/clases/src/Escudo.cpp modelo/clases/headers/Escudo.h modelo/clases/src/Invincibilidad.cpp modelo/clases/headers/Invincibilidad.h modelo/clases/src/Ring.cpp modelo/clases/headers/Ring.h modelo/clases/src/Bonus.cpp modelo/clases/headers/Bonus.h modelo/clases/src/EnemigoFinal.cpp modelo/clases/headers/EnemigoFinal.h modelo/clases/src/Fuego.cpp modelo/clases/headers/Fuego.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_gfx REQUIRED)

find_path(Jsoncpp_INCLUDE_DIR
        NAMES json/features.h
        PATH_SUFFIXES jsoncpp
        PATHS ${Jsoncpp_PKGCONF_INCLUDE_DIRS} # /usr/include/jsoncpp/json
        )

find_library(Jsoncpp_LIBRARY
        NAMES jsoncpp
        PATHS ${Jsoncpp_PKGCONF_LIBRARY_DIRS}
        #  PATH ./jsoncpp/
        )

include_directories(
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${SDL2_GFX_INCLUDE_DIR}
)

set(Jsoncpp_PROCESS_INCLUDES Jsoncpp_INCLUDE_DIR)
set(Jsoncpp_PROCESS_LIBS Jsoncpp_LIBRARY)
set(SOURCE_FILES modelo/main.cpp modelo/clases/src/Entidad.cpp modelo/clases/headers/Entidad.h modelo/clases/src/Circulo.cpp modelo/clases/headers/Circulo.h modelo/clases/src/Rectangulo.cpp modelo/clases/headers/Rectangulo.h modelo/clases/src/Configuracion.cpp modelo/clases/headers/Configuracion.h modelo/clases/src/Escenario.cpp modelo/clases/headers/Escenario.h modelo/clases/src/Juego.cpp modelo/clases/headers/Juego.h modelo/clases/src/Nivel.cpp modelo/clases/headers/Nivel.h modelo/clases/src/Ventana.cpp modelo/clases/headers/Ventana.h modelo/clases/src/Enemigo.cpp modelo/clases/headers/Enemigo.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

#EXCECUTABLE
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${JESS_FILES} ${NICO_FILES} ${DAMI_FILES} ${MARIAN_FILES} ${AURORE_FILES})

#TARGET
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_GFX_LIBRARIES} ${Jsoncpp_LIBRARY})