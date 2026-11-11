#include "game.h"
#include <limits>
#include "bow.h"
#include "balloon.h"
#include "arrow.h"

#include <SDL3_image/SDL_image.h>

#include <string>
#include <fstream>

using namespace std;

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
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
	
	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();
	
	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;
		string levelRoot = LEVEL_ROOT;
		
		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
		
		// Leemos el nivel 1
		levelToLoad = 1;
		ifstream levelFile((levelRoot + to_string(levelToLoad) + ".txt").c_str());
		if (!levelFile.is_open()) throw "Fichero del nivel no encontrado"s;
		else {
			LoadGame(levelFile);
			levelFile.close();
			++levelToLoad;
		}
	}
	catch (const string&) {
		
		// Destruimos las texturas
		for (std::size_t i = 0; i < textures.size(); ++i)
			delete textures[i];
		
		// Relanza la excepción
		throw;
	}
	
	// Creamos el arco
	bow = new Bow(this, Rectangle({0, (float)(WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / BOW_HEIGHT_DIV)},
		WINDOW_WIDTH / BOW_WIDTH_DIV, WINDOW_HEIGHT / BOW_HEIGHT_DIV), textures[BOW], arrows, false);
	
	// Añadimos el arco a la lista de SceneObjects
	addObject(bow);

	/*
	// Creamos la infoBar
	infoBar = new InfoBar(Rectangle({ 0, 0 }, WINDOW_WIDTH / (ARROW_WIDTH_DIV * INFOBAR_SCALE),
	WINDOW_HEIGHT / (ARROW_HEIGHT_DIV * INFOBAR_SCALE)), arrows, textures[ARROW]);
	*/

	// Configuramos la semilla del generador para la generación de globos
	generator.seed(std::random_device()());

	// Calculamos el tiempo que ha de pasar hasta que aparezca el primer globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Almacenamos el tiempo actual al iniciar el programa en la variable LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

Game::~Game()
{
	// Destruimos todos los objetos de la escena
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		delete *it;
		it = sceneObjects.erase(it);
	}
	
	// Destruimos las texturas
	for (std::size_t i = 0; i < textures.size(); ++i)
		delete textures[i];
	
	// Destruimos el renderer y la ventana
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::run()
{
	while (!exit) {

		// Calculamos frameStart y despues frameEnd
		int frameStart = SDL_GetTicks();

		// Manejamos los posibles eventos
		handleEvents();

		// Actualizamos el estado de todos los objetos del juego
		update();

		// Renderizamos todos los objetos del juego
		render();
		
		// Usamos frameEnd y frameStart para lograr que el juego se ciña a
		// FRAME_PERIOD, que se corresponde con el tiempo entre frames
		int frameEnd = SDL_GetTicks();
		if (frameEnd - frameStart < FRAME_PERIOD) {
			SDL_Delay(FRAME_PERIOD - (frameEnd - frameStart));
		}
	}
}

void
Game::createArrow(const Rectangle& bowRect)
{
	// Creamos una flecha y la añadimos a la lista de objetos de la escena
	// Antes de incorporarla a la lista, hacemos que el puntero lastArrow apunte a ella
	lastArrow = new Arrow(this, Rectangle({0,
		bowRect.getTopLeftCorner().getY() + (float)(WINDOW_HEIGHT / ARROW_OFFSET_DIV)},
		WINDOW_WIDTH / ARROW_WIDTH_DIV, WINDOW_HEIGHT / ARROW_HEIGHT_DIV),
		textures[ARROW], Vector2D(ARROW_SPEED, 0.0f));
	sceneObjects.push_back(lastArrow);

	// Actualizamos el valor de arrows al actual
	--arrows;
}

bool
Game::checkCollision(const Rectangle& rect)
{
	// Recorremos todos los objetos de la escena comprobando si se dan colisiones
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end() && !(*it)->hit(rect)) {
		++it;
	}
	return it != sceneObjects.end();
}

void
Game::increasePoints()
{
	playerPoints += balPoints;
}

// Lee un entero del fichero y comprueba que no sea menor que minValue.
// Si no es válido, lanza una excepción con el nombre de la palabra clave
int readValue(std::ifstream& file, const string& name, int minValue) {
	int value;
	if (!(file >> value) || value < minValue)
		throw "No se pudo leer un numero valido para "s + name;
	return value;
}

// Implementación del método LoadGame
void
Game::LoadGame(std::ifstream& file)
{
	// Asignamos unos valores por defecto a los atributos de Game por si el archivo
	// no los modifica
	backgroundColor = DEFAULT_BACKGROUND;
	arrows = DEFAULT_ARROWS;
	baldelay = DEFAULT_BALDELAY;
	balPoints = DEFAULT_BALPOINTS;
	goal = DEFAULT_GOAL;

	// Leemos el archivo y modificamos los atributos de Game
	string keyword;
	while (file >> keyword) {

		// Leemos la primera palabra de cada línea e intentamos almacenar el valor
		// escrito después
		if (keyword == "bgcolor") {
			backgroundColor.r = readValue(file, keyword, MIN_BACKGROUND);
			backgroundColor.g = readValue(file, keyword, MIN_BACKGROUND);
			backgroundColor.b = readValue(file, keyword, MIN_BACKGROUND);
		}
		else if (keyword == "arrows") arrows = readValue(file, keyword, MIN_ARROWS);
		else if (keyword == "baldelay") baldelay = readValue(file, keyword, MIN_BALDELAY);
		else if (keyword == "balpoints") balPoints = readValue(file, keyword, MIN_BALPOINTS);
		else if (keyword == "goal") goal = readValue(file, keyword, MIN_GOAL);
		else file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	// Fijamos los puntos del jugador a 0
	playerPoints = 0;
}

// Implementación del método addObject de la clase Game
void
Game::addObject(SceneObject* obj)
{
	if (obj != nullptr) {
		sceneObjects.push_back(obj);
	}
}

void
Game::createBalloon()
{
	// Calculamos, de manera pseudoaleatoria, la posición del borde inferior
	// en la que aparecerá el globo, junto con su color y su velocidad en el
	// eje y
	int color = uniform_int_distribution<int>(0, NUM_BALLOON_COLORS - 1)(generator);
	float speedY = uniform_real_distribution<float>(MIN_BALLOON_SPEED,
		MAX_BALLOON_SPEED)(generator);
	float posX = uniform_real_distribution<float>(WINDOW_WIDTH / BALLOON_MIN_X_DIV,
		WINDOW_WIDTH - WINDOW_WIDTH / BALLOON_WIDTH_DIV)(generator);

	// Creamos el globo y lo añadimos a sceneObjects
	sceneObjects.push_back(new Balloon(this, Rectangle({posX, WINDOW_HEIGHT}, 
		WINDOW_WIDTH / BALLOON_WIDTH_DIV, WINDOW_HEIGHT / BALLOON_HEIGHT_DIV), 
		textures[BALLOON], Vector2D<>(0, speedY), color));

	// Recalculamos el valor hasta el siguiente globo
	nextBalloon = exponential_distribution<double>(BALLOONS_PER_DELAY / baldelay)(generator);

	// Actualizamos el valor de LastBalloonTime
	LastBalloonTime = SDL_GetTicks();
}

void
Game::render() const
{
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g,
		backgroundColor.b, OPAQUE_ALPHA);

	SDL_RenderClear(renderer);

	// Renderizamos todos los objetos de la escena
	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); ++it) {
		(*it)->render();
	}

	// infoBar->render();

	SDL_RenderPresent(renderer);
}

void
Game::update()
{
	// Actualizamos todos los objetos de la escena
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		if ((*it)->update(FRAME_PERIOD)) ++it;
		else {

			// Comprobamos si se trata de la última flecha de la que disponía el
			// jugador
			if (arrows == 0 && *it == lastArrow) exit = true;
			delete *it;
			it = sceneObjects.erase(it);
		}
	}

	// Comprobamos si ya ha pasado suficiente tiempo desde la generación del último globo
	if (SDL_GetTicks() - LastBalloonTime > nextBalloon) createBalloon();

	// Si el jugador supera la meta, cargamos el siguiente nivel
	if (playerPoints >= goal) loadNextLevel();
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Solo quit se maneja directamente, todo lo demás se delega al arco (Bow)
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;
		else if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
			bow->handleEvent(event);
		}
	}
}

void
Game::loadNextLevel()
{
	// Borramos todos los objetos de la escena antes de pasar al siguiente nivel
	// (exceptuando el arco)
	auto it = ++sceneObjects.begin();
	while (it != sceneObjects.end()) {
		delete *it;
		it = sceneObjects.erase(it);
	}

	// Intentamos abrir el archivo correspondiente al siguiente nivel
	string levelRoot = LEVEL_ROOT;
	ifstream levelFile((levelRoot + to_string(levelToLoad) + ".txt").c_str());
	if (!levelFile.is_open())
		throw "Fichero del nivel no encontrado"s;

	LoadGame(levelFile);
	levelFile.close();
	/*
	infoBar->updateArrows(arrows);
	bow->updateArrows(arrows);
	*/
	++levelToLoad;
}
