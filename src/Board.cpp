#include <ctime>
#include <random>

#include "Board.h"

static constexpr const size_t m_size { 4 };
static constexpr const size_t m_suffleLoops { 1000 };

Board::Board()
    : m_tiles(m_size, std::vector<Tile>(m_size))
{}

bool Board::canMoveRight() const
{
    return m_empty_y != 0;
}

bool Board::canMoveLeft() const
{
    return m_empty_y == 0;
}

bool Board::canMoveUp() const
{
    return m_empty_x == 0;
}

bool Board::canMoveDown() const
{
    return m_empty_x != 0;
}

void Board::moveRight()
{
    size_t i = m_empty_x;
    for (size_t j = m_empty_y; j > 0; --j) {
        std::swap(m_tiles[i][j], m_tiles[i][j - 1]);
    }
    m_empty_y = 0;
}

void Board::moveLeft()
{
    size_t i = m_empty_x;
    for (size_t j = m_empty_y; j < m_size - 1; ++j) {
        std::swap(m_tiles[i][j], m_tiles[i][j + 1]);
    }
    m_empty_y = m_size - 1;
}

void Board::moveDown()
{
    size_t j = m_empty_y;
    for (size_t i = m_empty_x; i > 0; --i) {
        std::swap(m_tiles[i][j], m_tiles[i - 1][j]);
    }
    m_empty_x = 0;
}

void Board::moveUp()
{
    size_t j = m_empty_y;
    for (size_t i = m_empty_x; i < m_size - 1; ++i) {
        std::swap(m_tiles[i][j], m_tiles[i + 1][j]);
    }
    m_empty_x = m_size - 1;
}

void Board::init()
{
    Tile n {};
    m_empty_x = m_empty_y = 0;
    for (auto &row : m_tiles) {
        for (auto &node : row) {
            node = n++;
        }
    }
}

void Board::shuffle()
{
    typedef void (Board::*MoveFunc)();
    MoveFunc moves[2] { &Board::moveUp, &Board::moveLeft };
    std::mt19937 generator(static_cast<uint32_t>(time(nullptr)));
    for (size_t i = 0; i < m_suffleLoops; ++i) {
        std::invoke(moves[generator() % 2], this);
        moves[0] = m_empty_x != 0 ? &Board::moveDown  : &Board::moveUp;
        moves[1] = m_empty_y != 0 ? &Board::moveRight : &Board::moveLeft;
    }
}
