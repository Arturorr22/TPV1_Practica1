#include "infobar.h"

// Implementación del constructor de la infoBar
InfoBar::InfoBar(const Rectangle destRect, const int arrows, Texture* texture) 
    : rect(destRect)
    , arrows(arrows)
    , texture(texture) 
{}

// Implementación del método decrementArrows
void InfoBar::decrementArrows() { --arrows; }

void InfoBar::render() const {

    // Comprobamos que la textura exista
    if (texture != nullptr) {

        // Creamos un rectángulo auxiliar para dibujar cada flecha en una posición distinta 
        for (int i = 0; i < arrows; ++i) {
            Rectangle aux({rect.getWidth() * i, 0}, rect.getWidth(), rect.getHeight());
            texture->render(aux.to_sdl());
        }
    }
}

// Implementación del método updateArrows de InfoBar
void InfoBar::updateArrows(const int newArrowsValue) {
    arrows = newArrowsValue;
}