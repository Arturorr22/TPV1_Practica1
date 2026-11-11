#include "arrow.h"
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
Rectangle Arrow::getTipRect() const {
	return Rectangle({ rect.getTopLeftCorner().getX() + rect.getWidth() * TIP_X_FACTOR,
					   rect.getTopLeftCorner().getY() + rect.getHeight() * TIP_Y_FACTOR },
		rect.getWidth() * TIP_WIDTH_FACTOR,
		rect.getHeight() * TIP_HEIGHT_FACTOR);
}

// Implementación del método update de la clase Arrow
bool Arrow::update(const float delta) {

	// MobileObject mueve la flecha y devuelve false si ha salido de la pantalla
	bool active = MobileObject::update(delta);

	/*
	// Si sigue en pantalla, comprobamos si su punta ha alcanzado algo
	if (active && game->checkCollision(getTipRect())) {
		game->increasePoints();
	}
	*/

	return active;
}

// Implementación del método hit de la clase Arrow
bool Arrow::hit(const Rectangle&) {
	return false;
}