//
// Created by dami on 24/03/17.
//

#ifndef UNTITLED_PARSER_H
#define UNTITLED_PARSER_H

#include <fstream>
#include <jsoncpp/json/json.h>
#include <vector>
#include "../headers/Ventana.h"
#include "../headers/Configuracion.h"
#include "../headers/Entidad.h"
#include "../headers/Capa.h"
#include "../headers/Escenario.h"
#include "../headers/Log.h"
#include "../headers/Nivel.h"
#include "../headers/Rectangulo.h"
#include "../headers/Circulo.h"
#include "../headers/Constantes.h"
#include "../headers/Piedra.h"
#include "../headers/Pece.h"
#include "../headers/Cangrejo.h"
#include "../headers/Mosca.h"
#include "../headers/Pinche.h"
#include "../headers/Anillo.h"

using namespace std;

class Parser {

private:

    Json::Value datosDeConfiguracion;
    vector<string> keysFaltantes;
    vector<int> idsNiveles;
    vector<int> idsCapas;
    vector<int> idsEntidades;
    string ventanaString = "ventana";
    string dimensionesString = "dimensiones";
    string anchoString = "ancho";
    string altoString = "alto";
    string configuracionString = "configuracion";
    string nivelesString = "niveles";
    string idString = "id";
    string escenarioString = "escenario";
    string index_ZString = "index_z";
    string entidadesString = "entidades";
    string tipoString = "tipo";
    string colorString = "color";
    string cropString = "crop";
    string xCropString = "xCrop";
    string yCropString = "yCrop";
    string coordenadaString = "coordenada";
    string xString = "x";
    string yString = "y";
//    string rectanguloString = "rectangulo";
//    string piedraString = "piedra";
//    string pincheString = "pinche";
//    string anilloString = "anillo";
//    string escudoString = "escudo";
//    string ringString = "ring";
//    string invincibilidadString= "invincibilidad";
//    string circuloString = "circulo";
    string radioString = "radio";
    string rutaImagenString = "ruta_imagen";
    string personajesString = "personajes";
//    string cangrejoString = "cangrejo";
//    string peceString = "pece";
//    string moscaString = "mosca";
    string maxMoscasString = "cantidadMaxMoscas";
    string minMoscasString = "cantidadMinMoscas";
    string totalMoscasString = "cantidadTotalMoscas";
    string maxAnillosString = "cantidadMaxAnillos";
    string minAnillosString = "cantidadMinAnillos";
    string totalAnillosString = "cantidadTotalAnillos";
    string maxPecesString = "cantidadMaxPece";
    string minPecesString = "cantidadMinPece";
    string totalPecesString = "cantidadTotalPece";
    string maxCangrejosString = "cantidadMaxCangrejos";
    string minCangrejosString = "cantidadMinCangrejos";
    string totalCangrejosString = "cantidadTotalCangrejos";
    string maxBonusString = "cantidadMaxBonus";
    string minBonusString = "cantidadMinBonus";
    string totalBonusString = "cantidadTotalBonus";
    string maxPinchesString = "cantidadMaxPinches";
    string minPinchesString = "cantidadMinPinches";
    string totalPinchesString = "cantidadTotalPinches";
    string maxPiedrasString = "cantidadMaxPiedra";
    string minPiedrasString = "cantidadMinPiedra";
    string totalPiedrasString = "cantidadTotalPiedra";
    string maxInvincibilidadString = "cantidadMaxInvincibilidad";
    string minInvincibilidadString = "cantidadMinInvincibilidad";
    string totalInvincibilidadString = "cantidadTotalInvincibilidad";
    string maxEscudoString = "cantidadMaxEscudo";
    string minEscudoString = "cantidadMinEscudo";
    string totalEscudoString = "cantidadTotalEscudo";
    string maxBonusAnilloString = "cantidadMaxBonusAnillo";
    string minBonusAnilloString = "cantidadMinBonusAnillo";
    string totalBonusAnilloString = "cantidadTotalBonusAnillo";

public:
    Parser();

    ~Parser();

    Ventana *getVentana();

    Configuracion *getConfiguracion();

    vector<Nivel *> getNiveles();

    vector<Personaje *> getPersonajes();


private:

    Escenario *getEscenario(Json::Value escenarioJson);

    vector<Entidad *> getEntidades(Json::Value escenarioJson);

        vector <Enemigo *> getEnemigos(Json::Value escenarioJson);

    vector<Capa*> getCapas(Json::Value escenarioJson,int anchoEsc, int altoEsc);

    bool verificarCampos(vector<string> keys, Json::Value campos, string contexto);

    bool verificarCampos(vector<string> keys, Json::Value &campos, int i, string contexto);

    void cargarArchivoPorDefault();

    Json::Value pedirArchivoPorDefault();

    vector<string> cargarVectorDeClavesDeEntidades();

    vector<string> cargarVectorDeClavesDeCoordenadas();

    vector<string> cargarVectorDeClavesDeCrop();

    // vector<string> cargarVectorValoresTipo();

    int validarId(Json::Value id,vector<int>& ids);

    int validarCoord(Json::Value valor,string campo);

    int validarInt(Json::Value valor,string campo);

    string validarRuta(Json::Value valor);

    string validarColor(Json::Value color);

    bool estaElID(int id,vector<int>& ids);

    int generarID();
};


#endif //UNTITLED_PARSER_H
