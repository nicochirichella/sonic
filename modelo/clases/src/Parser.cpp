//
// Created by dami on 24/03/17.
//

#include "../headers/Parser.h"
#include "../headers/Constantes.h"
#include "../headers/Escudo.h"
#include "../headers/Ring.h"
#include "../headers/Invincibilidad.h"


constexpr unsigned int str2int(const char *str, int h = 0) {
    return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

Parser::Parser() {

    ifstream archivoDeConfiguracion(FILES_PATH"escenario.json");
    Json::Reader reader;

    if (!archivoDeConfiguracion) {
        Log::loguear(constantes::logEnumModo::BAJO, "Parser::Constructor",
                     "No se ha podido abrir el archivo de configuración, se abre el archivo por defecto.");
        cargarArchivoPorDefault();
    } else {

        bool parseOk = reader.parse(archivoDeConfiguracion, this->datosDeConfiguracion);

        if (!parseOk) {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser::Constructor",
                         "No se ha podido abrir el archivo de configuración, se abre el archivo por defecto.");
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser::Constructor", reader.getFormattedErrorMessages());

            cargarArchivoPorDefault();
        }
    }
    // PRUEBA

    if (this->datosDeConfiguracion["logModo"]["modo"].isString()) {
        string modo = this->datosDeConfiguracion["logModo"]["modo"].asString();
        if (modo == "bajo") {
            Log::iniciar(constantes::logEnumModo::BAJO, constantes::logEnumEscritura::ARCHIVOYCONSOLA);
        } else if (modo == "medio") {
            Log::iniciar(constantes::logEnumModo::MEDIO, constantes::logEnumEscritura::ARCHIVOYCONSOLA);
        }
    }

    Log::loguear(constantes::logEnumModo::ALTO, "Parser::Constructor",
                 "Se abrió el archivo de configuración correctamente.");
}

Parser::~Parser() {
    Log::loguear(constantes::logEnumModo::ALTO, "Parser::Destructor", "Parser destruido.");
}

Ventana *Parser::getVentana() {


    if (!this->datosDeConfiguracion.isMember(this->ventanaString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getVentana",
                     "No se encuentra el campo VENTANA en el archivo, se creará con valores por defecto.");
        return new Ventana();
    }

    Json::Value ventanaJson = this->datosDeConfiguracion[this->ventanaString];

    if (!ventanaJson.isMember(this->dimensionesString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getVentana",
                     "No se encuentran las dimensiones de la VENTANA en el archivo, se utilizarán valores por defecto.");
        return new Ventana();
    }

    Json::Value dimensiones = ventanaJson[this->dimensionesString];

    if (((!dimensiones.isMember(this->anchoString)) && (!dimensiones.isMember(this->altoString))) ||
        ((!dimensiones[this->anchoString].isInt()) && (!dimensiones[this->altoString].isInt()))) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getVentana",
                     "No se encuentra el ancho y el alto de la VENTANA en el archivo o poseen formatos erróneos, se utilizarán valores por defecto.");
        return new Ventana();

    } else if (!dimensiones.isMember(this->anchoString) || (!dimensiones[this->anchoString].isInt())) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getVentana",
                     "No se encuentra el ancho de la VENTANA en el archivo o posee un formato erróneo, se utilizará el valor por defecto.");
        return new Ventana(0, dimensiones[this->altoString].asInt());
    } else if (!dimensiones.isMember(this->altoString) || (!dimensiones[this->altoString].isInt())) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getVentana",
                     "No se encuentra el alto de la VENTANA en el archivo o posee un formato erróneo, se utilizará el valor por defecto.");
        return new Ventana(dimensiones[this->anchoString].asInt(), 0);
    }

    return new Ventana(dimensiones[this->anchoString].asInt(), dimensiones[this->altoString].asInt());
}

Configuracion *Parser::getConfiguracion() {

    if (!this->datosDeConfiguracion.isMember(this->configuracionString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getConfiguracion",
                     "No se encuentra el campo CONFIGURACION en el archivo, se utilizarán valores por defecto.");
        return new Configuracion();

    }

    Json::Value configuracionJson = this->datosDeConfiguracion[this->configuracionString];

    if (!configuracionJson.isMember("vel_scroll") || !configuracionJson["vel_scroll"].isInt()) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getConfiguracion",
                     "No se encuentra la VELOCIDAD DE SCROLL en el archivo o posee un formato erróneo, se utilizará el valor por defecto.");
        return new Configuracion();

    } else {
        Json::Value velScroll = configuracionJson["vel_scroll"];

        return new Configuracion(velScroll.asInt());
    }
}

vector<Nivel *> Parser::getNiveles() {

    int id;
    vector<Nivel *> nivelesDeJuego;

    if (!this->datosDeConfiguracion.isMember(this->nivelesString)) {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getNiveles",
                     "No se encuentra el campo NIVELES en el archivo, se cargará el nivel por defecto.");
        cargarArchivoPorDefault();
    }
    Json::Value &nivelesJson = this->datosDeConfiguracion[this->nivelesString];
    for (int i = 0; i < nivelesJson.size(); i++) {

        id = validarId(nivelesJson[i][this->idString], this->idsNiveles);
        Nivel *nivel = new Nivel(this->getEscenario(nivelesJson[i]), id);
        nivel->setCantMaxAnillos(nivelesJson[i][this->maxAnillosString].asInt());
        nivel->setCantMaxBonusAnillo(nivelesJson[i][this->maxBonusAnilloString].asInt());
        nivel->setCantMaxPiedras(nivelesJson[i][this->maxPiedrasString].asInt());
        nivel->setCantMaxPinches(nivelesJson[i][this->maxPinchesString].asInt());
        nivel->setCantMaxPeces(nivelesJson[i][this->maxPecesString].asInt());
        nivel->setCantMaxCangrejos(nivelesJson[i][this->maxCangrejosString].asInt());
        nivel->setCantMaxMoscas(nivelesJson[i][this->maxMoscasString].asInt());
        nivel->setCantMaxPeces(nivelesJson[i][this->maxPecesString].asInt());
        nivel->setCantMaxInvincibilidad(nivelesJson[i][this->maxInvincibilidadString].asInt());
        nivel->setCantMaxEscudo(nivelesJson[i][this->maxEscudoString].asInt());
        

        nivel->setCantMinAnillos(nivelesJson[i][this->minAnillosString].asInt());
        nivel->setCantMinBonusAnillo(nivelesJson[i][this->minBonusAnilloString].asInt());
        nivel->setCantMinPiedras(nivelesJson[i][this->minPiedrasString].asInt());
        nivel->setCantMinPinches(nivelesJson[i][this->minPinchesString].asInt());
        nivel->setCantMinPeces(nivelesJson[i][this->minPecesString].asInt());
        nivel->setCantMinCangrejos(nivelesJson[i][this->minCangrejosString].asInt());
        nivel->setCantMinMoscas(nivelesJson[i][this->minMoscasString].asInt());
        nivel->setCantMinPeces(nivelesJson[i][this->minPecesString].asInt());
        nivel->setCantMinInvincibilidad(nivelesJson[i][this->minInvincibilidadString].asInt());
        nivel->setCantMinEscudo(nivelesJson[i][this->minEscudoString].asInt());

        nivel->setCantTotalAnillos(nivelesJson[i][this->totalAnillosString].asInt());
        nivel->setCantTotalBonusAnillo(nivelesJson[i][this->totalBonusAnilloString].asInt());
        nivel->setCantTotalPiedras(nivelesJson[i][this->totalPiedrasString].asInt());
        nivel->setCantTotalPinches(nivelesJson[i][this->totalPinchesString].asInt());
        nivel->setCantTotalPeces(nivelesJson[i][this->totalPecesString].asInt());
        nivel->setCantTotalCangrejos(nivelesJson[i][this->totalCangrejosString].asInt());
        nivel->setCantTotalMoscas(nivelesJson[i][this->totalMoscasString].asInt());
        nivel->setCantTotalPeces(nivelesJson[i][this->totalPecesString].asInt());
        nivel->setCantTotalInvincibilidad(nivelesJson[i][this->totalInvincibilidadString].asInt());
        nivel->setCantTotalEscudo(nivelesJson[i][this->totalEscudoString].asInt());

        nivelesDeJuego.push_back(nivel);
    }

    return nivelesDeJuego;

}


Escenario *Parser::getEscenario(Json::Value nivelesJson) {

    if (!nivelesJson.isMember(this->escenarioString)) {

        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEscenario",
                     "No se encuentra el campo ESCENARIO en el archivo se cargará el escenario por defecto");

        Json::Value configuracionJson = pedirArchivoPorDefault();

        Json::Value escenarioJson = configuracionJson[this->nivelesString][0][this->escenarioString];
        Json::Value dimensionesEsc = escenarioJson[this->dimensionesString];

        Escenario *escenario = new Escenario(dimensionesEsc[this->anchoString].asInt(),
                                             dimensionesEsc[this->altoString].asInt());
        escenario->setCapas(
                this->getCapas(escenarioJson, dimensionesEsc[this->anchoString].asInt(),
                               dimensionesEsc[this->altoString].asInt()));
        escenario->setEntidades(this->getEntidades(escenarioJson));
        escenario->setEnemigos(this->getEnemigos(escenarioJson));
        return escenario;
    } else {

        Json::Value escenarioJson = nivelesJson[this->escenarioString];
        Escenario *escenario = NULL;

        if (escenarioJson.isMember(this->dimensionesString)) {
            Json::Value dimensionesEsc = escenarioJson[this->dimensionesString];


            if (((!dimensionesEsc.isMember(this->anchoString)) && (!dimensionesEsc.isMember(this->altoString))) ||
                ((!dimensionesEsc[this->anchoString].isInt()) && (!dimensionesEsc[this->altoString].isInt()))) {
                Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEscenario",
                             "No se encuentra el ancho y el alto del ESCENARIO en el archivo o poseen formatos erróneos, se utilizarán valores por defecto.");
                escenario = new Escenario();

            } else if (!dimensionesEsc.isMember(this->anchoString) || (!dimensionesEsc[this->anchoString].isInt())) {
                Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEscenario",
                             "No se encuentra el ancho de la ESCENARIO en el archivo o posee un formato erróneo, se utilizará el valor por defecto.");
                escenario = new Escenario(0, dimensionesEsc[this->altoString].asInt());
            } else if (!dimensionesEsc.isMember(this->altoString) || (!dimensionesEsc[this->altoString].isInt())) {
                Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEscenario",
                             "No se encuentra el alto de la ESCENARIO en el archivo o posee un formato erróneo, se utilizará el valor por defecto.");
                escenario = new Escenario(dimensionesEsc[this->anchoString].asInt(), 0);
            } else {
                escenario = new Escenario(dimensionesEsc[this->anchoString].asInt(),
                                          dimensionesEsc[this->altoString].asInt());
            }
        } else {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEscenario",
                         "No se encuentra el campo dimensiones en ESCENARIO en el archivo o poseen formatos erróneos, se utilizarán valores por defecto.");
            escenario = new Escenario();

        }


        int anchoEsc = escenario->getAncho();
        int altoEsc = escenario->getAlto();
        escenario->setCapas(this->getCapas(escenarioJson, anchoEsc, altoEsc));
        escenario->setEntidades(this->getEntidades(escenarioJson));
        escenario->setEnemigos(this->getEnemigos(escenarioJson));
        return escenario;
    }

}

vector<Entidad *> Parser::getEntidades(Json::Value escenarioJson) {

    bool estaEntidades = true;
    vector<Entidad *> entidades;
    if (!escenarioJson.isMember(this->entidadesString)) {

        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo ENTIDADES en el archivo, se cargarán las entidades por defecto.");


        estaEntidades = false;

    }
    Json::Value &entidadesJson = escenarioJson[this->entidadesString];

    if (!estaEntidades) {
        Json::Value configuracionJson = pedirArchivoPorDefault();
        entidadesJson = configuracionJson[this->nivelesString][0][this->escenarioString][this->entidadesString];
    }
    vector<string> keysEntidades = cargarVectorDeClavesDeEntidades();

    bool esEntidadDibujable;
    vector<string> keysEntidadesDibujables;
    vector<int> ids;
    keysEntidadesDibujables.push_back(this->tipoString);
    keysEntidadesDibujables.push_back(this->coordenadaString);
    vector<string> keysCrop = cargarVectorDeClavesDeCrop();
    vector<string> keysCoordenadas = cargarVectorDeClavesDeCoordenadas();

    for (int i = 0; i < entidadesJson.size(); i++) {

        esEntidadDibujable = true;
        /*if (this->verificarCampos(keysEntidadesDibujables, entidadesJson, i, this->entidadesString)) {
            if ((entidadesJson[i][this->tipoString].isString()) &&
                (this->verificarCampos(keysCoordenadas, entidadesJson[i][this->coordenadaString],
                                       this->entidadesString))) {
                if ((entidadesJson[i][this->coordenadaString][this->xString].isInt()) &&
                    (entidadesJson[i][this->coordenadaString][this->yString].isInt())) {

                    if ((entidadesJson[i][this->tipoString].asString() == this->rectanguloString) ||
                        (entidadesJson[i][this->tipoString].asString() == this->circuloString ||
                         entidadesJson[i][this->tipoString].asString() == this->piedraString ||
                         entidadesJson[i][this->tipoString].asString() == this->pincheString ||
                         entidadesJson[i][this->tipoString].asString() == this->anilloString
                         || entidadesJson[i][this->tipoString].asString() == this->escudoString ||
                         entidadesJson[i][this->tipoString].asString() == this->ringString ||
                         entidadesJson[i][this->tipoString].asString() == this->invincibilidadString)) {
                        esEntidadDibujable = true;
                    }
                }
            }
        }*/

        if (esEntidadDibujable) {

            int id = validarId(entidadesJson[i][this->idString], this->idsEntidades);
            int indexZ = validarInt(entidadesJson[i][this->index_ZString], "Index Z");
            string rutaImagen = validarRuta(entidadesJson[i][this->rutaImagenString]);
            string color = validarColor(entidadesJson[i][this->colorString]);

            Json::Value dimensiones = entidadesJson[i][this->dimensionesString];
            Json::Value coordenadas = entidadesJson[i][this->coordenadaString];
            Json::Value crop = entidadesJson[i][this->cropString];

            int coordX = validarCoord(coordenadas[this->xString], "COORD. X"), coordY = validarCoord(
                    coordenadas[this->yString],
                    "COORD. Y");
            int cropX = validarCoord(crop[this->xCropString], "CROP X"), cropY = validarCoord(crop[this->yCropString],
                                                                                              "CROP Y");
            int ancho, alto, radio;

            const char *tipo = (entidadesJson[i][this->tipoString].asString()).c_str();


            switch (str2int(tipo)) {
                case str2int("rectangulo") : {

                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Entidad *rec = new Rectangulo(id, color, coordX, coordY, cropX, cropY, rutaImagen, indexZ, ancho,
                                                  alto);
                    entidades.push_back(rec);
                    break;

                }
                case str2int("circulo") : {

                    radio = validarInt(dimensiones[this->radioString], "Radio");
                    Entidad *cir = new Circulo(id, color, coordX, coordY, cropX, cropY, rutaImagen, indexZ,
                                               radio);

                    entidades.push_back(cir);
                    break;
                }
                case str2int("piedra") : {
                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Entidad *pied = new Piedra(id, color, coordX, 570, 0, 0, rutaImagen, indexZ, 100,
                                               100);
                    entidades.push_back(pied);
                    break;
                }
                case str2int("pinche") : {
                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Entidad *pinch = new Pinche(id, color, coordX, 585, 0, 0, rutaImagen, indexZ, 70,
                                                80);
                    entidades.push_back(pinch);
                    break;
                }
                case str2int("anillo") : {

                    Entidad *anillo = new Anillo(id, color, coordX, coordY, 65, 710, rutaImagen, indexZ, 40, 40);
                    entidades.push_back(anillo);
                    break;
                }
                case str2int("escudo") : {
                                        Entidad *escu = new Escudo(id, color, coordX, coordY, 0, 0, rutaImagen, indexZ, 60, 60);
                    entidades.push_back(escu);
                    break;
                }
                case str2int("bonusAnillo") : {

                    Entidad *ring = new Ring(id, color, coordX, coordY, 0, 0, rutaImagen, indexZ, 60, 60);
                    entidades.push_back(ring);
                    break;
                }
                case str2int("invincibilidad") : {

                    Entidad *invin = new Invincibilidab(id, color, coordX, coordY, 0, 0, rutaImagen, indexZ, 60, 60);
                    entidades.push_back(invin);
                    break;
                }


                default :
                    break;
            }

        } else {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser->getEntidades",
                         "La ENTIDAD en posicion " + to_string(i + 1) +
                         " no se dibuja ya que su coordenada o tipo es erroneo o no existe ");

        }
    }

    return entidades;
}


vector<Enemigo *> Parser::getEnemigos(Json::Value escenarioJson) {

    bool estaEntidades = true;
    vector<Enemigo *> enemigos;
    if (!escenarioJson.isMember(this->entidadesString)) {

        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEnemigos",
                     "No se encuentra el campo ENTIDADES en el archivo, se cargarán los enemigos por defecto.");
        estaEntidades = false;

    }
    Json::Value &entidadesJson = escenarioJson[this->entidadesString];

    if (!estaEntidades) {
        Json::Value configuracionJson = pedirArchivoPorDefault();
        entidadesJson = configuracionJson[this->nivelesString][0][this->escenarioString][this->entidadesString];
    }
    vector<string> keysEntidades = cargarVectorDeClavesDeEntidades();

    bool esEnemigoDibujable;
    vector<string> keysEntidadesDibujables;
    vector<int> ids;
    keysEntidadesDibujables.push_back(this->tipoString);
    keysEntidadesDibujables.push_back(this->coordenadaString);
    vector<string> keysCrop = cargarVectorDeClavesDeCrop();
    vector<string> keysCoordenadas = cargarVectorDeClavesDeCoordenadas();

    for (int i = 0; i < entidadesJson.size(); i++) {

        esEnemigoDibujable = true;
        /*if (this->verificarCampos(keysEntidadesDibujables, entidadesJson, i, this->entidadesString)) {
            if ((entidadesJson[i][this->tipoString].isString()) &&
                (this->verificarCampos(keysCoordenadas, entidadesJson[i][this->coordenadaString],
                                       this->entidadesString))) {
                if ((entidadesJson[i][this->coordenadaString][this->xString].isInt()) &&
                    (entidadesJson[i][this->coordenadaString][this->yString].isInt())) {

                    if ((entidadesJson[i][this->tipoString].asString() == this->cangrejoString) ||
                        (entidadesJson[i][this->tipoString].asString() == this->peceString ||
                         entidadesJson[i][this->tipoString].asString() == this->moscaString)) {
                        esEnemigoDibujable = true;
                    }
                }
            }
        }*/

        if (esEnemigoDibujable) {

            int id = validarId(entidadesJson[i][this->idString], this->idsEntidades);
            int indexZ = validarInt(entidadesJson[i][this->index_ZString], "Index Z");
            string rutaImagen = validarRuta(entidadesJson[i][this->rutaImagenString]);
            string color = validarColor(entidadesJson[i][this->colorString]);

            Json::Value dimensiones = entidadesJson[i][this->dimensionesString];
            Json::Value coordenadas = entidadesJson[i][this->coordenadaString];
            Json::Value crop = entidadesJson[i][this->cropString];

            int coordX = validarCoord(coordenadas[this->xString], "COORD. X"), coordY = validarCoord(
                    coordenadas[this->yString],
                    "COORD. Y");
            int cropX = validarCoord(crop[this->xCropString], "CROP X"), cropY = validarCoord(crop[this->yCropString],
                                                                                              "CROP Y");
            int ancho, alto, radio;

            const char *tipo = (entidadesJson[i][this->tipoString].asString()).c_str();


            switch (str2int(tipo)) {
                case str2int("mosca") : {
                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Enemigo *mosc = new Mosca(id, color, coordX, coordY, cropX, cropY, rutaImagen, indexZ, ancho,
                                              alto);
                    enemigos.push_back(mosc);
                    break;

                }
                case str2int("pece") : {
                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Enemigo *pec = new Pece(id, color, coordX, coordY, cropX, cropY, rutaImagen, indexZ, ancho,
                                            alto);
                    enemigos.push_back(pec);
                    break;
                }
                case str2int("cangrejo") : {
                    ancho = validarInt(dimensiones[this->anchoString], "Ancho");
                    alto = validarInt(dimensiones[this->altoString], "Alto");

                    Enemigo *cang = new Cangrejo(id, color, coordX, coordY, cropX, cropY, rutaImagen, indexZ, ancho,
                                                 alto);
                    enemigos.push_back(cang);
                    break;
                }

                default :
                    break;
            }

        } else {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser->getEnemigos",
                         "El ENEMIGO en posicion " + to_string(i + 1) +
                         " no se dibuja ya que su coordenada o tipo es erroneo o no existe ");

        }
    }

    return enemigos;
}


vector<Personaje *> Parser::getPersonajes() {

    vector<Personaje *> personajes;

    Json::Value &personajesJson = this->datosDeConfiguracion[this->personajesString];
    Log::loguear(constantes::logEnumModo::MEDIO, "Parser:getPersonajes", "personaje ");

    for (int i = 0; i < personajesJson.size(); i++) {

        // El ID es el orden en el que vienen en el json
        int id = i;
        int indexZ = validarInt(personajesJson[i][this->index_ZString], "Index Z");
        string rutaImagen = "images/sonic_hd_sprite.png";
        switch (id) {

            case 0:
                rutaImagen = "images/sonic_hd_sprite_new.png";
                break;
            case 1:
                rutaImagen = "images/sonic_hd_sprite_pink.png";
                break;
            case 2:
                rutaImagen = "images/sonic_hd_sprite_red.png";
                break;
            case 3:
                rutaImagen = "images/sonic_hd_sprite_yellow.png";
                break;

        }

        string color = validarColor(personajesJson[i][this->colorString]);

        Json::Value dimensiones = personajesJson[i][this->dimensionesString];
        Json::Value coordenadas = personajesJson[i][this->coordenadaString];
        Json::Value crop = personajesJson[i][this->cropString];

        int coordX = validarCoord(coordenadas[this->xString], "COORD. X"), coordY = validarCoord(
                coordenadas[this->yString],
                "COORD. Y");
        int cropX = validarCoord(crop[this->xCropString], "CROP X"), cropY = validarCoord(crop[this->yCropString],
                                                                                          "CROP Y");
        int ancho = validarInt(dimensiones[this->anchoString], "Ancho");
        int alto = validarInt(dimensiones[this->altoString], "Alto");

        personajes.push_back(new Personaje(id, color, coordX, coordY, cropX, cropY,
                                           rutaImagen, indexZ,
                                           ancho, alto));
        Log::loguear(constantes::logEnumModo::BAJO, "getPersonajes", "Personaje creado, con ID: " + std::to_string(i));
    }
    Log::loguear(constantes::logEnumModo::BAJO, "getPersonajes", "Personajes Creados");

    return personajes;
}

vector<Capa *> Parser::getCapas(Json::Value escenarioJson, int anchoEsc, int altoEsc) {
    vector<Capa *> capas;

    if (!escenarioJson.isMember("capas")) {

        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getCapas",
                     "No se encuentra el campo CAPAS en el archivo se cargarán las capas por defecto.");
        Json::Value configuracionJson = pedirArchivoPorDefault();

        Json::Value &capasJson = configuracionJson[this->nivelesString][0][this->escenarioString]["capas"];

        int contadorCapas = 0;
        for (int i = 0; i < capasJson.size(); i++) {
            for (int j = 0; j < 10; j++){
                Capa *capa = new Capa(contadorCapas,
                                      capasJson[i][this->rutaImagenString].asString(),
                                      altoEsc, anchoEsc/10,
                                      capasJson[i][this->index_ZString].asInt(), j*anchoEsc/10);
                capas.push_back(capa);
                contadorCapas++;
            }
        }
        return capas;

    } else {
        const Json::Value &capasJson = escenarioJson["capas"];

        vector<Capa *> capas;
        int contadorCapas = 0;

        for (int i = 0; i < capasJson.size(); i++) {
            for (int j = 0; j < 10; j++){

                int indexZ = validarInt(capasJson[i][this->index_ZString], "INDEX_Z");
                string rutaImagen = validarRuta(capasJson[i][this->rutaImagenString]);
                Capa *capa = new Capa(contadorCapas, rutaImagen, altoEsc, anchoEsc/10, indexZ,  j*anchoEsc/10);
                contadorCapas++;
                capas.push_back(capa);

            }
        }
        return capas;
    }
}

bool Parser::verificarCampos(vector<string> keys, Json::Value campos, string contexto) {

    bool resultado = true;
    for (int i = 0; i < keys.size(); i++) {
        if (!campos.isMember(keys[i])) {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser",
                         "No se encuentra el campo " + keys[i] + " en " + contexto);
            this->keysFaltantes.push_back(keys[i]);
            resultado = false;
        }
    }
    return resultado;
}

bool Parser::verificarCampos(vector<string> keys, Json::Value &campos, int j, string contexto) {
    bool resultado = true;
    for (int i = 0; i < keys.size(); i++) {
        if (!campos[j].isMember(keys[i])) {
            Log::loguear(constantes::logEnumModo::MEDIO, "Parser",
                         "No se encuentra el campo " + keys[i] + " en " + contexto);
            this->keysFaltantes.push_back(keys[i]);
            resultado = false;
        }
    }

    return resultado;
}

void Parser::cargarArchivoPorDefault() {

    Json::Reader reader;
    ifstream archivoDeConfiguracion(DEFAULT_FILES_PATH"/default.json");
    reader.parse(archivoDeConfiguracion, this->datosDeConfiguracion);
}

Json::Value Parser::pedirArchivoPorDefault() {
    Json::Value configuracionDefatul;
    Json::Reader reader;
    ifstream archivoDeConfiguracion(DEFAULT_FILES_PATH"/default.json");
    reader.parse(archivoDeConfiguracion, configuracionDefatul);

    return configuracionDefatul;
}

vector<string> Parser::cargarVectorDeClavesDeEntidades() {
    vector<string> keysEntidades;
    keysEntidades.push_back(this->rutaImagenString);
    keysEntidades.push_back(this->idString);
    keysEntidades.push_back(this->coordenadaString);
    keysEntidades.push_back(this->dimensionesString);
    keysEntidades.push_back(this->colorString);
    keysEntidades.push_back(this->index_ZString);
    keysEntidades.push_back(this->cropString);
    keysEntidades.push_back(this->tipoString);

    return keysEntidades;
}

vector<string> Parser::cargarVectorDeClavesDeCoordenadas() {
    vector<string> keysCoordenadas;
    keysCoordenadas.push_back(this->xString);
    keysCoordenadas.push_back(this->yString);


    return keysCoordenadas;
}

vector<string> Parser::cargarVectorDeClavesDeCrop() {
    vector<string> keysCrop;

    keysCrop.push_back(this->yCropString);
    keysCrop.push_back(this->xCropString);
    return keysCrop;
}

/*vector<string> Parser::cargarVectorValoresTipo() {
    vector<string> vectorValoresTipo;

    vectorValoresTipo.push_back("rectangulo");
    vectorValoresTipo.push_back(this->circuloString);
    return vectorValoresTipo;
} */

int Parser::validarInt(Json::Value valor, string campo) {

    if (valor.isInt()) {
        return valor.asInt();
    } else {

        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo " + campo +
                     " en el archivo o posee un formato erróneo, se cargará el valor por defecto.");

        return -1;
    }
}

bool Parser::estaElID(int id, vector<int> &ids) {

    bool encontrado = false;

    if (id <= 0) { return true; }
    for (int i = 0; i < ids.size(); i++) {
        if (ids[i] == id) {
            encontrado = true;
        }
    }

    return encontrado;
}

int Parser::generarID() {

    return rand() % 99999 + 1;

}


int Parser::validarId(Json::Value id, vector<int> &ids) {
    return id.asInt();

    int idGenerado = 0;

    if (id.isInt()) {
        if (!estaElID(id.asInt(), ids)) {
            ids.push_back(id.asInt());
            return id.asInt();
        } else {

            while (estaElID(idGenerado, ids)) {
                idGenerado = generarID();
            }
            if (id.asInt() <= 0) {
                Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                             "Se generó un ID por defecto, al no ser positivo. ID: " +
                             std::to_string(idGenerado));
            } else {
                Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                             "Se generó un ID por defecto, al encontrarse repetido en el Json.ID: " +
                             std::to_string(idGenerado));
            }
            return idGenerado;
        }
    } else {

        while (estaElID(idGenerado, ids)) {
            idGenerado = generarID();
        }
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo ID en el archivo o posee un formato erróneo, se cargará el valor por defecto.ID: " +
                     std::to_string(idGenerado));
        return idGenerado;
    }
}

string Parser::validarColor(Json::Value color) {

    string colorJson = "negro";

    if (color.isString()) {

        bool colorExists = false;
        colorJson = color.asString();
        vector<string> colorVector;
        colorVector.push_back("rojo");
        colorVector.push_back("amarillo");
        colorVector.push_back("verde");
        colorVector.push_back("azul");
        colorVector.push_back("naranja");

        for (int i = 0; i < colorVector.size(); i++) {
            if (colorVector[i] == colorJson) {
                colorExists = true;
            }
        }
        if (colorExists) {
            return colorJson;
        } else {
            return "negro";
        }
    } else {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo COLOR en el archivo o posee un formato erróneo, se cargará el valor por defecto.");
        return "negro";
    }
}

int Parser::validarCoord(Json::Value valor, string campo) {

    if (valor.isInt()) {
        return valor.asInt();
    } else {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo " + campo +
                     " en el archivo o posee un formato erróneo, se cargará el valor por defecto.");

        return 0;
    }
}


string Parser::validarRuta(Json::Value valor) {

    if (valor.isString()) {
        return valor.asString();
    } else {
        Log::loguear(constantes::logEnumModo::MEDIO, "Parser::getEntidades",
                     "No se encuentra el campo RUTA IMAGEN en el archivo o posee un formato erróneo, se cargará el valor por defecto.");

        return IMAGES_NOTFOUND_PATH;

    }
}

