#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <random>
#include "Estadisticas.h"


class Carro;

#include "Carro.h"

class CabinaPeaje
{
private:
    queue<shared_ptr<Carro>> cola;
    bool ocupada;
    float tiempoRestanteServicio; 
    float tiempoTotalOcupada; 
    int cantidadCarrosAtendidos;
    float tiempoAtencionMin, tiempoAtencionMax; 

    shared_ptr<Carro> enServicio;

	// floats para posicion y espaciado
    float posX;
    float posY;
    float spacing; 

    void reposicionarCola();

public:
    CabinaPeaje(float pMin = 2.0f, float pMax = 5.0f, float px = 900.0f, float py = 290.0f, float pSpacing = 40.0f);
    ~CabinaPeaje() = default;

    void encolar(shared_ptr<Carro> carro);
    int colaSize() const;
    bool estaOcupada() const;
    void actualizar(float dt, float tiempoSim, class Estadisticas& estad); 
    void iniciarServicio(shared_ptr<Carro> carro, float servicioAsignado, float tiempoSim, class Estadisticas& estad);
    float getTiempoTotalOcupada() const;
    int getCantidadCarrosAtendidos() const;

    // Getters
    float getPosX() const;
    float getPosY() const;
};

string obtenerColorCarro(shared_ptr<Carro> nuevo);