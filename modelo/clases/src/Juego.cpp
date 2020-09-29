//
// Created by nicolas on 19/03/17.
//

#include "../headers/Juego.h"
#include <algorithm>

using namespace std;


Juego::Juego() {

    Parser jsonParser = Parser();

    /*TODO Configurar el log al iniciar*/
    //jsonParser.configurarLog();
    this->ventana = jsonParser.getVentana();
    this->configuracion = jsonParser.getConfiguracion();
    this->niveles = jsonParser.getNiveles();

    this->niveles[0]->getEscenario()->setPersonajes(jsonParser.getPersonajes());

    Log::loguear(constantes::logEnumModo::ALTO, "Juego::Constructor", "Juego creado.");
}

void Juego::InitSDL() {
    const std::string CONTEXTO = "Juego::IniciarJuego";
    //The window we'll be rendering to
    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::loguear(constantes::logEnumModo ::BAJO, CONTEXTO, "Fallo al iniciar SDL. SDL Error : " + std::string(SDL_GetError()));
    } else
    {
        IMG_Init(IMG_INIT_JPG);
        //Create window
        window = SDL_CreateWindow("Sonic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->ventana->getAncho(),
                                  this->ventana->getAlto(), SDL_WINDOW_SHOWN);

        if (window == NULL) {
            Log::loguear(constantes::logEnumModo ::BAJO, CONTEXTO, "Fallo al crear el window. SDL Error : " + std::string(SDL_GetError()));
        } else {


            renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
            if ( renderer ) {
                Menu(window,renderer);

            }
            else
            {
                Log::loguear(constantes::logEnumModo ::ALTO, CONTEXTO, "Fallo al crear el renderer. SDL Error: " + std::string(SDL_GetError()));

            }
        }
        this->cerrarJuego(renderer, window);
    }
    return;

}
void Juego::Menu(SDL_Window* window, SDL_Renderer* renderer){
    const std::string CONTEXTO = "Juego::Menu";
    bool quit = false;
    SDL_Event event;

    SDL_Surface *background = IMG_Load("images/bg_menu.png");

    if (background) {
                    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");
                    SDL_Texture *textBackground = SDL_CreateTextureFromSurface(renderer,background);
                    SDL_RenderCopy(renderer, textBackground, NULL, NULL);
                    SDL_FreeSurface(background);
                    SDL_DestroyTexture(textBackground);
                }

                SDL_Surface *botonCo = IMG_Load("images/button_conectar.png");
                int botonWidth, botonHeight;

                if (botonCo) {
                    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito a crear el boton conectar");
                    SDL_Texture *textBotonCo = SDL_CreateTextureFromSurface(renderer,botonCo);
                    SDL_QueryTexture(textBotonCo, NULL, NULL, &botonWidth, &botonHeight);
                    SDL_Rect dst ={this->ventana->getAncho()/2 - botonWidth/2,this->ventana->getAlto()/6,botonWidth,botonHeight};

                    SDL_RenderCopy(renderer, textBotonCo, NULL, &dst);
                    SDL_FreeSurface(botonCo);
                    SDL_DestroyTexture(textBotonCo);
                }

                SDL_Surface *botonDeco = IMG_Load("images/button_deconectar.png");

                if (botonDeco) {
                    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito a crear el boton deconectar");
                    SDL_Texture *textBotonDeco = SDL_CreateTextureFromSurface(renderer,botonDeco);
                    SDL_QueryTexture(textBotonDeco, NULL, NULL, &botonWidth, &botonHeight);
                    SDL_Rect dst ={this->ventana->getAncho()/2 - botonWidth/2,this->ventana->getAlto()/6*3,botonWidth,botonHeight};

                    SDL_RenderCopy(renderer, textBotonDeco, NULL, &dst);
                    SDL_RenderPresent(renderer);
                    SDL_FreeSurface(botonDeco);
                    SDL_DestroyTexture(textBotonDeco);
                }

                SDL_Surface *botonSalir = IMG_Load("images/button_salir.png");

                if (botonSalir) {
                    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito a crear el boton salir");
                    SDL_Texture *textBotonSalir = SDL_CreateTextureFromSurface(renderer,botonSalir);
                    SDL_QueryTexture(textBotonSalir, NULL, NULL, &botonWidth, &botonHeight);
                    SDL_Rect dst ={this->ventana->getAncho()/2 - botonWidth/2,this->ventana->getAlto()/6*5,botonWidth,botonHeight};

                    SDL_RenderCopy(renderer, textBotonSalir, NULL, &dst);
                    SDL_FreeSurface(botonSalir);
                    SDL_DestroyTexture(textBotonSalir);
                }


                SDL_RenderPresent(renderer);

                while (!quit) {
                    SDL_WaitEvent(&event);

                    switch (event.type) {
                        case SDL_QUIT:
                            cerrarJuego(renderer, window);
                            quit = true;
                            break;

                        case SDL_MOUSEBUTTONDOWN:
                            if(event.motion.x >= this->ventana->getAncho()/2 - botonWidth/2 && event.motion.x <= this->ventana->getAncho()/2 + botonWidth/2)
                            {
                                if (event.motion.y >= this->ventana->getAlto()/6 && event.motion.y <= this->ventana->getAlto()/6 + botonHeight){
                                    IniciarJuego(window, renderer);
                                    quit = true;
                                }
//                                if (event.motion.y >= this->ventana->getAlto()/6*3 && event.motion.y <= this->ventana->getAlto()/6*3 + botonHeight){
//                                    //
//                                }
                                if (event.motion.y >= this->ventana->getAlto()/6*5 && event.motion.y <= this->ventana->getAlto()/6*5+ botonHeight){
                                    cerrarJuego(renderer, window);
                                    quit = true;
                                }
                            }
                            break;
                    }
                }
}

void Juego::IniciarJuego(SDL_Window* window, SDL_Renderer* renderer) {
    const std::string CONTEXTO = "Juego::IniciarJuego";
    this->niveles[0]->getEscenario()->iniciarEscenario(renderer,0);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    //LOOP PRINCIPAL
    this->loop(window, renderer);

    return;
}


//void Juego::loop(SDL_Window* window, SDL_Renderer* renderer) {
//    bool quit = false;
//    SDL_Event event;
//    int x = 0, y = 0; //camara position
//    int x_old = 0, y_old = 0;
//
//    while (!quit) {
//        SDL_WaitEvent(&event);
//
//        switch (event.type) {
//            case SDL_QUIT:
//                quit = true;
//                break;
//
//            case SDL_KEYDOWN :
//            {
//                switch( event.key.keysym.sym )
//                {
//                    case SDLK_LEFT: if (x < 0 - this->configuracion->getVelocidadScroll() ){
//                            x += this->configuracion->getVelocidadScroll();
//                        } else {
//                            x = 0;
//                        }break;
//                    case SDLK_RIGHT: if (x > - this->niveles[0]->getEscenario()->getAncho() + this->ventana->getAncho() + this->configuracion->getVelocidadScroll() ) {
//                            x -= this->configuracion->getVelocidadScroll();
//                        } else {
//                            x = - this->niveles[0]->getEscenario()->getAncho() + this->ventana->getAncho();
//                        }
//                        break;
//                    case SDLK_F1 :
//                    {
//                        Menu(window,renderer);
//                        return;
//                    }
//                    default: break;
//                }
//                break;
//            }
//
//        }
//
//        if (x != x_old || y != y_old){
//            const char* mensaje =  "1 1 0 310 360 1 360 320 z";
//            vector<int> mensajeParseado = this->parsearMensaje(mensaje);
//
//            for (int i = 2; i < mensajeParseado.size(); i=i+3) {
//                mensajeParseado[i+1] =  mensajeParseado[i+1] + rand()/5000000;
//                mensajeParseado[i+2] =  mensajeParseado[i+2] + rand()/5000000;
//            }
//            for (int i = 2; i < mensajeParseado.size(); i=i+3) {
//                this->niveles[0]->getEscenario()->cambiarPosicionPersonaje(mensajeParseado[i], mensajeParseado[i+1], mensajeParseado[i+2]);
//            }
//            this->niveles[0]->getEscenario()->dibujar(renderer, x, y, this->ventana->getAlto(), this->ventana->getAncho(), this->configuracion->getVelocidadScroll());
//            x_old = x;
//            y_old = y;
//            // Aca dibuja realmente
//            SDL_RenderPresent(renderer);
//            SDL_RenderClear(renderer);
//        }
//
//
//    }
//}

void Juego::loop(SDL_Window* window, SDL_Renderer* renderer) {
    bool quit = false;
    SDL_Event event;
    bool change = false;
    int frameAvanzar=0;
    int frameRetroceder=0;
    int frameSaltar=0;
    bool estaSaltando = false;


    while (!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN :
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_LEFT:
                        if (frameRetroceder / 3 >= 14) {
                            frameRetroceder = 11*3;
                        }
                       // this->niveles[0]->getEscenario()->teclaIzquierda(0, this->configuracion->getVelocidadScroll(), this->ventana->getAncho(), frameRetroceder);
                        change = true;
                        frameRetroceder++;
                        
                        break;
                    case SDLK_RIGHT:
                        if (frameAvanzar / 3 >= 14) {
                            frameAvanzar = 11*3;
                        }
                   //     this->niveles[0]->getEscenario()->teclaDerecha(0, this->configuracion->getVelocidadScroll(), this->ventana->getAncho(), frameAvanzar);
                        change = true;
                        frameAvanzar++;

                        break;
                    case SDLK_UP:
                        if (frameSaltar / 3 >= 9) {
                            frameSaltar = 6*3;
                        }
                        estaSaltando = true;
                        this->niveles[0]->getEscenario()->teclaArriba(0, this->configuracion->getVelocidadScroll(), this->ventana->getAncho(), frameSaltar, estaSaltando);

                        frameSaltar++;

                        break;
                    case SDLK_F1 :
                    {
                        Menu(window,renderer);
                        return;
                    }
                    default:
                        break;
                }
                break;
            }
            default:


                    frameAvanzar=0;
                    frameRetroceder = 0;
                    frameSaltar = 0;
                    this->niveles[0]->getEscenario()->quieto(0, 0);
                    change = true;



                break;

        }

            if (change) {
                this->niveles[0]->getEscenario()->dibujar(renderer, this->ventana->getAlto(), this->ventana->getAncho(), this->configuracion->getVelocidadScroll(), 0);
                change = false;

                SDL_RenderPresent(renderer);
                SDL_RenderClear(renderer);
            }
            while(estaSaltando){
                if (frameSaltar / 3 >= 9) {
                    frameSaltar = 6*3;
                }
                this->niveles[0]->getEscenario()->teclaArriba(0, this->configuracion->getVelocidadScroll(), this->ventana->getAncho(), frameSaltar, estaSaltando);

                frameSaltar++;
                this->niveles[0]->getEscenario()->dibujar(renderer, this->ventana->getAlto(), this->ventana->getAncho(), this->configuracion->getVelocidadScroll(), 0);


                SDL_RenderPresent(renderer);
                SDL_RenderClear(renderer);
            }
    }
}

void Juego::cerrarJuego(SDL_Renderer* renderer, SDL_Window* window) {
    const std::string CONTEXTO = "Juego::cerrarJuego";
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        Log::loguear(constantes::logEnumModo ::ALTO, CONTEXTO, "Renderer destruido");
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        Log::loguear(constantes::logEnumModo ::ALTO, CONTEXTO, "Window destruida");
    }

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();


}


Juego::~Juego() {
    delete this->ventana;
    delete this->configuracion;
    for (int i = 0; i < this->niveles.size(); i++) {
        delete this->niveles[i];
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Juego::Destructor", "Juego destruido.");
}

vector<int> Juego::parsearMensaje(const char* mensaje) {
    char FIN = 'z';
    int charInt;
    int contador = 0;
    string valor="";
    vector<int> mensajeInts;
    while (mensaje[contador] != FIN) {
        char caracter = mensaje[contador];
        if (caracter == ' '){
            charInt =  atoi(valor.c_str());
            valor = "";
            mensajeInts.push_back(charInt);
        }else{
            valor = valor + caracter;
        }
        contador++;
    }
//    int suma = 0;
//    for (int i = 0; i < mensajeInts.size(); i++) {
//        suma += mensajeInts[i];
//        cout << mensajeInts[i] << endl;
//    }
//    cout << '\n';
//    cout << suma;
//    cout << '\n';
    return mensajeInts;
}
