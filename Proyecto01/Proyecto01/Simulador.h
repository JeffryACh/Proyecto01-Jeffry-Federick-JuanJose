#pragma once
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Carro.h"
#include "CabinaPeaje.h"
#include "Estadisticas.h"

class Simulador 
{
private:
    vector<shared_ptr<Carro>> vehiculos; 
    vector<CabinaPeaje> cabinas;
    Estadisticas estad;
    float tiempoSim; 
    int nextId;
    float probGeneracionPorSegundo; 
    float tiempoZonaPeajeX; 
    float simSpeed; 

    std::vector<std::shared_ptr<Carro>> nuevosVehiculos;

public:
    Simulador(int numCabinas = 5, float pGen = 0.5f);
    ~Simulador();
    void actualizar(float dt); 
    void generarVehiculosAleatorios(float dt);
    void procesarFinalizados(float dt);
    Estadisticas& getEstadisticas() { return estad; }
    float getTiempoSim() const { return tiempoSim; }
    int getNextId() { return nextId++; }
    void agregarVehiculo(shared_ptr<Carro> nuevo);

    const vector<CabinaPeaje>& getCabinas() const;

    std::vector<std::shared_ptr<Carro>> obtenerVehiculosGenerados();

    void setProbGeneracion(float p) { probGeneracionPorSegundo = p; }
    float getProbGeneracion() const { return probGeneracionPorSegundo; }
};

string obtenerColorCarro(shared_ptr<Carro> nuevo);