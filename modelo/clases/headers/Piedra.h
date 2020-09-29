//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_PIEDRA_H
#define SONIC_PIEDRA_H


#include "Rectangulo.h"

class Piedra : public Rectangulo {
public:
    Piedra(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
           const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_PIEDRA_H
