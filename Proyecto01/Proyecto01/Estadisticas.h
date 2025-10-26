#pragma once
#include "Simulador.h"
#include <string>

using namespace std;

class Estadisticas {
public:
    void registrarEventos(Simulador& pSimulador);
    void generarCSV(const string& pNombreArchivo);
    void calcularMetricas();
};