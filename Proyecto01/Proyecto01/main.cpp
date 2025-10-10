/*
* @Authors:
*   - Jeffry Araya Ch.
*   - Federick Fernandez C.
*   - Juan J. Rojas
*
* Created: 28/09/2025 15:40
* Modified: 10/10/2025 14:05
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

using namespace std;

const int SCREEN_W = 1200;
const int SCREEN_H = 600;
const float FPS = 60.0;



struct Auto {
    ALLEGRO_BITMAP* img;
    float x, y;
    float velocidad;
    bool vertical; 
    float w, h;    
    int tipo;      // color
};

/*
TO DO:-------------------------------------------------
hay q hacer:

-Cambiar imagen fondo para que sea unidireccional(paint)
-implementar colisiones
-Poner peaje
-carro verde (cambiar tamaño)

*/

void dibujarFondo(ALLEGRO_BITMAP* fondo) {
    al_draw_scaled_bitmap(
        fondo,
        0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo),
        0, 0, SCREEN_W, SCREEN_H,
        0
    );
}

void dibujarAutos(vector<Auto>& autos) {
    for (auto& a : autos) {
        if (a.vertical) {
            // Aqui es si va bajando
            float bw = al_get_bitmap_width(a.img);
            float bh = al_get_bitmap_height(a.img);
            float cx = bw / 2.0f;
            float cy = bh / 2.0f;
            float xscale = a.w / bw;
            float yscale = a.h / bh;
            al_draw_scaled_rotated_bitmap(a.img, cx, cy,
                a.x + a.w / 2.0f, a.y + a.h / 2.0f,
                xscale, yscale, ALLEGRO_PI / 2.0f, 0);
        }
        else {
            al_draw_scaled_bitmap(a.img, 0, 0, al_get_bitmap_width(a.img), al_get_bitmap_height(a.img),
                a.x, a.y, a.w, a.h, 0);
        }
    }
}



int main() {
    if (!al_init()) {
        cerr << "No se pudo inicializar Allegro culpa de Rodirgo chavez" << endl;
        return 1;
    }

    if (!al_init_image_addon()) {
        cerr << " No se pudo inicializar el addon de imágenes" << endl;
        return 1;
    }

    if (!al_install_keyboard()) {
        cerr << "No se pudo inicializar el teclado." << endl;
        return 1;
    }

    ALLEGRO_DISPLAY* ventana = al_create_display(SCREEN_W, SCREEN_H);
    if (!ventana) {
        cerr << "No se pudo crear la ventana." << endl;
        return 1;
    }

    al_set_window_title(ventana, "Simulador de Flujo de Tráfico y Peaje");

    ALLEGRO_BITMAP* fondo = al_load_bitmap("Fondo.jpg");
    if (!fondo) {
        cerr << "No se pudo cargar la imagen de fondo estres positv" << endl;
        al_destroy_display(ventana);
        return 1;
    }

    ALLEGRO_BITMAP* imgAmarillo = al_load_bitmap("carroAmarillo.png");
    ALLEGRO_BITMAP* imgRojo = al_load_bitmap("carroRojo.png");
    ALLEGRO_BITMAP* imgAzul = al_load_bitmap("carroAzul.png");
    ALLEGRO_BITMAP* imgVerde = al_load_bitmap("carroVerde.png");

    if (!imgAmarillo || !imgRojo || !imgAzul || !imgVerde) {
        cerr << "No se pudieron cargar las imágenes de los autos." << endl;
        al_destroy_bitmap(fondo);
        al_destroy_display(ventana);
        return 1;
    }

    vector<Auto> autos;
    srand(time(nullptr));

    float carrilesY[] = { 260, 290, 320 };
    float carrilesX[] = { 560, 590, 620 };

    for (int i = 0; i < 8; i++) {
        Auto a;
        a.tipo = rand() % 4;
        switch (a.tipo) {
        case 0: a.img = imgAmarillo; break;
        case 1: a.img = imgRojo; break;
        case 2: a.img = imgAzul; break;
        case 3: a.img = imgVerde; break;
        }

        bool horizontal = rand() % 2;
        a.vertical = !horizontal;
        a.velocidad = 2 + rand() % 3;

        if (horizontal) {
            a.y = carrilesY[rand() % 3];
            a.x = rand() % SCREEN_W;
            a.w = (a.tipo == 3) ? 100 : 60; //ancho verdee
            a.h = 30;
        }
        else {
            a.x = carrilesX[rand() % 3];
            a.y = rand() % SCREEN_H;
            a.w = (a.tipo == 3) ? 60 : 30;  
            a.h = 60;
        }

        autos.push_back(a);
    }



    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(ventana));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());

    bool salir = false;
    bool dibujar = true;
    al_start_timer(timer);

    while (!salir) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
            salir = true;
        else if (ev.type == ALLEGRO_EVENT_TIMER)
            dibujar = true;


        for (auto& a : autos) {
            if (!a.vertical) {
   
                a.x += a.velocidad;
                if (a.x > SCREEN_W) {
                    a.vertical = true; 
                    a.x = carrilesX[rand() % 3];
                    a.y = -60;
                    a.w = (a.tipo == 3) ? 60 : 30;
                    a.h = 60;
                }
            }
            else {
                // Movimiento hacia acbajo
                a.y += a.velocidad;
                if (a.y > SCREEN_H) {
                    a.vertical = false; 
                    a.y = carrilesY[rand() % 3];
                    a.x = -100;
                    a.w = (a.tipo == 3) ? 100 : 60;
                    a.h = 30;
                }
            }
        }

        if (dibujar && al_is_event_queue_empty(queue)) {
            dibujar = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            dibujarFondo(fondo);
            dibujarAutos(autos);
            al_flip_display();
        }
    }


    al_destroy_bitmap(fondo);
    al_destroy_bitmap(imgAmarillo);
    al_destroy_bitmap(imgRojo);
    al_destroy_bitmap(imgAzul);
    al_destroy_bitmap(imgVerde);
    al_destroy_display(ventana);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
