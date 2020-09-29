//
// Created by aurore on 23/05/17.
//

#ifndef SONIC_BONUS_H
#define SONIC_BONUS_H


#include "Rectangulo.h"

class Bonus : public Rectangulo {
private :


public:


    Bonus(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
          const std::string &rutaImagen, int indexZ, int ancho, int alto);


};


#endif //SONIC_BONUS_H
