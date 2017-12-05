#pragma once

#include "Board.h"

class GameObserver;

/**
 * @class Игра
 *
 * Основная логика игры пятнашки. 
 */
class Game
{
public:
    Game();
    /**
     * @brief Предикат окончания игры
     */
    bool isOver() const 
        { return m_isOver; }
    /**
     * @brief Метод возвращает доску игры в текущем состоянии
     */ 
    auto board() const  
        { return m_board; }
    /**
     * @brief Этот метод регистрирует наблюдателя.
     *        Метод не перехватывает владение объектом
     */ 
    void addObserver(GameObserver *obs);
    /**
     * @brief Этот метод создает новую игру.
     */
    void newGame();
    /**
     * @brief Сдвиг фишек вверх.
     */
    void moveUp();
    /**
     * @brief Сдвиг фишек вниз.
     */
    void moveDown();
    /**
     * @brief Сдвиг фишек вправо.
     */
    void moveRight();
    /**
     * @brief Сдвиг фишек влево.
     */
    void moveLeft();
    
private:
    void checkOver();
    void emitGameChanged();
    
    std::vector<GameObserver *> m_observers;
    Board m_board;
    bool m_isOver = false;
};

/**
 * @class Базовый класс наблюдателя за игрой
 */
class GameObserver
{
public:
    virtual ~GameObserver() = default;
    virtual void onGameChanged() = 0;
};
