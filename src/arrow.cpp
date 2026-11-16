#include "arrow.h"
#include "texture.h"
#include "game.h"
#include "error.h"

// Implementación del constructor de la clase Arrow
Arrow::Arrow(Game* game, const Rectangle& rect, Texture* texture, const Vector2D<>& speed
	, const int balloons)
	: MobileObject(game, rect, texture, speed)
	, balloons(balloons)
{}

// Implementación del método render de la clase Arrow
void Arrow::render() const {
	if (SceneObject::getTexture() != nullptr) {
		SceneObject::getTexture()->render(SceneObject::getRect().to_sdl());
	}
}

// Implementación del método getTipRect de la clase Arrow
Rectangle Arrow::getArrowHeadRect() const {
	return Rectangle({SceneObject::getRect().getTopLeftCorner().getX() + SceneObject::getRect().getWidth() * TIP_X_FACTOR,
		SceneObject::getRect().getTopLeftCorner().getY() + SceneObject::getRect().getHeight() * TIP_Y_FACTOR },
		SceneObject::getRect().getWidth() * TIP_WIDTH_FACTOR,
		SceneObject::getRect().getHeight() * TIP_HEIGHT_FACTOR);
}

// Implementación del método update de la clase Arrow
bool Arrow::update(const float delta) {

	if (GameObject::getGame()->checkCollision(getArrowHeadRect())) {
		GameObject::getGame()->increasePoints(balloons);
		++balloons;
	}

	return MobileObject::update(delta);
}

// Implementación del método hit de la clase Arrow
// Una flecha no es un objetivo: nunca recibe impactos
bool Arrow::hit(const Rectangle&) {
	return false;
}

// Implementación del método saveObject de la clase Arrow
void Arrow::saveObject(std::ostream& savedFile) const {

    // Escribimos una A mayúscula para indicar que se trata de una flecha
    savedFile << "A ";

    // Llamamos al método saveObject de MobileObject
    MobileObject::saveObject(savedFile);

	// Escribimos el número de globos explotados por la flecha 
	savedFile << balloons << "\n";
}

// Implementación del método loadObject de la clase Arrow
Arrow* Arrow::loadObject(std::istream& savedGameFile, Game* game, const int lineNumber) {

	// Leemos los datos del MobileObject
    Rectangle rect = SceneObject::loadObject(savedGameFile, lineNumber);
    Vector2D speed = MobileObject::loadObject(savedGameFile, lineNumber);

	// Leemos los datos propios de la flecha
	int balloons;

	if (!(savedGameFile >> balloons) || speed.getX() <= 0 || speed.getY() != 0 || balloons < 0) {
		throw FileFormatError("saved.txt", lineNumber, "Formato inválido de la flecha.");
	}

	// Devolvemos la flecha
    return new Arrow(game, rect, game->getTexture(Game::ARROW), speed, balloons);
} 