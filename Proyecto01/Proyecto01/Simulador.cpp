#include "Simulador.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <memory>

Simulador::Simulador(int numCabinas, float pGen)
    : tiempoSim(0.0f), nextId(1), probGeneracionPorSegundo(pGen), tiempoZonaPeajeX(900.0f), simSpeed(1.0f)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    float baseX = 900.0f;
    float baseY = 150.0f;
    float spacing = 60.0f;
    for (int i = 0; i < numCabinas; ++i) {
        cabinas.emplace_back(2.0f, 5.0f, baseX + i * 60.0f, baseY + i * 0.0f, 36.0f);
    }
}

Simulador::~Simulador() {
    vehiculos.clear(); 
}

void Simulador::agregarVehiculo(std::shared_ptr<Carro> nuevo) {
    if (nuevo != nullptr) {
        vehiculos.push_back(nuevo);
        estad.registrarGeneracion(nuevo->getId(), tiempoSim);
    }
}

void Simulador::generarVehiculosAleatorios(float dt) {

    float pTick = 1.0f - powf(1.0f - probGeneracionPorSegundo, dt);
    float r = static_cast<float>(rand()) / RAND_MAX;
    if (r < pTick) {
        auto c = std::make_shared<Carro>();
        c->setId(nextId++);
        c->setTiempoGenerado(tiempoSim);
        c->setTiempo(tiempoSim); 
        vehiculos.push_back(c);
        estad.registrarGeneracion(c->getId(), tiempoSim);
    }
}

void Simulador::procesarFinalizados(float dt) {
    for (auto v : vehiculos) {
        if (v->getCabinaAsignada() != nullptr) {
            if (v->getWaitTimer() <= 0.0f && v->getTiempoServicioAsignado() > 0.0f) {
                v->setTiempoSalida(tiempoSim);
                estad.registrarSalida(v->getId(), tiempoSim, v->getTiempoServicioAsignado());
                v->setCabinaAsignada(nullptr);
                v->setTiempoServicioAsignado(0.0f);
                v->setPeajeCooldown(3.0f);
                const float escapeDist = 48.0f;
                if (!v->getEstado()) {
                    v->setPosicion(tiempoZonaPeajeX - escapeDist, v->getPosicionY());
                } else {
                    v->setPosicion(v->getPosicionX(), v->getPosicionY() + escapeDist);
                }
            }
        }
    }
}

void Simulador::actualizar(float dt) {
    tiempoSim += dt;
    generarVehiculosAleatorios(dt);

    for (auto& cab : cabinas) {
        cab.actualizar(dt, tiempoSim, estad);
    }

    for (auto v : vehiculos) {
        float cd = v->getPeajeCooldown();
        if (cd > 0.0f) {
            v->setPeajeCooldown(std::max(0.0f, cd - dt));
        }

        float wt = v->getWaitTimer();
        if (wt > 0.0f) {
            v->setWaitTimer(std::max(0.0f, wt - dt));
        }

        if (v->getCabinaAsignada() == nullptr && v->getPeajeCooldown() <= 0.0f) {
            if (v->getPosicionX() >= tiempoZonaPeajeX) {
                int idx = v->decidirCabina(cabinas);
                cabinas[idx].encolar(v);
                v->setTiempoLlegadaCola(tiempoSim);
                estad.registrarLlegadaCola(v->getId(), tiempoSim);
            }
        }
    }

    procesarFinalizados(dt);

    estad.setTiempoSimTotal(tiempoSim);
}

const std::vector<CabinaPeaje>& Simulador::getCabinas() const {
    return cabinas;
}
