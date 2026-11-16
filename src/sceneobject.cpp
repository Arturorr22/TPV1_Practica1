#include "sceneobject.h"
#include <format>
#include "game.h"
#include "error.h"

// Implementación del constructor de la clase SceneObject
SceneObject::SceneObject(Game* game, const Rectangle& rect, Texture* texture) 
    : GameObject(game)
    , rect(rect)
    , texture(texture) 
{}

// Implementación del método hit de la clase SceneObject
bool SceneObject::hit(const Rectangle& obj) {
    return rect.hasIntersection(obj);
}

// Implementación del método saveObject de la clase SceneObject
void SceneObject::saveObject(std::ostream& savedFile) const {

    // Escribimos la posición en x e y del rectángulo, su ancho y su alto 
    savedFile << std::format("{} {} {} {} ", rect.getTopLeftCorner().getX(), 
        rect.getTopLeftCorner().getY(), rect.getWidth(), rect.getHeight());
}

// Implementación del método loadObject de la clase SceneObject
Rectangle SceneObject::loadObject(std::istream& savedGameFile, const int lineNumber) {

    // Leemos los datos del rectángulo
    float positionX, positionY, width, height;

    // Comprobamos que la lectura de los datos sea correcta y que estos sean válidos 
    if (!(savedGameFile >> positionX >> positionY >> width >> height) || positionX + width < 0 
        || positionX > Game::WINDOW_WIDTH || positionY + height < 0 
        || positionY > Game::WINDOW_HEIGHT || width <= 0 || height <= 0) {
        throw FileFormatError("saved.txt", lineNumber, "Formato inválido del rectángulo.");
    }

    // Devolvemos el rectangulo resultado
    return Rectangle({positionX, positionY}, width, height);
}

// Implementación de los getters y moveRect
const Rectangle& SceneObject::getRect() const {
    return rect;
}
void SceneObject::moveRect(const Vector2D<>& v) {
    rect.move(v);
}

Texture* SceneObject::getTexture() const {
    return texture;
}