#include "Estadisticas.h"
#include <fstream>
#include <iomanip>
#include <algorithm>

Estadisticas::Estadisticas() : tiempoSimTotal(0.0f) {}

void Estadisticas::registrarGeneracion(int id, float tiempoGen) {
    registros.emplace_back(id, tiempoGen, -1.0f, -1.0f, -1.0f);
}

void Estadisticas::registrarLlegadaCola(int id, float tiempoLlegada) {
    for (auto& r : registros) {
        if (std::get<0>(r) == id) {
            std::get<2>(r) = tiempoLlegada;
            break;
        }
    }
}

void Estadisticas::registrarInicioServicio(int id, float tiempoInicio) {
    (void)id; (void)tiempoInicio;
}

void Estadisticas::registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado) {
    for (auto& r : registros) {
        if (std::get<0>(r) == id) {
            std::get<3>(r) = tiempoSalida;
            std::get<4>(r) = tiempoServicioAsignado;
            break;
        }
    }
}
/*
* Método para generar un archivo CSV con las estadísticas de la simulación
* Observación: El archivo CSV contendrá columnas como ID del carro, tiempo de generación, tiempo de llegada, tiempo de salida, etc.
* @param:
*   - const string& pNombreArchivo: Nombre del archivo CSV a generar
* @return:
*   + Ninguno
*/
void Estadisticas::generarCSV(const std::string& nombreArchivo) {
    std::ofstream ofs(nombreArchivo);
    ofs << "ID,TiempoGenerado,TiempoLlegadaCola,TiempoSalida,TiempoServicio\n";
    ofs << std::fixed << std::setprecision(3);
    for (auto& r : registros) {
        ofs << std::get<0>(r) << ","
            << std::get<1>(r) << ","
            << std::get<2>(r) << ","
            << std::get<3>(r) << ","
            << std::get<4>(r) << "\n";
    }
    ofs.close();
}

int Estadisticas::totalProcesados() const {
    int cnt = 0;
    for (auto& r : registros) if (std::get<3>(r) >= 0.0f) ++cnt;
    return cnt;
}

float Estadisticas::tiempoPromedioEspera() const {
    float total = 0.0f; int cnt = 0;
    for (auto& r : registros) {
        float llegada = std::get<2>(r);
        float salida = std::get<3>(r);
        float servicio = std::get<4>(r);
        if (llegada >= 0 && salida >= 0) {
            float espera = (salida - llegada) - servicio;
            total += espera;
            ++cnt;
        }
    }
    return cnt ? total / cnt : 0.0f;
}

float Estadisticas::tiempoPromedioSistema() const {
    float total = 0.0f; int cnt = 0;
    for (auto& r : registros) {
        float gen = std::get<1>(r);
        float salida = std::get<3>(r);
        if (salida >= 0) {
            total += (salida - gen);
            ++cnt;
        }
    }
    return cnt ? total / cnt : 0.0f;
}

float Estadisticas::tasaFlujoPromedio() const {
    float minutos = (tiempoSimTotal > 0.0f) ? (tiempoSimTotal / 60.0f) : 1.0f;
    int procesados = totalProcesados();
    return minutos > 0.0f ? (static_cast<float>(procesados) / minutos) : 0.0f;
}
