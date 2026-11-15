#include "arrow.h"
#include "texture.h"
#include "game.h"

// Implementación del constructor de la clase Arrow
Arrow::Arrow(Game* game, const Rectangle& rect, Texture* texture, const Vector2D<>& speed)
	: MobileObject(game, rect, texture, speed)
{}

// Implementación del método render de la clase Arrow
void Arrow::render() const {
	if (texture != nullptr) {
		texture->render(rect.to_sdl());
	}
}

// Implementación del método getTipRect de la clase Arrow
Rectangle Arrow::getArrowHeadRect() const {
	return Rectangle({ rect.getTopLeftCorner().getX() + rect.getWidth() * TIP_X_FACTOR,
					   rect.getTopLeftCorner().getY() + rect.getHeight() * TIP_Y_FACTOR },
		rect.getWidth() * TIP_WIDTH_FACTOR,
		rect.getHeight() * TIP_HEIGHT_FACTOR);
}

// Implementación del método update de la clase Arrow
bool Arrow::update(const float delta) {

	game->checkCollision(getArrowHeadRect());

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

	savedFile << "\n";
}

// Implementación del método loadObject de la clase Arrow
Arrow* Arrow::loadObject(std::istream& savedFile, Game* game) {

    float x, y, w, h, sX, sY;
    savedFile >> x >> y >> w >> h >> sX >> sY;

    return new Arrow(game, Rectangle({x, y}, w, h), game->getTexture(Game::ARROW), 
		Vector2D(sX, sY));
} 