#include <Windows.h>
#include <fstream>
#include "Game.h"

Game::Game()
{
    newGame();
}

Game::~Game()
{
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
	checkOver();
    emitGameChanged();
}

void Game::moveDown()
{
    if (!m_board.canMoveDown()) return;
    m_board.moveDown();
	checkOver();
    emitGameChanged();
}

void Game::moveRight()
{
    if (!m_board.canMoveRight()) return;
    m_board.moveRight();
	checkOver();
	emitGameChanged();
}

void Game::moveLeft()
{
    if (!m_board.canMoveLeft()) return;
    m_board.moveLeft();
	checkOver();
    emitGameChanged();
}

void Game::save()
{
	std::ofstream os("./Fifteen.save");
	m_board.serialize(os);
}

void Game::load()
{
	std::ifstream is("./Fifteen.save");
	m_board.deserialize(is);
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
	if (isOver()) {
		MessageBoxA(0, "You win", "Fifteen", MB_ICONINFORMATION | MB_OK);
	}
}
