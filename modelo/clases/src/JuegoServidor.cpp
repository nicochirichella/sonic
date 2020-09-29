//
// Created by nicolas on 26/04/17.
//

#include "../headers/JuegoServidor.h"
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <unistd.h>

using namespace std;


JuegoServidor::JuegoServidor(int puerto, int maximasConexiones, int modo) {

    Parser *jsonParser = new Parser();

    /*TODO Configurar el log al iniciar*/
    //jsonParser.configurarLog();
    this->ventana = jsonParser->getVentana();
    this->configuracion = jsonParser->getConfiguracion();
    this->niveles = jsonParser->getNiveles();
    for (int i = 0; i < this->niveles.size(); i++) {
        this->niveles[i]->getEscenario()->setPersonajes(jsonParser->getPersonajes());
    }

    //Random
    randomizarEntidadesEnemigos();
    char buffer[2048];
    crearMensajeRandomizar(buffer);


    // Seteos para servidor
    this->puerto = puerto;
    this->cantidadMaximaConexiones = maximasConexiones;
    for (int i = 0; i < maximasConexiones; i++) {
        vector<int> cliente;
        // Seteo el id
        cliente.push_back(i);
        // Seteo el socketCliente en -1 (significa que el id no esta siendo usado)
        cliente.push_back(-1);
        this->vectorClientes.push_back(cliente);
    }
    this->modoDeJuego = modo;
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::Constructor", "JuegoServidor creado.");
}

void JuegoServidor::randomizarEntidadesEnemigos() {
    srand(time(NULL));
    Log::loguear(constantes::logEnumModo::BAJO, "RANDOMIZARENTIDADESENIMEGOS", "Inicio");
    for (int i = 0; i < this->niveles.size(); i++) {
        //cout << "NIVEL: " << i+1 << endl;
        Escenario *escenario = niveles[i]->getEscenario();

        vector<Entidad *> entidades = escenario->getEntidades();
        vector<Enemigo *> enemigos = escenario->getEnemigos();

        vector<Entidad *> entidadesCopia;
        vector<Enemigo *> enemigosCopia;

        //En el json las entidades vienen ordenadas segun PIEDRA, ANILLOS, PINCHES, BONUS
        // y los enemigos PECES, MOSCAS, CANGREJOS.

//        cout<<"////////CANTIDADES TOTALES//////////////"<<endl;
//        cout<<"cantidad total piedras: "<< this->niveles[i]->getCantTotalPiedras()<<endl;
//        cout<<"cantidad total anillos: "<< this->niveles[i]->getCantTotalAnillos()<<endl;
//        cout<<"cantidad total pinches: "<< this->niveles[i]->getCantTotalPinches()<<endl;
//        cout<<"cantidad total peces : "<< this->niveles[i]->getCantTotalPeces()<<endl;
//        cout<<"cantidad total cangrejo: "<< this->niveles[i]->getCantTotalCangrejos()<<endl;
//        cout<<"cantidad total moscas: "<< this->niveles[i]->getCantTotalMoscas()<<endl;
//
//        cout<<"////////CANTIDADES MAXIMAS//////////////"<<endl;
//        cout<<"cantidad MAXIMA piedras: "<< this->niveles[i]->getCantMaxPiedras()<<endl;
//        cout<<"cantidad MAXIMA anillos: "<< this->niveles[i]->getCantMaxAnillos()<<endl;
//        cout<<"cantidad MAXIMA pinches: "<< this->niveles[i]->getCantMaxPinches()<<endl;
//        cout<<"cantidad MAXIMA peces : "<< this->niveles[i]->getCantMaxPeces()<<endl;
//        cout<<"cantidad MAXIMA cangrejo: "<< this->niveles[i]->getCantMaxCangrejos()<<endl;
//        cout<<"cantidad MAXIMA moscas: "<< this->niveles[i]->getCantMaxMoscas()<<endl;

        int maximoAnterior = 0;
        int maximoActual = this->niveles[i]->getCantTotalPiedras() - 1;
        // Piedras
        int cantidadPiedras = random(this->niveles[i]->getCantMinPiedras(), this->niveles[i]->getCantMaxPiedras());
        //int cantidadPiedras  = 1;
        //cout << "RANDOM PIEDRA: " << cantidadPiedras << endl;
        for (int j = 0; j < cantidadPiedras; j++) {
            // Desde el inicio del vector hasta la ultima piedra
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
            //cout << "RANDOM PIEDRA INDICE: " << indice << endl;

        }
        for (int l = 0; l < entidadesCopia.size(); l++) {
            //cout << "ENTIDAD COPIA: " << entidadesCopia[l]->getId() << endl;
        }
        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalAnillos() - 1 + maximoAnterior;

        // Anillos
        int cantidadAnillos = random(this->niveles[i]->getCantMinAnillos(), this->niveles[i]->getCantMaxAnillos());
        //cout << "RANDOM ANILLOS: " << cantidadAnillos << endl;
        for (int j = 0; j < cantidadAnillos; j++) {
            // Desde primer anillo del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
            //cout << "RANDOM ANILLOS: " << indice << endl;
        }
        for (int l = 0; l < entidadesCopia.size(); l++) {
            //cout << "ENTIDAD COPIA: " << entidadesCopia[l]->getId() << endl;
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalPinches() - 1 + maximoAnterior;

        // PINCHES
        int cantidadPinches = random(this->niveles[i]->getCantMinPinches(), this->niveles[i]->getCantMaxPinches());
        //cantidadPinches = 0;
        for (int j = 0; j < cantidadPinches; j++) {
            // Desde primer pinche del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalInvincibilidad() - 1 + maximoAnterior;

        // Invincibilidad
        int cantidadInvincibilidad = random(this->niveles[i]->getCantMinInvincibilidad(),
                                            this->niveles[i]->getCantMaxInvincibilidad());
        for (int j = 0; j < cantidadInvincibilidad; j++) {
            // Desde primer Invincibilidad del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalEscudo() - 1 + maximoAnterior;

        // Escudo
        int cantidadEscudo = random(this->niveles[i]->getCantMinEscudo(), this->niveles[i]->getCantMaxEscudo());
        for (int j = 0; j < cantidadEscudo; j++) {
            // Desde primer Invincibilidad del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalBonusAnillo() - 1 + maximoAnterior;

        // Escudo
        int cantidadBonusAnillo = random(this->niveles[i]->getCantMinBonusAnillo(),
                                         this->niveles[i]->getCantMaxBonusAnillo());
        for (int j = 0; j < cantidadBonusAnillo; j++) {
            // Desde primer Invincibilidad del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < entidadesCopia.size(); k++) {
                if (entidadesCopia[k]->getId() == entidades[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                entidadesCopia.push_back(entidades[indice]);
            }
        }

        maximoAnterior = 0;
        maximoActual = this->niveles[i]->getCantTotalPeces() - 1;
        // Pez
        int cantidadPez = random(this->niveles[i]->getCantMinPeces(), this->niveles[i]->getCantMaxPeces());
        //int cantidadPez = 1;
        for (int j = 0; j < cantidadPez; j++) {
            // Desde el inicio del vector hasta el ultimo pez
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < enemigosCopia.size(); k++) {
                if (enemigosCopia[k]->getId() == enemigos[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                enemigosCopia.push_back(enemigos[indice]);
            }
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalMoscas() - 1 + maximoAnterior;

        // Mosca
        int cantidadMosca = random(this->niveles[i]->getCantMinMoscas(), this->niveles[i]->getCantMaxMoscas());
        for (int j = 0; j < cantidadMosca; j++) {
            // Desde la primera mosca del vector hasta la ultima
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < enemigosCopia.size(); k++) {
                if (enemigosCopia[k]->getId() == enemigos[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                enemigosCopia.push_back(enemigos[indice]);
            }
        }

        maximoAnterior = maximoActual + 1;
        maximoActual = this->niveles[i]->getCantTotalCangrejos() - 1 + maximoAnterior;

        // Cangrejo
        int cantidadCangrejo = random(this->niveles[i]->getCantMinCangrejos(), this->niveles[i]->getCantMaxCangrejos());
        for (int j = 0; j < cantidadCangrejo; j++) {
            // Desde el primer cangrejo del vector hasta el ultimo
            int indice = random(maximoAnterior, maximoActual);
            bool repetido = false;

            for (int k = 0; k < enemigosCopia.size(); k++) {
                if (enemigosCopia[k]->getId() == enemigos[indice]->getId()) {
                    repetido = true;
                    j--;
                    break;
                }
            }

            if (!repetido) {
                enemigosCopia.push_back(enemigos[indice]);
            }
        }
        escenario->setEnemigos(enemigosCopia);
        escenario->setEntidades(entidadesCopia);
    }
    Log::loguear(constantes::logEnumModo::BAJO, "RANDOMIZARENTIDADESENIMEGOS", "Salgo");


}

int JuegoServidor::random(int min, int max) {
    int random = min + (rand() % (max - min + 1));
    //Log::loguear(constantes::logEnumModo::BAJO, "RANDOM", to_string(random));
    return random;
}

void JuegoServidor::loopNuevosClientes() {
    while (true) {
        //Escuchar Nuevos Clientes
    }
}

void JuegoServidor::cerrarJuegoServidor() {
    const std::string CONTEXTO = "JuegoServidor::cerrarJuegoServidor";
    //Cerrar conexiones, etc.

}

void JuegoServidor::actualizarEstado(char *bufferLectura, char *bufferSend, int idCliente) {
    const std::string CONTEXTO = "JuegoServidor::actualizarEstado";
    // Parseo
    int eventoCliente = atoi(bufferLectura);

    Escenario *escenario = this->niveles[nivelActual]->getEscenario();
    Personaje *personaje = escenario->getPersonaje(idCliente);
    char buffer[2048];
    int bufferSize = 2048;
    int velocidadMaxima = 40;
    switch (eventoCliente) {
        case constantes::eventoEnumCliente::ESCUCHAR_LATIDO:
            if (!isCambiandoNivel()) {
                this->crearMensajeLatido(bufferSend);
                Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Recibi un pedido de latido");

                int socket = getSocketCliente(idCliente);
                int bytesEnviados = enviar(socket, bufferSend, bufferSize);

            }
            return;
        case constantes::eventoEnumCliente::EQUIPO1:
            this->equipo1.push_back(idCliente);
            personaje->setEquipo(1);
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "personaje " + to_string(idCliente) + " en equipo 1");
            contadorParaEquipos++;
            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketCliente = this->getSocketCliente(i);
                memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
                string bufferString =
                        convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO) + "," +
                        to_string(1) + "," + to_string(idCliente) + ",z";
                strcpy(buffer, bufferString.c_str());
                int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
            }
            return;
        case constantes::eventoEnumCliente::EQUIPO2:
            this->equipo2.push_back(idCliente);
            personaje->setEquipo(2);
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "personaje " + to_string(idCliente) + " en equipo 2");
            contadorParaEquipos++;
            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketCliente = this->getSocketCliente(i);
                memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
                string bufferString =
                        convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO) + "," +
                        to_string(2) + "," + to_string(idCliente) + ",z";
                strcpy(buffer, bufferString.c_str());
                int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
            }
            return;
        case constantes::eventoEnumCliente::NO_EQUIPO:
            this->equipo1.push_back(idCliente);

            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                         "personaje " + to_string(idCliente) + " deconectado antes de elegir equipo");
            contadorParaEquipos++;

            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketCliente = this->getSocketCliente(i);
                memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
                string bufferString =
                        convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO) + "," +
                        to_string(1) + "," + to_string(idCliente) + ",z";
                strcpy(buffer, bufferString.c_str());
                int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
            }
            return;
        case constantes::eventoEnumCliente::CAMBIO_NIVEL_HECHO:
            contadorParaCambioNivel++;
            if (contadorParaCambioNivel == cantidadMaximaConexiones - contadorDesconectado) {
                contadorParaCambioNivel = 0;
                mensajeNuevoNivel = true;
                for (int i = 0; i < this->vectorClientes.size(); i++) {
                    int socketCliente = this->getSocketCliente(i);
                    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
                    string bufferString =
                            convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::CAMBIO_NIVEL_LISTO);
                    strcpy(buffer, bufferString.c_str());
                    int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
                }
                mensajeNuevoNivel = false;
                usleep(5000);
                memset(bufferSend, 0, sizeof(bufferSend)); // borro el contenido del buffer
                this->crearMensajeEstado(bufferSend);
                for (int i = 0; i < this->vectorClientes.size(); i++) {
                    int socketEnviar = vectorClientes[i][1];
                    if (socketEnviar == -1) { continue; }
                    int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                }
            }
            return;
        case constantes::eventoEnumCliente::TECLA_IZQUIERDA:
            if (!personaje->isModoAtaque()) {
                personaje->setEnMovimiento(true);
                personaje->setMiraDerecha(false);
                personaje->setArrastrable(false);
                if (personaje->getFrameRetroceder() < 10 * 2) {
                    personaje->setVelocidadPersonaje(20);
                } else {
                    if (personaje->getVelocidadPersonaje() < velocidadMaxima) {
                        personaje->setVelocidadPersonaje(personaje->getVelocidadPersonaje() + 1);
                    } else {
                        personaje->setVelocidadPersonaje(velocidadMaxima);
                    }
                }

                if (personaje->getFrameRetroceder() / 3 >= 13) {
                    personaje->setFrameRetroceder(11 * 3);
                }

                escenario->teclaIzquierda(idCliente, personaje->getVelocidadPersonaje(), this->ventana->getAncho());
                personaje->setFrameRetroceder(personaje->getFrameRetroceder() + 1);

                if (personaje->getFrameRetroceder() == 1) {
                    personaje->setIndexSprite(1);
                } else {
                    personaje->setIndexSprite(personaje->getFrameRetroceder() / 3);
                }
                //Si estoy saltando no quiero cambiar el estado
                if (personaje->getEstado() != constantes::eventoEnumCliente::TECLA_ARRIBA) {
                    personaje->setEstado(constantes::eventoEnumCliente::TECLA_IZQUIERDA);
                }
                this->setTodasColisiones(idCliente, constantes::eventoEnumCliente::TECLA_IZQUIERDA,
                                         this->imortal, this->ventana->getAncho());

            }
            break;
        case constantes::eventoEnumCliente::TECLA_DERECHA:
            if (!personaje->isModoAtaque()) {
                personaje->setEnMovimiento(true);
                personaje->setMiraDerecha(true);
                personaje->setArrastrable(false);


                if (personaje->getFrameAvanzar() < 10 * 2) {
                    personaje->setVelocidadPersonaje(20);
                } else {
                    if (personaje->getVelocidadPersonaje() < velocidadMaxima) {
                        personaje->setVelocidadPersonaje(personaje->getVelocidadPersonaje() + 1);
                    } else {
                        personaje->setVelocidadPersonaje(velocidadMaxima);
                    }

                }
                if (personaje->getFrameAvanzar() / 3 >= 13) {
                    personaje->setFrameAvanzar(11 * 3);
                }
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                             "Frame:" + to_string(personaje->getFrameAvanzar()));
                escenario->teclaDerecha(idCliente, personaje->getVelocidadPersonaje(), this->ventana->getAncho());
                personaje->setFrameAvanzar(personaje->getFrameAvanzar() + 1);


                if (personaje->getFrameAvanzar() == 1) {
                    personaje->setIndexSprite(1);
                } else {
                    personaje->setIndexSprite(personaje->getFrameAvanzar() / 3);
                }

                //Si estoy saltando no quiero cambiar el estado
                if (personaje->getEstado() != constantes::eventoEnumCliente::TECLA_ARRIBA) {
                    personaje->setEstado(constantes::eventoEnumCliente::TECLA_DERECHA);
                }
                this->setTodasColisiones(idCliente, constantes::eventoEnumCliente::TECLA_DERECHA,
                                         this->imortal, this->ventana->getAncho());

            }
            break;
        case constantes::eventoEnumCliente::TECLA_ARRIBA:
            if (!personaje->isModoAtaque() && !personaje->isSaltando()) {

                personaje->setEnMovimiento(true);
                // Si estoy saltando no quiero iniciar un nuevo thread
                personaje->setArrastrable(true);


                if (personaje->getEstado() != constantes::eventoEnumCliente::TECLA_ARRIBA) {
                    personaje->setEstado(constantes::eventoEnumCliente::TECLA_ARRIBA);
                    thread miThread(&JuegoServidor::threadEstoySaltando, this, idCliente);
                    miThread.detach();
                }
            }

            break;
        case constantes::eventoEnumCliente::TECLA_IMORTAL:
            if (!lockImortal) {
                if (this->imortal) {
                    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::actualizarEstado",
                                 "Personajes no mas imortales ");
                    this->imortal = false;
                } else {
                    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::actualizarEstado",
                                 "Personajes imortales ");
                    this->imortal = true;
                }
                lockImortal = true;
                thread ImorThread(&JuegoServidor::threadImortalidad, this);
                ImorThread.detach();

            }
            break;

        case constantes::eventoEnumCliente::SPIN_ATACK:
            if (!personaje->isModoAtaque()) {
                personaje->setEnMovimiento(true);
                personaje->setArrastrable(false);
                if ((!personaje->isModoAtaque()) &&
                    (personaje->getEstado() != constantes::eventoEnumCliente::TECLA_ARRIBA)) {
                    personaje->setEstado(constantes::eventoEnumCliente::SPIN_ATACK);
                    personaje->setModoAtaque(true);
                    thread miThread(&JuegoServidor::threadSpinAtack, this, idCliente);
                    miThread.detach();
                }
            }
            break;

        case constantes::eventoEnumCliente::ESCAPE:
            if (persoDesconectado[idCliente] == true) {
                return;
            }
            persoDesconectado[idCliente] = true;
            contadorDesconectado++;
            Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::actualizarEstado",
                         "Perdi la conexion con el cliente id: " + to_string(idCliente));
            for (int i = 0; i < this->niveles.size(); i++) {
                this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setConnectado(false);
                this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setMargenDerecha(false);
                this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setMargenIzquierda(false);
                this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setEstado(
                        constantes::eventoEnumCliente::TECLA_DERECHA);
                this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setIndexSprite(14);
            }

            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                         "Numero clientes desconectados: " + to_string(contadorDesconectado));
            this->liberarIdCliente(idCliente);

            break;
        case constantes::eventoEnumCliente::QUIETO:
            if (personaje->getEstado() != constantes::eventoEnumCliente::TECLA_ARRIBA) {
                personaje->setFrameSaltar(0);
            }
            personaje->setEnMovimiento(false);
            personaje->setArrastrable(true);
            personaje->setFrameAvanzar(0);
            personaje->setFrameRetroceder(0);
            personaje->setIndexSprite(0);
            break;
    }
    this->manejarColisiones(idCliente);
    this->crearMensajeEstado(bufferSend);
    personaje->setEnMovimiento(false);
}


void JuegoServidor::manejarColisiones(int idCliente) {
    if (this->getVectorColision()[idCliente][0] != -1) {
        int bufferSize = 2048;
        char bufferReceive[bufferSize];
        string bufferString;
        string tipoMensaje;
        int bytesEnviados;
        if (this->getVectorColision()[idCliente][0] == constantes::tipoEntidades::GAME_OVER) {
            contadorGameOver++;
            jugadorGameOver[idCliente] = true;
            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
            bufferString = tipoMensaje + "," + to_string(idCliente) + ",z";
            if (contadorGameOver == cantidadMaximaConexiones) {
                finDeJuego = true;
                iniciarClienteParaGameOver();
            }
        } else {
            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
            bufferString = tipoMensaje + "," + to_string(idCliente) + "," +
                           to_string(this->getVectorColision()[idCliente][0]) + "," +
                           to_string(this->getVectorColision()[idCliente][1]) + "," + "z";
        }
        strcpy(bufferReceive, bufferString.c_str());

        for (int i = 0; i < this->vectorClientes.size(); i++) {
            int socketEnviar = vectorClientes[i][1];
            if (socketEnviar == -1) { continue; }
            bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
        }

        if (this->getVectorColision()[idCliente][0] == constantes::tipoEntidades::INVICIBILIDAD) {
            thread miThread(&JuegoServidor::threadInvincibilidad, this, idCliente);
            miThread.detach();
        }
        if (this->getVectorColision()[idCliente][0] == constantes::tipoEntidades::PERDIENDO ||
            this->getVectorColision()[idCliente][0] == constantes::tipoEntidades::FIN_ESCUDO) {
            thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this, idCliente);
            miSecondThread.detach();
        }
        this->setColision(idCliente, {-1, -1});
        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarColisiones", bufferReceive);
    }

    if (this->getVectorColisionEnemigos()[idCliente][0] != -1) {
        int bufferSize = 2048;
        char bufferReceive[bufferSize];
        string bufferString;
        string tipoMensaje;
        int bytesEnviados;
        if (this->getVectorColisionEnemigos()[idCliente][0] == constantes::tipoEntidades::GAME_OVER) {
            contadorGameOver++;
            jugadorGameOver[idCliente] = true;
            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
            bufferString = tipoMensaje + "," + to_string(idCliente) + ",z";
            if (contadorGameOver == cantidadMaximaConexiones) {
                finDeJuego = true;
                iniciarClienteParaGameOver();
            }
        } else {
            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
            bufferString =
                    tipoMensaje + "," + to_string(idCliente) + "," +
                    to_string(this->getVectorColisionEnemigos()[idCliente][0]) + "," +
                    to_string(this->getVectorColisionEnemigos()[idCliente][1]) + "," + "z";
        }
        strcpy(bufferReceive, bufferString.c_str());
        for (int i = 0; i < this->vectorClientes.size(); i++) {
            int socketEnviar = vectorClientes[i][1];
            if (socketEnviar == -1) { continue; }
            bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
        }
        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarColisiones   Enemigos", bufferReceive);
        if (this->getVectorColisionEnemigos()[idCliente][0] == constantes::tipoEntidades::PERDIENDO ||
            this->getVectorColisionEnemigos()[idCliente][0] == constantes::tipoEntidades::FIN_ESCUDO) {
            thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this, idCliente);
            miSecondThread.detach();
        }
        this->setColisionEnemigos(idCliente, {-1, -1});
    }
    if (nivelActual == 2) {
        if (this->getVectorColisionEnemigoFinal()[idCliente][0] != -1) {
            int bufferSize = 2048;
            char bufferReceive[bufferSize];
            string bufferString;
            string tipoMensaje;
            int bytesEnviados;
            if (this->getVectorColisionEnemigoFinal()[idCliente][0] == constantes::tipoEntidades::GAME_OVER) {
                contadorGameOver++;
                jugadorGameOver[idCliente] = true;
                tipoMensaje = convertirTipoMensajeEnString(
                        constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
                bufferString = tipoMensaje + "," + to_string(idCliente) + ",z";
                if (contadorGameOver == cantidadMaximaConexiones) {
                    finDeJuego = true;
                    iniciarClienteParaGameOver();
                }
            } else if (this->getVectorColisionEnemigoFinal()[idCliente][0] == constantes::tipoEntidades::VICTORY) {
                Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarColisiones",
                             "El enemigo final fue matado ");
                usleep(1400000);
                finDeJuego = true;
                iniciarClienteParaGameOver();

            } else if (this->getVectorColisionEnemigoFinal()[idCliente][0] ==
                       constantes::tipoEntidades::ENEMIGO_FINAL_INVINCIBLE) {
                thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVidaEnemigoFinal, this);
                miSecondThread.detach();
            } else {
                tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
                bufferString =
                        tipoMensaje + "," + to_string(idCliente) + "," +
                        to_string(this->getVectorColisionEnemigoFinal()[idCliente][0]) + "," +
                        to_string(this->getVectorColisionEnemigoFinal()[idCliente][1]) + "," + "z";
            }
            strcpy(bufferReceive, bufferString.c_str());
            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketEnviar = vectorClientes[i][1];
                if (socketEnviar == -1) { continue; }
                bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
            }
            Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarColisiones   Enemigo Final",
                         bufferReceive);
            if (this->getVectorColisionEnemigoFinal()[idCliente][0] == constantes::tipoEntidades::PERDIENDO ||
                this->getVectorColisionEnemigoFinal()[idCliente][0] == constantes::tipoEntidades::FIN_ESCUDO) {
                thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this, idCliente);
                miSecondThread.detach();
            }

            this->setColisionEnemigoFinal(idCliente, {-1, -1});
        }


        if (this->getVectorColisionFuegos()[idCliente][0] != -1) {
            int bufferSize = 2048;
            char bufferReceive[bufferSize];
            string bufferString;
            string tipoMensaje;
            int bytesEnviados;
            if (this->getVectorColisionFuegos()[idCliente][0] == constantes::tipoEntidades::GAME_OVER) {
                contadorGameOver++;
                jugadorGameOver[idCliente] = true;
                tipoMensaje = convertirTipoMensajeEnString(
                        constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
                bufferString = tipoMensaje + "," + to_string(idCliente) + ",z";
                if (contadorGameOver == cantidadMaximaConexiones) {
                    finDeJuego = true;
                    iniciarClienteParaGameOver();
                }
            } else {
                tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
                bufferString =
                        tipoMensaje + "," + to_string(idCliente) + "," +
                        to_string(this->getVectorColisionFuegos()[idCliente][0]) + "," +
                        to_string(this->getVectorColisionFuegos()[idCliente][1]) + "," + "z";
            }
            strcpy(bufferReceive, bufferString.c_str());
            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketEnviar = vectorClientes[i][1];
                if (socketEnviar == -1) { continue; }
                bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
            }
            Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarColisiones   Fuegos", bufferReceive);
            if (this->getVectorColisionFuegos()[idCliente][0] == constantes::tipoEntidades::PERDIENDO ||
                this->getVectorColisionFuegos()[idCliente][0] == constantes::tipoEntidades::FIN_ESCUDO) {
                thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this, idCliente);
                miSecondThread.detach();
            }
            this->setColisionFuegos(idCliente, {-1, -1});
        }
    }

}

void JuegoServidor::crearMensajeLatido(char *buffer) {
    string mensajeString = std::to_string(constantes::eventoEnumCliente::ESCUCHAR_LATIDO) + ",z";
    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());
}

void JuegoServidor::threadEstoySaltando(int idCliente) {
    bool estaSaltando = true;
    Personaje *personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente);
    personaje->setSaltando(true);
    bool gameOff = false;
    while (estaSaltando) {
        if (!cambiandoNivel) {
            if (personaje->getFrameSaltar() / 3 >= 9) {
                personaje->setFrameSaltar(6 * 3);
            }
            this->niveles[nivelActual]->getEscenario()->teclaArriba(idCliente,
                                                                    this->configuracion->getVelocidadScroll(),
                                                                    this->ventana->getAncho(),
                                                                    personaje->getFrameSaltar(),
                                                                    estaSaltando);
            this->setTodasColisiones(idCliente,
                                     constantes::eventoEnumCliente::TECLA_ARRIBA,
                                     this->imortal,
                                     this->ventana->getAncho());

            personaje->setIndexSprite(personaje->getFrameSaltar() / 3);
            personaje->setFrameSaltar(personaje->getFrameSaltar() + 1);

            /**** Enviar ****/
            int bufferSize = 2048;
            char bufferSend[bufferSize];
            char bufferReceive[bufferSize];
            this->crearMensajeEstadoSaltar(bufferSend, idCliente);
            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketEnviar = this->vectorClientes[i][1];
                if (socketEnviar == -1) { continue; }
                int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
            }
            this->manejarColisiones(idCliente);

            unsigned int microsegundos = 20000; //1000 / 16 (16 ciclos hace el while)
            usleep(microsegundos);
            if (gameOff) {
                return;
            }
        } else { return; }

    }
    // Ya no salta mas
    personaje->setSaltando(false);
    personaje->setFrameSaltar(0);
    if (personaje->isMiraDerecha()) {
        personaje->setEstado(constantes::eventoEnumCliente::TECLA_DERECHA);
    } else {
        personaje->setEstado(constantes::eventoEnumCliente::TECLA_IZQUIERDA);
    }

    int bufferSize = 2048;
    char bufferSend[bufferSize];
    char bufferReceive[bufferSize];
    this->crearMensajeEstadoSaltar(bufferSend, idCliente);
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        int socketEnviar = this->vectorClientes[i][1];
        if (socketEnviar == -1) { continue; }
        int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
    }
}

void JuegoServidor::setTodasColisiones(int idCliente, int tecla, bool imortal, int ventanaAncho) {
    this->setColision(idCliente, this->niveles[nivelActual]->getEscenario()->mirarColisiones(idCliente, tecla, imortal,
                                                                                             ventanaAncho));
    this->setColisionEnemigos(idCliente,
                              this->niveles[nivelActual]->getEscenario()->mirarColisionesEnemigos(idCliente, tecla,
                                                                                                  imortal));
    if (nivelActual == 2) {
        this->setColisionEnemigoFinal(idCliente,
                                      this->niveles[nivelActual]->getEscenario()->mirarColisionesEnemigoFinal(idCliente,
                                                                                                              tecla,
                                                                                                              imortal));
        this->setColisionFuegos(idCliente,
                                this->niveles[nivelActual]->getEscenario()->mirarColisionesFuegos(idCliente, tecla,
                                                                                                  imortal));
    }
}


void JuegoServidor::crearMensajeEstadoSaltar(char *buffer, int idCliente) {
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();

    std::string xCamara = std::to_string(escenario->getXCamara());
    std::string yCamara = std::to_string(escenario->getYCamara());
    std::string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::ACTUALIZA_VISTA);
    Personaje *personaje = escenario->getPersonaje(idCliente);
    std::string mensajeString = tipoMensaje + "," + xCamara + "," + yCamara + "," + std::to_string(idCliente) + "," +
                                std::to_string(personaje->getCoordenadaX()) + "," +
                                std::to_string(personaje->getCoordenadaY()) + "," +
                                std::to_string(personaje->getIndexSprite()) + "," +
                                std::to_string(personaje->getEstado()) + "," + std::to_string(personaje->getVida()) +
                                "," + std::to_string(personaje->getPuntos()) + "," +
                                std::to_string(personaje->getAnillos()) + ",z";
    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());
}

void JuegoServidor::crearMensajeEstado(char *buffer) {
    const std::string CONTEXTO = "JuegoServidor::crearMensajeEstado";
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();

    std::string xCamara = std::to_string(escenario->getXCamara());
    std::string yCamara = std::to_string(escenario->getYCamara());
    std::string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::ACTUALIZA_VISTA);
    std::string mensajeString = tipoMensaje + "," + xCamara + "," + yCamara + ",";

    for (int i = 0; i < cantidadMaximaConexiones; i++) {
        Personaje *personaje = escenario->getPersonaje(i);
        Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, "Estado de Conexion peronaje: " + to_string(i) + " es " +
                                                               to_string((int) personaje->isConnectado()));
        if (personaje->isGameOver()) {
            personaje->setCoordenadaX(-5000);
        }

        mensajeString += std::to_string(personaje->getId()) + "," + std::to_string(personaje->getCoordenadaX()) + "," +
                         std::to_string(personaje->getCoordenadaY()) + "," +
                         std::to_string(personaje->getIndexSprite()) + "," + std::to_string(personaje->getEstado()) +
                         "," + std::to_string(personaje->getVida()) + "," + std::to_string(personaje->getPuntos()) + ","
                         + std::to_string(personaje->getAnillos()) + ",";
    }
    mensajeString += "z";

    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());
    Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, buffer);
}

std::string JuegoServidor::convertirTipoMensajeEnString(int tipoMensaje) {
    std::string resultado = "";
    if (tipoMensaje < 10) {
        resultado = "0" + to_string(tipoMensaje);
    } else {
        resultado = to_string(tipoMensaje);
    }
    return resultado;
}

std::string JuegoServidor::convertirIdString(int id) {
    std::string resultado = "";
    if (id < 10) {
        resultado = "00" + to_string(id);
    } else if (id < 100) {
        resultado = "0" + to_string(id);
    } else {
        resultado = to_string(id);
    }
    return resultado;
}

void JuegoServidor::crearMensajeRandomizar(char *buffer) {
    const std::string CONTEXTO = "JuegoServidor::crearMensajeRandomizar";

    std::string tipoMensaje = convertirTipoMensajeEnString(
            constantes::tipoMensajeServidorCliente::RANDOMIZAR_ESCENARIO);
    std::string mensajeString = tipoMensaje;

    for (int i = 1; i <= this->niveles.size(); i++) {
        Escenario *escenario = this->niveles[i - 1]->getEscenario();
        vector<Entidad *> entidades = escenario->getEntidades();
        vector<Enemigo *> enemigos = escenario->getEnemigos();

        for (int j = 0; j < escenario->getEntidades().size(); j++) {
            //cout << "ID_ENTIDAD: "<< entidades[j]->getId() << endl;
            std::string idString = "," + convertirIdString(entidades[j]->getId());
            mensajeString += idString;
        }
        for (int j = 0; j < escenario->getEnemigos().size(); j++) {
            //cout << "ID_ENEMIGO: "<< enemigos[j]->getId() << endl;
            std::string idString = "," + convertirIdString(enemigos[j]->getId());
            mensajeString += idString;
        }
        mensajeString += ",n";
    }
    mensajeString += "z";

    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());

}

void JuegoServidor::manejarEnemigos() {
    const std::string CONTEXTO = "JuegoServidor::manejarEnemigos";
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                 "Inicio el thread para manejar los enemigos");
    int bufferSize = 2048;
    char bufferSend[bufferSize];
    char bufferSendEnemigoFinal[bufferSize];
    int bytesEnviados;
    string bufferString;
    int indexsprite = 0;
    int indexSpriteEnemigoFinal = 0;
    int indexSpriteMuerto = 0;
    string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);

    while (!finDeJuego) {
        if (!this->cambiandoNivel) {
            Escenario *escenario = this->niveles[nivelActual]->getEscenario();
            indexsprite = abs(indexsprite - 1);
            for (int i = 0; i < escenario->getEnemigos().size(); i++) {
                Enemigo *enemigo = escenario->getEnemigo(i);
                if (enemigo->isVivo()) {
                    if (enemigo->isDirecionIzquierda()) {
                        enemigo->setIndexSprite(abs(enemigo->getIndexSprite() - 1));
                    } else {
                        enemigo->setIndexSprite(2 + indexsprite);
                    }
                    enemigo->setNewX(escenario->getAncho());
                    enemigo->setNewY();
                } else {
                    enemigo->setCoordenadaY(800);
                }
            }
            if (nivelActual == 2) {
                EnemigoFinal *enemigoFinal = escenario->getEnemigoFinal();

                if (enemigoFinal->isVivo()) {
                    if (enemigoFinal->getCoordenadaY() == 2200) {
                        enemigoFinal->setCoordenadaY(enemigoFinal->getOriginalCoordenadaY());
                    }
                    if (enemigoFinal->isDirecionIzquierda()) {
                        if (enemigoFinal->isInvincible()) {
                            enemigoFinal->setIndexSprite(8);
                        } else {
                            enemigoFinal->setIndexSprite(indexSpriteEnemigoFinal);
                            if (indexSpriteEnemigoFinal < 3) {
                                indexSpriteEnemigoFinal++;
                            } else {
                                indexSpriteEnemigoFinal = 0;
                            }
                        }

                    } else {
                        if (enemigoFinal->isInvincible()) {
                            if (enemigoFinal->isDirecionIzquierda()) {
                                enemigoFinal->setIndexSprite(8);
                            } else {
                                enemigoFinal->setIndexSprite(9);
                            }
                        } else {
                            enemigoFinal->setIndexSprite(indexSpriteEnemigoFinal + 4);
                            if (indexSpriteEnemigoFinal < 3) {
                                indexSpriteEnemigoFinal++;
                            } else {
                                indexSpriteEnemigoFinal = 0;
                            }
                        }

                    }
                    if (!enemigoFinal->isInvincible()) {
                        enemigoFinal->setNewX(escenario->getAncho());
                    }
                    enemigoFinal->setNewY();
                    switch (enemigoFinal->getContadorFuego()) {
                        case -1:
                            break;
                        default:
                            escenario->getFuegos()[enemigoFinal->getContadorFuego()]->setVivo(true);
                            escenario->getFuegos()[enemigoFinal->getContadorFuego()]->setMovimientoBajando(true);
                            break;
                    }
                    for (int i = 0; i < escenario->getFuegos().size(); i++) {
                        if (escenario->getFuegos()[i]->isVivo()) {
                            escenario->getFuegos()[i]->setNewY(enemigoFinal->getCoordenadaY(),
                                                               enemigoFinal->getCoordenadaX(),
                                                               enemigoFinal->isDirecionIzquierda());
                            escenario->getFuegos()[i]->setNewX();
                            escenario->getFuegos()[i]->setIndexSprite(
                                    abs(escenario->getFuegos()[i]->getIndexSprite() - 1));
                        }
                    }
                } else if (enemigoFinal->getMuerto() > 0) {
                    enemigoFinal->setIndexSprite(10 + indexSpriteMuerto);
                    indexSpriteMuerto = abs(indexSpriteMuerto - 1);
                    enemigoFinal->setMuerto(enemigoFinal->getMuerto() - 1);
                } else {
                    enemigoFinal->setCoordenadaY(2000);
                }

            }

            this->colisionEnemigosForEnemigos = escenario->mirarColisionesEnemigosForEnemigos(
                    this->vectorClientes.size(), this->imortal);

            this->crearMensajeEnemigos(bufferSend);
            if (nivelActual == 2) {
                this->colisionFuegosForFuegos = escenario->mirarColisionesFuegosForFuegos(
                        this->vectorClientes.size(), this->imortal);

                this->crearMensajeEnemigoFinal(bufferSendEnemigoFinal);
            }

            for (int i = 0; i < this->vectorClientes.size(); i++) {
                int socketEnviar = vectorClientes[i][1];
                if (socketEnviar == -1) { continue; }
                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, bufferSend);
                bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                if (nivelActual == 2) {
                    usleep(5000);
                    bytesEnviados = enviar(socketEnviar, bufferSendEnemigoFinal, bufferSize);
                }
            }
            if (nivelActual == 2) {
                usleep(100000);
            } else {
                usleep(150000);
            }

            if (!cambiandoNivel) {
                if (this->colisionEnemigosForEnemigos[0] != -1) {
                    for (int j = 0; j < this->colisionEnemigosForEnemigos.size(); j = j + 3) {
                        if (this->colisionEnemigosForEnemigos[j + 1] == constantes::tipoEntidades::GAME_OVER) {
                            contadorGameOver++;
                            jugadorGameOver[this->colisionEnemigosForEnemigos[j]] = true;
                            tipoMensaje = convertirTipoMensajeEnString(
                                    constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
                            bufferString = tipoMensaje + "," + to_string(this->colisionEnemigosForEnemigos[j]) + ",z";
                            if (contadorGameOver == cantidadMaximaConexiones) {
                                finDeJuego = true;
                                iniciarClienteParaGameOver();
                            }
                        } else {
                            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
                            bufferString = tipoMensaje + "," + to_string(this->colisionEnemigosForEnemigos[j]) + "," +
                                           to_string(this->colisionEnemigosForEnemigos[j + 1]) + "," +
                                           to_string(this->colisionEnemigosForEnemigos[j + 2]) + "," + "z";
                        }
                        if (this->colisionEnemigosForEnemigos[j + 1] == constantes::tipoEntidades::PERDIENDO ||
                            this->colisionEnemigosForEnemigos[j + 1] == constantes::tipoEntidades::FIN_ESCUDO) {
                            thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this,
                                                  this->colisionEnemigosForEnemigos[j]);
                            miSecondThread.detach();
                        }

                        strcpy(bufferSend, bufferString.c_str());

                        for (int i = 0; i < this->vectorClientes.size(); i++) {
                            int socketEnviar = vectorClientes[i][1];
                            if (socketEnviar == -1) { continue; }
                            Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, bufferSend);
                            bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);

                        }
                    }
                }
                this->colisionEnemigosForEnemigos = {-1};
                memset(bufferSend, 0, sizeof(bufferSend)); // borro el contenido del buffe
            }
            if (!cambiandoNivel) {
                if (nivelActual == 2) {
                    usleep(1000);
                    if (this->colisionFuegosForFuegos[0] != -1) {
                        for (int j = 0; j < this->colisionFuegosForFuegos.size(); j = j + 3) {
                            if (this->colisionFuegosForFuegos[j + 1] == constantes::tipoEntidades::GAME_OVER) {
                                contadorGameOver++;
                                jugadorGameOver[this->colisionFuegosForFuegos[j]] = true;
                                tipoMensaje = convertirTipoMensajeEnString(
                                        constantes::tipoMensajeServidorCliente::GAME_OVER_PARA_CLIENTE);
                                bufferString =
                                        tipoMensaje + "," + to_string(this->colisionFuegosForFuegos[j]) + ",z";
                                if (contadorGameOver == cantidadMaximaConexiones) {
                                    finDeJuego = true;
                                    iniciarClienteParaGameOver();
                                }
                            } else {
                                tipoMensaje = convertirTipoMensajeEnString(
                                        constantes::tipoMensajeServidorCliente::BONUS);
                                bufferString =
                                        tipoMensaje + "," + to_string(this->colisionFuegosForFuegos[j]) + "," +
                                        to_string(this->colisionFuegosForFuegos[j + 1]) + "," +
                                        to_string(this->colisionFuegosForFuegos[j + 2]) + "," + "z";
                            }
                            if (this->colisionFuegosForFuegos[j + 1] == constantes::tipoEntidades::PERDIENDO ||
                                this->colisionFuegosForFuegos[j + 1] == constantes::tipoEntidades::FIN_ESCUDO) {
                                thread miSecondThread(&JuegoServidor::threadInvincibilidadPerdidaVida, this,
                                                      this->colisionFuegosForFuegos[j]);
                                miSecondThread.detach();
                            }

                            strcpy(bufferSend, bufferString.c_str());
                            for (int i = 0; i < this->vectorClientes.size(); i++) {
                                int socketEnviar = vectorClientes[i][1];
                                if (socketEnviar == -1) { continue; }
                                Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO, bufferSend);
                                bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                            }
                        }
                    }
                    this->colisionFuegosForFuegos = {-1};
                    memset(bufferSend, 0, sizeof(bufferSend)); // borro el contenido del buffer
                }
            }
        }

    }
    for (int i = 0; i < this->niveles[nivelActual]->getEscenario()->getFuegos().size(); i++) {
        this->niveles[nivelActual]->getEscenario()->getFuegos()[i]->setCoordenadaY(2000);
    }
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::ManejarEnemigos",
                 "Fin del thread ManejarEnemigos ");
}

void JuegoServidor::crearMensajeEnemigos(char *buffer) {
    const std::string CONTEXTO = "JuegoServidor::crearMensajeEnemigos";
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();

    std::string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::ACTUALIZA_ENEMIGOS);
    std::string mensajeString = tipoMensaje + ",";

    for (int i = 0; i < escenario->getEnemigos().size(); i++) {
        Enemigo *enemigo = escenario->getEnemigo(i);

        mensajeString += std::to_string(enemigo->getId()) + "," + std::to_string(enemigo->getCoordenadaX()) + "," +
                         std::to_string(enemigo->getCoordenadaY()) + "," +
                         std::to_string(enemigo->getIndexSprite()) + ",";
    }
    mensajeString += "z";

    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());

}

void JuegoServidor::crearMensajeEnemigoFinal(char *buffer) {
    const std::string CONTEXTO = "JuegoServidor::crearMensajeEnemigoFinal";
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();

    std::string tipoMensaje = convertirTipoMensajeEnString(
            constantes::tipoMensajeServidorCliente::ACTUALIZA_ENEMIGO_FINAL);
    std::string mensajeString = tipoMensaje + ",";
    EnemigoFinal *enemigoFinal = escenario->getEnemigoFinal();
    mensajeString += std::to_string(enemigoFinal->getCoordenadaX()) + "," +
                     std::to_string(enemigoFinal->getCoordenadaY()) + "," +
                     std::to_string(enemigoFinal->getIndexSprite()) + ",";

    for (int i = 0; i < escenario->getFuegos().size(); i++) {
        Fuego *fuego = escenario->getFuegos()[i];

        mensajeString += std::to_string(i) + "," + std::to_string(fuego->getCoordenadaX()) + "," +
                         std::to_string(fuego->getCoordenadaY()) + "," +
                         std::to_string(fuego->getIndexSprite()) + ",";
    }
    mensajeString += "z";

    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    strcpy(buffer, mensajeString.c_str());

}


void JuegoServidor::setColision(int idCliente, const vector<int> &colision) {
    this->vectorColision[idCliente] = colision;
}

void JuegoServidor::setColisionEnemigoFinal(int idCliente, const vector<int> &colision) {
    this->vectorColisionEnemigoFinal[idCliente] = colision;
}

void JuegoServidor::setColisionFuegos(int idCliente, const vector<int> &colision) {
    this->vectorColisionFuegos[idCliente] = colision;
}

bool JuegoServidor::isCambiandoNivel() const {
    return cambiandoNivel;
}

void JuegoServidor::setCambiandoNivel(bool cambiandoNivel) {
    JuegoServidor::cambiandoNivel = cambiandoNivel;
}

void JuegoServidor::threadInvincibilidad(int idCliente) {
    int bufferSize = 2048;
    char bufferReceive[bufferSize];
    string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
    string bufferString;
    int indexSprite = 0;
    for (int i = 1; i < 40; i++) {
        if (cambiandoNivel) { return; }
        this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente)->setInvincible(true);
        indexSprite = abs(indexSprite - 1);
        usleep(500000);
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        bufferString =
                tipoMensaje + "," + to_string(idCliente) + "," +
                to_string(constantes::tipoEntidades::INVICIBILIDAD_SPRITE) +
                "," + to_string(indexSprite) + "," + "z";
        strcpy(bufferReceive, bufferString.c_str());
        int bytesEnviados;
        for (int i = 0; i < this->vectorClientes.size(); i++) {
            int socketEnviar = vectorClientes[i][1];
            if (socketEnviar == -1) { continue; }
            bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
        }
    }
    usleep(500000);
    memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
    bufferString =
            tipoMensaje + "," + to_string(idCliente) + "," + to_string(constantes::tipoEntidades::FIN_INVINCIBILIDAD) +
            "," + to_string(0) + "," + "z";
    strcpy(bufferReceive, bufferString.c_str());
    int bytesEnviados;
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        int socketEnviar = vectorClientes[i][1];
        if (socketEnviar == -1) { continue; }
        bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
    }
    this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente)->setInvincible(false);

}

void JuegoServidor::threadInvincibilidadPerdidaVida(int idCliente) {
    sleep(2);
    this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente)->setInvincible(false);
}

void JuegoServidor::threadInvincibilidadPerdidaVidaEnemigoFinal() {
    sleep(1);
    this->niveles[nivelActual]->getEscenario()->getEnemigoFinal()->setInvincible(false);
}

void JuegoServidor::setColisionEnemigos(int idCliente, const vector<int> &colisionEnemigos) {
    this->vectorColisionEnemigos[idCliente] = colisionEnemigos;
}

void JuegoServidor::threadSpinAtack(int idCliente) {
    Personaje *personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente);
    Escenario *escenario = this->niveles[nivelActual]->getEscenario();
    int velocidadDeSprintAtack = 60;
    int numeroDePasos = 14;
    bool gameOff = false;
    unsigned int tiempoEntrePasos = 50000; //en microsegundos

    if (personaje->isMiraDerecha()) {
        //Defino una cantidad de pasos (a prueba y error para ver cuanto quiero que avance)
        for (int i = 0; i < numeroDePasos; i++) {
            if (!cambiandoNivel) {
                if (personaje->getFrameSaltar() / 3 >= 9) {
                    personaje->setFrameSaltar(6 * 3);
                }
                escenario->teclaDerecha(idCliente, velocidadDeSprintAtack, this->ventana->getAncho());
                personaje->setIndexSprite(personaje->getFrameSaltar() / 3);
                personaje->setFrameSaltar(personaje->getFrameSaltar() + 1);

                this->setTodasColisiones(idCliente,
                                         constantes::eventoEnumCliente::TECLA_ARRIBA,
                                         this->imortal,
                                         this->ventana->getAncho());

                /**** Enviar ****/
                int bufferSize = 2048;
                char bufferSend[bufferSize];
                char bufferReceive[bufferSize];

                if (i + 1 == numeroDePasos) {
                    personaje->setEstado(constantes::eventoEnumCliente::TECLA_DERECHA);
                    personaje->setFrameSaltar(0);
                    personaje->setIndexSprite(0);
                }

                this->crearMensajeEstado(bufferSend);
                for (int i = 0; i < this->vectorClientes.size(); i++) {
                    int socketEnviar = this->vectorClientes[i][1];
                    if (socketEnviar == -1) { continue; }
                    int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);

                }
                this->manejarColisiones(idCliente);
                //1000 / 16 (16 ciclos hace el while)
                usleep(tiempoEntrePasos);
                if (gameOff) {
                    personaje->setModoAtaque(false);
                    personaje->setEnMovimiento(false);
                    personaje->setArrastrable(true);
                    return;
                }

            } else {
                personaje->setModoAtaque(false);
                personaje->setEnMovimiento(false);
                personaje->setArrastrable(true);
                return;
            }
        }

    } else {

        for (int i = 0; i < numeroDePasos; i++) {
            if (!cambiandoNivel) {
                if (personaje->getFrameSaltar() / 3 >= 9) {
                    personaje->setFrameSaltar(6 * 3);
                }
                // MOVER GIRANDA HACIA LA IZQUIERDA


                escenario->teclaIzquierda(idCliente, velocidadDeSprintAtack, this->ventana->getAncho());
                personaje->setIndexSprite(personaje->getFrameSaltar() / 3);
                personaje->setFrameSaltar(personaje->getFrameSaltar() + 1);
                this->setTodasColisiones(idCliente,
                                         constantes::eventoEnumCliente::TECLA_ARRIBA,
                                         this->imortal,
                                         this->ventana->getAncho());
                int bufferSize = 2048;
                char bufferSend[bufferSize];


                if (i + 1 == numeroDePasos) {
                    personaje->setEstado(constantes::eventoEnumCliente::TECLA_IZQUIERDA);
                    personaje->setIndexSprite(0);
                    personaje->setFrameSaltar(0);
                }
                this->crearMensajeEstado(bufferSend);
                for (int i = 0; i < this->vectorClientes.size(); i++) {
                    int socketEnviar = this->vectorClientes[i][1];
                    if (socketEnviar == -1) { continue; }
                    int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);

                }
                this->manejarColisiones(idCliente);
                //1000 / 16 (16 ciclos hace el while)
                usleep(tiempoEntrePasos);
                if (gameOff) {
                    personaje->setModoAtaque(false);
                    personaje->setEnMovimiento(false);
                    personaje->setArrastrable(true);
                    return;
                }
            } else {
                personaje->setModoAtaque(false);
                personaje->setEnMovimiento(false);
                personaje->setArrastrable(true);
                return;
            }
        }
    }
    personaje->setModoAtaque(false);
    personaje->setIdPinche(-1);
    personaje->setEnMovimiento(false);
    personaje->setArrastrable(true);
}

JuegoServidor::~JuegoServidor() {
    delete this->ventana;
    delete this->configuracion;
    for (int i = 0; i < this->niveles.size(); i++) {
        delete this->niveles[i];
    }

    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::Destructor", "JuegoServidor destruido.");
}

/****************************** SERVIDOR *******************************************************************/



using namespace std;

void JuegoServidor::iniciarServidor() {
    const std::string CONTEXTO = "JuegoServidor::iniciarServidor";
    int PUERTO = this->puerto;
    int LIMITE_CONEXIONES_PENDIENTES_ENCOLADAS = 5;
    struct sockaddr_in direccionServidor;

    // Seteo los parametros del struct direccionServidor
    direccionServidor.sin_family = AF_INET; // tipo ipv4
    direccionServidor.sin_port = htons(PUERTO); // puerto en el que va a escuchar
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY); //llenado con la ip del servidor

    /***** Socket() *****/
    int socketServidor; // identificador del socket para escuchar conexiones
    // Creo el socket. Si falló devuelve -1
    socketServidor = socket(AF_INET, SOCK_STREAM, 0); //tipo ipv4, "reliable, 2-way, connection-based service", default
    if (socketServidor < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al crear el socket Servidor");
    } else {

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Socket Servidor creado con exito");
    }



    /***** Bind() ****/
    if ((bind(socketServidor, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor))) < 0) {

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el bind");

    } else {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Bind realizado con exito");

    }


    /***** Listen() *****/
    if (listen(socketServidor, LIMITE_CONEXIONES_PENDIENTES_ENCOLADAS) < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el listen");

    } else {

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Listen realizado con exito");
    }

    this->socketServidor = socketServidor;
}

int JuegoServidor::enviar(int socketCliente, char *buffer, int bufferSize) {
    const std::string CONTEXTO = "JuegoServidor::enviar";
    if (!cambiandoNivel) {
        this->enviarMutex.lock();
        int bytesMandados = send(socketCliente, buffer, bufferSize, 0);
        this->enviarMutex.unlock();
        if (bytesMandados < 0) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el send");
        }
        if (bytesMandados != bufferSize) {
            Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, "No se mandaron todos los bytes al cliente");
        }
        Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, buffer);
        return bytesMandados;
    } else if (mensajeNuevoNivel) {
        this->enviarMutex.lock();
        int bytesMandados = send(socketCliente, buffer, bufferSize, 0);
        this->enviarMutex.unlock();
        if (bytesMandados < 0) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el send");
        }
        if (bytesMandados != bufferSize) {
            Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, "No se mandaron todos los bytes al cliente");
        }
        return bytesMandados;
    }
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "No se mando el mensaje a causa del cambio de nivel");
    return 0;

}

int JuegoServidor::recibir(int socketCliente, char *buffer, int bufferSize) {
    const std::string CONTEXTO = "JuegoServidor::recibir";
    int contadorBytesRecibidos = recv(socketCliente, buffer, bufferSize, 0);
    if (contadorBytesRecibidos < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al hacer el Receive");
    } else {

        Log::loguear(constantes::logEnumModo::MEDIO, CONTEXTO, "Receive realizado con exito");
    }


    return contadorBytesRecibidos;
}

void JuegoServidor::cerrarServidor() {
    close(this->socketServidor);
}

void JuegoServidor::cerrarConexionCliente(int socketCliente) {
    close(socketCliente);
}

void JuegoServidor::aceptarConexiones() {

    const std::string CONTEXTO = "JuegoServidor::aceptarConexiones";
    int CANTIDAD_CLIENTES = this->cantidadMaximaConexiones;

    int socketServidor = this->socketServidor;

    /***** Accept() *****/
    int sizesockaddr_inr;
    int socketCliente;
    struct sockaddr_in direccionCliente;

    int bufferSize = 2048;
    char buffer[bufferSize];

    int contadorClientes = 0;
    while (contadorClientes < CANTIDAD_CLIENTES) {
        sizesockaddr_inr = sizeof(struct sockaddr_in);
        socketCliente = accept(socketServidor, (struct sockaddr *) &direccionCliente, (socklen_t *) &sizesockaddr_inr);
        if (socketCliente < 0) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al aceptar");

            exit(1);
        }

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                     "Accept realizado con exito cliente: " + to_string(contadorClientes));



        // Obtengo un idCliente liberado
        int idCliente = this->getIdClienteLiberado();
        // Bindeo el socketCliente con el id
        Personaje *personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente);


        this->bindSocketIdCliente(idCliente, socketCliente);

        //printf("Se obtuvo una conexión desde %s\n",inet_ntoa(direccionCliente.sin_addr) );  /* muestra la IP del cliente */


        /*** Send primera conexion***/
        memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
        string idClienteString = std::to_string(idCliente);
        string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::PRIMERA_CONEXION);
        string bufferString =
                tipoMensaje + "," + idClienteString + "," + to_string(modoDeJuego);
        strcpy(buffer, bufferString.c_str());
        int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
        /*** Send randomizar***/
        usleep(2000);
        memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
        this->crearMensajeRandomizar(buffer);
        bytesEnviados = enviar(socketCliente, buffer, bufferSize);

        contadorClientes++;
        for (int i = 0; i < this->niveles.size(); i++) {
            this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setConnectado(true);
            this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setIndexSprite(0);
        }


    }
    // Todos los clientes se conectaron, creo un thread por cada cliente para manejar las conexiones
    for (int i = 0; i < CANTIDAD_CLIENTES; i++) {
        socketCliente = this->getSocketCliente(i);
        memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
        string bufferString = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::INICIA_JUEGO) + "," +
                              to_string(CANTIDAD_CLIENTES);
        strcpy(buffer, bufferString.c_str());
        int bytesEnviados = enviar(socketCliente, buffer, bufferSize);


        thread miThread(&JuegoServidor::manejarReceive, this, i, false);
        miThread.detach();

    }

    if (this->modoDeJuego == constantes::modoDeJuego::EQUIPO) { //modo equipo
        while (contadorParaEquipos < CANTIDAD_CLIENTES) {

        }
        sleep(3);
        memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
        string bufferString = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPOS_LISTOS);
        strcpy(buffer, bufferString.c_str());
        for (int i = 0; i < this->vectorClientes.size(); i++) {
            int socketEnviar = vectorClientes[i][1];
            int bytesEnviados = enviar(socketEnviar, buffer, bufferSize);
        }
        sleep(1);
    } else {
        usleep(5000000);
    }

    sleep(3);
    memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Mandar el estado de principio");
    this->crearMensajeEstado(buffer);
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        int socketEnviar = vectorClientes[i][1];
        int bytesEnviados = enviar(socketEnviar, buffer, bufferSize);
    }

    thread miThread(&JuegoServidor::manejarEnemigos, this);
    miThread.detach();


    // Pueden venir mas conexiones
    while (!finDeJuego) {
        sizesockaddr_inr = sizeof(struct sockaddr_in);
        socketCliente = accept(socketServidor, (struct sockaddr *) &direccionCliente, (socklen_t *) &sizesockaddr_inr);
        if (socketCliente < 0) {
            Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al aceptar");
            exit(1);
        }


        // Obtengo un idCliente liberado
        int idCliente = this->getIdClienteLiberado();

        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                     "Accept realizado con exito cliente: " + to_string(idCliente));

        if (idCliente == -1) {
            // Todos los ids estan ocupados. Cierro la conexion con el cliente que se quiere comunicar
            memset(buffer, 0, sizeof(buffer)); // borro el contenido del buffer
            string bufferString = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::CUPO_LLENO);
            strcpy(buffer, bufferString.c_str());
            int bytesEnviados = enviar(socketCliente, buffer, bufferSize);
            this->cerrarConexionCliente(socketCliente);
            continue;
        }
        persoDesconectado[idCliente] = false;
        contadorDesconectado--;
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO,
                     "Numero clientes desconectados: " + to_string(contadorDesconectado));
        // Bindeo el socketCliente con el id
        this->bindSocketIdCliente(idCliente, socketCliente);
        // Manejo la conexion con el cliente (y le digo que es una reconexion para que le pase el modelo al cliente reconectado)

        thread miThread(&JuegoServidor::manejarReceive, this, idCliente, true);
        miThread.detach();
    }
    juegoGanado(socketServidor);
    sleep(6);
}

void JuegoServidor::manejarReceive(int idCliente, bool reconexion) {
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::manejarReceive",
                 "Inicio el thread para manejar el receive");
    int socketCliente = this->getSocketCliente(idCliente);
    int bufferSize = 2048;
    char bufferReceive[bufferSize];
    bool lock = false;
    Personaje *personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente);
    if (reconexion) {
        while (cambiandoNivel) {

        }

        string idClienteString = std::to_string(idCliente);
        string bufferString =
                convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::PRIMERA_CONEXION) + "," +
                idClienteString + "," + to_string(modoDeJuego);
        Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarReceive",
                     "modo de Juego :" + to_string(modoDeJuego));
        strcpy(bufferReceive, bufferString.c_str());
        int bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);

        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        while (cambiandoNivel) {

        }

        string tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::RECONEXION);
        bufferString = tipoMensaje + "," + to_string(nivelActual);
        strcpy(bufferReceive, bufferString.c_str());
        bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);

        usleep(2000);
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        this->crearMensajeRandomizar(bufferReceive);
        bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);

        if (this->modoDeJuego == constantes::modoDeJuego::EQUIPO) {
            for (int i = 0; i < equipo1.size(); i++) {
                while (cambiandoNivel) {

                }
                usleep(5000);
                memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
                bufferString =
                        convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO) + "," +
                        to_string(1) + "," + to_string(equipo1[i]) + ",z";
                strcpy(bufferReceive, bufferString.c_str());
                bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);
            }
            for (int i = 0; i < equipo2.size(); i++) {
                while (cambiandoNivel) {

                }
                usleep(5000);
                memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
                bufferString =
                        convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPO_ELEGIDO) + "," +
                        to_string(2) + "," + to_string(equipo2[i]) + ",z";
                strcpy(bufferReceive, bufferString.c_str());
                bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);
            }
        }

        while (cambiandoNivel) {

        }

        usleep(5000);
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        bufferString = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::EQUIPOS_LISTOS);
        strcpy(bufferReceive, bufferString.c_str());
        bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);

        while (cambiandoNivel) {

        }
        usleep(50000);
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        bufferString = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::INICIA_JUEGO) + "," +
                       to_string(cantidadMaximaConexiones);
        strcpy(bufferReceive, bufferString.c_str());
        bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);

        usleep(50000);
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        this->crearMensajeEstado(bufferReceive);
        bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);


        int i = 0;
        while (i < this->niveles[nivelActual]->getEscenario()->getEntidades().size()) {
            tipoMensaje = convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::BONUS);
            if (this->niveles[nivelActual]->getEscenario()->getEntitad(i)->getCoordenadaY() == 2000) {
                while (cambiandoNivel) {
                    i = 0;
                }
                usleep(50000);
                memset(bufferReceive, 0, sizeof(bufferReceive));
                string bufferString =
                        tipoMensaje + "," + to_string(0) + "," + to_string(constantes::tipoEntidades::ANILLO) + "," +
                        to_string(i) + "," + "z";
                strcpy(bufferReceive, bufferString.c_str());
                bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);
            }
            i++;

        }
        i = 0;
        while (i < this->vectorClientes.size()) {
            if (this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->isEscudo()) {
                while (cambiandoNivel) {
                    i = 0;
                }
                memset(bufferReceive, 0, sizeof(bufferReceive));
                string bufferString =
                        tipoMensaje + "," + to_string(i) + "," + to_string(constantes::tipoEntidades::ESCUDO_RECO) +
                        "," + to_string(0) + "," + "z";
                strcpy(bufferReceive, bufferString.c_str());
                bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);
                usleep(50000);
            }
            i++;
        }

        usleep(6000000);
        for (int i = 0; i < this->niveles.size(); i++) {
            this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setConnectado(true);
            this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setIndexSprite(0);
        }
        memset(bufferReceive, 0, sizeof(bufferReceive)); // borro el contenido del buffer
        this->crearMensajeEstado(bufferReceive);
        for (int i = 0; i < this->vectorClientes.size(); i++) {
            int socketEnviar = vectorClientes[i][1];
            if (socketEnviar == -1) { continue; }
            bytesEnviados = enviar(socketEnviar, bufferReceive, bufferSize);
        }

    }
    int bytesRecibidos = 1;
    memset(bufferReceive, 0, sizeof(bufferReceive));
    while (bytesRecibidos > 0) {
        if (!cambiandoNivel) {
            personaje = this->niveles[nivelActual]->getEscenario()->getPersonaje(idCliente);
            int contadorBytesRecibidos = 0;
            int bytesTotales = 2048;
            while (contadorBytesRecibidos != 2048) {
                /***** Receive() *****/
                bytesRecibidos = recibir(socketCliente, bufferReceive + contadorBytesRecibidos,
                                         bytesTotales - contadorBytesRecibidos);
                contadorBytesRecibidos += bytesRecibidos;
                if (bytesRecibidos <= 0) {
                    break;
                }

            }


            Log::loguear(constantes::logEnumModo::MEDIO, "JuegoServidor::manejarReceive", bufferReceive);
            /***** ActualizarModelo *****/
            char bufferSend[bufferSize];
            //Actualizo y obtengo el mensaje de respuesta
            int bytesEnviados;

            actualizarEstado(bufferReceive, bufferSend, idCliente);
            if (personaje->isLlegadoAlFinal()) {
                if (nivelActual < this->niveles.size() - 1) {
                    this->setCambiandoNivel(true);
                }
            }

            /***** Send() *****/
            if (!cambiandoNivel) {
                for (int i = 0; i < this->vectorClientes.size(); i++) {
                    if (personaje->isGameOver()) {
                        if (i != idCliente) {
                            int socketEnviar = vectorClientes[i][1];
                            if (socketEnviar == -1) { continue; }
                            bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                        }
                    } else {
                        int socketEnviar = vectorClientes[i][1];
                        if (socketEnviar == -1) { continue; }
                        bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                    }
                }
            }

            if (personaje->isLlegadoAlFinal()) {
                if (nivelActual < this->niveles.size() - 1) {
                    personaje->setLlegadoAlFinal(false);
                    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::manejarReceive",
                                 "Cambio de nivel");
                    for (int i = 0; i < cantidadMaximaConexiones; i++) {
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
                    usleep(700000);
                    nivelActual++;
                    for (int i = 0; i < this->vectorClientes.size(); i++) {
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setGameOver(
                                this->niveles[nivelActual - 1]->getEscenario()->getPersonaje(i)->isGameOver());
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setVida(
                                this->niveles[nivelActual - 1]->getEscenario()->getPersonaje(i)->getVida());
                        this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->setPuntos(
                                this->niveles[nivelActual - 1]->getEscenario()->getPersonaje(i)->getPuntos());
                    }
                    usleep(500000);

                    int bufferSize = 2048;
                    memset(bufferSend, 0, sizeof(bufferSend)); // borro el contenido del buffer
                    string bufferString = convertirTipoMensajeEnString(
                            constantes::tipoMensajeServidorCliente::NUEVO_NIVEL);
                    strcpy(bufferSend, bufferString.c_str());

                    usleep(1000000);
                    mensajeNuevoNivel = true;
                    for (int i = 0; i < this->vectorClientes.size(); i++) {
                        int socketEnviar = vectorClientes[i][1];
                        if (socketEnviar == -1) { continue; }
                        int bytesEnviados = enviar(socketEnviar, bufferSend, bufferSize);
                    }
                    mensajeNuevoNivel = false;
                    sleep(7);
                    this->setCambiandoNivel(false);

                }

            }
        }
    }
    if (persoDesconectado[idCliente] == true) {
        return;
    }
    persoDesconectado[idCliente] = true;
    contadorDesconectado++;
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::manejarReceive",
                 "Perdi la conexion con el cliente id: " + to_string(idCliente));
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::manejarReceive",
                 "Numeros de clientes desconectados: " + to_string(contadorDesconectado));

    for (int i = 0; i < this->niveles.size(); i++) {
        this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setConnectado(false);
        this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setMargenDerecha(false);
        this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setMargenIzquierda(false);
        this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setEstado(
                constantes::eventoEnumCliente::TECLA_DERECHA);
        this->niveles[i]->getEscenario()->getPersonaje(idCliente)->setIndexSprite(14);
    }
    this->liberarIdCliente(idCliente);
}

int JuegoServidor::getSocketCliente(int idCliente) {
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        vector<int> cliente = vectorClientes[i];
        if (cliente[0] == idCliente) {
            return cliente[1];
        }
    }
}

int JuegoServidor::getIdClienteLiberado() {
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        int socketCliente = this->vectorClientes[i][1];
        if (socketCliente == -1) {
            if (!this->niveles[nivelActual]->getEscenario()->getPersonaje(i)->isGameOver()) {
                return this->vectorClientes[i][0];
            }
        }
    }
    return -1;
}

void JuegoServidor::liberarIdCliente(int idCliente) {
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        if (this->vectorClientes[i][0] == idCliente) {
            this->vectorClientes[i][1] = -1;
        }
    }
}

void JuegoServidor::bindSocketIdCliente(int idCliente, int socketCliente) {
    for (int i = 0; i < this->vectorClientes.size(); i++) {
        if (this->vectorClientes[i][0] == idCliente) {
            this->vectorClientes[i][1] = socketCliente;
        }
    }
}

void JuegoServidor::juegoGanado(int socketServidor) {
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::JuegoGanado",
                 "El mensaje se envia al cliente ");
    for (int i = 0; i < 4; i++) {
        if (jugadorGameOver[i] != true) {
            int socketCliente = this->getSocketCliente(i);
            int bufferSize = 2048;
            char bufferReceive[bufferSize];
            string bufferString =
                    convertirTipoMensajeEnString(constantes::tipoMensajeServidorCliente::GANADOR);
            strcpy(bufferReceive, bufferString.c_str());
            int bytesEnviados = enviar(socketCliente, bufferReceive, bufferSize);
            sleep(3);
            cerrarConexionCliente(getSocketCliente(i));
        }
    }
    cerrarJuegoServidor();
    Log::loguear(constantes::logEnumModo::BAJO, "JuegoServidor::juegoGanado", "El servidor se cierra");
}

void JuegoServidor::iniciarClienteParaGameOver() {
    const std::string CONTEXTO = "JuegoServidor::iniciarClienteParaCliente";
    int PUERTO = this->puerto;
    struct sockaddr_in direccionServidor;

    // Seteo los parametros del struct direccionServidor
    direccionServidor.sin_family = AF_INET; // tipo ipv4
    direccionServidor.sin_port = htons(PUERTO); // puerto en el que va a escuchar
    //TODO Deberia setear la ip pasada en el constructor
    direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1"); //llenado con la ip del servidor

    /***** Socket() *****/
    int socketCliente; // identificador del socket para escuchar conexiones
    // Creo el socket. Si falló devuelve -1
    socketCliente = socket(AF_INET, SOCK_STREAM, 0); //tipo ipv4, "reliable, 2-way, connection-based service", default
    if (socketCliente < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al crear el Socket Cliente");
        return;
    }
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Socket Cliente creado con exito");
    if (TTF_Init() == -1) {
        Log::loguear(constantes::logEnumModo::ALTO, CONTEXTO,
                     "Fallo al inicializar SDL_ttf Error : " + std::string(TTF_GetError()));
    }
    /***** Connect() *****/
    if (connect(socketCliente, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Error al conectar con el servidor");
        return;
    }
    Log::loguear(constantes::logEnumModo::BAJO, CONTEXTO, "Conexion realizada con exito");
    return;

}

const vector<vector<int>> &JuegoServidor::getVectorColision() const {
    return vectorColision;
}

void JuegoServidor::setVectorColision(const vector<vector<int>> &vectorColision) {
    JuegoServidor::vectorColision = vectorColision;
}

const vector<vector<int>> &JuegoServidor::getVectorColisionEnemigos() const {
    return vectorColisionEnemigos;
}

void JuegoServidor::setVectorColisionEnemigos(const vector<vector<int>> &vectorColisionEnemigos) {
    JuegoServidor::vectorColisionEnemigos = vectorColisionEnemigos;
}

const vector<vector<int>> &JuegoServidor::getVectorColisionEnemigoFinal() const {
    return vectorColisionEnemigoFinal;
}

void JuegoServidor::setVectorColisionEnemigoFinal(const vector<vector<int>> &vectorColisionEnemigoFinal) {
    JuegoServidor::vectorColisionEnemigoFinal = vectorColisionEnemigoFinal;
}

const vector<vector<int>> &JuegoServidor::getVectorColisionFuegos() const {
    return vectorColisionFuegos;
}

void JuegoServidor::setVectorColisionFuegos(const vector<vector<int>> &vectorColisionFuegos) {
    JuegoServidor::vectorColisionFuegos = vectorColisionFuegos;
}

void JuegoServidor::threadImortalidad(){
    usleep(200000);
    lockImortal = false;
}