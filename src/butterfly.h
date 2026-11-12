#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include <numbers>

#include "mobileobject.h"

// Mariposa que vuela por la ventana de forma aleatoria. Al ser alcanzada por una
// flecha muere y su cadáver cae hasta salir por el borde inferior
class Butterfly : public MobileObject {
private:

	// Tiempo (en milisegundos) entre dos cambios aleatorios de dirección
	static constexpr float DIRECTION_CHANGE_TIME = 5000.0f;

	// Módulo de la velocidad de vuelo y velocidad de caída del cadáver (píxeles/ms)
	static constexpr float FLIGHT_SPEED = 0.08f;
	static constexpr float FALL_SPEED = 0.15f;

	// Una vuelta completa en radianes
	static constexpr float TWO_PI = 2.0f * std::numbers::pi_v<float>;

	// Número de frames de la animación de vuelo (columnas de butterfly.png)
	static constexpr int FLIGHT_FRAMES = 10;

	// Tiempo (en milisegundos) transcurrido desde el último cambio de dirección
	float timeSinceDirectionChange;

	// Frame actual de la animación de vuelo
	int animationFrame;

	// Indica si la mariposa ha sido alcanzada por una flecha
	bool dead;

	// Elige una dirección aleatoria manteniendo el módulo de la velocidad
	void randomizeDirection();

	// Invierte la componente de la velocidad que empuja a la mariposa fuera de
	// la ventana, de modo que se aleje del borde
	void bounceOffBorders();

public:

	// Constructor de la clase Butterfly. La dirección inicial es aleatoria
	Butterfly(Game* game, const Rectangle& rect, Texture* texture);

	// Método hit de la clase Butterfly: la mariposa viva muere al recibir el impacto
	bool hit(const Rectangle& obj) override;

	// Método render de la clase Butterfly
	void render() const override;

	// Método update de la clase Butterfly
	bool update(const float delta) override;

};

#endif // BUTTERFLY_H
