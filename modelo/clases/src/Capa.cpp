//
// Created by panasonic
//

#include <string>
#include "../headers/Capa.h"
#include "SDL_image.h"
#include "../headers/Log.h"
#include "../headers/Constantes.h"

Capa::Capa(int id, std::string rutaImagen, int alto, int ancho, int indexZ, int x) : Dibujable(indexZ, id, rutaImagen) {

    this->id=id;

    if (alto>0) {
        this->alto = alto;
    } else {
        this->alto = ALTO_ESCENARIO;
    }
    if (ancho>0) {
        this->ancho = ancho;
    } else {
        this->ancho = ANCHO_ESCENARIO;
    }
    this->dst->x=x;
    this->dst->y=0;
    this->dst->h=this->alto;
    this->dst->w=this->ancho;
    originalX = x;

    Log::loguear(constantes::logEnumModo::ALTO, "Capa::Constructor",
                 "Capa creada con Alto:"+std::to_string(this->alto)+" , Ancho: "+std::to_string(this->ancho)+", ID: "+std::to_string( this->id));
}

void Capa::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto) {
    dst->x = originalX +x;
    dst->y = y;
    SDL_RenderCopy(renderer,texture,NULL,dst);
}

Capa::~Capa() {

    Log::loguear(constantes::logEnumModo::ALTO, "Capa::Destructor", "Capa destruida.");
}

bool Capa::load(SDL_Renderer *renderer) {

    const std::string CONTEXTO = "Capa::load";

    SDL_Surface *image = IMG_Load(this->rutaImagen.c_str());
    SDL_Texture *texture = NULL;
    if (image) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");

        //TODO
        //SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0xFF, 0xFF));
        texture = SDL_CreateTextureFromSurface(renderer, image);

        if (!texture) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Fallo al crear la textura. SDL Error: " + std::string(SDL_GetError()));
            return false;
        }

        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al crear la textura");
        SDL_FreeSurface(image);
        this->texture = texture;
    } else {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Fallo al cargar la imagen. SDL Error: " + std::string(SDL_GetError()));
        std::string pathNotFound = IMAGES_NOTFOUND_PATH;
        SDL_Surface *imageNotFound = IMG_Load(pathNotFound.c_str());
        if (!imageNotFound) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Fallo al cargar la imagenNotFound. SDL Error: " + std::string(SDL_GetError()));
            return false;
        }
        SDL_Texture *textureNotFound = NULL;
        SDL_SetColorKey(imageNotFound, SDL_TRUE, SDL_MapRGB(imageNotFound->format, 0, 0xFF, 0xFF));

        textureNotFound = SDL_CreateTextureFromSurface(renderer, imageNotFound);

        if (!textureNotFound) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Fallo al crear la textureNotFound. SDL Error: " + std::string(SDL_GetError()));
            return false;
        }

        //if (textureNotFound) {
        //Get image dimensions -> cropea segun al ancho y el alto que vino en el JSON para esta entidad
        //this->ancho = this->ancho;
        //this->alto = this->alto;

        //TODO: loguear esto que se cropeo la imagen con las dimensiones que vino en el JSON (poner tamaño original de imagen y tamaño cropeado)
        // Para obtener el tamaño original de la imagen hacer ----> image->w y image->h
        //std::cout << " texture clear out\n";
        //} else {
        //fprintf(stdout, "failed to create the texture (%s)\n", SDL_GetError());
        //}
        this->texture = textureNotFound;
    }
    return true;
}


void Capa::setCoordenadaX(int x){
    return;
}