#include "../headers/Rectangulo.h"
#include <SDL.h>
#include <SDL_image.h>
#include "../headers/Log.h"
#include "../headers/Constantes.h"
SDL_Rect gSpriteClipsEntidad[0];

Rectangulo::Rectangulo(int id, std::string color, int coordenadaX, int coordenadaY, int cropX, int cropY,
                       std::string rutaImagen, int indexZ,
                       int ancho, int alto) : Entidad(id, color, coordenadaX, coordenadaY, rutaImagen, indexZ) {

    if (alto>0) {
        this->alto = alto;

    } else {
        this->alto = ALTO_RECTANGULO;
        Log::loguear(constantes::logEnumModo::MEDIO, "Rectangulo::Constructor",
                     "El alto debe ser un valor mayor a 0, se usa valor por defecto.");
    }

    if (ancho>0) {
        this->ancho = ancho;
    } else {
        this->ancho = ANCHO_RECTANGULO;
        Log::loguear(constantes::logEnumModo::MEDIO, "Rectangulo::Constructor",
                     "El ancho debe ser un valor mayor o igual a 0, se usa valor por defecto.");
    }

    if (cropX>=0) {
        this->cropX = cropX;
    } else {
        this->cropX = CROP_X;
        Log::loguear(constantes::logEnumModo::MEDIO, "Rectangulo::Constructor",
                     "El cropX debe ser un valor mayor o igual a 0, se usa valor por defecto.");
    }

    if (cropY>=0) {
        this->cropY = cropY;
    } else {
        this->cropY = CROP_Y;
        Log::loguear(constantes::logEnumModo::MEDIO, "Rectangulo::Constructor",
                     "El cropY debe ser un valor mayor a 0, se usa valor por defecto.");
    }

    this->texture = NULL;
    this->dst->x=this->coordenadaX;
    this->dst->y=this->coordenadaY;
    this->dst->h=this->alto;
    this->dst->w=this->ancho;

    Log::loguear(constantes::logEnumModo::ALTO, "Rectangulo::Constructor",
                 "Rectangulo creado con Alto: "+std::to_string(this->alto) +",Ancho: "+std::to_string(this->ancho)+", Coord X.: "+std::to_string( this->coordenadaX)+", Coord Y.: "+std::to_string( this->coordenadaY)+", Crop X: "+std::to_string( this->cropX)+", Crop Y: "+std::to_string( this->cropY)+", ID: "+std::to_string( this->id));
}

void Rectangulo::dibujar(SDL_Renderer *renderer, int x, int ventanaAncho, int y, int ventanaAlto) {
    if ( 0 < coordenadaX + x + this->getAncho() && coordenadaX + x < ventanaAncho) {
        dst->x = coordenadaX + x;
        dst->y = coordenadaY + y;
        SDL_Rect *newDst = dst;
        SDL_Rect *currentClip = NULL;
        int imageWidth, imageHeight;
        if (this->texture != NULL) {
            SDL_QueryTexture(this->texture, NULL, NULL, &imageWidth, &imageHeight);
            if(imageWidth < this->ancho || imageHeight < this->alto){
                setFill(renderer);
                setSpritesEntidad(dst->x,dst->y,imageWidth,imageHeight);
                newDst = &gSpriteClipsEntidad[0];
            }else{
                setSpritesEntidad(Rectangulo::getCropX(),Rectangulo::getCropY(),Rectangulo::getAncho(),Rectangulo::getAlto());
                currentClip = &gSpriteClipsEntidad[0];
            }
            /**
             * LO QUE HICE ACA FUE:
             * CURRENT CLIP ARRANCA EN NULL, SI ENTRA AL IF MANDO TOOOODA LA IMAGEN SIN CROPEARLA, SINO LA CROPEO.
             * NEW DST EN PRINCIPIO ES EL DST ORIGINAL, SI ENTRA AL IF, LO CAMBIO PARA LAS COORDENADAS QUE VINO EL JSON Y EL TAMAÑO DE LA IMAGEN.
             * PUEEEEDE QUE SEA ESO QUE HAGA QUE SE MUEVA LA IMAGEN CHIQUITA...
             */
            SDL_RenderCopyEx(renderer, this->texture, currentClip, newDst, 0.0, NULL, SDL_FLIP_NONE);
        } else {
            setFill(renderer);
        }
    }
}

void Rectangulo::setFill(SDL_Renderer *renderer) {
    colorEntidad(renderer);
    SDL_RenderFillRect(renderer, dst);
}

//CROPEO DESDE LA ESQUINA SUPERIOR IZQUIERDA (por default) HASTA LAS DIMENSIONES DADAS EN EL JSON.
//Si se desea cambiar desde donde cropear cambiar las coordenadas x e y;
//Si se desea cambiar hasta donde cropear cambiar las coordenadas w y h;
void Rectangulo::setSpritesEntidad(int x, int y, int w, int h) {
    gSpriteClipsEntidad[0].x = x;
    gSpriteClipsEntidad[0].y = y;
    gSpriteClipsEntidad[0].w = w;
    gSpriteClipsEntidad[0].h = h;
}

bool Rectangulo::load(SDL_Renderer *renderer) {
    const std::string CONTEXTO = "Rectangulo::load";
    SDL_Texture *texture = NULL;

    if(this->rutaImagen != ""){
        SDL_Surface *image = IMG_Load(this->rutaImagen.c_str());
        if (image) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");

//            if(image->w < this->getAncho() || image->h < this->getAlto()){
//                SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 0xFA, 0xBB));
//            }

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

    }else{
        return true;
    }

}

int Rectangulo::getAlto() {
    return alto;
}

int Rectangulo::getAncho() {
    return ancho;
}

void Rectangulo::setAncho(int ancho) {
    Rectangulo::ancho = ancho;
}

void Rectangulo::setAlto(int alto) {
    Rectangulo::alto = alto;
}

int Rectangulo::getCropX() const {
    return cropX;
}

void Rectangulo::setCropX(int cropX) {
    Rectangulo::cropX = cropX;
}

int Rectangulo::getCropY() const {
    return cropY;
}

void Rectangulo::setCropY(int cropY) {
    Rectangulo::cropY = cropY;
}