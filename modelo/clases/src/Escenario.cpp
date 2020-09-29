//
// Created by nicolas on 19/03/17.
//

#include "../headers/Capa.h"
#include "../headers/Entidad.h"
#include "../headers/Escenario.h"
#include "../headers/Parser.h"
#include "../headers/JuegoServidor.h"
#include "../headers/Fuego.h"
#include <algorithm>
#include <thread>

using namespace std;


Escenario::Escenario() {

    this->alto = ALTO_ESCENARIO;
    this->ancho = ANCHO_ESCENARIO;
    Log::loguear(constantes::logEnumModo::ALTO, "Escenario::Constructor",
                 "Escenario creada con Ancho:" + std::to_string(this->ancho) + " , Alto: " +
                 std::to_string(this->alto));
    this->margenIzquierda = false;
    this->margenDerecha = false;
    this->enemigoFinal = new EnemigoFinal(0, "rojo", this->ancho - 600, 150, 0, 0, "", 99, 190, 150);
    this->fuegos.push_back(new Fuego(0, "rojo", 400, 300, 0, 0, "", 99, 30, 60, true));
    this->fuegos.push_back(new Fuego(1, "rojo", 400, 300, 0, 0, "", 99, 30, 60, false));
    this->fuegos.push_back(new Fuego(2, "rojo", 400, 300, 0, 0, "", 99, 30, 60, true));
    this->fuegos.push_back(new Fuego(3, "rojo", 400, 300, 0, 0, "", 99, 30, 60, false));

    Entidad *agua = new Rectangulo(500, "azul", 10000, 660, 0, 0, "images/agua.png", 99, 3900, 200);
    this->aguas.push_back(agua);


    Entidad *agua2 = new Rectangulo(501, "azul", 13900, 660, 0, 0, "images/agua.png", 99, 3900, 200);
    this->aguas.push_back(agua2);

    Entidad *agua3 = new Rectangulo(502, "azul", 17800, 660, 0, 0, "images/agua.png", 99, 2200, 200);
    this->aguas.push_back(agua3);

    this->aguas.push_back( new Rectangulo(503, "amarillo", 38700, 515, 0, 0, "images/levelEndbig.png", 99, 150, 150));

}


Escenario::Escenario(int ancho, int alto) {

    if (alto > 0) {
        this->alto = alto;
    } else {
        this->alto = ALTO_ESCENARIO;
    }
    if (ancho > 0) {
        this->ancho = ancho;
    } else {
        this->ancho = ANCHO_ESCENARIO;
    }

    this->xCamara = 0;
    this->yCamara = 0;

    this->margenIzquierda = false;
    this->margenDerecha = false;
    this->enemigoFinal = new EnemigoFinal(0, "rojo", this->ancho - 600, 150, 0, 0, "", 99, 190, 150);
    this->fuegos.push_back(new Fuego(0, "rojo", 400, 300, 0, 0, "", 99, 30, 60, true));
    this->fuegos.push_back(new Fuego(0, "rojo", 400, 300, 0, 0, "", 99, 30, 60, false));
    this->fuegos.push_back(new Fuego(0, "rojo", 400, 300, 0, 0, "", 99, 30, 60, true));
    this->fuegos.push_back(new Fuego(0, "rojo", 400, 300, 0, 0, "", 99, 30, 60, false));

    Entidad *agua = new Rectangulo(500, "azul", 10000, 660, 0, 0, "images/agua.png", 99, 3900, 200);
    this->aguas.push_back(agua);


    Entidad *agua2 = new Rectangulo(501, "azul", 13900, 660, 0, 0, "images/agua.png", 99, 3900, 200);
    this->aguas.push_back(agua2);

    Entidad *agua3 = new Rectangulo(502, "azul", 17800, 660, 0, 0, "images/agua.png", 99, 2200, 200);
    this->aguas.push_back(agua3);

    this->aguas.push_back( new Rectangulo(503, "amarillo", 38700, 515, 0, 0, "images/levelEndbig.png", 99, 150, 150));

    Log::loguear(constantes::logEnumModo::ALTO, "Escenario::Constructor",
                 "Escenario creada con Ancho:" + std::to_string(this->ancho) + " , Alto: " +
                 std::to_string(this->alto));
}


Escenario::~Escenario() {
    for (int i = 0; i < this->capas.size(); i++) {
        delete this->capas[i];
        Log::loguear(constantes::logEnumModo::ALTO, "Escenario::Destructor", "Elimine la capa "+ to_string(i));
    }
    for (int i = 0; i < this->entidades.size(); i++) {
        delete this->entidades[i];
        Log::loguear(constantes::logEnumModo::ALTO, "Escenario::Destructor", "Elimine la entidad "+ to_string(i));
    }
    for (int i = 0; i < this->enemigos.size(); i++) {
        delete this->enemigos[i];
        Log::loguear(constantes::logEnumModo::ALTO, "Escenario::Destructor", "Elimine el enemigo "+ to_string(i));
    }
    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::Destructor", "Escenario destruido.");
}


void
Escenario::iniciarEscenario(SDL_Renderer *renderer, int idCliente) {
    std::string CONTEXTO = "Escenario::iniciarEscenario";

    //personajes[idCliente]->setIndexZ(personajes[idCliente]->getIndexZ()+1);

    //Ordeno los Dibujables y los dibujo
    sort(this->capas.begin(), this->capas.end(), Dibujable::sortDibujable);
    for (int i = 0; i < this->capas.size(); i++) {
        capas[i]->load(renderer);
    }

    for (int i = 0; i < this->entidades.size(); i++) {
        entidades[i]->load(renderer);
    }
    for (int i = 0; i < this->aguas.size(); i++) {
        aguas[i]->load(renderer);
    }

    for (int i = 0; i < this->enemigos.size(); i++) {
        enemigos[i]->load(renderer);
    }

    for (int i = 0; i < this->fuegos.size(); i++) {
        fuegos[i]->load(renderer);
    }

    enemigoFinal->load(renderer);

    for (int i = 0; i < this->personajes.size(); i++) {
        if (i != idCliente) {
            personajes[i]->load(renderer);
            personajes[i]->setCoordenadaX(-100);
        }
        personajes[idCliente]->load(renderer);
        personajes[idCliente]->setCoordenadaX(-100);
    }

    Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, "Inicie el escenario");

}

int Escenario::getAlto() {
    return alto;
}

int Escenario::getAncho() {
    return ancho;
}

void Escenario::setAlto(int alto) {
    this->alto = alto;
}

void Escenario::setAncho(int ancho) {
    this->ancho = ancho;
}

void Escenario::setCapas(std::vector<Capa *> capas) {
    std::vector<Capa *> mycapas;
    for (int i = 0; i < capas.size(); i++) {
        mycapas.push_back(capas[i]);
    }

    this->capas = mycapas;
}

void Escenario::setEntidades(std::vector<Entidad *> entidades) {
    this->entidades = entidades;
}

void Escenario::dibujar(SDL_Renderer *renderer, int ventanaAlto, int ventanaAncho, int velScroll, int idCliente) {
    for (int i = 0; i < this->capas.size(); i++) {
        capas[i]->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);
    }
    for (int i = 0; i < this->entidades.size(); i++) {
        entidades[i]->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);
    }
    for (int i = 0; i < this->aguas.size(); i++) {
        aguas[i]->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);
    }
    for (int i = 0; i < this->enemigos.size(); i++) {
        enemigos[i]->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);
    }
    for (int i = 0; i < this->fuegos.size(); i++) {
        fuegos[i]->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);
    }
    enemigoFinal->dibujar(renderer, this->xCamara, ventanaAncho, this->yCamara, ventanaAlto);

    for (int i = 0; i < this->personajes.size(); i++) {
        if (i != idCliente) {
            personajes[i]->dibujar(renderer, personajes[i]->getCoordenadaX(), ventanaAncho,
                                   personajes[i]->getCoordenadaX(),
                                   ventanaAlto, false, personajes[i]->getPuntos());
        }

        personajes[idCliente]->dibujar(renderer, personajes[idCliente]->getCoordenadaX(), ventanaAncho,
                                       personajes[idCliente]->getCoordenadaX(),
                                       ventanaAlto, true, personajes[idCliente]->getPuntos());

        Log::loguear(constantes::logEnumModo::ALTO, "Escenario::dibujar", "Dibuje en personaje");
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Escenario::dibujar", "Dibuje el escenario");

}

int Escenario::getXCamara() {
    return this->xCamara;
}

int Escenario::getYCamara() {
    return this->yCamara;
}

void Escenario::setXCamara(int xCamara) {
    this->xCamara = xCamara;
}

void Escenario::setYCamara(int YCamara) {
    this->yCamara = YCamara;
}

void Escenario::cambiarPosicionPersonaje(int id, int x, int y, int vida, int puntos, int anillos, int idCliente) {
    //OPTIMIZAR (vector ordenado por id y acceder por id a la posicion. El id es el indice del vector
    // de personajes)
    for (int i = 0; this->personajes.size(); i++) {
        if (personajes[i]->getId() == id) {
            personajes[i]->setCoordenadaX(x);
            personajes[i]->setCoordenadaY(y);
            personajes[i]->setVida(vida);
            if (personajes[i]->getPuntos() != puntos) {
                personajes[i]->setPuntos(puntos);
                personajes[i]->setActualizarPuntosAnillos(true);
            }
            if (personajes[i]->getAnillos() != anillos) {
                personajes[i]->setAnillos(anillos);
                personajes[i]->setActualizarPuntosAnillos(true);
            }

            if (i == idCliente) {
                if (vida != 0) {
                    personajes[i]->setCurrentClipVida(3 * idCliente + vida - 1);
                }
            }
            return;
        }
    }
}

void Escenario::setPersonajes(std::vector<Personaje *> personajes) {
    this->personajes = personajes;

}

void Escenario::teclaIzquierda(int id, int velScroll, int ventanaAncho) {
    bool margenDerechoTocado = false;
    bool margenIzquierdoTocado = false;
    Personaje *personajeDelMargen;

    for (int i = 0; i < this->personajes.size(); i++) {
        if (personajes[i]->getId() == id) {
            personajes[i]->setMargenDerecha(false);
            int Xperso = personajes[i]->getCoordenadaX();
            if (Xperso - velScroll - margenParaVista <= 0) {
                personajes[i]->setMargenIzquierda(true);
                personajes[i]->setCoordenadaX(0 + margenParaVista);
                this->mooveCamara(velScroll, ventanaAncho, i);
            } else {
                for (int j = 0; j < this->personajes.size(); j++) {
                    if (personajes[j]->isMargenDerecha()) {
                        margenDerechoTocado = true;
                    }
                    if (personajes[j]->isMargenIzquierda()) {
                        margenIzquierdoTocado = true;
                    }
                }
                personajes[i]->setMargenIzquierda(false);

                personajes[i]->setCoordenadaX(Xperso - velScroll);


                /* if (margenDerechoTocado && !margenIzquierdoTocado){
                     personajes[i]->setCoordenadaX(personajes[i]->getCoordenadaX() - personajeDelMargen->getVelocidadPersonaje());
                 }
                 if (!margenDerechoTocado && margenIzquierdoTocado){
                     personajes[i]->setCoordenadaX(personajes[i]->getCoordenadaX() + personajeDelMargen->getVelocidadPersonaje());
                 }*/


            }

        }
    }

}

void Escenario::teclaDerecha(int id, int velScroll, int ventanaAncho) {
    bool margenDerechoTocado = false;
    bool margenIzquierdoTocado = false;

    Personaje *personajeDelMargen;

    for (int i = 0; i < this->personajes.size(); i++) {
        if (personajes[i]->getId() == id) {
            personajes[i]->setMargenIzquierda(false);
            int Xperso = personajes[i]->getCoordenadaX();

            if (Xperso + personajes[i]->getAncho() + velScroll + margenParaVista >= ventanaAncho) {
                personajes[i]->setMargenDerecha(true);
                personajes[i]->setCoordenadaX(ventanaAncho - personajes[i]->getAncho() - margenParaVista);
                this->mooveCamara(velScroll, ventanaAncho, i);

            } else {
                for (int j = 0; j < this->personajes.size(); j++) {
                    if (personajes[j]->isMargenDerecha()) {
                        margenDerechoTocado = true;
                        personajeDelMargen = personajes[j];

                    }
                    if (personajes[j]->isMargenIzquierda()) {
                        margenIzquierdoTocado = true;
                        personajeDelMargen = personajes[j];
                    }
                }
                personajes[i]->setMargenDerecha(false);

                personajes[i]->setCoordenadaX(Xperso + velScroll);


                /*if (margenDerechoTocado && !margenIzquierdoTocado){
                    personajes[i]->setCoordenadaX(personajes[i]->getCoordenadaX() - personajeDelMargen->getVelocidadPersonaje());
                }
                if (!margenDerechoTocado && margenIzquierdoTocado){
                    personajes[i]->setCoordenadaX(personajes[i]->getCoordenadaX() + personajeDelMargen->getVelocidadPersonaje());
                }*/

            }
        }
    }
}

void Escenario::quieto(int id, int indexSprite) {
    for (int i = 0; i < this->personajes.size(); i++) {
        if (personajes[i]->getId() == id) {
            personajes[i]->setCurrentClipAvanzar(indexSprite);
        }
    }
}


void Escenario::teclaF1(int id) {
    for (int i = 0; i < this->personajes.size(); i++) {
        if (personajes[i]->getId() == id) {
            personajes[i]->setPausa(true);
        }
    }
}

void Escenario::mooveCamara(int velScroll, int ventanaAncho, int id) {
    this->setMargenIzquierda(false);
    this->setMargenDerecha(false);
    bool margenConNuevoX = false;
    for (int i = 0; i < this->personajes.size(); i++) {
        if (personajes[i]->isMargenDerecha()) {
            this->setMargenDerecha(true);
        }
        if (personajes[i]->isMargenIzquierda()) {
            this->setMargenIzquierda(true);
        }
    }
    if (this->margenDerecha) {
        if (!margenIzquierda) {
            if (this->xCamara > -this->getAncho() + ventanaAncho + velScroll) {
                for (int j = 0; j < this->personajes.size(); j++) {
                    if (j != id) {
                        if (personajes[j]->isArrastrable()) {
                            if(personajes[j]->setCoordenadaXforCamaraMove(
                                    personajes[j]->getCoordenadaX() - velScroll,
                                    ventanaAncho, margenParaVista)){
                                margenConNuevoX = true;
                            }
                        }
                    }
                }
                if (!margenConNuevoX) {
                    this->setXCamara(this->xCamara - velScroll);
                    for (int j = 0; j < this->entidades.size(); j++) {
                        if (entidades[j]->getCoordenadaY() != 2000) {
                            entidades[j]->setCoordenadaX(entidades[j]->getCoordenadaXOrigen() + this->getXCamara());
                        }
                    }
                }
            } else {
                this->setXCamara(-this->getAncho() + ventanaAncho);
                for (int j = 0; j < this->entidades.size(); j++) {
                    if (entidades[j]->getCoordenadaY() != 2000) {
                        entidades[j]->setCoordenadaX(entidades[j]->getCoordenadaXOrigen() + this->getXCamara());
                    }
                }
                if (!this->nivelCambiado) {
                    personajes[id]->setLlegadoAlFinal(true);
                    Log::loguear(constantes::logEnumModo::BAJO, "Escenario::mooveCamara", "Personaje "+ to_string(id) + " llego al final");
                    this->nivelCambiado = true;
                }
            }
        }
    } else {
        if (margenIzquierda) {
            if (this->xCamara < 0 - velScroll) {
                for (int j = 0; j < this->personajes.size(); j++) {
                    if (j != id) {
                        if (personajes[j]->isArrastrable()) {
                            if(personajes[j]->setCoordenadaXforCamaraMove(
                                    personajes[j]->getCoordenadaX() + velScroll,
                                    ventanaAncho, margenParaVista)){
                                margenConNuevoX = true;
                            }
                        }
                    }
                }
                if (!margenConNuevoX) {
                    this->setXCamara(this->xCamara + velScroll);
                    for (int j = 0; j < this->entidades.size(); j++) {
                        if (entidades[j]->getCoordenadaY() != 2000) {
                            entidades[j]->setCoordenadaX(entidades[j]->getCoordenadaXOrigen() + this->getXCamara());
                        }
                    }
                }
            } else {
                this->setXCamara(0);
                for (int j = 0; j < this->entidades.size(); j++) {
                    if (entidades[j]->getCoordenadaY() != 2000) {
                        entidades[j]->setCoordenadaX(entidades[j]->getCoordenadaXOrigen() + this->getXCamara());
                    }
                }

            }
        }
    }
}

void Escenario::setMargenIzquierda(bool margen) {
    this->margenIzquierda = margen;
}

void Escenario::setMargenDerecha(bool margen) {
    this->margenDerecha = margen;
}

void Escenario::teclaArriba(int id, int velScroll, int ventanaAncho, int frame, bool &estaSaltando) {
    if (personajes[id]->getId() == id) {
        int Yperso = personajes[id]->getCoordenadaY();
        personajes[id]->setCurrentClipSaltar(frame);
        //velScroll = 15;
        if (frame == 0) {
            personajes[id]->setVelocidadYPersonaje(25);
        }

        if (Yperso <= 545) {
            personajes[id]->setCoordenadaY(Yperso - personajes[id]->getVelocidadYPersonaje());
            personajes[id]->setVelocidadYPersonaje(personajes[id]->getVelocidadYPersonaje() - 1); //aplicar gravedad
            if (personajes[id]->getCoordenadaY() == 545) {
                estaSaltando = false;
                personajes[id]->setFrameSaltar(0);
                personajes[id]->setEstado(constantes::eventoEnumCliente::TECLA_DERECHA);
                personajes[id]->setVelocidadYPersonaje(0);
            }
        }
        /*if ((frame >= 16) && (Yperso < 545)) {

            personajes[id]->setCoordenadaY(Yperso + velScroll);
            if (personajes[id]->getCoordenadaY() == 545) {
                estaSaltando = false;
                personajes[id]->setFrameSaltar(0);
                personajes[id]->setEstado(constantes::eventoEnumCliente::TECLA_DERECHA);
            }

        } else {

            personajes[id]->setCoordenadaY(Yperso - velScroll);

        }*/

    }

}

Personaje *Escenario::getPersonaje(int id) {
    return this->personajes[id];
}


void Escenario::setEnemigos(const vector<Enemigo *> &enemigos) {
    Escenario::enemigos = enemigos;
}

const vector<Enemigo *> &Escenario::getEnemigos() const {
    return enemigos;
}

Enemigo *Escenario::getEnemigo(int i) {
    return this->enemigos[i];
}


Enemigo *Escenario::getEnemigoById(int id) {
    for (int i = 0; i < this->enemigos.size(); i++) {
        if (this->enemigos[i]->getId() == id) {
            return this->enemigos[i];
        }
    }
}


void Escenario::cambiarPosicionEnemigos(int id, int x, int y) {
    for (int i = 0; this->enemigos.size(); i++) {
        if (enemigos[i]->getId() == id) {
            enemigos[i]->setCoordenadaX(x);
            enemigos[i]->setCoordenadaY(y);
            return;

        }
    }
}

void Escenario::cambiarPosicionFuegos(int id, int x, int y) {
    fuegos[id]->setCoordenadaX(x);
    fuegos[id]->setCoordenadaY(y);
    return;
}

void Escenario::cambiarPosicionEnemigoFinal(int x, int y) {
    this->enemigoFinal->setCoordenadaX(x);
    this->enemigoFinal->setCoordenadaY(y);
}

vector<int> Escenario::mirarColisiones(int id, int tecla, bool imortal, int ventanaAncho) {
    vector<int> retorno = {-1, -1};
    Personaje *personaje = personajes[id];
    for (int i = 0; i < this->entidades.size(); i++) {
        if (personaje->getCoordenadaX() + personaje->getAncho() - 10 > entidades[i]->getCoordenadaX() &&
            personaje->getCoordenadaX() + 10 < entidades[i]->getCoordenadaX() + entidades[i]->getAncho()) {
            if (entidades[i]->getCoordenadaY() + entidades[i]->getAlto() > personaje->getCoordenadaY() &&
                entidades[i]->getCoordenadaY() < personaje->getCoordenadaY() + personaje->getAlto()) {
                switch (entidades[i]->getTipo()) {
                    case constantes::tipoEntidades::ANILLO:
                        personaje->setAnillos(personaje->getAnillos() + 1);
                        entidades[i]->setCoordenadaY(2000);
                        retorno[0] = constantes::tipoEntidades::ANILLO;
                        retorno[1] = i;
                        return retorno;
                    case constantes::tipoEntidades::ESCUDO:
                        personaje->setEscudo(true);
                        entidades[i]->setCoordenadaY(2000);
                        retorno[0] = constantes::tipoEntidades::ESCUDO;
                        retorno[1] = i;
                        return retorno;
                    case constantes::tipoEntidades::INVICIBILIDAD:
                        personaje->setInvincible(true);
                        entidades[i]->setCoordenadaY(2000);
                        retorno[0] = constantes::tipoEntidades::INVICIBILIDAD;
                        retorno[1] = i;
                        return retorno;
                    case constantes::tipoEntidades::RING:
                        personaje->setAnillos(personaje->getAnillos() + 10);
                        entidades[i]->setCoordenadaY(2000);
                        retorno[0] = constantes::tipoEntidades::RING;
                        retorno[1] = i;
                        return retorno;
                    case constantes::tipoEntidades::PIEDRA:
                        personaje->setArrastrable(true);
                        switch (tecla) {
                            case constantes::eventoEnumCliente::TECLA_DERECHA:
                                if (personaje->isMargenDerecha()) {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX()/* - /*personaje->getVelocidadPersonaje()*/ -
                                            personaje->getAncho());
                                } else {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() - personaje->getVelocidadPersonaje());
                                }
                                break;
                            case constantes::eventoEnumCliente::TECLA_IZQUIERDA :
                                if (personaje->isMargenIzquierda()) {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() /*+ /*personaje->getVelocidadPersonaje()*/ +
                                                    personaje->getAncho());
                                } else {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() + personaje->getVelocidadPersonaje());
                                }
                                break;

                            case constantes::eventoEnumCliente::TECLA_ARRIBA :
                                if (personaje->getCoordenadaX() + personaje->getAncho() / 2 >
                                    entidades[i]->getCoordenadaX() + entidades[i]->getAncho() / 2) {
                                    if (entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5 +
                                        margenParaVista +
                                        personaje->getAncho() < ventanaAncho) {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5);
                                    } else {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() - personaje->getAncho() - 5);
                                    }

                                } else {
                                    if (entidades[i]->getCoordenadaX() - personaje->getAncho() - 5 > margenParaVista) {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() - personaje->getAncho() - 5);
                                    } else {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5);
                                    }
                                }
                                break;
                            default :
                                break;

                        }
                        return retorno;
                    case constantes::tipoEntidades::PINCHE:
                        personaje->setArrastrable(true);
                        switch (tecla) {
                            case constantes::eventoEnumCliente::TECLA_DERECHA:
                                if (personaje->isMargenDerecha()) {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX()/* - personaje->getVelocidadPersonaje()*/ -
                                            personaje->getAncho());
                                } else {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() - personaje->getVelocidadPersonaje());
                                }
                                break;
                            case constantes::eventoEnumCliente::TECLA_IZQUIERDA :
                                if (personaje->isMargenIzquierda()) {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() /*+ personaje->getVelocidadPersonaje()*/ +
                                                    personaje->getAncho());
                                } else {
                                    personaje->setCoordenadaX(
                                            personaje->getCoordenadaX() + personaje->getVelocidadPersonaje());
                                }
                                break;
                            case constantes::eventoEnumCliente::TECLA_ARRIBA :
                                if (personaje->getCoordenadaX() + personaje->getAncho() / 2 >
                                    entidades[i]->getCoordenadaX() + entidades[i]->getAncho() / 2) {
                                    if (entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5 +
                                        margenParaVista +
                                        personaje->getAncho() < ventanaAncho) {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5);
                                    } else {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() - personaje->getAncho() - 5);
                                    }

                                } else {
                                    if (entidades[i]->getCoordenadaX() - personaje->getAncho() - 5 > margenParaVista) {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() - personaje->getAncho() - 5);
                                    } else {
                                        personaje->setCoordenadaX(
                                                entidades[i]->getCoordenadaX() + entidades[i]->getAncho() + 5);
                                    }
                                }
                                break;
                            default :
                                break;
                        }
                        if (!imortal) {
                            if (personaje->getIdPinche() == -1) {
                                if (personaje->getVida() > 0) {
                                    if (!personaje->isInvincible()) {
                                        if (personaje->isEscudo()) {
                                            personaje->setEscudo(false);
                                            personaje->setInvincible(true);
                                            retorno[0] = constantes::tipoEntidades::FIN_ESCUDO;
                                        } else {
                                            if (personaje->getAnillos() != 0) {
                                                personaje->setAnillos(0);
                                                personaje->setInvincible(true);
                                                retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                                retorno[1] = constantes::tipoPerdido::ANILLOS;
                                            } else {
                                                retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                                retorno[1] = constantes::tipoPerdido::VIDA;
                                                personaje->setVida(personaje->getVida() - 1);
                                                personaje->setInvincible(true);
                                            }
                                        }
                                        if (personaje->getVida() == 0) {
                                            personaje->setGameOver(true);
                                            retorno[0] = constantes::tipoEntidades::GAME_OVER;
                                            Log::loguear(constantes::logEnumModo::BAJO, "Escenario::mirarColisiones", "Game over del personaje "+to_string(id));
                                        }
                                        Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisiones", "El personaje "+to_string(id) + " tiene " + to_string(personaje->getVida()) + " vidas");
                                    }
                                }
                            }
                            if (personaje->isModoAtaque()) {
                                    personaje->setIdPinche(i);
                            }
                        }
                        return retorno;
                    default :
                        break;
                }

            }
        }
    }
}

vector<int> Escenario::mirarColisionesEnemigoFinal(int id, int tecla, bool imortal) {
    Personaje *personaje = personajes[id];
    vector<int> retorno = {-1, -1};
    if (personaje->getCoordenadaX() + personaje->getAncho() > enemigoFinal->getCoordenadaX() + this->getXCamara() &&
        personaje->getCoordenadaX() <
        enemigoFinal->getCoordenadaX() + enemigoFinal->getAncho() + this->getXCamara()) {
        if (enemigoFinal->getCoordenadaY() + enemigoFinal->getAlto() > personaje->getCoordenadaY() &&
            enemigoFinal->getCoordenadaY() < personaje->getCoordenadaY() + personaje->getAlto()) {
            switch (tecla) {
                case constantes::eventoEnumCliente::TECLA_ARRIBA :
                    if (enemigoFinal->isVivo()) {
                        if (!enemigoFinal->isInvincible()) {
                            if (enemigoFinal->getVida() > 0) {
                                enemigoFinal->setInvincible(true);
                                enemigoFinal->setVida(enemigoFinal->getVida() - 1);
                                Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigoFinal", "El enemigo final tiene " + to_string(enemigoFinal->getVida()) + " vidas");
                                retorno[0] = constantes::tipoEntidades::ENEMIGO_FINAL_INVINCIBLE;
                                return retorno;
                            } else {
                                enemigoFinal->setVivo(false);
                                personaje->setPuntos(personaje->getPuntos() + enemigoFinal->getPuntos());
                                retorno[0] = constantes::tipoEntidades::VICTORY;
                                return retorno;
                            }
                        }
                    }
                    return retorno;
                default:
                    if (personaje->isSaltando()) {
                        if (enemigoFinal->isVivo()) {
                            if (!enemigoFinal->isInvincible()) {
                                if (enemigoFinal->getVida() > 0) {
                                    enemigoFinal->setInvincible(true);
                                    enemigoFinal->setVida(enemigoFinal->getVida() - 1);
                                    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigoFinal", "El enemigo final tiene " + to_string(enemigoFinal->getVida()) + " vidas");
                                    retorno[0] = constantes::tipoEntidades::ENEMIGO_FINAL_INVINCIBLE;
                                    return retorno;
                                } else {
                                    enemigoFinal->setVivo(false);
                                    personaje->setPuntos(personaje->getPuntos() + enemigoFinal->getPuntos());
                                    retorno[0] = constantes::tipoEntidades::VICTORY;
                                    return retorno;
                                    ///fin de juego !
                                }
                            }
                        }
                        return retorno;
                    }
                    if (!imortal) {
                        if (personaje->getVida() > 0) {
                            if (!personaje->isInvincible()) {
                                if (personaje->isEscudo()) {
                                    personaje->setEscudo(false);
                                    personaje->setInvincible(true);
                                    retorno[0] = constantes::tipoEntidades::FIN_ESCUDO;
                                } else {
                                    if (personaje->getAnillos() != 0) {
                                        personaje->setAnillos(0);
                                        retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                        retorno[1] = constantes::tipoPerdido::ANILLOS;
                                        personaje->setInvincible(true);
                                    } else {
                                        retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                        retorno[1] = constantes::tipoPerdido::VIDA;
                                        personaje->setVida(personaje->getVida() - 1);
                                        personaje->setInvincible(true);
                                        Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigoFinal", "El personaje " + to_string(id)+" tiene " + to_string(personaje->getVida()) + " vidas");
                                    }
                                }
                                if (personaje->getVida() == 0) {
                                    personaje->setGameOver(true);
                                    retorno[0] = constantes::tipoEntidades::GAME_OVER;
                                    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigoFinal", "El personaje " + to_string(id)+ " esta Game over");
                                }
                            }
                        }

                    }
                    return retorno;
            }
        }
    }

    return retorno;
}

vector<int> Escenario::mirarColisionesEnemigos(int id, int tecla, bool imortal) {
    Personaje *personaje = personajes[id];
    vector<int> retorno = {-1, -1};

    for (int i = 0; i < this->enemigos.size(); i++) {
        if (personaje->getCoordenadaX() + personaje->getAncho() > enemigos[i]->getCoordenadaX() + this->getXCamara() &&
            personaje->getCoordenadaX() <
            enemigos[i]->getCoordenadaX() + enemigos[i]->getAncho() + this->getXCamara()) {
            if (enemigos[i]->getCoordenadaY() + enemigos[i]->getAlto() > personaje->getCoordenadaY() &&
                enemigos[i]->getCoordenadaY() < personaje->getCoordenadaY() + personaje->getAlto()) {
                if (personaje->getIdEnemigo() != i) {
                    switch (tecla) {
                        case constantes::eventoEnumCliente::TECLA_ARRIBA :
                            if (enemigos[i]->isVivo()) {
                                enemigos[i]->setVivo(false);
                                personaje->setPuntos(personaje->getPuntos() + enemigos[i]->getPuntos());
                            }
                            Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigos", "El personaje " + to_string(id) + " tiene " + to_string(personaje->getPuntos())+ " puntos");
                            return retorno;
                        default:
                            if (personaje->isSaltando()) {
                                if (enemigos[i]->isVivo()) {
                                    enemigos[i]->setVivo(false);
                                    personaje->setPuntos(personaje->getPuntos() + enemigos[i]->getPuntos());
                                }
                                Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigos", "El personaje " + to_string(id) + " tiene " + to_string(personaje->getPuntos())+ " puntos");
                                return retorno;
                            }
                            if (!imortal) {
                                if (personaje->getVida() > 0) {
                                    if (!personaje->isInvincible()) {
                                        personaje->setIdEnemigo(i);
                                        if (personaje->isEscudo()) {
                                            personaje->setEscudo(false);
                                            personaje->setInvincible(true);
                                            retorno[0] = constantes::tipoEntidades::FIN_ESCUDO;
                                        } else {
                                            if (personaje->getAnillos() != 0) {
                                                personaje->setAnillos(0);
                                                personaje->setInvincible(true);
                                                retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                                retorno[1] = constantes::tipoPerdido::ANILLOS;
                                            } else {
                                                retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                                retorno[1] = constantes::tipoPerdido::VIDA;
                                                personaje->setInvincible(true);
                                                personaje->setVida(personaje->getVida() - 1);
                                                Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigos", "El personaje " + to_string(id) + " tiene " + to_string(personaje->getVida())+ " vidas");
                                            }
                                        }
                                        if (personaje->getVida() == 0) {
                                            personaje->setGameOver(true);
                                            retorno[0] = constantes::tipoEntidades::GAME_OVER;
                                            Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigos", "El personaje " + to_string(id) + " esta Game Over ");
                                        }
                                    }
                                }

                            }
                            break;
                    }
                }

            } else {

                if (personaje->getIdEnemigo() == i) {
                    personaje->setIdEnemigo(-1);
                }
            }
        } else {

            if (personaje->getIdEnemigo() == i) {
                personaje->setIdEnemigo(-1);
            }
        }
    }
    return retorno;
}

vector<int> Escenario::mirarColisionesFuegos(int id, int tecla, bool imortal) {
    Personaje *personaje = personajes[id];
    vector<int> retorno = {-1, -1};
    for (int i = 0; i < this->fuegos.size(); i++) {
        if (personaje->getCoordenadaX() + personaje->getAncho() > fuegos[i]->getCoordenadaX() + this->getXCamara() &&
            personaje->getCoordenadaX() <
            fuegos[i]->getCoordenadaX() + fuegos[i]->getAncho() + this->getXCamara()) {
            if (fuegos[i]->getCoordenadaY() + fuegos[i]->getAlto() > personaje->getCoordenadaY() &&
                fuegos[i]->getCoordenadaY() < personaje->getCoordenadaY() + personaje->getAlto()) {
                switch (tecla) {
                    default:
                        if (!imortal) {
                            if (personaje->getVida() > 0) {
                                if (!personaje->isInvincible()) {
                                    fuegos[i]->setVivo(false);
                                    fuegos[i]->setCoordenadaY(2000);
                                    fuegos[i]->setCoordenadaX(100);
                                    if (personaje->isEscudo()) {
                                        personaje->setEscudo(false);
                                        personaje->setInvincible(true);
                                        retorno[0] = constantes::tipoEntidades::FIN_ESCUDO;
                                    } else {
                                        if (personaje->getAnillos() != 0) {
                                            personaje->setAnillos(0);
                                            retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                            retorno[1] = constantes::tipoPerdido::ANILLOS;
                                            personaje->setInvincible(true);
                                        } else {
                                            retorno[0] = constantes::tipoEntidades::PERDIENDO;
                                            retorno[1] = constantes::tipoPerdido::VIDA;
                                            personaje->setVida(personaje->getVida() - 1);
                                            personaje->setInvincible(true);
                                            Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesFuegos", "El personaje " + to_string(id) + " tiene " + to_string(personaje->getVida())+ " vidas");

                                        }
                                    }
                                    if (personaje->getVida() == 0) {
                                        personaje->setGameOver(true);
                                        retorno[0] = constantes::tipoEntidades::GAME_OVER;
                                        Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesFuegos", "El personaje " + to_string(id) + "esta Game Over");
                                    }
                                }
                            }

                        }
                        return retorno;
                }
            }
        }
    }
    return retorno;
}


vector<int> Escenario::mirarColisionesEnemigosForEnemigos(int nbCliente, bool imortal) {
    vector<int> retorno = {-1};
    bool primeraCol = true;
    int finEscu = constantes::tipoEntidades::FIN_ESCUDO;
    int perd = constantes::tipoEntidades::PERDIENDO;
    int vida = constantes::tipoPerdido::VIDA;
    int anillos = constantes::tipoPerdido::ANILLOS;
    int gameOver = constantes::tipoEntidades::GAME_OVER;
    for (int j = 0; j < nbCliente; j++) {
        if (personajes[j]->isConnectado()) {
            if (!personajes[j]->isEnMovimiento()) {
                for (int i = 0; i < this->enemigos.size(); i++) {
                    if (personajes[j]->getCoordenadaX() + personajes[j]->getAncho() >
                        enemigos[i]->getCoordenadaX() + this->getXCamara() &&
                        personajes[j]->getCoordenadaX() <
                        enemigos[i]->getCoordenadaX() + enemigos[i]->getAncho() + this->getXCamara()) {
                        if (enemigos[i]->getCoordenadaY() + enemigos[i]->getAlto() >
                            personajes[j]->getCoordenadaY() &&
                            enemigos[i]->getCoordenadaY() <
                            personajes[j]->getCoordenadaY() + personajes[j]->getAlto()) {
                            if (personajes[j]->getIdEnemigo() != i) {
                                if (primeraCol) {
                                    retorno.pop_back();
                                    primeraCol = false;
                                }

                                if (!personajes[j]->isSaltando()) {
                                    if (!imortal) {
                                        if (personajes[j]->getVida() > 0) {
                                            if (!personajes[j]->isInvincible()) {
                                                personajes[j]->setIdEnemigo(i);
                                                if (personajes[j]->isEscudo()) {
                                                    personajes[j]->setInvincible(true);
                                                    personajes[j]->setEscudo(false);
                                                    retorno.push_back(j);
                                                    retorno.push_back(finEscu);
                                                    retorno.push_back(-1);
                                                } else {
                                                    if (personajes[j]->getAnillos() != 0) {
                                                        personajes[j]->setAnillos(0);
                                                        personajes[j]->setInvincible(true);
                                                        retorno.push_back(j);
                                                        retorno.push_back(perd);
                                                        retorno.push_back(anillos);
                                                    } else {
                                                        retorno.push_back(j);
                                                        retorno.push_back(perd);
                                                        retorno.push_back(vida);
                                                        personajes[j]->setInvincible(true);
                                                        personajes[j]->setVida(personajes[j]->getVida() - 1);
                                                        Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigosForEnemigos", "El personaje " + to_string(j) + " tiene "+ to_string(personajes[j]->getVida()) + " vidas");
                                                    }
                                                }

                                                if (personajes[j]->getVida() == 0) {
                                                    personajes[j]->setGameOver(true);
                                                    retorno.push_back(j);
                                                    retorno.push_back(gameOver);
                                                    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigosForEnemigos", "El personaje " + to_string(j) + " esta game over");
                                                }
                                            }
                                        }
                                    }
                                } else {
                                    if (enemigos[i]->isVivo()) {
                                        enemigos[i]->setVivo(false);
                                        personajes[j]->setPuntos(
                                                personajes[j]->getPuntos() + enemigos[i]->getPuntos());
                                    }
                                    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigosForEnemigos", "El personaje " + to_string(j) + " tiene " + to_string(personajes[j]->getPuntos()) + " puntos");
                                    return retorno;
                                }
                            }
                        } else {

                            if (personajes[j]->getIdEnemigo() == i) {
                                personajes[j]->setIdEnemigo(-1);
                            }
                        }
                    } else {
                        if (personajes[j]->getIdEnemigo() == i) {
                            personajes[j]->setIdEnemigo(-1);
                        }
                    }
                }
            }
        }
    }
    return retorno;
}


vector<int> Escenario::mirarColisionesFuegosForFuegos(int nbCliente, bool imortal) {
    vector<int> retorno = {-1};
    bool primeraCol = true;
    int finEscu = constantes::tipoEntidades::FIN_ESCUDO;
    int perd = constantes::tipoEntidades::PERDIENDO;
    int vida = constantes::tipoPerdido::VIDA;
    int anillos = constantes::tipoPerdido::ANILLOS;
    int gameOver = constantes::tipoEntidades::GAME_OVER;
    for (int j = 0; j < nbCliente; j++) {
        if (personajes[j]->isConnectado()) {
            if (!personajes[j]->isEnMovimiento()) {
                for (int i = 0; i < this->fuegos.size(); i++) {
                    if (personajes[j]->getCoordenadaX() + personajes[j]->getAncho() >
                        fuegos[i]->getCoordenadaX() + this->getXCamara() &&
                        personajes[j]->getCoordenadaX() <
                        fuegos[i]->getCoordenadaX() + fuegos[i]->getAncho() + this->getXCamara()) {
                        if (fuegos[i]->getCoordenadaY() + fuegos[i]->getAlto() >
                            personajes[j]->getCoordenadaY() &&
                            fuegos[i]->getCoordenadaY() <
                            personajes[j]->getCoordenadaY() + personajes[j]->getAlto()) {
                            if (primeraCol) {
                                retorno.pop_back();
                                primeraCol = false;
                            }
                            if (!personajes[j]->isSaltando()) {
                                if (!imortal) {
                                    if (personajes[j]->getVida() > 0) {
                                        if (!personajes[j]->isInvincible()) {
                                            fuegos[i]->setVivo(false);
                                            fuegos[i]->setCoordenadaY(2000);
                                            fuegos[i]->setCoordenadaX(100);
                                            if (personajes[j]->isEscudo()) {
                                                personajes[j]->setInvincible(true);
                                                personajes[j]->setEscudo(false);
                                                retorno.push_back(j);
                                                retorno.push_back(finEscu);
                                                retorno.push_back(-1);
                                            } else {
                                                if (personajes[j]->getAnillos() != 0) {
                                                    personajes[j]->setAnillos(0);
                                                    personajes[j]->setInvincible(true);
                                                    retorno.push_back(j);
                                                    retorno.push_back(perd);
                                                    retorno.push_back(anillos);
                                                } else {
                                                    retorno.push_back(j);
                                                    retorno.push_back(perd);
                                                    retorno.push_back(vida);
                                                    personajes[j]->setInvincible(true);
                                                    personajes[j]->setVida(personajes[j]->getVida() - 1);
                                                    Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigosForFuegos", "El personaje " + to_string(j) + " tiene "+ to_string(personajes[j]->getVida()) + " vidas");
                                                }
                                            }

                                            if (personajes[j]->getVida() == 0) {
                                                personajes[j]->setGameOver(true);
                                                retorno.push_back(j);
                                                retorno.push_back(gameOver);
                                                Log::loguear(constantes::logEnumModo::MEDIO, "Escenario::mirarColisionesEnemigosForFuegos", "El personaje " + to_string(j) + " esta game over ");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retorno;
}

Entidad *Escenario::getEntitad(int id) {
    return this->entidades[id];
}

const vector<Entidad *> &Escenario::getEntidades() const {
    return entidades;
}

EnemigoFinal *Escenario::getEnemigoFinal() const {
    return enemigoFinal;
}

const vector<Fuego *> &Escenario::getFuegos() const {
    return fuegos;
}

void Escenario::setFuegos(const vector<Fuego *> &fuegos) {
    Escenario::fuegos = fuegos;
}


void cambiarVidaPuntos(int id, int vida, int puntos) {

}