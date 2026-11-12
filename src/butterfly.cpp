#include "butterfly.h"
#include "game.h"

#include <cmath>

// Implementación del constructor de la clase Butterfly
Butterfly::Butterfly(Game* game, const Rectangle& rect, Texture* texture)
	: MobileObject(game, rect, texture, Vector2D<>())
	, timeSinceDirectionChange(0.0f)
	, animationFrame(0)
	, dead(false)
{
	// La dirección inicial también es aleatoria
	randomizeDirection();
}

// Implementación del método hit de la clase Butterfly
bool Butterfly::hit(const Rectangle& obj) {

	// Una mariposa muerta o que no toca el rectángulo no recibe el impacto
	if (dead || !SceneObject::hit(obj))
		return false;

	// La mariposa muere y su cadáver pasa a caer a velocidad constante
	game->decreasePoints();
	dead = true;
	speed = Vector2D<>(0.0f, FALL_SPEED);
	return true;
}

// Implementación del método randomizeDirection de la clase Butterfly
void Butterfly::randomizeDirection() {

	// Sorteamos un ángulo y lo convertimos en velocidad con el módulo fijo
	const float angle = game->randomFloat(0.0f, TWO_PI);
	speed = Vector2D<>(FLIGHT_SPEED * std::cos(angle), FLIGHT_SPEED * std::sin(angle));
}

// Implementación del método bounceOffBorders de la clase Butterfly
void Butterfly::bounceOffBorders() {
	const Point2D<> corner = rect.getTopLeftCorner();
	float speedX = speed.getX();
	float speedY = speed.getY();

	// Solo invertimos la componente que apunta hacia fuera de la ventana
	if ((corner.getX() < 0 && speedX < 0) ||
		(corner.getX() + rect.getWidth() > Game::WINDOW_WIDTH && speedX > 0))
		speedX = -speedX;

	if ((corner.getY() < 0 && speedY < 0) ||
		(corner.getY() + rect.getHeight() > Game::WINDOW_HEIGHT && speedY > 0))
		speedY = -speedY;

	speed = Vector2D<>(speedX, speedY);
}

// Implementación del método render de la clase Butterfly
void Butterfly::render() const {
	if (texture != nullptr) {
		texture->renderFrame(rect.to_sdl(), 0, animationFrame);
	}
}

// Implementación del método update de la clase Butterfly
bool Butterfly::update(const float delta) {

	// El cadáver cae hasta salir por el borde inferior de la ventana
	if (dead) {
		rect.move(speed * delta);
		return rect.getTopLeftCorner().getY() < Game::WINDOW_HEIGHT;
	}

	// Cada cierto tiempo, la mariposa viva cambia de dirección al azar
	timeSinceDirectionChange += delta;
	if (timeSinceDirectionChange > DIRECTION_CHANGE_TIME) {
		randomizeDirection();
		timeSinceDirectionChange = 0.0f;
	}

	// Se mueve, rebota si intenta salirse de la ventana y avanza su animación
	rect.move(speed * delta);
	bounceOffBorders();
	animationFrame = (animationFrame + 1) % FLIGHT_FRAMES;
	return true;
}
