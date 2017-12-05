#pragma once

#include <vector>

/**
 * @brief Фишки
 */
typedef unsigned char Tile;

/**
 * @brief Доска
 *
 * Игровая доска с фишками
 */
class Board
{
public:
    Board();
    /**
     * @brief Доступ к фишкам (0 - пустая клетка доски)
     */
    auto tiles() const
        { return m_tiles; }
    /**
     * @brief Предикат проверяющий возможность сдвига вправо
     */
    bool canMoveRight() const;
    /**
     * @brief Предикат проверяющий возможность сдвига влево
     */
    bool canMoveLeft() const;
    /**
     * @brief Предикат проверяющий возможность сдвига вверх
     */
    bool canMoveUp() const;
    /**
     * @brief Предикат проверяющий возможность сдвига вниз
     */
    bool canMoveDown() const;
    /**
     * @brief Сдвинуть фишки вправо
     */
    void moveRight();
    /**
     * @brief Сдвинуть фишки влево
     */
    void moveLeft();
    /**
     * @brief Сдвинуть фишки вниз
     */
    void moveDown();
    /**
     * @brief Сдвинуть фишки вверх
     */
    void moveUp();
    /**
     * @brief Начальное положение фишек на доске
     */
    void init();
    /**
     * @brief Тасовка фишек
     */
    void shuffle();
	/**
	 * @brief Сериализация
	 */
	void serialize(std::ostream &os);
	/**
	 * @brief Десериализация
	 */
	void deserialize(std::istream &is);

private:
    std::vector<std::vector<Tile>> m_tiles;
    size_t m_empty_x;
    size_t m_empty_y;
};
