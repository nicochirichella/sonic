//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_CANGREJO_H
#define SONIC_CANGREJO_H


#include "Enemigo.h"

class Cangrejo : public Enemigo {
private :
    int originalCoordenadaX;
    int zonaDeMovimiento = 50;


public:
    Cangrejo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
             const std::string &rutaImagen, int indexZ, int ancho, int alto);

    void setNewX(int escenarioAncho);

    void setNewY();

    int getOriginalCoordenadaX() const;

    void setOriginalCoordenadaX(int originalCoordenadaX);

    int getZonaDeMovimiento() const;

    void setZonaDeMovimiento(int zonaDeMovimiento);

    bool isDirectionDerecha() const;

    void setDirectionDerecha(bool directionDerecha);

    void setSprite();

    };


#endif //SONIC_CANGREJO_H
