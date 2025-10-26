#pragma once
#include "Carro.h"

using namespace std;

class CabinaPeaje {
private:
    float tiempoAtencion; // Tiempo en horas que tarda en atender un carro
    int cantidadCarrosAtendidos;

public:
    CabinaPeaje(float pTiempoAtencion);

    // Getters
    float getTiempoAtencion() const;
    int getCantidadCarrosAtendidos() const;

    // Setters
    void setTiempoAtencion(float pTiempoAtencion);
    void setCantidadCarrosAtendidos(int pCantidadCarrosAtendidos);

    // Métodos
    void procesarCarro(Carro* pCarro);
};