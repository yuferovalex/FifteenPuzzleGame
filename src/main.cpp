#include <Windows.h>
#include <GL/freeglut.h>

#include <regex>

#include "Application.h"
#include "Game.h"
#include "Menu.h"
#include "Renderer.h"

static const char s_title[] = "Fifteen";

static const char s_about[] =
    "Fifteen Puzzle Game\n"
    "Created by Yuferov Aleksandr, ET-414, SUSU 2017\n"
    "\n"
    "Use W, A, S, D for game movements\n"
    "Use Up, Down, Left, Right for camera movement";

int main(int argc, char *argv[]) 
{
    Application app(argc, argv);

    app.show(s_title);
    // app.showFullScreen(s_title);

    Game game(app);
    game.load();

    Renderer renderer(app, game);
	fs::path defaultTexPath{ "../shared/default.png" };

	try {
		renderer.setTexturePath(defaultTexPath);
	}
	catch (std::exception &) {
        MessageBoxA(0, "Can't find or open file \"default.png\"", s_title, MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

    app.setDisplayFunction([&]() { 
        renderer.display(); 
    });
    app.setKeyBoardFunction([&](char key, int, int) {
		constexpr const unsigned char esc = 27;
        switch (key) {
        case 'w':
		case 'W':
		case 'ö':
		case 'Ö':
            game.moveUp(); 
            break;
        case 'a':
		case 'A':
		case 'ô':
		case 'Ô':
            game.moveLeft();
            break;
        case 's':
		case 'S':
		case 'û':
		case 'Û':
            game.moveDown(); 
            break;
        case 'd':
		case 'D':
		case 'â':
		case 'Â':
            game.moveRight(); 
            break;
		case esc:
			app.exit();
			break;
        };
    });
	app.setSpecialFunction([&](int key, int, int, int) {
		constexpr const double angle = 10.0;
		switch (key) {
		case GLUT_KEY_UP:
			renderer.rotareCameraX(+angle);
			break;
		case GLUT_KEY_DOWN:
			renderer.rotareCameraX(-angle);
			break;
		case GLUT_KEY_LEFT:
			renderer.rotareCameraY(-angle);
			break;
		case GLUT_KEY_RIGHT:
			renderer.rotareCameraY(+angle);
			break;
		}
	});

	using std::regex;
	using std::regex_match;
	regex re("\\.(png|jpg|jpeg|bmp)");

	Menu textureMenu;
	for (auto &&entry : fs::directory_iterator("../shared")) {
		if (fs::is_regular_file(entry) && regex_match(entry.path().extension().string(), re)) {
			textureMenu.addMenuEntry(entry.path().stem().string(), [&renderer, defaultTexPath, entry]() {
				try {
					renderer.setTexturePath(entry.path());
				}
				catch (std::exception &) {
                    MessageBoxA(0, "Can't use this file", s_title, MB_ICONERROR | MB_OK);
					renderer.setTexturePath(defaultTexPath);
				}
			});
		}
	}

	auto gameMenu = Menu()
		.addMenuEntry("New game", [&]() { game.newGame(); })
		.addMenuEntry("Exit", [&]() { app.exit(); });

	auto mainMenu = Menu()
		.addSubMenu("Game", std::move(gameMenu))
        .addSubMenu("Texture", std::move(textureMenu))
        .addMenuEntry("About", [](){ MessageBoxA(0, s_about, s_title, MB_OK); });

	app.setMenu(std::move(mainMenu));

	app.exec();
    game.save();
}
