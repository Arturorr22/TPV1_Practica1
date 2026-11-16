#include "gameobject.h"

// Implementación del constructor de la clase GameObject
GameObject::GameObject(Game* game) 
    : game(game)
{}

// Implementación del método getGame
Game* GameObject::getGame() const {
    return game;
}