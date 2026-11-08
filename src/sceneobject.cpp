#include "sceneobject.h"

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