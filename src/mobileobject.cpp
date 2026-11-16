#include "mobileobject.h"
#include "game.h"
#include <format>
#include "error.h"

// Implementación del constructor de la clase MobileObject
MobileObject::MobileObject(Game* game, const Rectangle& rect, Texture* texture, 
    const Vector2D<>& speed) 
    : SceneObject(game, rect, texture)
    , speed(speed)
{}

// Implementación del método update de la clase MobileObject
bool MobileObject::update(const float delta) {
    SceneObject::moveRect(speed * delta);
    if ((SceneObject::getRect().getTopLeftCorner().getY() + SceneObject::getRect().getHeight()) < 0 || 
        SceneObject::getRect().getTopLeftCorner().getX() > Game::WINDOW_WIDTH) {
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

// Implementación del método loadObject de la clase MobileObject
void MobileObject::loadObject(std::istream& savedGameFile, const int lineNumber) {

    // Modificamos el rectángulo
    SceneObject::loadObject(savedGameFile, lineNumber);

    // Leemos los datos de la velocidad
    float speedX, speedY;

    // Comprobamos que la lectura de los datos sea correcta y que estos sean válidos 
    if (!(savedGameFile >> speedX >> speedY)) {
        throw FileFormatError("saved.txt", lineNumber, "Formato inválido del vector velocidad.");
    }

    // Modificamos la velocidad
    speed = Vector2D<>(speedX, speedY);
}

// Implementación del getter de la velocidad
const Vector2D<>& MobileObject::getSpeed() const {
    return speed;
}

// Implementación del método setSpeed
void MobileObject::setSpeed(const Vector2D<>& newSpeed) {
    speed = newSpeed;
}