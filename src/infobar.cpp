#include "infobar.h"
#include "texture.h"

// Implementación del método constructor de la clase InfoBar
InfoBar::InfoBar(Game* game, Texture* arrowTexture) 
    : GameObject(game)
    , arrowTexture(arrowTexture)
{}

// Implementación del método render de la clase InfoBar
void InfoBar::render() const {
    if (arrowTexture != nullptr) {
        
        /*
        // Creamos un rectángulo auxiliar para dibujar cada flecha en una posición distinta 
        for (int i = 0; i < ... ; ++i) {
            Rectangle aux({arrow.getWidth() * i, 0}, rect.getWidth(), rect.getHeight());
            texture->render(aux.to_sdl());
        }
        */
    }
}

/*

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
*/
