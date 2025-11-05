#include "CabinaPeaje.h"


CabinaPeaje::CabinaPeaje(float pMin, float pMax, float px, float py, float pSpacing)
    : ocupada(false), tiempoRestanteServicio(0.0f),
    tiempoTotalOcupada(0.0f), cantidadCarrosAtendidos(0),
    tiempoAtencionMin(pMin), tiempoAtencionMax(pMax),
    enServicio(nullptr),
    posX(px), posY(py), spacing(pSpacing)
{
}

/*
* Metodo para reposicionar los carros en la cola de la cabina de peaje
* Observación: Recalcula las posiciones de los carros en la cola basándose en su orden
* @param:
*   - Ninguno
* @return:
*   + Ninguno
*/
void CabinaPeaje::reposicionarCola() {
    vector<shared_ptr<Carro>> temp;
    while (!cola.empty()) {
        temp.push_back(cola.front());
        cola.pop();
    }
    for (size_t i = 0; i < temp.size(); ++i) {
        auto& carro = temp[i];
        if (!carro) continue;
        if (!carro->getEstado()) {
            float x = posX - static_cast<float>(i + 1) * spacing;
            float y = carro->getPosicionY(); 
            carro->setPosicion(x, y);
        } else {
            float x = carro->getPosicionX(); 
            float y = posY - static_cast<float>(i + 1) * spacing;
            carro->setPosicion(x, y);
        }
        cola.push(carro);
    }
}

/*
* Método para encolar un carro en la cabina de peaje
* Observación: Asigna la cabina al carro y lo añade a la cola
* @param:
*   - shared_ptr<Carro> carro: Carro a encolar
* @return:
*   + Ninguno
*/
void CabinaPeaje::encolar(shared_ptr<Carro> carro)
{
    if (!carro) return;
    carro->setCabinaAsignada(this);
    cola.push(carro);
    reposicionarCola();
    carro->setTiempoLlegadaCola(carro->getTiempo());
}

/*
* Método para obtener el tamaño de la cola de la cabina de peaje
* Observación: Retorna el número de carros en la cola
* @param:
*   - Ninguno
* @return:
*   + int: Tamaño de la cola
*/
int CabinaPeaje::colaSize() const
{
    return static_cast<int>(cola.size());
}

/*
* Método para verificar si la cabina de peaje está ocupada
* Observación: Retorna el estado de ocupación de la cabina
* @param:
*   - Ninguno
* @return:
*   + bool: true si está ocupada, false en caso contrario
*/
bool CabinaPeaje::estaOcupada() const 
{
    return ocupada;
}

/*
* Método para iniciar el servicio de un carro en la cabina de peaje
* Observación: Configura el servicio del carro y actualiza las estadísticas
* @param:
*   - shared_ptr<Carro> carro: Carro a atender
*   - float servicioAsignado: Tiempo de servicio asignado
*   - float tiempoSim: Tiempo de simulación actual
*   - Estadisticas& estad: Objeto de estadísticas para registrar eventos
* @return:
*   + Ninguno
*/
void CabinaPeaje::iniciarServicio(shared_ptr<Carro> carro, float servicioAsignado, float tiempoSim, Estadisticas& estad) 
{
    if (!carro) return;
    ocupada = true;
    tiempoRestanteServicio = servicioAsignado;
    cantidadCarrosAtendidos++;
    estad.registrarInicioServicio(carro->getId(), tiempoSim, carro->getPlaca(), carro->getEstado(), obtenerColorCarro(carro));
    enServicio = carro;
    carro->setTiempoServicioAsignado(servicioAsignado);
    carro->setWaitTimer(servicioAsignado);
    carro->setCabinaAsignada(this);
    if (!carro->getEstado())
        carro->setPosicion(posX, carro->getPosicionY());
    else 
        carro->setPosicion(carro->getPosicionX(), posY);
}

/*
* Método para actualizar el estado de la cabina de peaje
* Observación: Actualiza el tiempo de servicio y maneja la cola de carros
* @param:
*   - float dt: Delta time desde la última actualización
*   - float tiempoSim: Tiempo de simulación actual
*   - Estadisticas& estad: Objeto de estadísticas para registrar eventos
* @return:
*   + Ninguno
*/
void CabinaPeaje::actualizar(float dt, float tiempoSim, Estadisticas& estad) 
{
    if (ocupada) 
    {
        float reduccion = dt;
        tiempoRestanteServicio -= reduccion;
        tiempoTotalOcupada += reduccion;
        if (tiempoRestanteServicio <= 0.0f) 
        {
            ocupada = false;
            tiempoRestanteServicio = 0.0f;
            enServicio = nullptr;
        }
    }
    if (!ocupada && !cola.empty())
    {
        shared_ptr<Carro> siguiente = cola.front();
        cola.pop();
        static mt19937 rng((random_device())());
        uniform_real_distribution<float> dist(tiempoAtencionMin, tiempoAtencionMax);
        float servicio = dist(rng);
        iniciarServicio(siguiente, servicio, tiempoSim, estad);

        
        reposicionarCola();
    }
}

// Getters
float CabinaPeaje::getTiempoTotalOcupada() const { return tiempoTotalOcupada; }
int CabinaPeaje::getCantidadCarrosAtendidos() const { return cantidadCarrosAtendidos; }

// Setters
float CabinaPeaje::getPosX() const { return posX; }
float CabinaPeaje::getPosY() const { return posY; }
