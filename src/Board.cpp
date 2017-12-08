#include <algorithm>
#include <ctime>
#include <istream>
#include <ostream>
#include <functional>
#include <random>
#include <stdexcept>

#include "Board.h"

constexpr size_t m_size { 4 };
constexpr size_t m_suffleLoops { 1000 };

Board::Board()
    : m_tiles(m_size, std::vector<Tile>(m_size))
{}

bool Board::canMoveRight() const
{
    return m_empty_y < m_size - 1;
}

bool Board::canMoveLeft() const
{
    return m_empty_y > 0;
}

bool Board::canMoveUp() const
{
    return m_empty_x > 0;
}

bool Board::canMoveDown() const
{
    return m_empty_x < m_size - 1;
}

void Board::moveRight()
{
    if (!canMoveRight()) return;
    std::swap(m_tiles[m_empty_x][m_empty_y], m_tiles[m_empty_x][m_empty_y + 1]);
    ++m_empty_y;
}

void Board::moveLeft()
{
    if (!canMoveLeft()) return;
    std::swap(m_tiles[m_empty_x][m_empty_y], m_tiles[m_empty_x][m_empty_y - 1]);
    --m_empty_y;
}

void Board::moveDown()
{
    if (!canMoveDown()) return;
    std::swap(m_tiles[m_empty_x][m_empty_y], m_tiles[m_empty_x + 1][m_empty_y]);
    ++m_empty_x;
}

void Board::moveUp()
{
    if (!canMoveUp()) return;
    std::swap(m_tiles[m_empty_x][m_empty_y], m_tiles[m_empty_x - 1][m_empty_y]);
    --m_empty_x;
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
    MoveFunc move[] = { &Board::moveUp, &Board::moveDown, &Board::moveLeft, &Board::moveRight };
    std::mt19937 rand(static_cast<uint32_t>(time(nullptr)));
    std::uniform_int_distribution<> dist(0, std::size(move) - 1);
    for (auto i = m_suffleLoops; i > 0; --i) {
        std::invoke(move[dist(rand)], this);
    }
}

void Board::serialize(std::ostream &os)
{
    for (auto &&row : m_tiles) {
        for (auto &&tile : row) {
            os << int(tile) << ' ';
        }
        os << '\n';
    }
}

void Board::deserialize(std::istream &is)
{
    std::vector<std::vector<Tile>> tiles(m_size, std::vector<Tile>(m_size));
    std::vector<Tile> check;
    check.reserve(m_size * m_size);
    int temp;
    int empty_x = 0;
    int empty_y = 0;
    int x = 0;
    int y = 0;
    for (auto &row : tiles) {
        y = 0;
        for (auto &tile : row) {
            is >> temp;
            tile = static_cast<Tile>(temp);
            check.push_back(static_cast<Tile>(temp));
            if (temp == 0) {
                empty_x = x;
                empty_y = y;
            }
            ++y;
        }
        ++x;
    }
    // Check
    std::sort(check.begin(), check.end());
    Tile expected{ 0 };
    for (auto &&actual : check) {
        if (expected != actual) {
            throw std::runtime_error("Can't deserialize board");
        }
        ++expected;
    }
    // If ok 
    m_tiles = tiles;
    m_empty_x = empty_x;
    m_empty_y = empty_y;
}
