#include "Estadisticas.h"

Estadisticas::Estadisticas() : tiempoSimTotal(0.0f) {}

/*
* Método para registrar la generación de un carro
* Observación: Se almacena el ID del carro y el tiempo de generación
* @param:
*   - int id: ID del carro generado
*   - float tiempoGen: Tiempo de generación del carro
*   - string placa: Placa del carro
*   - bool estado: Estado del carro (en pantalla o fuera de pantalla)
*   - string color: Color del carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarGeneracion(int id, float tiempoGen, string placa, bool estado, string color)
{
    registros.emplace_back(id, tiempoGen, -1.0f, -1.0f, -1.0f, estado, placa, color);
}

/*
* Método para registrar la llegada de un carro a la cola del peaje
* Observación: Se actualiza el tiempo de llegada en el registro correspondiente
* @param:
*   - int id: ID del carro que llega a la cola
*   - float tiempoLlegada: Tiempo de llegada del carro a la cola
*   - string placa: Placa del carro
*   - bool estado: Estado del carro (en pantalla o fuera de pantalla)
*   - string color: Color del carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarLlegadaCola(int id, float tiempoLlegada, string placa, bool estado, string color)
{
    for (auto& r : registros) 
    {
        if (get<0>(r) == id) 
        {
            get<2>(r) = tiempoLlegada;
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
*   - string placa: Placa del carro
*   - bool estado: Estado del carro (en pantalla o fuera de pantalla)
*   - string color: Color del carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarInicioServicio(int id, float tiempoInicio, string placa, bool estado, string color)
{
    (void)id; (void)tiempoInicio;
	(void)placa; (void)estado; (void)color;
}

/*
* Método para registrar la salida de un carro del peaje
* Observación: Se actualiza el tiempo de salida y el tiempo de servicio asignado en el registro correspondiente
* @param:
*   - int id: ID del carro que sale del peaje
*   - float tiempoSalida: Tiempo de salida del carro del peaje
*   - float tiempoServicioAsignado: Tiempo de servicio asignado al carro
*   - string placa: Placa del carro
*   - bool estado: Estado del carro (en pantalla o fuera de pantalla)
*   - string color: Color del carro
* @return:
*   + Ninguno
*/
void Estadisticas::registrarSalida(int id, float tiempoSalida, float tiempoServicioAsignado, string placa, bool estado, string color)
{
    for (auto& r : registros) 
    {
        if (get<0>(r) == id) 
        {
            get<3>(r) = tiempoSalida;
            get<4>(r) = tiempoServicioAsignado;
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
void Estadisticas::generarCSV(const string& nombreArchivo) 
{
    ofstream ofs(nombreArchivo);
	ofs << "ID,TiempoGenerado,TiempoLlegadaCola,TiempoSalida,TiempoServicio,Estado,Placa,Color\n";
    ofs << fixed << setprecision(3);
    for (auto& r : registros)
    {
        ofs << get<0>(r) << ","
            << get<1>(r) << ","
            << get<2>(r) << ","
            << get<3>(r) << ","
            << get<4>(r) << ","
			<< (get<5>(r) ? "En Pantalla" : "Fuera de Pantalla") << ","
            << get<6>(r) << ","
			<< get<7>(r) << "\n";

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
    for (auto& r : registros) if (get<3>(r) >= 0.0f) ++cnt;
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
        float llegada = get<2>(r);
        float salida = get<3>(r);
        float servicio = get<4>(r);
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
        float gen = get<1>(r);
        float salida = get<3>(r);
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
