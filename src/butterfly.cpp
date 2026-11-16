#include "butterfly.h"
#include "game.h"
#include <cmath>
#include "texture.h"
#include <format>
#include "error.h"

// Implementación del constructor de la clase Butterfly
Butterfly::Butterfly(Game* game, const Rectangle& rect, Texture* texture, 
	const Vector2D<>& speed, const float timeSinceDirectionChange, 
	const int animationFrame, const bool dead, std::mt19937_64& generator)
	: MobileObject(game, rect, texture, speed)
	, timeSinceDirectionChange(timeSinceDirectionChange)
	, animationFrame(animationFrame)
	, dead(dead)
	, generator(generator)
{
	if (timeSinceDirectionChange == 0.0f) randomizeDirection();
}

// Implementación del método hit de la clase Butterfly
bool Butterfly::hit(const Rectangle& obj) {

	// La mariposa muere y su cadáver pasa a caer a velocidad constante
	if (!dead && SceneObject::hit(obj)) {
		GameObject::getGame()->decreasePoints();
		dead = true;
		MobileObject::setSpeed(Vector2D<>(0.0f, FALL_SPEED));
	}

	// Devolvemos falso para que así la flecha no incremente el número de puntos
	return false;
}

// Implementación del método randomizeDirection de la clase Butterfly
void Butterfly::randomizeDirection() {

	// Sorteamos un ángulo y lo convertimos en velocidad con el módulo fijo
	const float angle = std::uniform_real_distribution<float>(0.0f, TWO_PI)(generator);
	MobileObject::setSpeed(Vector2D<>(FLIGHT_SPEED * std::cos(angle), FLIGHT_SPEED * std::sin(angle)));
}

// Implementación del método bounceOffBorders de la clase Butterfly
void Butterfly::bounceOffBorders() {
	const Point2D<> corner = SceneObject::getRect().getTopLeftCorner();
	float speedX = MobileObject::getSpeed().getX();
	float speedY = MobileObject::getSpeed().getY();

	// Solo invertimos la componente que apunta hacia fuera de la ventana
	if ((corner.getX() < 0 && speedX < 0) ||
		(corner.getX() + SceneObject::getRect().getWidth() > Game::WINDOW_WIDTH && speedX > 0))
		speedX = -speedX;

	if ((corner.getY() < 0 && speedY < 0) ||
		(corner.getY() + SceneObject::getRect().getHeight() > Game::WINDOW_HEIGHT && speedY > 0))
		speedY = -speedY;

	MobileObject::setSpeed(Vector2D<>(speedX, speedY));
}

// Implementación del método render de la clase Butterfly
void Butterfly::render() const {
	if (SceneObject::getTexture() != nullptr) {
		SceneObject::getTexture()->renderFrame(SceneObject::getRect().to_sdl(), 0, animationFrame);
	}
}

// Implementación del método update de la clase Butterfly
bool Butterfly::update(const float delta) {

	// El cadáver cae hasta salir por el borde inferior de la ventana
	if (dead) {
		SceneObject::moveRect(MobileObject::getSpeed() * delta);
		return SceneObject::getRect().getTopLeftCorner().getY() < Game::WINDOW_HEIGHT;
	}

	// Cada cierto tiempo, la mariposa viva cambia de dirección al azar
	timeSinceDirectionChange += delta;
	if (timeSinceDirectionChange > DIRECTION_CHANGE_TIME) {
		randomizeDirection();
		timeSinceDirectionChange = 0.0f;
	}

	// Se mueve, rebota si intenta salirse de la ventana y avanza su animación
	SceneObject::moveRect(MobileObject::getSpeed() * delta);
	bounceOffBorders();
	animationFrame = (animationFrame + 1) % FLIGHT_FRAMES;
	return true;
}

// Implementación del método saveObject de la clase Butterfly
void Butterfly::saveObject(std::ostream& savedFile) const {

    // Escribimos una f minúscula para indicar que se trata de una mariposa
    savedFile << "f ";

    // Llamamos al método saveObject de MobileObject
    MobileObject::saveObject(savedFile);

    // Añadimos los atributos propios de la mariposa
    savedFile << std::format("{} {} {}", timeSinceDirectionChange, 
		animationFrame, dead ? 1 : 0) << "\n";
}

// Implementación del método loadObject de la clase Butterfly
void Butterfly::loadObject(std::istream& savedGameFile, const int lineNumber) {

	// Leemos los datos del MobileObject
	MobileObject::loadObject(savedGameFile, lineNumber);

	// Leemos los datos propios de la mariposa
    float directionChangeTimer;
	int animationFrame, deadFlag;

	// Comprobamos que los datos propios de la mariposa estén en el formato correcto
	if (!(savedGameFile >> directionChangeTimer >> animationFrame >> deadFlag) || directionChangeTimer < 0.0f 
		|| animationFrame < 0 || animationFrame >= FLIGHT_FRAMES || (deadFlag != 0 && deadFlag != 1)) {
		throw FileFormatError("saved.txt", lineNumber, "Formato inválido de la mariposa.");
	}
	
	// Modificamos los datos propios de la mariposa
    timeSinceDirectionChange = directionChangeTimer;
	this->animationFrame = animationFrame;
	dead = (deadFlag == 1 ? true : false); 
}