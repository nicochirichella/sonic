//
// Created by nicolas on 17/03/17.
//

#include <iostream>
#include "../headers/Circulo.h"
#include <SDL2_gfxPrimitives.h>
SDL_Rect gSpriteClipsEntidadCirculo[0];
void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

Circulo::Circulo(int id, std::string color, int coordenadaX , int coordenadaY, int cropX, int cropY, std::string rutaImagen, int indexZ,
                 int radio ) :Entidad(id, color, coordenadaX, coordenadaY, rutaImagen, indexZ){
    if (radio>0) {
        this->radio = radio;
    } else {
        this->radio = RADIO_CIRCULO;
        Log::loguear(constantes::logEnumModo::MEDIO, "Circulo::Constructor",
                     "El radio debe ser un valor mayor a 0, se usa valor por defecto.");

    }

    if (cropX>=0) {
        this->cropX = cropX;
    } else {
        this->cropX = CROP_X;
        Log::loguear(constantes::logEnumModo::MEDIO, "Circulo::Constructor",
                     "El cropX debe ser un valor mayor o igual a 0, se usa valor por defecto.");
    }

    if (cropY>=0) {
        this->cropY = cropY;
    } else {
        this->cropY = CROP_Y;
        Log::loguear(constantes::logEnumModo::MEDIO, "Circulo::Constructor",
                     "El cropY debe ser un valor mayor o igual a 0, se usa valor por defecto.");
    }

    this->dst->h= 2 * this->radio;
    this->dst->w= 2 * this->radio;
    this->dst->x=this->coordenadaX;
    this->dst->y=this->coordenadaY;
    this->texture = NULL;
    Log::loguear(constantes::logEnumModo::ALTO, "Circulo::Constructor",
                 "Circulo creado con Radio: "+std::to_string(this->radio) +", Coord X.: "+std::to_string( this->coordenadaX)+", Coord Y.: "+std::to_string( this->coordenadaY)+", Crop X: "+std::to_string( this->cropX)+", Crop Y: "+std::to_string( this->cropY)+", ID: "+std::to_string( this->id));

}
int Circulo::getRadio() {
    return this->radio;
}

void Circulo::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto) {

    if ( 0 < coordenadaX + x + this->getRadio() && coordenadaX + x + this->getRadio()< ventanaAncho) {

        dst->x = coordenadaX + x - this->getRadio();
        dst->y = coordenadaY + y - this->getRadio();
        int imageWidth, imageHeight;

        if (this->texture != NULL) {
            SDL_QueryTexture(this->texture, NULL, NULL, &imageWidth, &imageHeight);
            if(imageWidth < 2 * this->getRadio() || imageHeight < 2 * this->getRadio()){
                setFill(renderer);
            }
            dst->x = coordenadaX + x - imageWidth/2;
            dst->y = coordenadaY + y - imageHeight/2;
            dst->h = imageHeight;
            dst->w = imageWidth;

            SDL_RenderCopy(renderer, texture, NULL, dst);

        } else {
            setFill(renderer);
        }
    }
}

void Circulo::setFill(SDL_Renderer *renderer){
    colorEntidad(renderer);
    filledCircleColor(renderer, dst->x + this->radio , dst->y + this->radio, this->radio, this->colorEntidad(renderer));
}

//CROPEO DESDE LA ESQUINA SUPERIOR IZQUIERDA (por default) HASTA LAS DIMENSIONES DADAS EN EL JSON.
//Si se desea cambiar desde donde cropear cambiar las coordenadas x e y;
//Si se desea cambiar hasta donde cropear cambiar las coordenadas w y h;
void Circulo::setSpritesEntidad(int x, int y, int w, int h) {
    gSpriteClipsEntidadCirculo[0].x = x;
    gSpriteClipsEntidadCirculo[0].y = y;
    gSpriteClipsEntidadCirculo[0].w = w;
    gSpriteClipsEntidadCirculo[0].h = h;
}


int Circulo::getCropX() const {
    return cropX;
}

void Circulo::setCropX(int cropX) {
    Circulo::cropX = cropX;
}

int Circulo::getCropY() const {
    return cropY;
}

void Circulo::setCropY(int cropY) {
    Circulo::cropY = cropY;
}

bool Circulo::load(SDL_Renderer *renderer) {
    const std::string CONTEXTO = "Circulo::load";
    SDL_Texture *texture = NULL;


    if(this->rutaImagen != ""){
        SDL_Surface *image = IMG_Load(this->rutaImagen.c_str());


        if (image) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");

            SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

            if(image->w*image->w + image->h * image->h >=  this->getRadio() * this->getRadio()){
                int i, j;
                int X, Y;
                Uint32 transparent = SDL_MapRGBA( image->format, 0xff, 0x01, 0x02, 0x03 );

                SDL_LockSurface(image);
                for (j=0;j<image->h;j++)
                {
                    for (i=0;i<image->w;i++)
                    {   X =  i - image->w/2;
                        Y =  j - image->h/2;
                        if ( X*X + Y*Y >= this->getRadio()*this->getRadio()) {
                            definirPixel(image, i, j, transparent);
                        }

                    }

                }
                SDL_UnlockSurface(image);
                SDL_SetColorKey(image, SDL_TRUE, transparent);
            }


            texture = SDL_CreateTextureFromSurface(renderer,image);

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
            SDL_Texture *textureNotFound = NULL;
            SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

            if(imageNotFound->w*imageNotFound->w + imageNotFound->h * imageNotFound->h >=  this->getRadio() * this->getRadio()){
                int i, j;
                int X, Y;
                Uint32 transparent = SDL_MapRGBA( imageNotFound->format, 0xff, 0x01, 0x02, 0x03 );

                SDL_LockSurface(imageNotFound);
                for (j=0;j<imageNotFound->h;j++)
                {
                    for (i=0;i<imageNotFound->w;i++)
                    {   X =  i - imageNotFound->w/2;
                        Y =  j - imageNotFound->h/2;
                        if ( X*X + Y*Y >= this->getRadio()*this->getRadio()) {
                            definirPixel(imageNotFound, i, j, transparent);
                        }

                    }

                }
                SDL_UnlockSurface(imageNotFound);
                SDL_SetColorKey(imageNotFound, SDL_TRUE, transparent);
            }


            textureNotFound = SDL_CreateTextureFromSurface(renderer,imageNotFound);

            if (!imageNotFound) {
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Fallo al cargar la imagenNotFound. SDL Error: " + std::string(SDL_GetError()));
                return false;
            }

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

    }else{
        return true;
    }

}

void definirPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    /*nbOctetsParPixel = nb byte.
    multipying it by 8 (un byte = 8 bits), we get the depth of the color
    of the image : 8, 16, 24 or 32 bits.*/

    int nbOctetsParPixel = surface->format->BytesPerPixel ;

    /* p i the address of the pixel we want to access */

    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel;


    switch(nbOctetsParPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:

            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {

                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

int Circulo::getAncho() {
    return this->getRadio() * 2 ;
}

int Circulo::getAlto() {
    return this->getRadio() * 2 ;
}