//
// Created by nicolas on 26/04/17.
//


#include "../headers/JuegoCliente.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <SDL_ttf.h>

#include <algorithm>

using namespace std;


JuegoCliente::JuegoCliente(int puerto, char *ip) {

    Parser *jsonParser = new Parser();

    /*TODO Configurar el log al iniciar*/
    //jsonParser.configurarLog();
    this->ventana = jsonParser->getVentana();
    this->configuracion = jsonParser->getConfiguracion();
    this->niveles = jsonParser->getNiveles();
    this->puerto = puerto;
    this->ip = ip;
    this->reconexion = false;
    this->serverDown = false;
    this->irMenu = false;

    for (int i = 0; i < this->niveles.size(); i++) {
        this->niveles[i]->getEscenario()->setPersonajes(jsonParser->getPersonajes());
    }

    // this->niveles[nivelActual]->getEscenario()->setPersonajes(jsonParser->getPersonajes());

    Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::Constructor", ip);
}

void JuegoCliente::InitSDL() {
    const std::string CONTEXTO = "JuegoCliente::IniciarJuegoCliente";
    //The window we'll be rendering to
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                     "Fallo al iniciar SDL. SDL Error : " + std::string(SDL_GetError()));
    } else {
        IMG_Init(IMG_INIT_JPG);
        //Create window
        window = SDL_CreateWindow("Sonic", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, this->ventana->getAncho(),
                                  this->ventana->getAlto(), SDL_WINDOW_SHOWN);

        if (window == NULL) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                         "Fallo al crear el window. SDL Error : " + std::string(SDL_GetError()));
        } else {


            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |
                                                      SDL_RENDERER_TARGETTEXTURE);
            if (renderer) {
                LoadMenu(window, renderer);
                Menu(window, renderer);

            } else {
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                             "Fallo al crear el renderer. SDL Error: " + std::string(SDL_GetError()));
            }
        }
    }
    return;

}

void JuegoCliente::LoadMenu(SDL_Window *window, SDL_Renderer *renderer) {
    const std::string CONTEXTO = "JuegoCliente::Menu";
    SDL_RenderClear(renderer);
    SDL_Surface *background = IMG_Load("images/bg_menu.png");
    this->irMenu = false;
    if (background) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito al cargar la imagen");
        SDL_Texture *textBackground = SDL_CreateTextureFromSurface(renderer, background);
        SDL_RenderCopy(renderer, textBackground, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(background);
        SDL_DestroyTexture(textBackground);
    }

    SDL_Surface *botonCo = IMG_Load("images/button_conectar.png");
    int botonWidth, botonHeight;

    if (botonCo) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito a crear el boton conectar");
        SDL_Texture *textBotonCo = SDL_CreateTextureFromSurface(renderer, botonCo);
        SDL_QueryTexture(textBotonCo, NULL, NULL, &botonWidth, &botonHeight);
        SDL_Rect dst = {this->ventana->getAncho() / 2 - botonWidth / 2, this->ventana->getAlto() / 6 * 2, botonWidth,
                        botonHeight};

        SDL_RenderCopy(renderer, textBotonCo, NULL, &dst);
        SDL_RenderPresent(renderer);
        SDL_FreeSurface(botonCo);
        SDL_DestroyTexture(textBotonCo);

    }

    SDL_Surface *botonSalir = IMG_Load("images/button_salir.png");

    if (botonSalir) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Exito a crear el boton salir");
        SDL_Texture *textBotonSalir = SDL_CreateTextureFromSurface(renderer, botonSalir);
        SDL_QueryTexture(textBotonSalir, NULL, NULL, &botonWidth, &botonHeight);
        SDL_Rect dst = {this->ventana->getAncho() / 2 - botonWidth / 2, this->ventana->getAlto() / 6 * 4, botonWidth,
                        botonHeight};

        SDL_RenderCopy(renderer, textBotonSalir, NULL, &dst);
        SDL_FreeSurface(botonSalir);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(textBotonSalir);

    }
}

void JuegoCliente::Menu(SDL_Window *window, SDL_Renderer *renderer) {
    const std::string CONTEXTO = "JuegoCliente::Menu";
    bool quit = false;
    SDL_Event event;
    int botonWidth = BOTON_WIDTH;
    int botonHeight = BOTON_HEIGHT;

    while (!quit) {
        SDL_WaitEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                cerrarJuegoCliente(renderer, window);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.motion.x >= this->ventana->getAncho() / 2 - botonWidth / 2 &&
                    event.motion.x <= this->ventana->getAncho() / 2 + botonHeight / 2) {
                    if (event.motion.y >= this->ventana->getAlto() / 6 * 2 &&
                        event.motion.y <= this->ventana->getAlto() / 6 * 2 + botonHeight) {
                        SDL_RenderClear(renderer);

                        SDL_Surface *wait = IMG_Load("images/loading.png");
                        SDL_Texture *waitTexture = SDL_CreateTextureFromSurface(renderer, wait);
                        SDL_RenderCopy(renderer, waitTexture, NULL, NULL);
                        SDL_FreeSurface(wait);
                        SDL_RenderPresent(renderer);
                        SDL_DestroyTexture(waitTexture);
                        IniciarJuegoCliente(window, renderer);
                        quit = true;
                        if (this->irMenu || this->gameOver) {
                            this->noIniciaJuego = true;
                            this->setPrimeraConexion(false);
                            this->gameOver = false;
                            this->equiposListos = false;
                            this->equipo1 = {};
                            this->equipo2 = {};
                            this->LoadMenu(window, renderer);
                            quit = false;
                        } else if (juegoGanado) {
                            for (int i = 0; i < cantidadJugador; i++) {
                                int anillos = this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getAnillos();
                                if (anillos < 50) {
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setPuntos(
                                            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos() +
                                            anillos * 10);
                                } else {
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setPuntos(
                                            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos() +
                                            anillos * 20);
                                }
                            }
                            SDL_Surface *imageCambio;
                            SDL_Texture *cambioTexture;
                            string path = "images/finJuego" + to_string(this->cantidadJugador) + ".jpg";
                            imageCambio = IMG_Load(path.c_str());
                            cambioTexture = SDL_CreateTextureFromSurface(renderer, imageCambio);
                            SDL_RenderCopy(renderer, cambioTexture, NULL, NULL);

                            SDL_Surface *surfacePuntos;
                            SDL_Texture *puntosTexture;
                            TTF_Font *gFont = TTF_OpenFont("fonts/sonic_comic.ttf", 70);
                            SDL_Rect renderQuad;
                            switch (cantidadJugador) {
                                case 1:
                                    renderQuad = {425, 640, 150, 50};
                                    break;
                                case 2:
                                    renderQuad = {215, 640, 550, 50};
                                    break;
                                case 3:
                                    renderQuad = {125, 640, 775, 50};
                                    break;
                                default:
                                    renderQuad = {25, 640, 950, 50};
                                    break;
                            }

                            SDL_Color textColor = {255, 193, 20};
                            string puntos = "Puntos :" + std::to_string(
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(0)->getPuntos());
                            for (int i = 1; i < cantidadJugador; i++) {
                                puntos = puntos + "             " + "Puntos :" + std::to_string(
                                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos());
                            }

                            surfacePuntos = TTF_RenderText_Blended(gFont, puntos.c_str(), textColor);
                            puntosTexture = SDL_CreateTextureFromSurface(renderer, surfacePuntos);
                            SDL_RenderCopyEx(renderer, puntosTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
                            if (modoDeJuego == constantes::modoDeJuego::COLABORATIVO) {
                                SDL_Surface *surfacePuntosTotal;
                                SDL_Texture *puntosTextureTotal;
                                SDL_Rect renderQuadTotal = {400, 175, 200, 50};
                                int total = this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                        0)->getPuntos();
                                for (int i = 1; i < cantidadJugador; i++) {
                                    total += this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                            i)->getPuntos();
                                }
                                string puntosTotal = "Puntos total :" + std::to_string(total);
                                surfacePuntosTotal = TTF_RenderText_Blended(gFont, puntosTotal.c_str(), textColor);
                                puntosTextureTotal = SDL_CreateTextureFromSurface(renderer, surfacePuntosTotal);
                                SDL_RenderCopyEx(renderer, puntosTextureTotal, NULL, &renderQuadTotal, 0.0, NULL,
                                                 SDL_FLIP_NONE);
                            }
                            if (modoDeJuego == constantes::modoDeJuego::EQUIPO) {
                                SDL_Surface *surfacePuntosTeam;
                                SDL_Texture *puntosTextureTeam;
                                SDL_Rect renderQuadTeam = {200, 175, 600, 50};
                                string mensaje1 = " Team 1       Puntos : ";
                                string mensaje2 = " Team 2       Puntos : ";
                                int totalpuntos1 = 0;
                                int totalpuntos2 = 0;
                                for (int i = 0; i < equipo1.size(); i++) {
                                    totalpuntos1 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                            equipo1[i])->getPuntos();
                                }
                                for (int i = 0; i < equipo2.size(); i++) {
                                    totalpuntos2 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                            equipo2[i])->getPuntos();
                                }
                                mensaje1 += to_string(totalpuntos1);
                                mensaje2 += to_string(totalpuntos2);
                                mensaje1 += "              " + mensaje2;
                                surfacePuntosTeam = TTF_RenderText_Blended(gFont, mensaje1.c_str(), textColor);
                                puntosTextureTeam = SDL_CreateTextureFromSurface(renderer, surfacePuntosTeam);
                                SDL_RenderCopyEx(renderer, puntosTextureTeam, NULL, &renderQuadTeam, 0.0, NULL,
                                                 SDL_FLIP_NONE);
                            }
                            SDL_Surface *surfacePuntosFinal;
                            SDL_Texture *puntosTextureFinal;
                            SDL_Rect renderQuadFinal = {50, 10, 950, 100};
                            string puntosFinal;

                            if (modoDeJuego == constantes::modoDeJuego::COLABORATIVO) {
                                puntosFinal = "VICTORY ! ";
                                surfacePuntosFinal = TTF_RenderText_Blended(gFont, puntosFinal.c_str(), textColor);
                            } else if (modoDeJuego == constantes::modoDeJuego::INDIVIUAL) {
                                puntosFinal = "The winner is ";
                                int idGanador = 0;
                                for (int i = 1; i < cantidadJugador; i++) {
                                    if (this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos() >
                                        this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                                idGanador)->getPuntos()) {
                                        idGanador = i;
                                    }
                                }
                                puntosFinal += "player " + to_string(idGanador + 1);
                                path = "images/perso" + to_string(idGanador + 1) + ".png";
                                SDL_Surface *jug = IMG_Load(path.c_str());
                                SDL_Texture *textJug = SDL_CreateTextureFromSurface(renderer, jug);
                                SDL_Rect dstJug = {440, 110, 85, 150};
                                SDL_RenderCopy(renderer, textJug, NULL, &dstJug);
                                surfacePuntosFinal = TTF_RenderText_Blended(gFont, puntosFinal.c_str(), textColor);

                            } else {
                                puntosFinal = " The team ";
                                int totalpuntos1 = 0;
                                int totalpuntos2 = 0;
                                for (int i = 0; i < equipo1.size(); i++) {
                                    totalpuntos1 += this->niveles[nivelActual]->getEscenario()->getPersonaje(equipo1[i])->getPuntos();
                                }
                                for (int i = 0; i < equipo2.size(); i++) {
                                    totalpuntos2 += this->niveles[nivelActual]->getEscenario()->getPersonaje(equipo2[i])->getPuntos();
                                }
                                if (totalpuntos1 > totalpuntos2) {
                                    puntosFinal += to_string(1) + " won";
                                } else {
                                    puntosFinal += to_string(2) + " won";
                                }
                                surfacePuntosFinal = TTF_RenderText_Blended(gFont, puntosFinal.c_str(), textColor);
                            }

                            puntosTextureFinal = SDL_CreateTextureFromSurface(renderer, surfacePuntosFinal);
                            SDL_RenderCopyEx(renderer, puntosTextureFinal, NULL, &renderQuadFinal, 0.0, NULL, SDL_FLIP_NONE);

                            SDL_RenderPresent(renderer);
                            SDL_Delay(13000);
                            SDL_FreeSurface(imageCambio);
                            SDL_DestroyTexture(cambioTexture);
                            SDL_FreeSurface(surfacePuntosFinal);
                            SDL_DestroyTexture(puntosTextureFinal);
                            SDL_RenderPresent(renderer);




                            this->noIniciaJuego = true;
                            this->setPrimeraConexion(false);
                            this->gameOver = false;
                            this->equiposListos = false;
                            this->equipo1 = {};
                            this->equipo2 = {};
                            this->LoadMenu(window, renderer);
                            quit = false;

                        } else if (serverDown) {

                            if (!this->cupoLleno) {
                                SDL_Surface *down = IMG_Load("images/serverDown.png");
                                SDL_Texture *downTexture = SDL_CreateTextureFromSurface(renderer, down);
                                SDL_RenderCopy(renderer, downTexture, NULL, NULL);
                                SDL_FreeSurface(down);
                                SDL_RenderPresent(renderer);
                                SDL_DestroyTexture(downTexture);
                            } else {
                                SDL_Surface *down = IMG_Load("images/cupoLleno.png");
                                SDL_Texture *downTexture = SDL_CreateTextureFromSurface(renderer, down);
                                SDL_RenderCopy(renderer, downTexture, NULL, NULL);
                                SDL_FreeSurface(down);
                                SDL_RenderPresent(renderer);
                                SDL_DestroyTexture(downTexture);
                            }
                            SDL_Delay(3000);
                            cerrarJuegoCliente(renderer, window);
                        }
                        break;
                    }
                    if (event.motion.y >= this->ventana->getAlto() / 6 * 4 &&
                        event.motion.y <= this->ventana->getAlto() / 6 * 4 + botonHeight) {
                        cerrarJuegoCliente(renderer, window);
                        quit = true;
                    }
                }
                break;
        }
    }
}

void JuegoCliente::MenuEquipo() {
    bool quit = false;
    SDL_Event event;
    const std::string CONTEXTO = "JuegoCliente::MenuEquipo";
    int botonHeight = BOTON_HEIGHT;
    int botonWidth = BOTON_WIDTH;

    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                this->quitMenu = true;
                return;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.motion.y >= 50 &&
                    event.motion.y <= 50 + botonHeight) {
                    if (event.motion.x >= 200 &&
                        event.motion.x <= 200 + botonWidth) {
                        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::actualizarEquipos",
                                     "mi personaje es en el equipo 1");
                        int size = 2048;
                        char buffer[size];
                        string bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::EQUIPO1);
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, bufferString.c_str());
                        this->enviar(buffer, size);
                        quit = true;
                        break;
                    }
                    if (event.motion.x >= 600 &&
                        event.motion.x <= 600 + botonWidth) {
                        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::actualizarEquipos",
                                     "mi personaje es en el equipo 2");
                        int size = 2048;
                        char buffer[size];
                        string bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::EQUIPO2);
                        memset(buffer, 0, sizeof(buffer));
                        strcpy(buffer, bufferString.c_str());
                        this->enviar(buffer, size);
                        quit = true;
                    }
                }
                break;
            default :
                break;
        }
    }

    while (!equiposListos) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                this->quitMenu = true;
                return;
            default:
                break;
        }
    }
}


void JuegoCliente::loopParaMenuEquipo(SDL_Window *window, SDL_Renderer *renderer) {

    thread MenuEquipoThread(&JuegoCliente::MenuEquipo, this);
    MenuEquipoThread.detach();
    SDL_RenderClear(renderer);

    SDL_Surface *background = IMG_Load("images/menuEquipo5.jpg");
    SDL_Texture *textBackground = SDL_CreateTextureFromSurface(renderer, background);
    SDL_RenderCopy(renderer, textBackground, NULL, NULL);

    SDL_Surface *boton1 = IMG_Load("images/button_equipo1.png");
    int botonWidth, botonHeight;

    SDL_Texture *textBoton1 = SDL_CreateTextureFromSurface(renderer, boton1);
    SDL_QueryTexture(textBoton1, NULL, NULL, &botonWidth, &botonHeight);
    SDL_Rect dst = {200, 50, botonWidth,
                    botonHeight};

    SDL_RenderCopy(renderer, textBoton1, NULL, &dst);

    SDL_Surface *boton2 = IMG_Load("images/button_equipo2.png");
    SDL_Texture *textBoton2 = SDL_CreateTextureFromSurface(renderer, boton2);
    SDL_Rect dst2 = {600, 50, botonWidth, botonHeight};
    SDL_RenderCopy(renderer, textBoton2, NULL, &dst2);

    SDL_Surface *jug1 = IMG_Load("images/perso1.png");
    SDL_Texture *textJug1 = SDL_CreateTextureFromSurface(renderer, jug1);
    SDL_Rect dstJug1 = {460, 150, 85, 150};
    SDL_RenderCopy(renderer, textJug1, NULL, &dstJug1);

    SDL_Surface *jug2 = IMG_Load("images/perso2.png");
    SDL_Texture *textJug2 = SDL_CreateTextureFromSurface(renderer, jug2);
    SDL_Rect dstJug2 = {460, 300, 85, 150};

    SDL_Surface *jug3 = IMG_Load("images/perso3.png");
    SDL_Texture *textJug3 = SDL_CreateTextureFromSurface(renderer, jug3);
    SDL_Rect dstJug3 = {450, 450, 85, 150};

    SDL_Surface *jug4 = IMG_Load("images/perso4.png");
    SDL_Texture *textJug4 = SDL_CreateTextureFromSurface(renderer, jug4);
    SDL_Rect dstJug4 = {460, 600, 85, 150};

    SDL_Surface *jug5 = IMG_Load("images/myPerso.png");
    SDL_Texture *textJug5 = SDL_CreateTextureFromSurface(renderer, jug5);
    switch (idCliente) {
        case 0:
            SDL_RenderCopy(renderer, textJug5, NULL, &dstJug1);
            break;
        case 1:
            SDL_RenderCopy(renderer, textJug5, NULL, &dstJug2);
            break;
        case 2:
            SDL_RenderCopy(renderer, textJug5, NULL, &dstJug3);
            break;
        default:
            SDL_RenderCopy(renderer, textJug5, NULL, &dstJug4);
            break;
    }

    SDL_RenderCopy(renderer, textJug1, NULL, &dstJug1);
    if (cantidadJugador >= 2) {
        SDL_RenderCopy(renderer, textJug2, NULL, &dstJug2);
        if (cantidadJugador >= 3) {
            SDL_RenderCopy(renderer, textJug3, NULL, &dstJug3);
            if (cantidadJugador == 4) {
                SDL_RenderCopy(renderer, textJug4, NULL, &dstJug4);
            }
        }
    }
    SDL_RenderPresent(renderer);

    while (!equiposListos) {
        if (this->quitMenu) {
            return;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, textBackground, NULL, NULL);
        SDL_RenderCopy(renderer, textBoton1, NULL, &dst);
        SDL_RenderCopy(renderer, textBoton2, NULL, &dst2);
        for (int i = 0; i < equipo1.size(); i++) {
            switch (equipo1[i]) {
                case 0 :
                    dstJug1.x = 250;
                    break;
                case 1 :
                    dstJug2.x = 250;
                    break;
                case 2 :
                    dstJug3.x = 250;
                    break;
                default :
                    dstJug4.x = 250;
                    break;
            }
        }
        for (int i = 0; i < equipo2.size(); i++) {
            switch (equipo2[i]) {
                case 0 :
                    dstJug1.x = 650;
                    break;
                case 1 :
                    dstJug2.x = 650;
                    break;
                case 2 :
                    dstJug3.x = 650;
                    break;
                default :
                    dstJug4.x = 650;
                    break;
            }
        }
        switch (idCliente) {
            case 0:
                SDL_RenderCopy(renderer, textJug5, NULL, &dstJug1);
                break;
            case 1:
                SDL_RenderCopy(renderer, textJug5, NULL, &dstJug2);
                break;
            case 2:
                SDL_RenderCopy(renderer, textJug5, NULL, &dstJug3);
                break;
            default:
                SDL_RenderCopy(renderer, textJug5, NULL, &dstJug4);
                break;
        }
        SDL_RenderCopy(renderer, textJug1, NULL, &dstJug1);
        if (cantidadJugador >= 2) {
            SDL_RenderCopy(renderer, textJug2, NULL, &dstJug2);
            if (cantidadJugador >= 3) {
                SDL_RenderCopy(renderer, textJug3, NULL, &dstJug3);
                if (cantidadJugador == 4) {
                    SDL_RenderCopy(renderer, textJug4, NULL, &dstJug4);
                }
            }
        }

        SDL_RenderPresent(renderer);
    }
    SDL_FreeSurface(background);
    SDL_DestroyTexture(textBackground);
    SDL_FreeSurface(boton1);
    SDL_DestroyTexture(textBoton1);
    SDL_FreeSurface(boton2);
    SDL_DestroyTexture(textBoton2);
    SDL_FreeSurface(jug1);
    SDL_DestroyTexture(textJug1);
    SDL_FreeSurface(jug2);
    SDL_DestroyTexture(textJug2);
    SDL_FreeSurface(jug3);
    SDL_DestroyTexture(textJug3);
    SDL_FreeSurface(jug4);
    SDL_DestroyTexture(textJug4);
    SDL_FreeSurface(jug5);
    SDL_DestroyTexture(textJug5);
}

void JuegoCliente::IniciarJuegoCliente(SDL_Window *window, SDL_Renderer *renderer) {
    const std::string CONTEXTO = "JuegoCliente::IniciarJuegoCliente";
    //TODO VER QUE PASA SI FALLA EL INICIO DE LA CONEXION CON EL SERVIDOR
    // Esto ya crea un nuevo thread
    bool value = this->iniciarCliente();
    if (value) {
        while (this->noIniciaJuego) {
            // Mostrar mensaje "esperando jugadores" en el menu
            if (this->serverDown) {
                return;
            }
        }
        sleep(3);

        if (this->modoDeJuego == constantes::modoDeJuego::EQUIPO) {
            if (!equiposListos) {
                this->loopParaMenuEquipo(window, renderer);
            }
        }

        if (this->quitMenu) {
            int size = 2048;
            char buffer[size];
            string bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::NO_EQUIPO);
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, bufferString.c_str());
            this->enviar(buffer, size);
            usleep(1000);
            bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::ESCAPE);
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, bufferString.c_str());
            this->enviar(buffer, size);
            return;
        }
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "empiezo a renderizar");
        this->niveles[nivelActual]->getEscenario()->iniciarEscenario(renderer, this->idCliente);

        this->evento = -1;
        // Thread para enviar eventos
        thread enviarEventoThread(&JuegoCliente::threadMandarEventoMejorado, this, window, renderer);
        enviarEventoThread.detach();
        //Thread principal que dibuja
        this->loop(window, renderer);
    } else {
        this->serverDown = true;
        this->LoadMenu(window, renderer);
    }
    return;
}

void JuegoCliente::loop(SDL_Window *window, SDL_Renderer *renderer) {
    const std::string CONTEXTO = "JuegoCliente::loop";
    bool cambioLatidoState = false;
//    bool cambioNivelState = false;
    int size = 2048;
    char buffer[size];
    string bufferString;
    while (this->evento != constantes::eventoEnumCliente::ESCAPE) {
        if (this->gameOver) {
            this->gameOverGrafico(renderer);
            return;
        }
        if (this->isimagenCableDesconectado()) {
            if (cambioLatidoState) {
                int width, height;
                SDL_Surface *down = IMG_Load("images/conectionPB.png");
                SDL_Texture *downTexture = SDL_CreateTextureFromSurface(renderer, down);
                SDL_Rect dst = {this->ventana->getAncho() / 2 - 500 / 2, this->ventana->getAlto() / 2 - 200, 500,
                                400};
                SDL_RenderCopy(renderer, downTexture, NULL, &dst);
                SDL_FreeSurface(down);
                SDL_RenderPresent(renderer);
                SDL_DestroyTexture(downTexture);
                cambioLatidoState = false;
            }
        } else {
            SDL_RenderClear(renderer);
            cambioLatidoState = true;
            if (this->isCambioNivel()) {
//                if (cambioNivelState) {
                if (nivelActual < this->niveles.size()) {
                    usleep(50000);
                    for (int i = 0; i < cantidadJugador; i++) {
                        int anillos = this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getAnillos();
                        if (anillos < 50) {
                            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setPuntos(
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos() +
                                    anillos * 10);
                        } else {
                            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setPuntos(
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos() +
                                    anillos * 20);
                        }

                    }
                    SDL_Surface *imageCambio;
                    SDL_Texture *cambioTexture;
//                    if (modoDeJuego == constantes::modoDeJuego::INDIVIUAL ||
//                        modoDeJuego == constantes::modoDeJuego::COLABORATIVO) {
                        string path = "images/cambioNivel" + to_string(this->cantidadJugador) + ".jpg";
                        imageCambio = IMG_Load(path.c_str());
                        cambioTexture = SDL_CreateTextureFromSurface(renderer, imageCambio);
                        SDL_RenderCopy(renderer, cambioTexture, NULL, NULL);

                        SDL_Surface *surfacePuntos;
                        SDL_Texture *puntosTexture;
                        TTF_Font *gFont = TTF_OpenFont("fonts/sonic_comic.ttf", 70);
                        SDL_Rect renderQuad;
                        switch (cantidadJugador) {
                            case 1:
                                renderQuad = {425, 640, 150, 50};
                                break;
                            case 2:
                                renderQuad = {215, 640, 550, 50};
                                break;
                            case 3:
                                renderQuad = {125, 640, 775, 50};
                                break;
                            default:
                                renderQuad = {25, 640, 950, 50};
                                break;
                        }

                        SDL_Color textColor = {255, 193, 20};
                        string puntos = "Puntos :" + std::to_string(
                                this->niveles[nivelActual]->getEscenario()->getPersonaje(0)->getPuntos());
                        for (int i = 1; i < cantidadJugador; i++) {
                            puntos = puntos + "             " + "Puntos :" + std::to_string(
                                    this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos());
                        }

                        surfacePuntos = TTF_RenderText_Blended(gFont, puntos.c_str(), textColor);
                        puntosTexture = SDL_CreateTextureFromSurface(renderer, surfacePuntos);
                        SDL_RenderCopyEx(renderer, puntosTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
                        if (modoDeJuego == constantes::modoDeJuego::COLABORATIVO) {
                            SDL_Surface *surfacePuntosTotal;
                            SDL_Texture *puntosTextureTotal;
                            SDL_Rect renderQuadTotal = {400, 100, 200, 50};
                            int total = this->niveles[nivelActual]->getEscenario()->getPersonaje(0)->getPuntos();
                            for (int i = 1; i < cantidadJugador; i++) {
                                total += this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->getPuntos();
                            }
                            string puntosTotal = "Puntos total :" + std::to_string(total);
                            surfacePuntosTotal = TTF_RenderText_Blended(gFont, puntosTotal.c_str(), textColor);
                            puntosTextureTotal = SDL_CreateTextureFromSurface(renderer, surfacePuntosTotal);
                            SDL_RenderCopyEx(renderer, puntosTextureTotal, NULL, &renderQuadTotal, 0.0, NULL,
                                             SDL_FLIP_NONE);
                        }
                    if (modoDeJuego == constantes::modoDeJuego::EQUIPO) {
                        SDL_Surface *surfacePuntosTeam;
                        SDL_Texture *puntosTextureTeam;
                        SDL_Rect renderQuadTeam = {200, 175, 600, 50};
                        string mensaje1 = " Team 1       Puntos : ";
                        string mensaje2 = " Team 2       Puntos : ";
                        int totalpuntos1 = 0;
                        int totalpuntos2 = 0;
                        for (int i = 0; i < equipo1.size(); i++) {
                            totalpuntos1 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                    equipo1[i])->getPuntos();
                        }
                        for (int i = 0; i < equipo2.size(); i++) {
                            totalpuntos2 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
                                    equipo2[i])->getPuntos();
                        }
                        mensaje1 += to_string(totalpuntos1);
                        mensaje2 += to_string(totalpuntos2);
                        mensaje1 += "              " + mensaje2;
                        surfacePuntosTeam = TTF_RenderText_Blended(gFont, mensaje1.c_str(), textColor);
                        puntosTextureTeam = SDL_CreateTextureFromSurface(renderer, surfacePuntosTeam);
                        SDL_RenderCopyEx(renderer, puntosTextureTeam, NULL, &renderQuadTeam, 0.0, NULL,
                                         SDL_FLIP_NONE);
                    }

//                    } else {
//                        imageCambio = IMG_Load("images/menuEquipo6.jpg");
//                        cambioTexture = SDL_CreateTextureFromSurface(renderer, imageCambio);
//                        SDL_RenderCopy(renderer, cambioTexture, NULL, NULL);
//                        SDL_Color textColor = {255, 255, 255};
//                        SDL_Surface *surfacePuntos;
//                        SDL_Texture *puntosTexture;
//                        TTF_Font *gFont = TTF_OpenFont("fonts/sonic_comic.ttf", 70);
//                        SDL_Rect renderQuad = {200, 400, 600, 50};
//                        string mensaje1 = " Team 1       Puntos : ";
//                        string mensaje2 = " Team 2       Puntos : ";
//                        int totalpuntos1 = 0;
//                        int totalpuntos2 = 0;
//                        for (int i = 0; i < equipo1.size(); i++) {
//                            totalpuntos1 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
//                                    equipo1[i])->getPuntos();
//                        }
//                        for (int i = 0; i < equipo2.size(); i++) {
//                            totalpuntos2 += this->niveles[nivelActual]->getEscenario()->getPersonaje(
//                                    equipo2[i])->getPuntos();
//                        }
//                        mensaje1 += to_string(totalpuntos1);
//                        mensaje2 += to_string(totalpuntos2);
//                        mensaje1 += "              " + mensaje2;
//                        surfacePuntos = TTF_RenderText_Blended(gFont, mensaje1.c_str(), textColor);
//                        puntosTexture = SDL_CreateTextureFromSurface(renderer, surfacePuntos);
//                        SDL_RenderCopyEx(renderer, puntosTexture, NULL, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
//                    }

                    SDL_RenderPresent(renderer);
                    delete niveles[this->nivelActual];
                    this->nivelActual++;
                    this->niveles[nivelActual]->getEscenario()->iniciarEscenario(renderer, this->idCliente);
                    sleep(5);
                    SDL_FreeSurface(imageCambio);
                    SDL_DestroyTexture(cambioTexture);
//                    }
                    this->setCambioNivel(false);
                    bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::CAMBIO_NIVEL_HECHO);
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, bufferString.c_str());
                    this->enviar(buffer, size);
                    while (!cambioNivelListo) {

                    }
//                    cambioNivelState = false;
                }
            } else {
//                cambioNivelState = true;
                cambioNivelListo = false;
                this->niveles[nivelActual]->getEscenario()->dibujar(renderer, this->ventana->getAlto(),
                                                                    this->ventana->getAncho(),
                                                                    this->configuracion->getVelocidadScroll(),
                                                                    this->idCliente);
                // Aca dibuja realmente
                SDL_RenderPresent(renderer);
            }
        }
    }

}

void JuegoCliente::threadMandarEvento(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Event event;
    const std::string CONTEXTO = "JuegoCliente::threadMandarEvento";

    while (this->evento != constantes::eventoEnumCliente::ESCAPE) {
        // Si no esta corriendo, no hago nada
        if (!this->threadLatidoEstaCorriendo) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Thread Latido no esta corriendo");
            //continue;
        }

        if (this->evento == constantes::eventoEnumCliente::SERVEUR_DOWN) { return; }
        SDL_WaitEvent(&event);
        this->mandarEvento(event, window, renderer);
    }

    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Cerrar thread mandar");

}

void JuegoCliente::threadMandarEventoMejorado(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_Event event;
    const std::string CONTEXTO = "JuegoCliente::threadMandarEvento";

    while (this->evento != constantes::eventoEnumCliente::ESCAPE) {
        // Si no esta corriendo, no hago nada
        if (!this->threadLatidoEstaCorriendo) {
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Thread Latido no esta corriendo");
            //continue;
        }

        if (this->evento == constantes::eventoEnumCliente::SERVEUR_DOWN) { return; }
        //Handle events on queue
        while (SDL_PollEvent(&event) != 0) {
            //User requests quit
            if (event.type == SDL_QUIT) {
                this->evento = constantes::eventoEnumCliente::ESCAPE;
            }
        }
        this->mandarEventoMejorado(window, renderer);
    }

    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Cerrar thread mandar");

}

void JuegoCliente::mandarEventoEscucharLatido() {
    if (isCambioNivel()) { return; }
    int size = 2048;
    char buffer[size];
    string bufferString = convertirTipoMensajeEnString(constantes::eventoEnumCliente::ESCUCHAR_LATIDO);
    memset(buffer, 0, sizeof(buffer));
    strcpy(buffer, bufferString.c_str());
    this->enviar(buffer, size);
//    cout << "ENVIE: ";
//    cout << buffer << endl;

}

void JuegoCliente::threadEscucharLatido() {
    string CONTEXTO = "JuegoCliente::threadEscucharLatido";
    this->threadLatidoEstaCorriendo = true;
    while (this->cableEstaConectado) {
        if (!isCambioNivel()) {
            this->cableEstaConectado = false;
            mandarEventoEscucharLatido();
            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Mande un evento escuchar latido");
            usleep(50000); //50ms (changui que le damos al servidor para mandar el latido)
        }
    }
    this->threadLatidoEstaCorriendo = false;
    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Paro el thread escuchar latido");
    this->setimagenCableDesconectado(true);
    //TODO TIENE QUE MOSTRAR LA IMAGEN "SE CORTO EL CABLE"
}


void JuegoCliente::cerrarJuegoCliente(SDL_Renderer *renderer, SDL_Window *window) {
    const std::string CONTEXTO = "JuegoCliente::cerrarJuegoCliente";
    this->evento = -1;
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Renderer destruido");
    }
    if (window != NULL) {
        SDL_DestroyWindow(window);
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Window destruida");
    }

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


JuegoCliente::~JuegoCliente() {
    delete this->ventana;
    delete this->configuracion;
    for (int i = 0; i < this->niveles.size(); i++) {
        delete this->niveles[i];
    }

    Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::Destructor", "JuegoCliente destruido.");
}

void JuegoCliente::mandarEvento(SDL_Event event, SDL_Window *window, SDL_Renderer *renderer) {
    int eventoCliente = this->obtenerEvento(event, window, renderer);
    // Si es -1 no hubo evento o es un evento que no me interesa
    if (!this->serverDown) {
        if (eventoCliente != -1) {
            int size = 2048;
            char buffer[size];
            string bufferString = convertirTipoMensajeEnString(eventoCliente);
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, bufferString.c_str());
            this->enviar(buffer, size);
            Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", buffer);
        }
        this->evento = eventoCliente;
    } else {
        this->evento = constantes::eventoEnumCliente::SERVEUR_DOWN;
    }

    if (eventoCliente == constantes::eventoEnumCliente::ESCAPE) {
        Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", "Cerrando socket...");
        this->cerrarConexion();
        Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", "Socket cerrado");
    }

}

void JuegoCliente::mandarEventoMejorado(SDL_Window *window, SDL_Renderer *renderer) {
    int eventoCliente = this->obtenerEventoMejorado();
    // Si es -1 no hubo evento o es un evento que no me interesa
    if (!this->serverDown) {
        if (eventoCliente != -1) {
            int size = 2048;
            char buffer[size];
            string bufferString = convertirTipoMensajeEnString(eventoCliente);
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, bufferString.c_str());
                this->enviar(buffer, size);
            Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", buffer);
        }
        this->evento = eventoCliente;
    } else {
        this->evento = constantes::eventoEnumCliente::SERVEUR_DOWN;
    }

    if (eventoCliente == constantes::eventoEnumCliente::ESCAPE) {
        Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", "Cerrando socket...");
            this->cerrarConexion();
        Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::mandarEvento", "Socket cerrado");
    }

}

int JuegoCliente::obtenerEvento(SDL_Event event, SDL_Window *window, SDL_Renderer *renderer) {
    const std::string CONTEXTO = "JuegoCliente::obtenerEvento";
    if (this->isCambioNivel()) {
        return -1;
    }

    switch (event.type) {
        case SDL_QUIT:
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Cerrar juego obtener evento");
            this->cerrarJuegoCliente(renderer, window);
            return constantes::eventoEnumCliente::ESCAPE;

        case SDL_KEYDOWN : {
            switch (event.key.keysym.sym) {
                case SDLK_LEFT:
                    return constantes::eventoEnumCliente::TECLA_IZQUIERDA;
                case SDLK_RIGHT:
                    return constantes::eventoEnumCliente::TECLA_DERECHA;
                case SDLK_F1 :
                    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Retorno al menu");
                    this->irMenu = true;
                    for (int i = 0; i < cantidadJugador; i++) {
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setInvincible(false);
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setEscudo(false);
                    }
                    return constantes::eventoEnumCliente::ESCAPE;
                case SDLK_UP:
                    return constantes::eventoEnumCliente::TECLA_ARRIBA;
                case SDLK_SPACE:
                    return constantes::eventoEnumCliente::SPIN_ATACK;
                case SDLK_i:
                    return constantes::eventoEnumCliente::TECLA_IMORTAL;
                default:
                    return -1;
            }
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        return constantes::eventoEnumCliente::QUIETO;
                    case SDLK_RIGHT:
                        return constantes::eventoEnumCliente::QUIETO;
                    case SDLK_SPACE:
                        return constantes::eventoEnumCliente::QUIETO;
                }
        }
        default:
            return -1;

    }
}

int JuegoCliente::obtenerEventoMejorado() {
    const std::string CONTEXTO = "JuegoCliente::obtenerEvento";
    if (this->isCambioNivel()) {
        return -1;
    }

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    usleep(35000);
    if (currentKeyStates[SDL_SCANCODE_F1]) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Retorno al menu");
        this->irMenu = true;
        for (int i = 0; i < cantidadJugador; i++) {
            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setInvincible(false);
            this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setEscudo(false);
        }
        return constantes::eventoEnumCliente::ESCAPE;
    }

    if (currentKeyStates[SDL_SCANCODE_I]) {
        return constantes::eventoEnumCliente::TECLA_IMORTAL;
    }

    if (currentKeyStates[SDL_SCANCODE_UP]) {
        mandeEventoQuieto = false;
        return constantes::eventoEnumCliente::TECLA_ARRIBA;
    }
    if (currentKeyStates[SDL_SCANCODE_SPACE]) {
        mandeEventoQuieto = false;
        return constantes::eventoEnumCliente::SPIN_ATACK;
    }

    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        mandeEventoQuieto = false;
        return constantes::eventoEnumCliente::TECLA_IZQUIERDA;
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        mandeEventoQuieto = false;
        return constantes::eventoEnumCliente::TECLA_DERECHA;
    }
    if (!mandeEventoQuieto) {
        mandeEventoQuieto = true;
        return constantes::eventoEnumCliente::QUIETO;
    }
    return -1;
}

vector<int> JuegoCliente::parsearMensaje(const char *mensaje) {
    char FIN = 'z';
    char DELIMITADOR = ',';
    int charInt;
    int contador = 3;
    string valor = "";
    vector<int> mensajeInts;
    while (mensaje[contador] != FIN) {
        char caracter = mensaje[contador];
        if (caracter == DELIMITADOR) {
            charInt = atoi(valor.c_str());
            valor = "";
            mensajeInts.push_back(charInt);
        } else {
            valor = valor + caracter;
        }
        contador++;
    }

    Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::parsearMensaje", "Mensaje parseado");
    return mensajeInts;
}

/*********************************** ARRANCA CLIENTE *************************************************/

bool JuegoCliente::iniciarCliente() {
    const std::string CONTEXTO = "JuegoCliente::iniciarCliente";
    int PUERTO = this->puerto;

    struct sockaddr_in direccionServidor;



    // Seteo los parametros del struct direccionServidor
    direccionServidor.sin_family = AF_INET; // tipo ipv4
    direccionServidor.sin_port = htons(PUERTO); // puerto en el que va a escuchar
    //TODO Deberia setear la ip pasada en el constructor
    direccionServidor.sin_addr.s_addr = inet_addr(this->ip); //llenado con la ip del servidor

    /***** Socket() *****/
    int socketCliente; // identificador del socket para escuchar conexiones
    // Creo el socket. Si falló devuelve -1
    socketCliente = socket(AF_INET, SOCK_STREAM, 0); //tipo ipv4, "reliable, 2-way, connection-based service", default
    if (socketCliente < 0) {

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al crear el Socket Cliente");

        return false;
    }

    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Socket Cliente creado con exito");
    if (TTF_Init() == -1) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                     "Fallo al inicializar SDL_ttf Error : " + std::string(TTF_GetError()));
    }
    /***** Connect() *****/
    if (connect(socketCliente, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al conectar con el servidor");
        return false;
    }
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Conexion realizada con exito");
    this->socketCliente = socketCliente;
    this->serverDown = false;


    thread miThread(&JuegoCliente::manejarReceive, this);
    miThread.detach();


    return true;
}

void JuegoCliente::manejarReceive() {
    const std::string CONTEXTO = "JuegoCliente::manejarReceive";

    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Thread iniciado");

    int bytesRecibidos = 1;
    while (bytesRecibidos > 0) {
        // Si no esta corriendo el thread de latido lo creo

        this->serverDown = false;

        int bytesTotales = 2048;
        char buffer[bytesTotales];
        int contadorBytesRecibidos = 0;
        while (contadorBytesRecibidos != 2048) {
            /***** Receive() *****/
            bytesRecibidos = recibir(buffer + contadorBytesRecibidos, bytesTotales - contadorBytesRecibidos);
            contadorBytesRecibidos += bytesRecibidos;
            if (bytesRecibidos <= 0) {
                break;
            }

        }
        this->procesarMensaje(buffer);
    }
    this->serverDown = true;
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Se perdio la conexion con el servidor");
    this->cerrarConexion();
    this->evento = constantes::eventoEnumCliente::ESCAPE;
}

void JuegoCliente::actualizarNovedades(char *mensaje) {
    const std::string CONTEXTO = "JuegoCliente::actualizarNovedades";
//    if (!this->threadLatidoEstaCorriendo) {
//      Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "El thread latido no esta corriendo, debo iniciarlo!");
//      this->cableEstaConectado = true;
//      this->setimagenCableDesconectado(false);
//      thread miThread(&JuegoCliente::threadEscucharLatido, this);
//      miThread.detach();
//  }
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    //cout << "actualizarNovedades : get escenario " << endl;
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();
    this->reconexion = false;

    //TODO WARNING MUTEX ACA
    escenario->setXCamara(mensajeParseado[0]);
    escenario->setYCamara(mensajeParseado[1]);
    for (int i = 2; i < mensajeParseado.size(); i = i + 8) {
        escenario->cambiarPosicionPersonaje(mensajeParseado[i], mensajeParseado[i + 1], mensajeParseado[i + 2],
                                            mensajeParseado[i + 5], mensajeParseado[i + 6], mensajeParseado[i + 7],
                                            idCliente);
        int eventoCliente = mensajeParseado[i + 4];
        int indexSprite = mensajeParseado[i + 3];

        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "IndexSprite" + std::to_string(indexSprite));

        Personaje *personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[i]);
        switch (eventoCliente) {
            case constantes::eventoEnumCliente::TECLA_IZQUIERDA:
                personaje->setCurrentClipRetroceder(indexSprite);
                break;

            case constantes::eventoEnumCliente::TECLA_DERECHA:
                personaje->setCurrentClipAvanzar(indexSprite);
                break;

            case constantes::eventoEnumCliente::TECLA_ARRIBA:
                personaje->setCurrentClipSaltar(indexSprite);
                break;
                //Habria que ver depende el lado
            case constantes::eventoEnumCliente::SPIN_ATACK:
                personaje->setCurrentClipSaltar(indexSprite);
                break;
        }
    }
    //TODO WARNING MUTEX ACA
    Log::loguear(constantes::logEnumModo::ALTO, "JuegoCliente::actualizarNovedades", "Novedades actualizadas");
}

void JuegoCliente::procesarMensaje(char *buffer) {
    int cambioDeNivelMientrasReco = false;
    if (!this->gameOver) {
        const std::string CONTEXTO = "JuegoCliente::procesarMensaje";
        int tipoMensaje = atoi(buffer);
        if (isCambioNivel()) {
            return;
        }

//        if (tipoMensaje != 6){
//            cout << "buffer " << buffer << endl;
//        }

        if (tipoMensaje == constantes::tipoMensajeServidorCliente::NUEVO_NIVEL && noIniciaJuego) {
            cambioDeNivelMientrasReco = true;
        }
        switch (tipoMensaje) {
            case constantes::tipoMensajeServidorCliente::PRIMERA_CONEXION:
                //this->actualizarPrimeraConexion(buffer);
                if (!primeraConexion) {
                    this->idCliente = atoi(buffer + 3);
                    this->modoDeJuego = atoi(buffer + 5);
                    this->primeraConexion = true;
                }
                break;
            case constantes::tipoMensajeServidorCliente::INICIA_JUEGO:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Inicia el Juego");
                this->cantidadJugador = buffer[3] - 48;
                if (cambioDeNivelMientrasReco) {
                    nivelActual++;
                }
                this->noIniciaJuego = false;
                break;
            case constantes::tipoMensajeServidorCliente::EQUIPOS_LISTOS:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Equipos listos");
                this->equiposListos = true;
                break;
            case constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Equipo elegido");
                actualizarEquipos(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::RECONEXION:
                nivelActual = buffer[3] - 48;
                this->reconexion = true;
                break;
            case constantes::tipoMensajeServidorCliente::CAMBIO_NIVEL_LISTO:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                             "Todos los clientes son listos para renderizar el nuevo nivel");
                this->cambioNivelListo = true;
                break;
            case constantes::tipoMensajeServidorCliente::ACTUALIZA_VISTA:
                actualizarNovedades(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::ACTUALIZA_ENEMIGOS:
                actualizarEnemigos(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::ACTUALIZA_ENEMIGO_FINAL:
                actualizarEnemigoFinal(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::CUPO_LLENO:
                this->cupoLleno = true;
                break;
            case constantes::eventoEnumCliente::ESCUCHAR_LATIDO:
                this->cableEstaConectado = true;
                this->setimagenCableDesconectado(false);
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Me llego el latido");
                break;
            case constantes::tipoMensajeServidorCliente::NUEVO_NIVEL:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Cambio de nivel");
                this->setCambioNivel(true);
                break;
            case constantes::tipoMensajeServidorCliente::BONUS:
                actualizarBonus(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::RANDOMIZAR_ESCENARIO:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Randomizar Escenario");
                randomizarEscenario(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE:
                this->actualizarGameOver(buffer);
                break;
            case constantes::tipoMensajeServidorCliente::GANADOR:
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Fin del juego");
                this->juegoGanado = true;
                usleep(5000);
                this->evento = constantes::eventoEnumCliente::ESCAPE ;
                break;
            default:
                break;
        }

        Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, buffer);
    }
}

void JuegoCliente::randomizarEscenario(char *buffer) {
    vector<int> idsNivel1;
    vector<int> idsNivel2;
    vector<int> idsNivel3;

//    cout << "RANDOM RECIBI: " << buffer << endl;
    parsearMensajeRandomizado(buffer, idsNivel1, idsNivel2, idsNivel3);

    // Elimino las entidades y enemigos que no fueron seleccionados por el randomization
    eliminarEntidadesEnemigosNivel(idsNivel1, 0);
    eliminarEntidadesEnemigosNivel(idsNivel2, 1);
    eliminarEntidadesEnemigosNivel(idsNivel3, 2);

    //TEST
    for (int i = 0; i < this->niveles[1]->getEscenario()->getEntidades().size(); i++) {
//        cout << "ID ENTIDAD NIVEL 2: " << this->niveles[1]->getEscenario()->getEntidades()[i]->getId() << endl;
    }
}

void JuegoCliente::eliminarEntidadesEnemigosNivel(vector<int> idsNivel, int indiceNivel) {
    vector<Entidad *> entidades = this->niveles[indiceNivel]->getEscenario()->getEntidades();
    vector<Enemigo *> enemigos = this->niveles[indiceNivel]->getEscenario()->getEnemigos();

    vector<Entidad *> entidadesCopia;
    vector<Enemigo *> enemigosCopia;
    for (int i = 0; i < idsNivel.size(); i++) {
        for (int j = 0; j < entidades.size(); j++) {
            if (idsNivel[i] == entidades[j]->getId()) {
                entidadesCopia.push_back(entidades[j]);
                break;
            }
        }
        for (int j = 0; j < enemigos.size(); j++) {
            if (idsNivel[i] == enemigos[j]->getId()) {
                enemigosCopia.push_back(enemigos[j]);
                break;
            }
        }
    }
    this->niveles[indiceNivel]->getEscenario()->setEntidades(entidadesCopia);
    this->niveles[indiceNivel]->getEscenario()->setEnemigos(enemigosCopia);
}

void
JuegoCliente::parsearMensajeRandomizado(char *mensaje, vector<int> &nivel1, vector<int> &nivel2, vector<int> &nivel3) {
    char FIN = 'z';
    char FIN_NIVEL = 'n';
    char DELIMITADOR = ',';
    int charInt;
    int contador = 3;
    string valor = "";
    int i = 0;
    while (mensaje[contador] != FIN) {
        while (mensaje[contador] != FIN_NIVEL) {
            char caracter = mensaje[contador];
            if (caracter == DELIMITADOR) {
                charInt = atoi(valor.c_str());
                valor = "";

                switch (i) {
                    case 0:
                        nivel1.push_back(charInt);
                        break;
                    case 1:
                        nivel2.push_back(charInt);
                        break;
                    case 2:
                        nivel3.push_back(charInt);
                        break;
                }
            } else {
                valor = valor + caracter;
            }
            contador++;
        }
        contador++;
        i++;

    }

    Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::parsearMensajeRandomizado",
                 "Mensaje randomizado parseado");
}

void JuegoCliente::cerrarConexion() {
    close(this->socketCliente);
}

int JuegoCliente::enviar(char *buffer, int bufferSize) {
    const std::string CONTEXTO = "JuegoCliente::enviar";
    int bytesMandados = send(this->socketCliente, buffer, bufferSize, 0);

    if (bytesMandados < 0) {

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el send");

    }

    if (bytesMandados != bufferSize) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "No se mandaron todos los bytes al servidor");
    }

    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Send realizado con exito");
    return bytesMandados;
}

int JuegoCliente::recibir(char *buffer, int bufferSize) {
    const std::string CONTEXTO = "JuegoCliente::recibir";
    int contadorBytesRecibidos = recv(this->socketCliente, buffer, bufferSize, 0);
    if (contadorBytesRecibidos < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el Receive");

    } else {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Receive realizado con exito");
    }

    return contadorBytesRecibidos;
}

void JuegoCliente::enviarMensajes() {
    while (true) {
        int bufferSize = 2048;
        char buffer[bufferSize];
        string mensaje;
        cin >> mensaje;
        if (mensaje.compare("SALIR") == 0) {
            return;
        }
        char const *mensajeChar = mensaje.c_str();
        strcpy(buffer, mensajeChar);
        int bytesMandados = this->enviar(buffer, bufferSize);
    }
}

bool JuegoCliente::isimagenCableDesconectado() const {
    return imagenCableDesconectado;
}

void JuegoCliente::setimagenCableDesconectado(bool setImagenCableDesconectado) {
    JuegoCliente::imagenCableDesconectado = setImagenCableDesconectado;
}

bool JuegoCliente::isCambioNivel() const {
    return cambioNivel;
}

void JuegoCliente::setCambioNivel(bool cambioNivel) {
    JuegoCliente::cambioNivel = cambioNivel;
}

void JuegoCliente::actualizarEnemigos(char *mensaje) {
    const std::string CONTEXTO = "JuegoCliente::actualizarEnemigos";
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();

    for (int i = 0; i < mensajeParseado.size(); i = i + 4) {
        escenario->cambiarPosicionEnemigos(mensajeParseado[i], mensajeParseado[i + 1], mensajeParseado[i + 2]);
        int indexSprite = mensajeParseado[i + 3];

        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "IndexSprite" + std::to_string(indexSprite));

        Enemigo *enemigo = this->niveles[nivelActual]->getEscenario()->getEnemigoById(mensajeParseado[i]);
        enemigo->setCurrentClipAvanzar(indexSprite);

    }
    //TODO WARNING MUTEX ACA
    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, mensaje);
}

void JuegoCliente::actualizarEnemigoFinal(char *mensaje) {
    const std::string CONTEXTO = "JuegoCliente::actualizarEnemigoFinal";
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();
    escenario->cambiarPosicionEnemigoFinal(mensajeParseado[0], mensajeParseado[1]);
    int indexSprite = mensajeParseado[2];
    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "IndexSprite" + std::to_string(indexSprite));

    EnemigoFinal *enemigoFinal = this->niveles[nivelActual]->getEscenario()->getEnemigoFinal();
    enemigoFinal->setCurrentClipAvanzarFinal(indexSprite);

    for (int i = 3; i < mensajeParseado.size(); i = i + 4) {
        escenario->cambiarPosicionFuegos(mensajeParseado[i], mensajeParseado[i + 1], mensajeParseado[i + 2]);
        int indexSprite = mensajeParseado[i + 3];
        Fuego *fuego = this->niveles[nivelActual]->getEscenario()->getFuegos()[mensajeParseado[i]];
        fuego->setCurrentClipAvanzar(indexSprite);

    }

}


void JuegoCliente::actualizarBonus(const char *mensaje) {
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::actualizarBonus", mensaje);

    switch (mensajeParseado[1]) {
        case constantes::tipoEntidades::ESCUDO:
            this->niveles[nivelActual]->getEscenario()->getEntitad(mensajeParseado[2])->setCoordenadaX(-1000);
            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setEscudo(true);
            break;
        case constantes::tipoEntidades::ESCUDO_RECO:
            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setEscudo(true);
            break;
        case constantes::tipoEntidades::FIN_ESCUDO:
            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setEscudo(false);
            break;
        case constantes::tipoEntidades::INVICIBILIDAD:
            this->niveles[nivelActual]->getEscenario()->getEntitad(mensajeParseado[2])->setCoordenadaX(-1000);
            break;
        case constantes::tipoEntidades::INVICIBILIDAD_SPRITE:
            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setInvincible(true);
            this->niveles[nivelActual]->getEscenario()->getPersonaje(
                    mensajeParseado[0])->setIndiceInvincibilidadTexture(mensajeParseado[2]);
            break;
        case constantes::tipoEntidades::FIN_INVINCIBILIDAD:
            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setInvincible(false);
            break;
        case constantes::tipoEntidades::PERDIENDO:
            if (mensajeParseado[2] == constantes::tipoPerdido::VIDA) {
                this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setPerdiendoVida(50);
                this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setVida(
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->getVida() - 1);
                if (mensajeParseado[0] == idCliente) {
                    this->niveles[nivelActual]->getEscenario()->getPersonaje(
                            mensajeParseado[0])->setActualizarPuntosAnillos(true);
                    this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setCurrentClipVida(
                            3 * idCliente +
                            this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->getVida() -
                            1);
                }
            } else {
                this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setPerdiendoAnillos(50);
                this->niveles[nivelActual]->getEscenario()->getPersonaje(mensajeParseado[0])->setAnillos(0);
                if (mensajeParseado[0] == idCliente) {
                    this->niveles[nivelActual]->getEscenario()->getPersonaje(
                            mensajeParseado[0])->setActualizarPuntosAnillos(true);
                }
            }

            break;
        default:
            this->niveles[nivelActual]->getEscenario()->getEntitad(mensajeParseado[2])->setCoordenadaX(-1000);
            break;

    }
}


void JuegoCliente::gameOverGrafico(SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    SDL_Surface *wait = IMG_Load("images/gameOver.png");
    SDL_Texture *waitTexture = SDL_CreateTextureFromSurface(renderer, wait);
    SDL_RenderCopy(renderer, waitTexture, NULL, NULL);
    SDL_FreeSurface(wait);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(waitTexture);
    SDL_Delay(3000);

}

void JuegoCliente::actualizarGameOver(const char *mensaje) {
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    if (this->idCliente == mensajeParseado[0]) {
        this->gameOver = true;
        Log::loguear(constantes::logEnumModo::BAJO, "JuegoCliente::actualizarGameOver", "personaje Game over");
        int eventoCliente = constantes::eventoEnumCliente::ESCAPE;
        int size = 2048;
        char buffer[size];
        string bufferString = convertirTipoMensajeEnString(eventoCliente);
        memset(buffer, 0, sizeof(buffer));
        strcpy(buffer, bufferString.c_str());
        this->enviar(buffer, size);
        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoCliente::actualizarGameOver", "evento ESCAPE");
        usleep(50000);

        Log::loguear(constantes::logEnumModo::BAJO, "JuegoCliente::actualizarGameOver", "Cerrando socket...");
        this->cerrarConexion();
        Log::loguear(constantes::logEnumModo::BAJO, "JuegoCliente::actualizarGameOver", "Socket cerrado");


    }


}


void JuegoCliente::actualizarPrimeraConexion(char *mensaje) {

    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    this->idCliente = mensajeParseado[0];
}

bool JuegoCliente::isPrimeraConexion() const {
    return primeraConexion;
}

void JuegoCliente::setPrimeraConexion(bool primeraConexion) {
    JuegoCliente::primeraConexion = primeraConexion;
}


void JuegoCliente::actualizarEquipos(const char *mensaje) {
    vector<int> mensajeParseado = this->parsearMensaje(mensaje);
    if (mensajeParseado[0] == 1) {
        Log::loguear(constantes::logEnumModo::BAJO, "JuegoCliente::actualizarEquipos",
                     "personaje " + to_string(mensajeParseado[1]) + " en equipo 1");
        equipo1.push_back(mensajeParseado[1]);

    } else {
        Log::loguear(constantes::logEnumModo::BAJO, "JuegoCliente::actualizarEquipos",
                     "personaje " + to_string(mensajeParseado[1]) + " en equipo 2");
        equipo2.push_back(mensajeParseado[1]);
    }

}

string JuegoCliente::convertirTipoMensajeEnString(int tipoMensaje) {
    std::string resultado = "";
    if (tipoMensaje < 10) {
        resultado = "0" + to_string(tipoMensaje);
    } else {
        resultado = to_string(tipoMensaje);
    }
    return resultado;
}
