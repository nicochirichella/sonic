//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_ANILLO_H
#define SONIC_ANILLO_H


#include "Bonus.h"

class Anillo : public Bonus {
public:
    Anillo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
           const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_ANILLO_H
