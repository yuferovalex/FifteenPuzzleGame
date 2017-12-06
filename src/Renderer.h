#pragma once

#include "Game.h"

#ifndef __MINGW32__
#  include <experimental/filesystem>
   namespace fs = std::experimental::filesystem;
#else
#  include <boost/filesystem.hpp>
   namespace fs = boost::filesystem;
#endif

class Application;
struct RendererPrivate;

/**
 * @brief Отрисовщик
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
    /**
	 * @brief Этот метод осуществляет поворот камеры относительно оси X
	 */
	void rotareCameraX(double angle);
	/**
	 * @brief Этот метод осуществляет поворот камеры относительно оси Y
	 */
	void rotareCameraY(double angle);
	/**
	 * @brief Этот метод устанавливает текстуру для клеток
	 */
	void setTexturePath(fs::path texture);

private:
    RendererPrivate *d;
};
