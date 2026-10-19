#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "arrow.h"

// GENERACIÓN ALEATORIA DE GLOBOS
// GENERACIÓN ALEATORIA DE GLOBOS

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList {
	TextureSpec{"balloons.png", 7, 6},
	{"arrow.png"},  
	{"bow.png", 1, 2},
};

Game::Game()
  : exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego

	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Almacenamos el tiempo actual al iniciar el programa en las variable LastBalloonTime y LastUpdateTime
	LastBalloonTime = SDL_GetTicks();

	// Creamos el arco
	Rectangle bowRect = Rectangle({0, (float)(WINDOW_HEIGHT / 2 - textures[BOW]->getFrameHeight() / 2)}, 
								  textures[BOW]->getFrameHeight() / 2, textures[BOW]->getFrameWidth() / 2);
	bow = Bow(bowRect, {0, 0}, arrows, false, textures[BOW]);
}

Game::~Game()
{
	// TODO: liberar la memoria reservada por la clase
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	// Generación de flechas
	for (std::size_t i = 0; i < arrowsvector.size(); ++i) {
		arrowsvector[i].render();
	}

	// GENERACIÓN ALEATORIA DE GLOBOS {

	for (std::size_t i = 0; i < balloons.size(); ++i){
		balloons[i].render();
	}
	
	// { GENERACIÓN ALEATORIA DE GLOBOS

	// ARCO
	bow.render();
	// ARCO

	SDL_RenderPresent(renderer);
}

void
Game::update()
{

	// GENERACIÓN ALEATORIA DE GLOBOS {

	frameTime = SDL_GetTicks();

	if (frameTime - LastBalloonTime > 2000) {

		// Calculamos, de manera pseudoaleatoria, la posición en el borde inferior en la que aparecerá
		// el globo, su color y su velocidad en y
		int c =  uniform_int_distribution<int>(0, 6)(generator);
		float sY = uniform_real_distribution<float>(-0.05f, -0.02f)(generator);
		float x =  uniform_real_distribution<float>(WINDOW_WIDTH / 3, WINDOW_WIDTH - textures[BALLOON]->getFrameWidth() / 2)(generator);
	
		// Creamos el rectángulo y el vector velocidad en base a esto último 
		// El rectángulo se encontrará en el borde inferior de la pantalla, 
		// con un tamaño equivalente a la mitad del tamaño de un solo frame de la textura
		Rectangle bowRect = Rectangle({x, WINDOW_HEIGHT}, 
			textures[BALLOON]->getFrameWidth() / 2,
			textures[BALLOON]->getFrameHeight() / 2
		);
		Vector2D<> s = Vector2D(0.0f, sY);
	
		// Con todo calculado, creamos un globo y lo incorporamos al vector de globos
		Balloon b = Balloon(bowRect, c, s, textures[BALLOON]);
		balloons.push_back(b);

		// Actualizamos el valor de LastBalloonTime
		LastBalloonTime = SDL_GetTicks();
	}

		// Una vez hemos generado (o no) un globo, actualizamos todos los globos del vector de globos
		for (std::size_t i = 0; i < balloons.size(); ++i) {

			// Si el globo se sale de los límites, lo eliminamos
			if(!(balloons[i].update(FRAME_PERIOD))) {
				balloons.erase(balloons.begin() + i);
			}
		}

		// ARCO
		bow.update(FRAME_PERIOD);
		// ARCO

		if (bow.getArrows() < arrows) {
			
			// Guardamos el rectángulo del arco para usarlo para crear el rectángulo de la flecha
			Rectangle bowRect = bow.getRect();
			Rectangle arrowRect({0, bowRect.getTopLeftCorner().getY() + bowRect.getHeight() / 2},
								textures[ARROW]->getFrameWidth() / 3,
								textures[ARROW]->getFrameHeight() / 3);

			// Creamos la flecha
			Arrow arrow = Arrow(this, arrowRect, Vector2D<>(0.1f, 0.0f), textures[ARROW]);
			arrowsvector.push_back(arrow);

			// Actualizamos el valor de arrows al actual 
			arrows = bow.getArrows();
		}

		// Actualización de flechas
		for (std::size_t i = 0; i < arrowsvector.size(); ++i) {
			if (!(arrowsvector[i].update(FRAME_PERIOD))) {
				arrowsvector.erase(arrowsvector.begin() + i);
			}
		}

	// } GENERACIÓN ALEATORIA DE GLOBOS
}

void
Game::run()
{
	while (!exit) {
		int frameStart = SDL_GetTicks();
		handleEvents();
		update();
		render();
		int frameEnd = SDL_GetTicks();
		if (frameEnd - frameStart < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - (frameEnd - frameStart));
		}
	}
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			bow.handleEvent(event);
		}
		// TODO
	}
}

bool
Game::checkCollision(const Rectangle& rect) {
	// TODO
	std::size_t i = 0;
	while (i < balloons.size() && !balloons[i].hit(rect)) ++i;
	return false;
}
