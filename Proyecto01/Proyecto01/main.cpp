/*
* @Authors:
*   - Jeffry Araya Ch.
*   - Federick Fernadez C.
*   - Juan J. Rojas
*
* Created: 28/09/2025 15:40
*
* Modified: 07/10/2025 17:40
*/

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
const int SCREEN_W = 1200;
const int SCREEN_H = 600;

using namespace std;

int main() {
    if (!al_init()) {
        cerr << "No se pudo inicializar Allegro culpa de la amazonas." << endl;
        return 1;
    }

    if (!al_init_image_addon()) {
        cerr << "No se pudo inicializar el addon de imágenes culpa del gobierno. " << endl;
        return 1;
    }

    ALLEGRO_DISPLAY* ventana = al_create_display(SCREEN_W, SCREEN_H);
    if (!ventana) {
        cerr << "No se pudo crear la ventana culpa del gobierno." << endl;
        return 1;
    }
    al_set_window_title(ventana, "Simulador");
    ALLEGRO_BITMAP* fondo = al_load_bitmap("fondo.jpg");
    if (!fondo) {
        cerr << "No se pudo cargar la imagen de fondo." << endl;
        al_destroy_display(ventana);
        return 1;
    }

    al_draw_scaled_bitmap(
        fondo,
        0, 0, al_get_bitmap_width(fondo), al_get_bitmap_height(fondo), 
        0, 0, SCREEN_W, SCREEN_H, 
        0
    );


    al_flip_display();

    
    while (true) {
        ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
        al_register_event_source(queue, al_get_display_event_source(ventana));
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_event_queue(queue);
            break;
        }
        al_destroy_event_queue(queue);
    }

    al_destroy_bitmap(fondo);
    al_destroy_display(ventana);
    return 0;
}
