//
// Created by aurore on 18/05/17.
//

#include "../headers/Escudo.h"

Escudo::Escudo(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
               const std::string &rutaImagen, int indexZ, int ancho, int alto) : Bonus(id, color, coordenadaX,
                                                                                            coordenadaY, cropX, cropY,
                                                                                            rutaImagen, indexZ, ancho,
                                                                                            alto) {
    this->tipo = constantes::tipoEntidades::ESCUDO;
    this->rutaImagen ="images/escudoCaja.png";

}
