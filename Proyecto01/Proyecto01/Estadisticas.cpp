#include "Estadisticas.h"
#include <fstream>
#include <iomanip>
#include <algorithm>

Estadisticas::Estadisticas() : tiempoSimTotal(0.0f) {}

/*
* Método para registrar la generación de un carro
* Observación: Se almacena el ID del carro y el tiempo de generación
* @param:
*   - int id: ID del carro generado
*   - float tiempoGen: Tiempo de generación del carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarGeneracion(int id, float tiempoGen)
{
    registros.emplace_back(id, tiempoGen, -1.0f, -1.0f, -1.0f);
}

/*
* Método para registrar la llegada de un carro a la cola del peaje
* Observación: Se actualiza el tiempo de llegada en el registro correspondiente
* @param:
*   - int id: ID del carro que llega a la cola
*   - float tiempoLlegada: Tiempo de llegada del carro a la cola
* @return:
*   + Ninguno
*/
void Estadisticas::registrarLlegadaCola(int id, float tiempoLlegada) 
{
    for (auto& r : registros) 
    {
        if (std::get<0>(r) == id) 
        {
            std::get<2>(r) = tiempoLlegada;
            break;
        }
    }
}

/*
* Método para registrar el inicio del servicio de un carro en el peaje
* Observación: Actualmente no realiza ninguna acción, pero está definido para futuras implementaciones
* @param:
*   - int id: ID del carro que inicia el servicio
*   - float tiempoInicio: Tiempo de inicio del servicio
* @return:
*   + Ninguno
*/
void Estadisticas::registrarInicioServicio(int id, float tiempoInicio) 
{
    (void)id; (void)tiempoInicio;
}

/*
* Método para registrar la salida de un carro del peaje
* Observación: Se actualiza el tiempo de salida y el tiempo de servicio asignado en el registro correspondiente
* @param:
*   - int id: ID del carro que sale del peaje
*   - float tiempoSalida: Tiempo de salida del carro del peaje
*   - float tiempoServicioAsignado: Tiempo de servicio asignado al carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado) 
{
    for (auto& r : registros) 
    {
        if (std::get<0>(r) == id) 
        {
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
void Estadisticas::generarCSV(const std::string& nombreArchivo) 
{
    std::ofstream ofs(nombreArchivo);
    ofs << "ID,TiempoGenerado,TiempoLlegadaCola,TiempoSalida,TiempoServicio\n";
    ofs << std::fixed << std::setprecision(3);
    for (auto& r : registros)
    {
        ofs << std::get<0>(r) << ","
            << std::get<1>(r) << ","
            << std::get<2>(r) << ","
            << std::get<3>(r) << ","
            << std::get<4>(r) << "\n";
    }
    ofs.close();
}

/*
* Método para calcular el total de carros procesados en el peaje
* Observación: Se cuenta el número de carros que han salido del peaje
* @param:
*   - Ninguno
* @return:
*   + int: Total de carros procesados
*/
int Estadisticas::totalProcesados() const 
{
    int cnt = 0;
    for (auto& r : registros) if (std::get<3>(r) >= 0.0f) ++cnt;
    return cnt;
}

/*
* Método para calcular el tiempo promedio de espera en la cola del peaje
* Observación: Se calcula el tiempo promedio que los carros han esperado en la cola antes de ser atendidos
* @param:
*   - Ninguno
* @return:
*   + float: Tiempo promedio de espera
*/
float Estadisticas::tiempoPromedioEspera() const 
{
    float total = 0.0f; int cnt = 0;
    for (auto& r : registros) 
    {
        float llegada = std::get<2>(r);
        float salida = std::get<3>(r);
        float servicio = std::get<4>(r);
        if (llegada >= 0 && salida >= 0) 
        {
            float espera = (salida - llegada) - servicio;
            total += espera;
            ++cnt;
        }
    }
    return cnt ? total / cnt : 0.0f;
}

/*
* Método para calcular el tiempo promedio en el sistema (desde generación hasta salida)
* Observación: Se calcula el tiempo promedio que los carros han pasado en el sistema desde su generación hasta su salida
* @param:
*   - Ninguno
* @return:
*   + float: Tiempo promedio en el sistema
*/
float Estadisticas::tiempoPromedioSistema() const 
{
    float total = 0.0f; int cnt = 0;
    for (auto& r : registros) 
    {
        float gen = std::get<1>(r);
        float salida = std::get<3>(r);
        if (salida >= 0)
        {
            total += (salida - gen);
            ++cnt;
        }
    }
    return cnt ? total / cnt : 0.0f;
}

/*
* Método para calcular la tasa de flujo promedio (carros por minuto)
* Observación: Se calcula la tasa de flujo promedio de carros procesados por minuto en el peaje
* @param:
*   - Ninguno
* @return:
*   + float: Tasa de flujo promedio (carros por minuto)
*/
float Estadisticas::tasaFlujoPromedio() const 
{
    float minutos = (tiempoSimTotal > 0.0f) ? (tiempoSimTotal / 60.0f) : 1.0f;
    int procesados = totalProcesados();
    return minutos > 0.0f ? (static_cast<float>(procesados) / minutos) : 0.0f;
}
