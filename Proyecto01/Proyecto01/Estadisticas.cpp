#include "Estadisticas.h"
#include <fstream>

void Estadisticas::registrarEventos(Simulador& pSimulador) 
{
    // Registrar eventos de simulación
}

/*
* Método para generar un archivo CSV con las estadísticas de la simulación
* Observación: El archivo CSV contendrá columnas como ID del carro, tiempo de generación, tiempo de llegada, tiempo de salida, etc.
* @param:
*   - const string& pNombreArchivo: Nombre del archivo CSV a generar
* @return:
*   + Ninguno
*/
void Estadisticas::generarCSV(const string& pNombreArchivo)
{
    ofstream archivo(pNombreArchivo);
    if (archivo.is_open()) {
        archivo << "ID,Tiempo Generado,Tiempo Llegada,Tiempo Salida\n";
        // Escribir datos
        archivo.close();
    }
}

void Estadisticas::calcularMetricas() 
{
    // Calcular métricas de rendimiento
}