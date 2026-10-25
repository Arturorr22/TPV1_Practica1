//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

using namespace std;

int main(int argc, char* argv[])
{
	try {
		Game().run();
	}
	catch (const string& errorMessage) {
		cout << errorMessage << "\n";
	}
	// TODO: manejar excepciones

	return 0;
}
