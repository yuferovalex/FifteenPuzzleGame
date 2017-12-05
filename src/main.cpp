#include "Application.h"
#include "Game.h"
#include "Renderer.h"

int main(int argc, char *argv[]) 
{
    Application app(argc, argv);

    app.show("Fifteen");
    // app.showFullScreen();
    
    Game game;
    Renderer renderer(app, game);
    
    app.setDisplayFunction([&]() { 
        renderer.display(); 
    });
    app.setKeyBoardFunction([&](char key, int, int) {
        switch (key) {
        case 'w': 
            //game.moveUp(); 
			renderer.rotareCameraX(+10.0);
            break;
        case 'a': 
            //game.moveLeft();
			renderer.rotareCameraY(-10.0);
            break;
        case 's': 
            //game.moveDown(); 
			renderer.rotareCameraX(-10.0);
            break;
        case 'd': 
            //game.moveRight(); 
			renderer.rotareCameraY(+10.0);
            break;
        };
    });
    
    app.exec();
}
