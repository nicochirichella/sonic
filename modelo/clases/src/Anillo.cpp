//
// Created by aurore on 18/05/17.
//

#include "../headers/Anillo.h"

Anillo::Anillo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
               const std::string &rutaImagen, int indexZ, int ancho, int alto) : Bonus(id, color, coordenadaX,
                                                                                            coordenadaY, cropX, cropY,
                                                                                            rutaImagen, indexZ, ancho,
                                                                                            alto) {
    this->rutaImagen = "images/sonic_hd_sprite.png";
    this->tipo =  this->tipo = constantes::tipoEntidades::ANILLO;

}
