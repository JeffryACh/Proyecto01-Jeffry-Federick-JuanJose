#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Estadisticas {
private:
	vector<tuple<int, float, float, float, float>> registros; // id, tiempoGeneracion, tiempoLlegadaCola, tiempoInicioServicio, tiempoSalida

    float tiempoSimTotal; 
public:
    Estadisticas();
    void registrarGeneracion(int id, float tiempoGen);
    void registrarLlegadaCola(int id, float tiempoLlegada);
    void registrarInicioServicio(int id, float tiempoInicio); 
    void registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado);
    void generarCSV(const string& nombreArchivo);
    int totalProcesados() const;
    float tiempoPromedioEspera() const;
    float tiempoPromedioSistema() const;
    float tasaFlujoPromedio() const;
    void setTiempoSimTotal(float t) { tiempoSimTotal = t; }
};
