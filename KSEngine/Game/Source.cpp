#include "KSEngine/Game.h"

int main(int argc, char** argv) {

	Game::GetGameInstance().Start("KSEngine | An OpenGL Engine", false, 1440, 720); //Screen 2:1

	Game::DestroyGameInstance();

	return 0;
}