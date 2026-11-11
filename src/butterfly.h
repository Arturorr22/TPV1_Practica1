#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include "mobileobject.h"

class Butterfly : public MobileObject {
	private:

		// Variable para controlar el número de mariposas en la escena
		int numberOfButterflies;

		// Variable para controlar cuanto tiempo tarda en cambiar a otra dirección
		static constexpr float DIRECTION_CHANGE_TIME = 5.0f;

		// Variable para controlar la animación de vuelo de la mariposa
		int animationFrame;

		// Número de frames de la animación de vuelo de la mariposa
		static constexpr int FLIGHT_FRAMES = 4;

	public:

		// Constructor de la clase Butterfly
		Butterfly(Game* game, const Rectangle& rect, Texture* texture,
			const Vector2D<>& speed);

		// Método hit de la clase Balloon
		bool hit(const Rectangle& obj) override;

		// Método ChangeDirection de la clase Butterfly
		void ChangeDirection();

		// Método RandomizeDirection de la clase Butterfly
		void RandomizeDirection();

		// Método render de la clase Butterfly
		void render() const override;

		// Método update de la clase Butterfly
		bool update(const float delta) override;

};
