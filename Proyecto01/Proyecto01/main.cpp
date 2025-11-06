/*
* Proyecto 01: Simulador de Flujo de Tráfico y Peaje
* @Authors:
*   - Jeffry Araya Ch.
*   - Federick Fernandez C.
*   - Juan J. Rojas
*
* Created: 28/09/2025 15:40
* Modified: 06/11/2025 18:00
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <memory>
#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include "Simulador.h"

Simulador sim(5 , 0.6f );

#include "Carro.h"

//NOTA IMPORTANTE:  Cambien la imagen del peaje cuando ya este subido este archivo en el repo :)
using namespace std;

const int SCREEN_W = 1200;
const int SCREEN_H = 600;
const float FPS = 60.0f;

/*
* Dibuja el fondo escalado a la pantalla
* Observación: Utiliza al_draw_scaled_bitmap para ajustar la imagen al tamaño de la ventana
* @param:
*   - ALLEGRO_BITMAP* fondo: Puntero a la imagen del fondo
* @return:
*   + Ninguno
*/
void dibujarFondo(ALLEGRO_BITMAP* fondo) 
{
    if (fondo)
    {
        al_draw_scaled_bitmap(
            fondo,
            0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo),
            0, 0, SCREEN_W, SCREEN_H,
            0
        );
    }
    else
    {
        al_clear_to_color(al_map_rgb(50, 50, 50));
    }
}

/*
* Dibuja todos los autos en la pantalla
* Observación: Itera sobre el vector de autos y llama al método dibujar de cada auto
* @param:
*   - const vector<shared_ptr<Carro>>& autos: Vector de punteros compartidos a los autos
* @return:
*   + Ninguno
*/
void dibujarAutos(const vector<shared_ptr<Carro>>& autos) 
{
    for (const auto& a : autos)
        a->dibujar();
}

/*
* Verifica si hay un carro adelante del carro actual
* Observación: Compara la posición del carro actual con la de los otros carros en el mismo carril
* @param:
*   - const Carro& actual: Referencia al carro actual
*   - const vector<shared_ptr<Carro>>& autos: Vector de punteros compartidos a los autos
*   - float distanciaMinima: Distancia mínima para considerar que hay un carro adelante
*   - float toleranciaCarril: Tolerancia para determinar si están en el mismo carril
* @return:
*   + bool: true si hay un carro adelante, false en caso contrario
*/
bool hayCarroAdelante(const Carro& actual, const vector<shared_ptr<Carro>>& autos, float distanciaMinima, float toleranciaCarril)
{
    if (!actual.getEstado())
    {
        for (const auto& otroPtr : autos)
        {
            const Carro* otro = otroPtr.get();
            if (otro == &actual) continue;

            if (fabs(actual.getPosicionY() - otro->getPosicionY()) > toleranciaCarril) continue;

            float dx = otro->getPosicionX() - actual.getPosicionX();
            if (dx <= 0.0f) continue;

            float frenteActual = actual.getPosicionX() + actual.getAncho();
            float colaOtro = otro->getPosicionX();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }
    else // vertical 
    {
        for (const auto& otroPtr : autos)
        {
            const Carro* otro = otroPtr.get();
            if (otro == &actual) continue;

            if (fabs(actual.getPosicionX() - otro->getPosicionX()) > toleranciaCarril) continue;

            float dy = otro->getPosicionY() - actual.getPosicionY();
            if (dy <= 0.0f) continue;

            float frenteActual = actual.getPosicionY() + actual.getAlto();
            float colaOtro = otro->getPosicionY();
            float separacion = colaOtro - frenteActual;

            if (separacion < distanciaMinima) return true;
        }
    }

    return false;
}

/*
* Resuelve solapamientos entre autos
* Observación: Ajusta las posiciones de los autos para evitar solapamientos
* @param:
*   - vector<shared_ptr<Carro>>& autos: Vector de punteros compartidos a los autos
* @return:
*   + Ninguno
*/
void resolverSolapamientos(vector<shared_ptr<Carro>>& autos)
{
    const float toleranciaCarril = 12.0f;

    for (size_t i = 0; i < autos.size(); ++i)
    {
        for (size_t j = i + 1; j < autos.size(); ++j)
        {
            Carro& a = *autos[i];
            Carro& b = *autos[j];


            if (a.getCabinaAsignada() != nullptr || b.getCabinaAsignada() != nullptr) continue;

            if (a.getEstado() != b.getEstado()) continue;

            if (!a.getEstado()) 
            {
                if (fabs(a.getPosicionY() - b.getPosicionY()) > toleranciaCarril) continue;

                float ax1 = a.getPosicionX();
                float ax2 = a.getPosicionX() + a.getAncho();
                float bx1 = b.getPosicionX();
                float bx2 = b.getPosicionX() + b.getAncho();

                if (!(ax2 < bx1 || ax1 > bx2))
                {
                    if (ax1 < bx1)
                    {
                        float nuevaX = bx1 - a.getAncho() - 1.0f;
                        a.setPosicion(nuevaX, a.getPosicionY());
                        a.setWaitTimer(1.0f);
                    }
                    else
                    {
                        float nuevaX = ax1 - (ax2 - bx1) - 1.0f;
                        b.setPosicion(nuevaX, b.getPosicionY());
                        b.setWaitTimer(1.0f);
                    }
                }
            }
            else // vertical
            {
                if (fabs(a.getPosicionX() - b.getPosicionX()) > toleranciaCarril) continue;

                float ay1 = a.getPosicionY();
                float ay2 = a.getPosicionY() + a.getAlto();
                float by1 = b.getPosicionY();
                float by2 = b.getPosicionY() + b.getAlto();

                if (!(ay2 < by1 || ay1 > by2))
                {
                    if (ay1 < by1)
                    {
                        float nuevaY = by1 - a.getAlto() - 1.0f;
                        a.setPosicion(a.getPosicionX(), nuevaY);
                        a.setWaitTimer(1.0f);
                    }
                    else
                    {
                        float nuevaY = ay1 - (ay2 - by1) - 1.0f;
                        b.setPosicion(b.getPosicionX(), nuevaY);
                        b.setWaitTimer(1.0f);
                    }
                }
            }
        }
    }
}

/*
* Método para generar una placa aleatoria
* Observación: La placa tiene el formato 3 letras '-' 3 números
* @param:
*   - Ninguno
* @return:
*   + string: Placa generada aleatoriamente
*/
string generarPlacaAleatoria() 
{
    string letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string numeros = "0123456789";
    string placaGenerada;
    for (int i = 0; i < 3; ++i)
        placaGenerada += letras[rand() % letras.size()];
    placaGenerada += '-';
    for (int i = 0; i < 3; ++i)
        placaGenerada += numeros[rand() % numeros.size()];
    return placaGenerada;
}

/*
* Función para verificar si una placa ya existe en un vector de carros
* Observación: Recorre el vector y compara las placas
* @param:
*   - const string& placa: Placa a verificar
*   - const vector<shared_ptr<Carro>>& autos: Vector de carros
* @return:
*   + bool: true si la placa existe, false en caso contrario
*/
bool exisePlaca(const string& placa, const vector<shared_ptr<Carro>>& autos)
{
    for (const auto& a : autos)
    {
        if (a->getPlaca() == placa)
            return true;
    }
    return false;
}

int main() 
{

    if (!al_init()) 
    {
        cerr << "No se pudo inicializar Allegro culpa de Rodirgo chavez" << endl;
        return 1;
    }

    if (!al_init_image_addon()) 
    {
        cerr << " No se pudo inicializar el addon de imágenes" << endl;
        return 1;
    }

    if (!al_install_keyboard()) 
    {
        cerr << "No se pudo inicializar el teclado." << endl;
        return 1;
    }

    if (!al_install_mouse())
    {
        cerr << "No se pudo inicializar el mouse." << endl;
        return 1;
    }

    // Inicializar addons para texto y primitivas (para dibujar el botón y campos)
    al_init_font_addon();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* ventana = al_create_display(SCREEN_W, SCREEN_H);
    if (!ventana) 
    {
        cerr << "No se pudo crear la ventana." << endl;
        return 1;
    }

    al_set_window_title(ventana, "Simulador de Flujo de Tráfico y Peaje");

    ALLEGRO_BITMAP* fondo = al_load_bitmap("Fondo.jpg");
    if (!fondo) 
    {
        cerr << "No se pudo cargar la imagen de fondo estres positivo" << endl;
        // continuar sin fondo (se dibuja fondo sólido)
    }

    ALLEGRO_BITMAP* imgAmarillo = al_load_bitmap("carroAmarillo.png");
    ALLEGRO_BITMAP* imgRojo = al_load_bitmap("carroRojo.png");
    ALLEGRO_BITMAP* imgAzul = al_load_bitmap("carroAzul.png");
    ALLEGRO_BITMAP* imgVerde = al_load_bitmap("carroVerde.png");

    if (!imgAmarillo || !imgRojo || !imgAzul || !imgVerde) 
    {
        cerr << "No se pudieron cargar las imágenes de los autos." << endl;
        if (fondo) al_destroy_bitmap(fondo);
        if (imgAmarillo) al_destroy_bitmap(imgAmarillo);
        if (imgRojo) al_destroy_bitmap(imgRojo);
        if (imgAzul) al_destroy_bitmap(imgAzul);
        if (imgVerde) al_destroy_bitmap(imgVerde);
        al_destroy_display(ventana);
        return 1;
    }

    ALLEGRO_BITMAP* imgPeaje = al_load_bitmap("casetaDePeaje.png");
    if (!imgPeaje) 
    {
        cerr << "No se pudo cargar la imagen del peaje (Image 2 nov 2025, 12_15_24.png)." << endl;
        imgPeaje = nullptr;
    }   

    // --- PANTALLA DE INICIO con inputs y botón "empezar" ---
    int serviceMin = 2;      // valor por defecto si el usuario no escribe nada
    int serviceMax = 5;      // valor por defecto si el usuario no escribe nada
    int simDurationMin = 5;  // valor por defecto (minutos)

    {
        ALLEGRO_EVENT_QUEUE* startQueue = al_create_event_queue();
        if (!startQueue)
        {
            cerr << "No se pudo crear la cola de eventos (start screen)." << endl;
            // continuar sin pantalla de inicio
        }
        else
        {
            al_register_event_source(startQueue, al_get_display_event_source(ventana));
            al_register_event_source(startQueue, al_get_mouse_event_source());
            al_register_event_source(startQueue, al_get_keyboard_event_source());

            // fuente integrada (falla suave si no existe)
            ALLEGRO_FONT* font = al_create_builtin_font();

            bool startPressed = false;
            bool redraw = true;
            int mouseX = 0, mouseY = 0;

            // botón
            const int btnW = 220;
            const int btnH = 72;
            const int btnX = (SCREEN_W - btnW) / 2;
            const int btnY = (SCREEN_H - btnH) / 2 + 120;

            // inputs: dos para rango (min, max) y uno para duración en minutos
            string bufMin;
            string bufMax;
            string bufDuration;
            int focusedField = 0; // 0=min, 1=max, 2=duration, 3=botón

            const int inputW = 120;
            const int inputH = 36;
            const int labelX = SCREEN_W / 2 - 290;
            const int fieldX = SCREEN_W / 2 - 120;
            const int startY = SCREEN_H / 2 - 80;
            const int spacingY = 56;

            // helper lambdas
            auto isDigitOrSign = [](int c) -> bool {
                return (c >= '0' && c <= '9') || c == '-' || c == '+';
            };

            while (!startPressed)
            {
                ALLEGRO_EVENT ev;
                if (al_get_next_event(startQueue, &ev))
                {
                    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                    {
                        // limpiar recursos y salir del programa
                        al_destroy_font(font);
                        al_destroy_event_queue(startQueue);
                        if (fondo) al_destroy_bitmap(fondo);
                        al_destroy_bitmap(imgAmarillo);
                        al_destroy_bitmap(imgRojo);
                        al_destroy_bitmap(imgAzul);
                        al_destroy_bitmap(imgVerde);
                        if (imgPeaje) al_destroy_bitmap(imgPeaje);
                        al_destroy_display(ventana);
                        return 0;
                    }
                    else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
                    {
                        mouseX = ev.mouse.x;
                        mouseY = ev.mouse.y;
                        redraw = true;
                    }
                    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (ev.mouse.button & 1) // izquierdo
                        {
                            mouseX = ev.mouse.x;
                            mouseY = ev.mouse.y;
                            // detectar click en inputs
                            int fy = startY;
                            if (mouseX >= fieldX && mouseX <= fieldX + inputW)
                            {
                                if (mouseY >= fy && mouseY <= fy + inputH) { focusedField = 0; }
                                else if (mouseY >= fy + spacingY && mouseY <= fy + spacingY + inputH) { focusedField = 1; }
                                else if (mouseY >= fy + spacingY * 2 && mouseY <= fy + spacingY * 2 + inputH) { focusedField = 2; }
                                else if (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= btnY && mouseY <= btnY + btnH) { focusedField = 3; startPressed = true; }
                            }
                            else
                            {
                                // click en botón (si fuera)
                                if (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= btnY && mouseY <= btnY + btnH)
                                {
                                    focusedField = 3;
                                    startPressed = true;
                                }
                            }
                            redraw = true;
                        }
                    }
                    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        if (ev.keyboard.keycode == ALLEGRO_KEY_TAB)
                        {
                            focusedField = (focusedField + 1) % 4;
                            redraw = true;
                        }
                        else if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER)
                        {
                            // ENTER en campos confirma (si el foco está en botón o fuera)
                            if (focusedField == 3) startPressed = true;
                            else startPressed = true; // simplificamos: ENTER inicia
                        }
                        else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        {
                            al_destroy_font(font);
                            al_destroy_event_queue(startQueue);
                            if (fondo) al_destroy_bitmap(fondo);
                            al_destroy_bitmap(imgAmarillo);
                            al_destroy_bitmap(imgRojo);
                            al_destroy_bitmap(imgAzul);
                            al_destroy_bitmap(imgVerde);
                            if (imgPeaje) al_destroy_bitmap(imgPeaje);
                            al_destroy_display(ventana);
                            return 0;
                        }
                    }
                    else if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
                    {
                        // manejo de escritura en el campo enfocado
                        int c = ev.keyboard.unichar;
                        if (focusedField >= 0 && focusedField <= 2)
                        {
                            string* target = (focusedField == 0) ? &bufMin : (focusedField == 1) ? &bufMax : &bufDuration;
                            if (c == 8) // backspace
                            {
                                if (!target->empty()) target->pop_back();
                            }
                            else if (c == 13 || c == 10) // enter
                            {
                                // nada especial aquí (enter inicia más arriba)
                            }
                            else if (isDigitOrSign(c))
                            {
                                target->push_back(static_cast<char>(c));
                            }
                            // ignorar otros caracteres
                            redraw = true;
                        }
                    }
                }

                if (redraw && al_is_event_queue_empty(startQueue))
                {
                    redraw = false;
                    // Dibujar pantalla de inicio con campos
                    al_clear_to_color(al_map_rgb(0, 0, 0));
                    

                    // Título
                    if (font)
                        al_draw_text(font, al_map_rgb(255,255,255), SCREEN_W/2, SCREEN_H/2 - 180, ALLEGRO_ALIGN_CENTRE, "Simulador - Ajustes iniciales");

                    // Labels y campos
                    int y = startY;

                    // Min servicio
                    if (font) al_draw_text(font, al_map_rgb(220,220,220), labelX, y + 8, 0, "Rango servicio (min):");
                    ALLEGRO_COLOR boxCol = (focusedField == 0) ? al_map_rgb(200,200,120) : al_map_rgb(80,80,80);
                    al_draw_filled_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), boxCol);
                    al_draw_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), al_map_rgb(255,255,255), 2.0f);
                    if (font)
                    {
                        if (bufMin.empty())
                            al_draw_text(font, al_map_rgb(160,160,160), fieldX + 8, y + 6, 0, "2");
                        else
                            al_draw_text(font, al_map_rgb(255,255,255), fieldX + 8, y + 6, 0, bufMin.c_str());
                    }

                    // Max servicio
                    y += spacingY;
                    if (font) al_draw_text(font, al_map_rgb(220,220,220), labelX, y + 8, 0, "Rango servicio (max):");
                    boxCol = (focusedField == 1) ? al_map_rgb(200,200,120) : al_map_rgb(80,80,80);
                    al_draw_filled_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), boxCol);
                    al_draw_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), al_map_rgb(255,255,255), 2.0f);
                    if (font)
                    {
                        if (bufMax.empty())
                            al_draw_text(font, al_map_rgb(160,160,160), fieldX + 8, y + 6, 0, "5");
                        else
                            al_draw_text(font, al_map_rgb(255,255,255), fieldX + 8, y + 6, 0, bufMax.c_str());
                    }

                    // Duración en minutos
                    y += spacingY;
                    if (font) al_draw_text(font, al_map_rgb(220,220,220), labelX, y + 8, 0, "Duración (minutos):");
                    boxCol = (focusedField == 2) ? al_map_rgb(200,200,120) : al_map_rgb(80,80,80);
                    al_draw_filled_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), boxCol);
                    al_draw_rectangle((float)fieldX, (float)y, (float)(fieldX + inputW), (float)(y + inputH), al_map_rgb(255,255,255), 2.0f);
                    if (font)
                    {
                        if (bufDuration.empty())
                            al_draw_text(font, al_map_rgb(160,160,160), fieldX + 8, y + 6, 0, "5");
                        else
                            al_draw_text(font, al_map_rgb(255,255,255), fieldX + 8, y + 6, 0, bufDuration.c_str());
                    }

                    // Botón empezar
                    bool hover = (mouseX >= btnX && mouseX <= btnX + btnW && mouseY >= btnY && mouseY <= btnY + btnH);
                    ALLEGRO_COLOR btnColor = hover ? al_map_rgb(70, 150, 70) : al_map_rgb(50, 120, 50);
                    ALLEGRO_COLOR borde = al_map_rgb(255, 255, 255);
                    al_draw_filled_rectangle((float)btnX, (float)btnY, (float)(btnX + btnW), (float)(btnY + btnH), btnColor);
                    al_draw_rectangle((float)btnX, (float)btnY, (float)(btnX + btnW), (float)(btnY + btnH), borde, 3.0f);
                    if (font)
                    {
                        int textH = al_get_font_line_height(font);
                        al_draw_text(font, al_map_rgb(255,255,255), btnX + btnW / 2, btnY + (btnH - textH) / 2, ALLEGRO_ALIGN_CENTRE, "empezar");
                    }

                    // Nota de uso
                    if (font)
                        al_draw_text(font, al_map_rgb(200,200,200), SCREEN_W / 2, btnY + btnH + 20, ALLEGRO_ALIGN_CENTRE, "Deje el campo vacío para usar el valor por defecto");

                    al_flip_display();
                }
            }

            // Al confirmar: parsear buffers y aplicar valores por defecto en caso de error/vacío
            auto parseOrDefault = [](const string& s, int def) -> int {
                if (s.empty()) return def;
                try {
                    size_t idx = 0;
                    int v = stoi(s, &idx);
                    (void)idx;
                    return v;
                } catch (...) {
                    return def;
                }
            };

            int parsedMin = parseOrDefault(bufMin, 2);
            int parsedMax = parseOrDefault(bufMax, 5);
            int parsedDuration = parseOrDefault(bufDuration, 5);

            // Validaciones sencillas: asegurar min <= max y valores positivos
            if (parsedMin <= 0) parsedMin = 2;
            if (parsedMax <= 0) parsedMax = 5;
            if (parsedMin > parsedMax) swap(parsedMin, parsedMax);
            if (parsedDuration <= 0) parsedDuration = 5;

            serviceMin = parsedMin;
            serviceMax = parsedMax;
            simDurationMin = parsedDuration;

            // Mostrar en consola (y más adelante puede pasarse a Simulador/CabinaPeaje)
            cout << "Rango servicio seleccionado: [" << serviceMin << ", " << serviceMax << "]" << endl;
            cout << "Duración simulación (minutos): " << simDurationMin << " (=" << (simDurationMin*60) << "s)" << endl;

            // limpiar recursos de pantalla de inicio
            al_destroy_font(font);
            al_destroy_event_queue(startQueue);

            // NOTA: en esta implementación sólo recogemos y validamos los valores.
            // Para aplicar los tiempos al motor de simulación:
            // - Si su clase `Simulador` ofrece métodos como `setTiempoServicioMinMax(int min,int max)`
            //   o `setDuracionSimulacionSegundos(float segundos)` llámelos aquí después de asignar
            //   `serviceMin`, `serviceMax` y `simDurationMin`.
            // - También es posible reconstruir las `CabinaPeaje` con los nuevos parámetros si el diseño
            //   actual lo permite.
            //
            // Ejemplo (si existen las funciones, descomentar y ajustar nombres):
            // sim.setTiempoServicioRango(serviceMin, serviceMax);
            // sim.setDuracionSimulacion(simDurationMin * 60.0f);
        }
    }
    // --- FIN pantalla de inicio ---

    // A partir de aquí sigue la creación de autos y ejecución de simulación
    vector<shared_ptr<Carro>> autos;
    srand(time(nullptr));

    float carrilesY[] = { 260, 290, 320 };
    float carrilesX[] = { 560, 590, 620 };

    for (int i = 0; i < 8; i++) 
    {
        auto a = make_shared<Carro>();
        a->setColor(rand() % 4);
        switch (a->getColor()) 
        {
        case 0: a->setImg(imgAmarillo); break;
        case 1: a->setImg(imgRojo); break;
        case 2: a->setImg(imgAzul); break;
        case 3: a->setImg(imgVerde); break;
        }

        bool horizontal = rand() % 2;
        a->setEstado(!horizontal);
        a->setVelocidad(2 + rand() % 3);
        a->setTiempoGenerado(sim.getTiempoSim());
        a->setId(sim.getNextId());

        if (horizontal) 
        {
            a->setDimension(60.0f, 30.0f);
            int maxX = max(50, SCREEN_W - 400); 
            a->setPosicion(static_cast<float>(rand() % maxX), carrilesY[rand() % 3]);
        }
        else 
        {
            a->setDimension(30.0f, 60.0f);
            int maxY = max(50, SCREEN_H - 200);
            a->setPosicion(carrilesX[rand() % 3], static_cast<float>(rand() % maxY));
        }

        string placa = generarPlacaAleatoria();
        while (exisePlaca(placa, autos))
            placa = generarPlacaAleatoria();

        a->setPlaca(placa);

        sim.agregarVehiculo(a);
        autos.push_back(a);
    }

    resolverSolapamientos(autos);

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(ventana));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool salir = false;
    bool dibujar = true;
    al_start_timer(timer);


    const float distanciaMinima = 50.0f;
    const float toleranciaCarril = 12.0f;
    const float dt = 1.0f / FPS; 

    // duración objetivo en segundos (establecida desde la pantalla de inicio)
    const float duracionObjetivoSeg = static_cast<float>(simDurationMin) * 60.0f;

    while (!salir) 
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_TIMER)
            dibujar = true;

        // actualizar simulador (avanza tiempo interno)
        sim.actualizar(dt);

        // --- NUEVO: terminar simulación cuando se alcance la duración predeterminada ---
        if (sim.getTiempoSim() >= duracionObjetivoSeg)
        {
            cout << "Duración predeterminada alcanzada (" << duracionObjetivoSeg << "s). Finalizando simulación." << endl;
            salir = true;
        }

        for (auto& a : autos) 
        {
            Carro* ptr = a.get();

            if (ptr->getCabinaAsignada() == nullptr && ptr->getWaitTimer() <= 0.0f)
            {
                bool bloqueado = hayCarroAdelante(*ptr, autos, distanciaMinima, toleranciaCarril);
                float moveSpeed = bloqueado ? 0.0f : ptr->getVelocidad();

                if (!ptr->getEstado())
                {
                    ptr->setPosicion(ptr->getPosicionX() + moveSpeed, ptr->getPosicionY());
                    if (ptr->getPosicionX() > SCREEN_W) 
                    {
                        ptr->setEstado(true);
                        ptr->setPosicion(carrilesX[rand() % 3], -60);
                    }
                }
                else 
                {
                    ptr->setPosicion(ptr->getPosicionX(), ptr->getPosicionY() + moveSpeed);
                    if (ptr->getPosicionY() > SCREEN_H)
                    {
                        ptr->setEstado(false);
                        ptr->setPosicion(-100, carrilesY[rand() % 3]);
                    }
                }
            }
        }

        resolverSolapamientos(autos);

        if (dibujar && al_is_event_queue_empty(queue)) 
        {
            dibujar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            dibujarFondo(fondo);

            dibujarAutos(autos);

            if (imgPeaje) 
            {
                int pw = al_get_bitmap_width(imgPeaje);
                int ph = al_get_bitmap_height(imgPeaje);
                const float targetW = 180.0f;                 
                const float scale = targetW / static_cast<float>(pw);
                const float heightScale = 0.6f;                
                const float targetH = static_cast<float>(ph) * scale * heightScale;
                const auto& cabinas = sim.getCabinas();
                if (!cabinas.empty()) 
                {
                    const auto& cab = cabinas.front();              
                    const float offsetY = 146.0f;                      // bajar la imagen de peaje
                    const float offsetX = 80.0f;                       // mover a la derecha de peaje
                    float dx = cab.getPosX() - targetW * 0.5f + offsetX;
                    float dy = cab.getPosY() - targetH * 0.5f + offsetY;
                    al_draw_scaled_bitmap(
                        imgPeaje,
                        0, 0, pw, ph,
                        dx, dy,
                        targetW, targetH,
                        0
                    );
                }
            }

            al_flip_display();
        }
    }
    sim.getEstadisticas().generarCSV("DatosGenerales.csv");


    if (fondo) al_destroy_bitmap(fondo);
    al_destroy_bitmap(imgAmarillo);
    al_destroy_bitmap(imgRojo);
    al_destroy_bitmap(imgAzul);
    al_destroy_bitmap(imgVerde);
    if (imgPeaje) al_destroy_bitmap(imgPeaje);
    al_destroy_display(ventana);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
