#include "Simulador.h"

Simulador::Simulador() {}

void Simulador::iniciar() {
    // Inicializar carriles y cabinas
}

void Simulador::actualizar(float pTiempo) {
    for (auto& carril : carriles) {
        carril.actualizarPosiciones(pTiempo);
    }
    // Actualizar cabinas
}

void Simulador::detener() {
    // Finalizar simulación
}