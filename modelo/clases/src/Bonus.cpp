//
// Created by aurore on 23/05/17.
//

#include "../headers/Bonus.h"

Bonus::Bonus(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
             const std::string &rutaImagen, int indexZ, int ancho, int alto) : Rectangulo(id, color, coordenadaX,
                                                                                          coordenadaY, cropX, cropY,
                                                                                          rutaImagen, indexZ, ancho,
                                                                                          alto) {}

