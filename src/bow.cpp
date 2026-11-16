#include "bow.h"
#include "game.h"
#include "texture.h"
#include "error.h"

// Implementación del constructor de la clase Bow
Bow::Bow(Game* game, const Rectangle& rect, Texture* texture, 
    const int numArrows, const bool loaded) 
    : SceneObject(game, rect, texture)
    , numArrows(numArrows)
    , loaded(loaded)
    , bowDir(dir::none)
{}

// Implementación del método handleEvent de la clase Bow
void Bow::handleEvent(const SDL_Event& event) {

    // Detectamos si se pulsa una tecla
    if (event.type == SDL_EVENT_KEY_DOWN) {

        // Comprobamos si el jugador quiere mover el arco
        if (event.key.key == SDLK_DOWN) {
            bowDir = dir::down;
        }
        else if (event.key.key == SDLK_UP) {
            bowDir = dir::up;
        }

        // Comprobamos si el jugador quiere cargar o disparar el arco
        if (event.key.key == SDLK_LEFT && numArrows > 0) loaded = true;
        else if (event.key.key == SDLK_RIGHT && loaded) {
            --numArrows;
            GameObject::getGame()->createArrow(SceneObject::getRect());
            loaded = false;
        }
    }
    else {
        bowDir = dir::none;
    }
}

// Implementación del método render de la clase Bow
void Bow::render() const {
    if (SceneObject:: getTexture() != nullptr) {
        SceneObject:: getTexture()->renderFrame(SceneObject::getRect().to_sdl(), BOW_ROW, (loaded ? LOADED_FRAME : UNLOADED_FRAME));
    }
}

// Implemetación del método update de la clase Bow
bool Bow::update(const float delta) {

    // En función de la dirección comprobamos si se saldría de los límites de la pantalla
    // Si no se saldría, movemos el arco
    if (bowDir == dir::down && 
        SceneObject::getRect().getTopLeftCorner().getY() + SceneObject::getRect().getHeight() + BOW_SPEED * delta <= Game::WINDOW_HEIGHT) {
        SceneObject::moveRect(Vector2D(0.0f, BOW_SPEED * delta));
    }
    else if (bowDir == dir::up && SceneObject::getRect().getTopLeftCorner().getY() - BOW_SPEED * delta > 0) {
        SceneObject::moveRect(Vector2D(0.0f, -BOW_SPEED * delta));
    }
    
    return true;
}

// Implementación del método hit de la clase Bow
bool Bow::hit(const Rectangle&) {

    // Las colisiones con el arco no las contamos, por lo que devolvemos falso
    return false;
}

// Implementación del método getArrows de la clase Bow
int Bow::getArrows() const {
    return numArrows;
}

// Implementación de la sobreesctritura del método saveObject
void Bow::saveObject(std::ostream& savedFile) const {

    // Escribimos una B para identificar el arco
    savedFile << "B ";
    SceneObject::saveObject(savedFile);

    // Incluimos el número de flechas y el estado de carga del arco
    savedFile << numArrows << " " << (loaded ? 1 : 0) << "\n";
}

// Implementación de la sobreescritura del método loadObject
Bow* Bow::loadObject(std::istream& savedGameFile, Game* game, const int lineNumber) {

    // Llamamos al loadObject de SceneObject para comprobar que sea válido
    Rectangle rect = SceneObject::loadObject(savedGameFile, lineNumber);

    // Leemos los datos propios del arco
    int arrowCount, loadedFlag;

    // Comprobamos que la lectura de los datos sea correcta y que estos sean válidos 
    if (!(savedGameFile >> arrowCount >> loadedFlag)|| arrowCount < 0 
        || (loadedFlag != 0 && loadedFlag != 1)) {
        throw FileFormatError("saved.txt", lineNumber, "Formato inválido del arco.");
    }

    // Creamos un arco y lo pasamos por referencia
    return new Bow(game, rect, game->getTexture(Game::BOW), arrowCount, loadedFlag == 1);
}