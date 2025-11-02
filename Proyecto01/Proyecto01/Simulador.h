#pragma once
#include <vector>
#include <memory>
#include "Carro.h"
#include "CabinaPeaje.h"
#include "Estadisticas.h"

class Simulador {
private:
    std::vector<std::shared_ptr<Carro>> vehiculos; 
    std::vector<CabinaPeaje> cabinas;
    Estadisticas estad;
    float tiempoSim; 
    int nextId;
    float probGeneracionPorSegundo; 
    float tiempoZonaPeajeX; 
    float simSpeed; 

public:
    Simulador(int numCabinas = 5, float pGen = 0.5f);
    ~Simulador();
    void actualizar(float dt); 
    void generarVehiculosAleatorios(float dt);
    void procesarFinalizados(float dt);
    Estadisticas& getEstadisticas() { return estad; }
    float getTiempoSim() const { return tiempoSim; }
    int getNextId() { return nextId++; }
    void agregarVehiculo(std::shared_ptr<Carro> nuevo);


    const std::vector<CabinaPeaje>& getCabinas() const;
};


