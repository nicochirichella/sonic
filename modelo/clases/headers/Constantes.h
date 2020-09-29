//
// Created by dami on 19/03/17.
//

#ifndef UNTITLED_CONSTANTES_H
#define UNTITLED_CONSTANTES_H

#define DEFAULT_FILES_PATH "archivos/default/"
#define FILES_PATH "archivos/"
#define IMAGES_NOTFOUND_PATH "images/default/image_not_found.png";

/************************ CONSTANTES DE CLASES ************************/

/******** VENTANA ********/
#define ANCHO_VENTANA 800;
#define ALTO_VENTANA 600;

/***** CONFIGURACION ****/
#define VEL_SCROLL 20;

/****** ESCENARIO ******/
#define ANCHO_ESCENARIO 800;
#define ALTO_ESCENARIO 600;


/****** RECTANGULO ******/
#define ANCHO_RECTANGULO 200;
#define ALTO_RECTANGULO 200;

/****** CIRCULO ******/
#define RADIO_CIRCULO 20;

/****** DIBUJABLE ******/
#define INDEX_Z_DIBUJABLE 99;
#define CROP_X 0;
#define CROP_Y 0;

/****** MENU ******/
#define BOTON_WIDTH 200;
#define BOTON_HEIGHT 50;


/****** CAMPOS ESPERABLES EN EL JSON ******/
#define VENTANA "ventana";
#define DIMENSIONES "dimensiones";
#define ANCHO "ancho";
//#define ALTO "alto";
#define CONFIGURACION "configuracion";
#define NIVELES "niveles";
#define ID "id";
#define ESCENARIO "escenario";
#define INDEX_Z "index_z";
#define ENTIDADES "entidades";
#define TIPO "tipo";
#define COLOR "color";
#define CROP "crop";
#define XCROP "xCrop";
#define YCROP "yCrop";
#define COORDENADA "coordenada";
//#define X "x";
//#define Y "y";

namespace constantes
{
    // Para definir una constante
    //const int constante = 0xFF;

    enum logEnumModo { ALTO, MEDIO, BAJO, NADA };
    enum logEnumEscritura {ARCHIVO, CONSOLA, ARCHIVOYCONSOLA};
    enum eventoEnumCliente {TECLA_IZQUIERDA, TECLA_DERECHA, QUIETO, ESCAPE, IR_MENU, TECLA_ARRIBA, SERVEUR_DOWN, ESCUCHAR_LATIDO, SPIN_ATACK, TECLA_IMORTAL,EQUIPO1,EQUIPO2, NO_EQUIPO, CAMBIO_NIVEL_HECHO};
    enum tipoMensajeServidorCliente {PRIMERA_CONEXION, INICIA_JUEGO, RECONEXION, ACTUALIZA_VISTA, CUPO_LLENO, NUEVO_NIVEL, ACTUALIZA_ENEMIGOS, NOTHING, BONUS, RANDOMIZAR_ESCENARIO, GAME_OVER_PARA_CLIENTE, EQUIPOS_LISTOS, EQUIPO_ELEGIDO, GANADOR, ACTUALIZA_ENEMIGO_FINAL, CAMBIO_NIVEL_LISTO};
    enum tipoEntidades {ANILLO,ESCUDO, ESCUDO_RECO, FIN_ESCUDO, INVICIBILIDAD,INVICIBILIDAD_SPRITE, FIN_INVINCIBILIDAD, RING, PIEDRA, PINCHE, PERDIENDO, GAME_OVER, VICTORY, ENEMIGO_FINAL_INVINCIBLE};
    enum modoDeJuego{EQUIPO,INDIVIUAL,COLABORATIVO};
    enum tipoPerdido{VIDA, ANILLOS};
}

#endif //UNTITLED_CONSTANTES_H
