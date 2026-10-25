#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <vector>
#include <random>
#include <fstream>


// Declaraciones anticipadas
#include "rectangle.h"
#include "vector2D.h"

#include "balloon.h"
#include "bow.h"
#include "infobar.h"

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

	int levelToLoad = 1;

	// Número de flechas
	int arrows = 1;
	
	// tiempo de espera en milisegundos entre la generación de un globo y su 
	// siguiente
	int baldelay = 500;

	// Meta de puntos, puntos por globo y color de fondo
	int goal = 100;
	int balPoints = 10;
	int playerPoints = 0;

	struct Color { int r, g, b; };
	Color backgroundColor;
	
	// Arco
	Bow* bow;

	// InfoBar
	InfoBar* infoBar; 
	
	// Generador de números pseudoaleatorios
	std::mt19937_64 generator;
	
	// Variables para gestionar el tiempo
	uint64_t LastBalloonTime;
	uint64_t nextBalloon;
	
	// Vector que almacena los globos generados
	std::vector<Balloon*> balloonsVector;
	
	// Elementos del juego
	std::vector<Arrow*> arrowsVector;
	
	// Método que lee de un archivo de entrada los datos de baldelay, arrows, etc...
	void LoadGame(std::ifstream& file);

	void createBalloon();
	void createArrow();
public:

	Game();
	~Game();
	
	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;
	
	// Ejecuta el bucle principal del juego
	void run();

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	// Método que aumenta el total de puntos del jugador respecto a los puntos que suelta 
	// cada globo
	void increasePoints();
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
