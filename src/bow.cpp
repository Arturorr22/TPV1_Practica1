#include "bow.h"
#include "game.h"

// Implementación del constructor arco
Bow::Bow(const Rectangle destRect, const Vector2D<>& speed, const int arrows, 
    const bool loaded, Texture* texture) 
    : rect(destRect)
    , speed(speed)
    , arrows(arrows)
    , loaded(loaded)
    , texture(texture)
{
}

// Implementación del constructor vacío del arco
Bow::Bow() {
    Bow(Rectangle(), {0, 0}, 0, false, nullptr);
}

// Implementación del método handleEvent
// Recibe un evento del tipo SDL_Event_KEY_DOWN o SDL_EVENT_KEY_UP
void Bow::handleEvent(const SDL_Event& event) {

    // Si se pulsa una tecla, modificamos los atributos correspondientes
    if (event.type == SDL_EVENT_KEY_DOWN) {

        // Comprobamos si se quiere mover, cargar o disparar el arco
        if (event.key.key == SDLK_DOWN) {
            speed = Vector2D(0.0f, 0.10f);
        }
        else if (event.key.key == SDLK_UP) {
            speed = Vector2D(0.0f, -0.10f);
        }
        if (event.key.key == SDLK_LEFT) {
            if (arrows > 0) {
                loaded = true;
            }
        }
        else if (event.key.key == SDLK_RIGHT) {
            if (loaded) {
                --arrows;
                loaded = false;
            }
        }
    }

    // Si se deja de pulsar una tecla, paralizamos el arco 
    else {
        speed = Vector2D();
    }
}

// Implementación del render del arco
void Bow::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), 0, (loaded ? 0 : 1));
    }
}

// Implementación del método update del arco
bool Bow::update(const float delta) {
    
    // Si el arco no se sale de la pantalla, modificamos su posición
    if ((rect.getTopLeftCorner() + (speed * delta)).getY() >= 0 && 
        (rect.getTopLeftCorner() + (speed * delta)).getY() + rect.getHeight() <= Game::WINDOW_HEIGHT) {
            rect.move(speed * delta);
    }
    return true;
}

// Implementación del método getArrows del arco
int Bow::getArrows() const {
    return arrows;
}

// Implementación del método getRect del arco
Rectangle Bow::getRect() const {
    return rect;
}