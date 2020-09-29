//
// Created by aurore on 18/05/17.
//

#include "../headers/Pinche.h"

Pinche::Pinche(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
               const std::string &rutaImagen, int indexZ, int ancho, int alto) : Rectangulo(id, color, coordenadaX,
                                                                                            coordenadaY, cropX, cropY,
                                                                                            rutaImagen, indexZ, ancho,
                                                                                            alto) {
    this->rutaImagen = "images/pinche.png";
    this->tipo = constantes::tipoEntidades::PINCHE;
}
