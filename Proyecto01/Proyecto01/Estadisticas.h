#pragma once
#include "Simulador.h"

using namespace std;

class Estadisticas
{
public:
	void registrarEventos(Simulador& pSimulador);
	void generarCSV(const string& pNombreArchivo);
	void caldularMetricas();
}