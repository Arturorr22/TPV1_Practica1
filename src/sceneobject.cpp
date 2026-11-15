#include "sceneobject.h"
#include <format>

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