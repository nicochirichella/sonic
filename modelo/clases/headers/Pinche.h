// Created by aurore on 18/05/17.
//

#ifndef SONIC_PINCHE_H
#define SONIC_PINCHE_H


#include "Rectangulo.h"

class Pinche : public Rectangulo {
public:
    Pinche(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
           const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_PINCHE_H
