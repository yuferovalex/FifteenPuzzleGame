#include <algorithm>
#include <ctime>
#include <istream>
#include <ostream>
#include <random>

#include "Board.h"

static constexpr const size_t m_size { 4 };
static constexpr const size_t m_suffleLoops { 10000 };

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
        (this->*moves[generator() % 2])();
        moves[0] = m_empty_x != 0 ? &Board::moveDown  : &Board::moveUp;
        moves[1] = m_empty_y != 0 ? &Board::moveRight : &Board::moveLeft;
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
	for (auto &row : tiles) {
		for (auto &tile : row) {
			is >> temp;
			tile = static_cast<Tile>(temp);
			check.push_back(static_cast<Tile>(temp));
		}
	}
	// Check
	std::sort(check.begin(), check.end());
	Tile expected{ 0 };
	for (auto &&actual : check) {
		if (expected != actual) {
			return;
			//throw std::exception("Can't deserialize board");
		}
        ++expected;
	}
	// If ok 
	m_tiles = tiles;
}
