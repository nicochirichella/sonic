//
// Created by nicolas on 17/03/17.
//

#include "../headers/Entidad.h"
#include <iostream>


Entidad::Entidad(int id, std::string color, int coordenadaX, int coordenadaY, std::string rutaImagen, int indexZ)
        : Dibujable(indexZ, id, rutaImagen) {
    this->color = color;
    this->coordenadaX = coordenadaX;
    this->coordenadaY = coordenadaY;
    this->coordenadaXOrigen = coordenadaX;

    Log::loguear(constantes::logEnumModo::ALTO, "Entidad::Constructor",
                 "Entidad creada con Color:"+this->color+" , Coord.X: "+std::to_string(this->coordenadaX)+" , Coord.Y: "+std::to_string(this->coordenadaY));
}

std::string Entidad::getColor() {
    return color;

}

void Entidad::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto) {
    Log::loguear(constantes::logEnumModo::ALTO, "Entidad::dibujar",
                 "Entidad "+std::to_string(this->id)+" dibujada");
}

bool Entidad::load(SDL_Renderer *renderer) {


}

Uint32 Entidad::colorEntidad(SDL_Renderer* renderer){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    if (this->color == "rojo"){
        SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
        return 0xFF0000FF;
    }
    if (this->color == "verde"){
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255 );
        return 0xFF00FF00;
    }
    if (this->color == "azul"){
        SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
        return 0xFFFF0000;
    }
    if (this->color == "amarillo"){
        SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
        return 0xFF00FFFF;
    }
    if (this->color == "naranja"){
        SDL_SetRenderDrawColor( renderer, 255, 170, 0, 255 );
        return 0xFF00AAFF;
    }

    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    return 0xFF000000;

}

void Entidad::setCoordenadaX(int coordenadaX) {
    this->coordenadaX = coordenadaX;
}

void Entidad::setCoordenadaY(int coordenadaY) {
    this->coordenadaY = coordenadaY;

}

int Entidad::getCoordenadaX(){
    return this->coordenadaX;
}

int Entidad::getCoordenadaY() {
    return this->coordenadaY;
}

int Entidad::getAncho(){
    return 0;
}

int Entidad::getAlto(){
    return 0;
}

int Entidad::getTipo() const {
    return tipo;
}

void Entidad::setTipo(int tipo) {
    Entidad::tipo = tipo;
}

int Entidad::getCoordenadaXOrigen() const {
    return coordenadaXOrigen;
}

int Entidad::getTipo(){
    return this->tipo;
}