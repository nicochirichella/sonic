//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_RING_H
#define SONIC_RING_H

#include "Bonus.h"

class Ring : public Bonus {
public:
    Ring(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
         const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_RING_H
