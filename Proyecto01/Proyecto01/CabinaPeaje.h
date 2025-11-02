#pragma once
#include <queue>
#include <vector>
#include <memory>


class Carro;

#include "Carro.h"

class CabinaPeaje {
private:
    std::queue<std::shared_ptr<Carro>> cola;
    bool ocupada;
    float tiempoRestanteServicio; 
    float tiempoTotalOcupada; 
    int cantidadCarrosAtendidos;
    float tiempoAtencionMin, tiempoAtencionMax; 

    std::shared_ptr<Carro> enServicio;

	// floats para posicion y espaciado
    float posX;
    float posY;
    float spacing; 

    void reposicionarCola();

public:
    CabinaPeaje(float pMin = 2.0f, float pMax = 5.0f, float px = 900.0f, float py = 290.0f, float pSpacing = 40.0f);
    ~CabinaPeaje() = default;

    void encolar(std::shared_ptr<Carro> carro);
    int colaSize() const;
    bool estaOcupada() const;
    void actualizar(float dt, float tiempoSim, class Estadisticas& estad); 
    void iniciarServicio(std::shared_ptr<Carro> carro, float servicioAsignado, float tiempoSim, class Estadisticas& estad);
    float getTiempoTotalOcupada() const;
    int getCantidadCarrosAtendidos() const;

    // Getters
    float getPosX() const;
    float getPosY() const;
};
