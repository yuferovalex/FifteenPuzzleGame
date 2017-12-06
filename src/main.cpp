#include <Windows.h>
#include <GL/freeglut.h>

#include <regex>

#include "Application.h"
#include "Game.h"
#include "Menu.h"
#include "Renderer.h"

template<typename F> 
struct FinalAction
{
public:
	FinalAction(F f) : clean{ f } {}
	~FinalAction() { clean(); }

private:
	F clean;
};

template<class F>
FinalAction<F> finally(F f)
{
	return FinalAction<F> { f };
}

int main(int argc, char *argv[]) 
{
    Application app(argc, argv);

    app.show("Fifteen");
    // app.showFullScreen("Fifteen");

    Game game;
	game.load();
	auto _ = ::finally([&]() { 
		game.save(); 
	});

    Renderer renderer(app, game);
	fs::path defaultTexPath{ "../shared/default.png" };

	try {
		renderer.setTexturePath(defaultTexPath);
	}
	catch (std::exception &) {
		MessageBoxA(0, "Can't find or open file \"default.png\"", "Fifteen", MB_ICONERROR | MB_OK);
		return EXIT_FAILURE;
	}

    app.setDisplayFunction([&]() { 
        renderer.display(); 
    });
    app.setKeyBoardFunction([&](unsigned char key, int, int) {
		constexpr const unsigned char esc = 27;
        switch (key) {
        case 'w': 
            game.moveUp(); 
            break;
        case 'a': 
            game.moveLeft();
            break;
        case 's': 
            game.moveDown(); 
            break;
        case 'd': 
            game.moveRight(); 
            break;
		case esc:
			glutLeaveMainLoop();
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
					MessageBoxA(0, "Can't use this file", "Fifteen", MB_ICONERROR | MB_OK);
					renderer.setTexturePath(defaultTexPath);
				}
			});
		}
	}

	auto gameMenu = Menu()
		.addMenuEntry("New game", [&]() { game.newGame(); })
		.addMenuEntry("Exit", [&]() { glutLeaveMainLoop(); });

	auto mainMenu = Menu()
		.addSubMenu("Game", std::move(gameMenu))
		.addSubMenu("Texture", std::move(textureMenu));

	app.setMenu(std::move(mainMenu));

	app.exec();
}
