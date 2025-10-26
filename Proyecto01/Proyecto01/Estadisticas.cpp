#include "Estadisticas.h"
#include <fstream>

void Estadisticas::registrarEventos(Simulador& pSimulador) 
{
    // Registrar eventos de simulaci�n
}

/*
* M�todo para generar un archivo CSV con las estad�sticas de la simulaci�n
* Observaci�n: El archivo CSV contendr� columnas como ID del carro, tiempo de generaci�n, tiempo de llegada, tiempo de salida, etc.
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
    // Calcular m�tricas de rendimiento
}