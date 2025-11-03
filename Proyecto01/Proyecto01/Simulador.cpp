#include "Simulador.h"

/*
* Constructor de la clase Simulador
* Observación: Inicializa el simulador con un número dado de cabinas y una probabilidad de generación de vehículos por segundo
* @param:
*   - int numCabinas: Número de cabinas de peaje en el simulador
*   - float pGen: Probabilidad de generación de vehículos por segundo
* @return:
*   + Ninguno
*/
Simulador::Simulador(int numCabinas, float pGen)
    : tiempoSim(0.0f), nextId(1), probGeneracionPorSegundo(pGen), tiempoZonaPeajeX(900.0f), simSpeed(1.0f)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    float baseX = 900.0f;
    float baseY = 150.0f;
    float spacing = 60.0f;
    for (int i = 0; i < numCabinas; ++i)
    {
        cabinas.emplace_back(2.0f, 5.0f, baseX + i * 60.0f, baseY + i * 0.0f, 36.0f);
    }
}

/*
* Destructor de la clase Simulador
* Observación: Limpia los recursos utilizados por el simulador
* @param:
*   - Ninguno
* @return:
*   + Ninguno
*/
Simulador::~Simulador() 
{
    vehiculos.clear(); 
}

/*
* Método para agregar un vehículo al simulador
* Observación: Agrega un nuevo vehículo a la lista de vehículos del simulador y registra su generación en las estadísticas
* @param:
*   - shared_ptr<Carro> nuevo: Puntero compartido al nuevo vehículo a agregar
* @return:
*   + Ninguno
*/
void Simulador::agregarVehiculo(shared_ptr<Carro> nuevo) 
{
    if (nuevo != nullptr) 
    {
        vehiculos.push_back(nuevo);
        estad.registrarGeneracion(nuevo->getId(), tiempoSim);
    }
}

/*
* Método para generar vehículos aleatorios en el simulador
* Observación: Genera vehículos basados en una probabilidad por segundo y los agrega al simulador
* @param:
*   - float dt: Delta time desde la última actualización
* @return:
*   + Ninguno
*/
void Simulador::generarVehiculosAleatorios(float dt)
{
    float pTick = 1.0f - powf(1.0f - probGeneracionPorSegundo, dt);
    float r = static_cast<float>(rand()) / RAND_MAX;
    if (r < pTick) 
    {
        auto c = make_shared<Carro>();
        c->setId(nextId++);
        c->setTiempoGenerado(tiempoSim);
        c->setTiempo(tiempoSim); 
        vehiculos.push_back(c);
        estad.registrarGeneracion(c->getId(), tiempoSim);
    }
}

/*
* Método para procesar vehículos que han finalizado su servicio en el peaje
* Observación: Revisa los vehículos que han terminado su servicio y los libera de la cabina asignada
* @param:
*   - float dt: Delta time desde la última actualización
* @return:
*   + Ninguno
*/
void Simulador::procesarFinalizados(float dt) 
{
    for (auto v : vehiculos) 
    {
        if (v->getCabinaAsignada() != nullptr) 
        {
            if (v->getWaitTimer() <= 0.0f && v->getTiempoServicioAsignado() > 0.0f) 
            {
                v->setTiempoSalida(tiempoSim);
                estad.registrarSalida(v->getId(), tiempoSim, v->getTiempoServicioAsignado());
                v->setCabinaAsignada(nullptr);
                v->setTiempoServicioAsignado(0.0f);
                v->setPeajeCooldown(3.0f);
                const float escapeDist = 48.0f;
                if (!v->getEstado()) 
                {
                    v->setPosicion(tiempoZonaPeajeX - escapeDist, v->getPosicionY());
                } else
                {
                    v->setPosicion(v->getPosicionX(), v->getPosicionY() + escapeDist);
                }
            }
        }
    }
}

/*
* Método para actualizar el estado del simulador
* Observación: Actualiza el tiempo de simulación, genera vehículos aleatorios, actualiza las cabinas y procesa vehículos finalizados
* @param:
*   - float dt: Delta time desde la última actualización
* @return:
*   + Ninguno
*/
void Simulador::actualizar(float dt) 
{
    tiempoSim += dt;
    generarVehiculosAleatorios(dt);

    for (auto& cab : cabinas) 
    {
        cab.actualizar(dt, tiempoSim, estad);
    }

    for (auto v : vehiculos) 
    {
        float cd = v->getPeajeCooldown();
        if (cd > 0.0f) 
            v->setPeajeCooldown(max(0.0f, cd - dt));

        float wt = v->getWaitTimer();
        if (wt > 0.0f) 
            v->setWaitTimer(max(0.0f, wt - dt));

        if (v->getCabinaAsignada() == nullptr && v->getPeajeCooldown() <= 0.0f)
        {
            if (v->getPosicionX() >= tiempoZonaPeajeX) 
            {
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

/*
* Método para obtener las cabinas de peaje del simulador
* Observación: Devuelve una referencia constante al vector de cabinas de peaje
* @param:
*   - Ninguno
* @return:
*   + const vector<CabinaPeaje>&: Referencia constante al vector de cabinas de peaje
*/
const vector<CabinaPeaje>& Simulador::getCabinas() const 
{
    return cabinas;
}
