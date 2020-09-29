//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_INVINCIBILIDAB_H
#define SONIC_INVINCIBILIDAB_H


#include "Bonus.h"

class Invincibilidab : public Bonus
{
public:
    Invincibilidab(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                   const std::string &rutaImagen, int indexZ, int ancho, int alto);

};


#endif //SONIC_INVINCIBILIDAB_H
