#include "bow.h"

// Implementación del constructor de la clase Bow
Bow::Bow(Game* game, const Rectangle& rect, Texture* texture, 
    const int numArrows, const bool loaded) 
    : SceneObject(game, rect, texture)
    , numArrows(numArrows)
    , loaded(loaded)
{}

// Implementación del método handleEvent de la clase Bow
void Bow::handleEvent(const SDL_Event& event) {

    // Detectamos si se pulsa una tecla
    if (event.type == SDL_EVENT_KEY_DOWN) {

        // Comprobamos si el jugador quiere mover el arco
        if (event.key.key == SDLK_DOWN) rect.move(Vector2D(0.0f, BOW_SPEED));
        else if (event.key.key == SDLK_UP) rect.move(Vector2D(0.0f, -BOW_SPEED));

        // Comprobamos si el jugador quiere cargar o disparar el arco
        if (event.key.key == SDLK_LEFT && numArrows > 0) loaded = true;
        else if (event.key.key == SDLK_RIGHT && loaded) {
            --numArrows;
            loaded = false;
        }
    }
}

// Implementación del método render de la clase Bow
void Bow::render() const {
    if (texture != nullptr) {
        texture->renderFrame(rect.to_sdl(), BOW_ROW, (loaded ? LOADED_FRAME : UNLOADED_FRAME));
    }
}

// Implemetación del método update de la clase Bow
bool Bow::update(const float delta) {

    if (rect.getTopLeftCorner().getY() < 0) {
        rect.move(Vector2D(0.0f, abs(rect.getTopLeftCorner().getY())));
    }
    else if (rect.getTopLeftCorner().getY() + rect.getHeight() > Game::WINDOW_HEIGHT) {
        float moveDistanceY = -((rect.getTopLeftCorner().getY() + rect.getHeight()) - Game::WINDOW_HEIGHT);
        rect.move(Vector2D(0.0f, moveDistanceY));
    }
    return true;
}