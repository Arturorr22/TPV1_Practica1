//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include "error.h"

#include <cstdlib>
#include <exception>
#include <iostream>

using namespace std;

namespace {

	// Título del cuadro de mensaje de los errores irrecuperables
	constexpr const char* ERROR_TITLE = "Error";

	// Informa al jugador de un error irrecuperable, con un cuadro de mensaje y por
	// la salida de error. SDL permite mostrar el cuadro aunque SDL_Init no se haya
	// ejecutado, así que sirve también para los errores de inicialización
	void reportError(const char* message) {
		cerr << message << "\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, ERROR_TITLE, message, nullptr);
	}
}

int main()
{
	try {
		Game().run();
	}
	catch (const GameError& e) {
		reportError(e.what());
		return EXIT_FAILURE;
	}
	catch (const exception& e) {
		reportError(e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
