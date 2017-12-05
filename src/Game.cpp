#include "Game.h"

Game::Game()
{
    newGame();
}

void Game::addObserver(GameObserver *obs)
{
    m_observers.emplace_back(std::move(obs));
}

void Game::newGame()
{
    m_board.init();
    m_board.shuffle();
    emitGameChanged();
}

void Game::moveUp()
{
    if (!m_board.canMoveUp()) return;
    m_board.moveUp();
    emitGameChanged();
}

void Game::moveDown()
{
    if (!m_board.canMoveDown()) return;
    m_board.moveDown();
    emitGameChanged();
}

void Game::moveRight()
{
    if (!m_board.canMoveRight()) return;
    m_board.moveRight();
    emitGameChanged();
}

void Game::moveLeft()
{
    if (!m_board.canMoveLeft()) return;
    m_board.moveLeft();
    emitGameChanged();
}

void Game::checkOver()
{
    Tile expected { 0 };
    m_isOver = false;
    for (auto &&row : m_board.tiles()) {
        for (auto &&actual : row) {
            if (actual != expected) {
                return ;
            }
            ++expected;
        }
    }
    m_isOver = true;
}

void Game::emitGameChanged()
{
    for (auto &&obs : m_observers) {
        obs->onGameChanged();
    }
}
