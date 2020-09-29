//
// Created by aurore on 18/05/17.
//

#include <string>
#include "../headers/Rectangulo.h"
#include "../headers/Invincibilidad.h"

Invincibilidab::Invincibilidab(int id, const std::string &color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                               const std::string &rutaImagen, int indexZ, int ancho, int alto) : Bonus(id, color,
                                                                                                            coordenadaX,
                                                                                                            coordenadaY,
                                                                                                            cropX,
                                                                                                            cropY,
                                                                                                            rutaImagen,
                                                                                                            indexZ,
                                                                                                            ancho,
                                                                                                            alto) {
    this->tipo = constantes::tipoEntidades::INVICIBILIDAD;
    this->rutaImagen ="images/invincibilidad.png";
}
