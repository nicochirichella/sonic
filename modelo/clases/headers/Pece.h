//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_PECE_H
#define SONIC_PECE_H


#include "Enemigo.h"

class Pece : public Enemigo {

private :
    int originalCoordenadaY;
    int zonaDeMovimiento = 20;
    bool directionArriba = 1;

public:
    int getOriginalCoordenadaY() const;

    void setOriginalCoordenadaY(int originalCoordenadaY);

    int getZonaDeMovimiento() const;

    void setZonaDeMovimiento(int zonaDeMovimiento);

    bool isDirectionArriba() const;

    void setDirectionArriba(bool directionArriba);

    Pece(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
         const std::string &rutaImagen, int indexZ, int ancho, int alto);

    void setNewX(int escenarioAncho);

    void setNewY();

    void setSprite();


};


#endif //SONIC_PECE_H
