//
// Created by mariano on 3/25/17.
//

#include "../headers/Dibujable.h"
#include "../headers/Log.h"
#include "../headers/Constantes.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

Dibujable::Dibujable(int indexZ, int id, std::string rutaImagen) {
    if(indexZ>0){
        this->indexZ = indexZ;
    }else{
        this->indexZ = INDEX_Z_DIBUJABLE;
    }

    this->id = id;
    this->rutaImagen = rutaImagen;
    this->dst = new SDL_Rect;
    Log::loguear(constantes::logEnumModo::ALTO, "Dibujable::Constructor",
                 "Dibujable creado con ID "+std::to_string(this->id) +", rutaImagen:'"+this->rutaImagen+"', indexZ: "+std::to_string(this->indexZ));
}

Dibujable::~Dibujable() {
    delete this->dst;
    Log::loguear(constantes::logEnumModo::ALTO, "Dibujable::Destructor", "Dibujable destruido.");
}

int Dibujable::getIndexZ() {
    return this->indexZ;
}

void Dibujable::setIndexZ(int indexZ) {
    this->indexZ = indexZ;
}

bool Dibujable::sortDibujable(Dibujable *instanciaA, Dibujable *instanciaB) {
    int indexZInstanciaA = instanciaA->getIndexZ();
    int indexZInstanciaB = instanciaB->getIndexZ();

    return indexZInstanciaA < indexZInstanciaB;

}

int Dibujable::getId() {
    return id;
}

std::string Dibujable::getRutaImagen() {
    return rutaImagen;
}

void Dibujable::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto) {
}

bool Dibujable::load(SDL_Renderer *renderer) {
}


void Dibujable::setCoordenadaX(int i){

}

