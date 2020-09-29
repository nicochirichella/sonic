//
// Created by nicolas on 19/03/17.
//

#include "../headers/Nivel.h"



Nivel::Nivel(Escenario* escenario,int id) {

    this->id=id;
    if(escenario){
        this->escenario = escenario;
        Log::loguear(constantes::logEnumModo::ALTO, "Nivel::Constructor", "Nivel creado con Escenario en archivo.ID: "+std::to_string(this->id));
    }else{
        this->escenario =new Escenario();
        Log::loguear(constantes::logEnumModo::ALTO, "Nivel::Constructor", "Nivel creado con Escenario por default.ID: "+std::to_string(this->id));
    }
    this->CantMaxPeces=5;
    this->CantMaxCangrejos=5;
    this->CantMaxPiedras=5;
    this->CantMaxBonus=5;
    this->CantMaxMoscas=5;
    this->CantMaxAnillos=5;
    this->CantMaxPinches=5;
    this->CantMinPeces=1;
    this->CantMinCangrejos=1;
    this->CantMinPiedras=1;
    this->CantMinBonus=1;
    this->CantMinMoscas=1;
    this->CantMinAnillos=1;
    this->CantMinPinches=1;

}

Nivel::~Nivel() {
    delete this->escenario;
    Log::loguear(constantes::logEnumModo::ALTO, "Nivel::Destructor", "Nivel destruido.");
}

Escenario* Nivel::getEscenario(){
    return this->escenario;
}


void Nivel::setCantMaxPeces(int cantidad){ this->CantMaxPeces=cantidad;}
int Nivel::getCantMaxPeces(){ return this->CantMaxPeces;}
void Nivel::setCantMaxCangrejos(int cantidad){this->CantMaxCangrejos=cantidad;}
int Nivel::getCantMaxCangrejos(){ return this->CantMaxCangrejos;}
void Nivel::setCantMaxPiedras(int cantidad){this->CantMaxPiedras=cantidad;}
int Nivel::getCantMaxPiedras(){ return this->CantMaxPiedras;}
void Nivel::setCantMaxBonus(int cantidad){this->CantMaxBonus=cantidad;}
int Nivel::getCantMaxBonus(){ return this->CantMaxBonus;}
void Nivel::setCantMaxMoscas(int cantidad){this->CantMaxMoscas=cantidad;}
int Nivel::getCantMaxMoscas(){ return this->CantMaxMoscas;}
void Nivel::setCantMaxAnillos(int cantidad){this->CantMaxAnillos=cantidad;}
int Nivel::getCantMaxAnillos(){ return this->CantMaxAnillos;}
void Nivel::setCantMaxPinches(int cantidad){this->CantMaxPinches=cantidad;}
int Nivel::getCantMaxPinches(){ return this->CantMaxPinches;}

void Nivel::setCantMinPeces(int cantidad){ this->CantMinPeces=cantidad;}
int Nivel::getCantMinPeces(){ return this->CantMinPeces;}
void Nivel::setCantMinCangrejos(int cantidad){this->CantMinCangrejos=cantidad;}
int Nivel::getCantMinCangrejos(){ return this->CantMinCangrejos;}
void Nivel::setCantMinPiedras(int cantidad){this->CantMinPiedras=cantidad;}
int Nivel::getCantMinPiedras(){ return this->CantMinPiedras;}
void Nivel::setCantMinBonus(int cantidad){this->CantMinBonus=cantidad;}
int Nivel::getCantMinBonus(){ return this->CantMinBonus;}
void Nivel::setCantMinMoscas(int cantidad){this->CantMinMoscas=cantidad;}
int Nivel::getCantMinMoscas(){ return this->CantMinMoscas;}
void Nivel::setCantMinAnillos(int cantidad){this->CantMinAnillos=cantidad;}
int Nivel::getCantMinAnillos(){ return this->CantMinAnillos;}
void Nivel::setCantMinPinches(int cantidad){this->CantMinPinches=cantidad;}
int Nivel::getCantMinPinches(){ return this->CantMinPinches;}

int Nivel::getCantTotalPeces() const {
    return CantTotalPeces;
}

void Nivel::setCantTotalPeces(int CantTotalPeces) {
    Nivel::CantTotalPeces = CantTotalPeces;
}

int Nivel::getCantTotalCangrejos() const {
    return CantTotalCangrejos;
}

void Nivel::setCantTotalCangrejos(int CantTotalCangrejos) {
    Nivel::CantTotalCangrejos = CantTotalCangrejos;
}

int Nivel::getCantTotalPiedras() const {
    return CantTotalPiedras;
}

void Nivel::setCantTotalPiedras(int CantTotalPiedras) {
    Nivel::CantTotalPiedras = CantTotalPiedras;
}

int Nivel::getCantTotalBonus() const {
    return CantTotalBonus;
}

void Nivel::setCantTotalBonus(int CantTotalBonus) {
    Nivel::CantTotalBonus = CantTotalBonus;
}

int Nivel::getCantTotalMoscas() const {
    return CantTotalMoscas;
}

void Nivel::setCantTotalMoscas(int CantTotalMoscas) {
    Nivel::CantTotalMoscas = CantTotalMoscas;
}

int Nivel::getCantTotalAnillos() const {
    return CantTotalAnillos;
}

void Nivel::setCantTotalAnillos(int CantTotalAnillos) {
    Nivel::CantTotalAnillos = CantTotalAnillos;
}

int Nivel::getCantTotalPinches() const {
    return CantTotalPinches;
}

void Nivel::setCantTotalPinches(int CantTotalPinches) {
    Nivel::CantTotalPinches = CantTotalPinches;
}

int Nivel::getCantMaxInvincibilidad() const {
    return CantMaxInvincibilidad;
}

void Nivel::setCantMaxInvincibilidad(int CantMaxInvincibilidad) {
    Nivel::CantMaxInvincibilidad = CantMaxInvincibilidad;
}

int Nivel::getCantMaxBonusAnillo() const {
    return CantMaxBonusAnillo;
}

void Nivel::setCantMaxBonusAnillo(int CantMaxBonusAnillo) {
    Nivel::CantMaxBonusAnillo = CantMaxBonusAnillo;
}

int Nivel::getCantMaxEscudo() const {
    return CantMaxEscudo;
}

void Nivel::setCantMaxEscudo(int CantMaxEscudo) {
    Nivel::CantMaxEscudo = CantMaxEscudo;
}

int Nivel::getCantMinInvincibilidad() const {
    return CantMinInvincibilidad;
}

void Nivel::setCantMinInvincibilidad(int CantMinInvincibilidad) {
    Nivel::CantMinInvincibilidad = CantMinInvincibilidad;
}

int Nivel::getCantMinBonusAnillo() const {
    return CantMinBonusAnillo;
}

void Nivel::setCantMinBonusAnillo(int CantMinBonusAnillo) {
    Nivel::CantMinBonusAnillo = CantMinBonusAnillo;
}

int Nivel::getCantMinEscudo() const {
    return CantMinEscudo;
}

void Nivel::setCantMinEscudo(int CantMinEscudo) {
    Nivel::CantMinEscudo = CantMinEscudo;
}

int Nivel::getCantTotalInvincibilidad() const {
    return CantTotalInvincibilidad;
}

void Nivel::setCantTotalInvincibilidad(int CantTotalInvincibilidad) {
    Nivel::CantTotalInvincibilidad = CantTotalInvincibilidad;
}

int Nivel::getCantTotalBonusAnillo() const {
    return CantTotalBonusAnillo;
}

void Nivel::setCantTotalBonusAnillo(int CantTotalBonusAnillo) {
    Nivel::CantTotalBonusAnillo = CantTotalBonusAnillo;
}

int Nivel::getCantTotalEscudo() const {
    return CantTotalEscudo;
}

void Nivel::setCantTotalEscudo(int CantTotalEscudo) {
    Nivel::CantTotalEscudo = CantTotalEscudo;
}
