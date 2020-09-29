//
// Created by aurore on 10/06/17.
//

#ifndef SONIC_FUEGO_H
#define SONIC_FUEGO_H


#include "Enemigo.h"

class Fuego : public Enemigo {
private :
    int zonaDeMovimiento = 90;
    bool movimientoLateral = false;
    bool movimientoBajando = false;
    bool movimientoIniciado = false;
    int coordenadaXOrigen = 0;

public:
    void setMovimientoBajando(bool movimientoBajando);

    Fuego(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
          const std::string &rutaImagen, int indexZ, int ancho, int alto, bool direcionIzquierda);

    void setNewX();

    void setNewY(int yEnemigoFinal, int xEnemigoFinal, bool direcionIzquierda);

    void setSprite();

};


#endif //SONIC_FUEGO_H
