//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_ESCUDO_H
#define SONIC_ESCUDO_H


#include "Bonus.h"

class Escudo : public Bonus {
public:
    Escudo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
           const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_ESCUDO_H
