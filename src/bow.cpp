#include "bow.h"
#include "game.h"

// Velocidad de movimiento vertical del arco
constexpr float BOW_SPEED = 0.10f;
const Vector2D<> BOW_MOVE_DOWN{ 0.0f, BOW_SPEED };
const Vector2D<> BOW_MOVE_UP{ 0.0f, -BOW_SPEED };

// Frames de la textura del arco
constexpr int BOW_ROW = 0;
constexpr int LOADED_FRAME = 0;
constexpr int UNLOADED_FRAME = 1;

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

// Implementación del método handleEvent
// Recibe un evento del tipo SDL_Event_KEY_DOWN o SDL_EVENT_KEY_UP
void Bow::handleEvent(const SDL_Event& event) {

    // Si se pulsa una tecla, modificamos los atributos correspondientes
    if (event.type == SDL_EVENT_KEY_DOWN) {

        // Comprobamos si se quiere mover, cargar o disparar el arco
        if (event.key.key == SDLK_DOWN) {
            speed = BOW_MOVE_DOWN;
        }
        else if (event.key.key == SDLK_UP) {
            speed = BOW_MOVE_UP;
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
        if (event.key.key == SDLK_UP || event.key.key == SDLK_DOWN) {
            speed = Vector2D<>();
        }
    }
}

// Implementación del render del arco
void Bow::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), BOW_ROW, (loaded ? LOADED_FRAME : UNLOADED_FRAME));
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

// Implementación del método updateArrows del arco
void Bow::updateArrows(const int newArrowsValue) {
    arrows = newArrowsValue;
}
