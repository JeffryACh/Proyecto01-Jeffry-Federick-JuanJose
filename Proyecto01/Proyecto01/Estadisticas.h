#pragma once
#include <vector>
#include <string>
#include <tuple>

class Estadisticas {
private:
    std::vector<std::tuple<int, float, float, float, float>> registros;

    float tiempoSimTotal; 
public:
    Estadisticas();
    void registrarGeneracion(int id, float tiempoGen);
    void registrarLlegadaCola(int id, float tiempoLlegada);
    void registrarInicioServicio(int id, float tiempoInicio); 
    void registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado);
    void generarCSV(const std::string& nombreArchivo);
    int totalProcesados() const;
    float tiempoPromedioEspera() const;
    float tiempoPromedioSistema() const;
    float tasaFlujoPromedio() const;
    void setTiempoSimTotal(float t) { tiempoSimTotal = t; }
};
