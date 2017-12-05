#pragma once

#include "Game.h"

class Application;
struct RendererPrivate;

/**
 * @class Отрисовщик
 * 
 * Этот класс отрисовывает игру на экране
 */
class Renderer : public GameObserver
{
public:
    Renderer(Application &app, Game &game);
    ~Renderer();
    
    /**
     * @brief Этот метод перерисовывает контекст
     */
    void display();
    /**
     * @brief Этот метод будет вызван, при изменениях в игре
     */
    void onGameChanged() override;
    
	void rotareCameraX(double angle);
	void rotareCameraY(double angle);

private:
    RendererPrivate *d;
};
