//
// Created by aurore on 18/05/17.
//

#ifndef SONIC_MOSCA_H
#define SONIC_MOSCA_H


#include "Enemigo.h"

class Mosca : public Enemigo{

public:
    Mosca(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
          const std::string &rutaImagen, int indexZ, int ancho, int alto);

    void setNewX(int escenarioAncho);

    void setNewY();

    void setSprite();

};


#endif //SONIC_MOSCA_H
