#include "mobileobject.h"
#include "game.h"

// Implementación del constructor de la clase MobileObject
MobileObject::MobileObject(Game* game, const Rectangle& rect, Texture* texture, 
    const Vector2D<>& speed) 
    : SceneObject(game, rect, texture)
    , speed(speed)
{}

// Implementación del método update de la clase MobileObject
bool MobileObject::update(const float delta) {
    rect.move(speed * delta);
    if ((rect.getTopLeftCorner().getY() + rect.getHeight()) < 0 || 
        rect.getTopLeftCorner().getX() > Game::WINDOW_WIDTH) {
            return false;
    }
    return true;
}