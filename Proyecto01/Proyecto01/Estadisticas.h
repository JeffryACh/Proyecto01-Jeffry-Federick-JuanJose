#pragma once
#include <vector>
#include <string>
#include <tuple>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

class Estadisticas 
{
private:
	vector<tuple<int, float, float, float, float, bool, string, string>> registros; // id, tiempoGeneracion, tiempoLlegadaCola, tiempoInicioServicio, tiempoSalida, estado, placa, color

    float tiempoSimTotal; 
public:
    Estadisticas();
    void registrarGeneracion(int id, float tiempoGen, string placa, bool estado, string color);
    void registrarLlegadaCola(int id, float tiempoLlegada, string placa, bool estado, string color);
    void registrarInicioServicio(int id, float tiempoInicio, string placa, bool estado, string color);
    void registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado, string placa, bool estado, string color);
    void generarCSV(const string& nombreArchivo);
    int totalProcesados() const;
    float tiempoPromedioEspera() const;
    float tiempoPromedioSistema() const;
    float tasaFlujoPromedio() const;
    void setTiempoSimTotal(float t) { tiempoSimTotal = t; }
};
