#include "mobileobject.h"
#include "game.h"
#include <format>

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

// Implementación del método saveObject heredado de la clase SceneObject
void MobileObject::saveObject(std::ostream& savedFile) const {

    // Llamamos al saveObject de SceneObject
    SceneObject::saveObject(savedFile);

    // Escribimos en el archivo la velocidad del objeto
    savedFile << std::format("{} {} ", speed.getX(), speed.getY());
}