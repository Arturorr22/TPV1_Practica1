#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>

// GENERACIÓN ALEATORIA DE GLOBOS
#include <random>
#include "balloon.h"
// GENERACIÓN ALEATORIA DE GLOBOS

// ARCO
#include "bow.h"
// ARCO

// Declaraciones anticipadas
#include "rectangle.h"
#include "vector2D.h"

// Declaraciones anticipadas
class Texture;
class Arrow;

/**
 * Clase principal del juego.
 */
class Game
{
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BALLOON = 0,
		ARROW,
		BOW,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	void render() const;
	void update();
	void handleEvents();

	bool exit;

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	std::vector<Arrow*> arrows;

	// GENERACIÓN ALEATORIA DE GLOBOS {

	// Generador de números pseudoaleatorios
	std::mt19937_64 generator;

	// Variables para gestionar el tiempo
	uint64_t frameTime, LastBalloonTime, LastUpdateTime;

	// Vector que almacena los globos generados
	std::vector<Balloon> balloons;

	// } GENERACIÓN ALEATORIA DE GLOBOS

	// ARCO 
	Bow bow;
	// ARCO


public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	// bool checkCollision(const Rectangle& rect);
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
