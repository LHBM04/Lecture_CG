#include <algorithm>
#include <array>
#include <cmath>
#include <conio.h>
#include <cstddef>
#include <cstdlib>
#include <format>
#include <iostream>

/**
 * @brief 좌표를 정의합니다.
 */
struct Coordinate
{
    [[nodiscard]]
    unsigned long& operator[] (const std::size_t index_) noexcept
    {
        return (index_ == 0) ? x : y;
    }

    [[nodiscard]]
    constexpr unsigned long operator[] (const std::size_t index_) const noexcept
    {
        return (index_ == 0) ? x : y;
    }

    [[nodiscard]]
    constexpr auto operator<=>(const Coordinate& other_) const noexcept
        = default;

    friend std::istream& operator>>(std::istream& is_, Coordinate& coordinate_)
    {
        is_ >> coordinate_.x
            >> coordinate_.y;
        return is_;
    }

    friend std::ostream& operator<<(std::ostream& os_, const Coordinate& coordinate_)
    {
        os_ << std::format("({:d}, {:d})", coordinate_.x, coordinate_.y);
        return os_;
    }

    /**
     * @brief X 좌표.
     */
    unsigned long x;

    /**
     * @brief Y 좌표.
     */
    unsigned long y;
};

/**
 * @brief 직사각형을 정의합니다.
 */
struct Rectangle
{
    [[nodiscard]]
    Coordinate& operator[](const std::size_t index_)
    {
        switch (index_)
        {
            case 0:
            {
                return min;
            }
            case 1:
            {
                return max;
            }
            default:
            {
                throw std::out_of_range("Index out of range");
            }
        }
    }

    [[nodiscard]]
    const Coordinate& operator[](const std::size_t index_) const
    {
        switch (index_)
        {
            case 0:
            {
                return min;
            }
            case 1:
            {
                return max;
            }
            default:
            {
                throw std::out_of_range("Index out of range");
            }
        }
    }

    friend std::istream& operator>>(std::istream& is_, Rectangle& rectangle_)
    {
        is_ >> rectangle_.min[0]
            >> rectangle_.min[1]
            >> rectangle_.max[0]
            >> rectangle_.max[1];
        return is_;
    }

    friend std::ostream& operator<<(std::ostream& os_, const Rectangle& rectangle_)
    {
        os_ << rectangle_.min[0]
            << " - "
            << rectangle_.min[1]
            << " - "
            << rectangle_.max[0]
            << " - "
            << rectangle_.max[1];
        return os_;
    }

    /**
     * @brief 최소.
     */
    Coordinate min;

    /**
     * @brief 최대.
     */
    Coordinate max;
};

/**
 * @brief 월드를 정의합니다.
 */
class World
{
public:
    /**
     * @brief 생성자.
     *
     * @param width_ 월드 높이.
     * @param height_ 월드 너비.
     */
    explicit World(const std::size_t width_  = World::DEFAULT_WIDTH_SIZE,
                   const std::size_t height_ = World::DEFAULT_HEIGHT_SIZE) noexcept
        : _width(width_)
        , _height(height_)
        , _cursor(0)
        , _isSimulating(false)
    {
        _Init();
    }

    /**
     * @brief 월드를 시뮬레이션합니다.
     */
    void Simulate()
    {
        _isSimulating = true;
        while (_isSimulating)
        {
            _Draw();
            _Update();
        }
    }
private:
    /**
     * @brief 월드를 초기화합니다.
     */
    void _Init()
    {
        for (auto& [min, max] : _rectangle)
        {
            std::cout << "Enter two coordinates (x1 y1 x2 y2): ";
            std::cin >> min >> max;
        }
    }

    /**
     * @brief 월드를 그립니다.
     */
    void _Draw() const noexcept
    {
        system("cls");

        for (std::size_t row = 0; row < _height; ++row)
        {
            for (std::size_t col = 0; col < _width; ++col)
            {
                int cellValue = 0;

                for (std::size_t index = 0; index < RECTANGLE_COUNTS; ++index)
                {
                    const Rectangle& rectangle = _rectangle[index];

                    const unsigned long minX = rectangle.min.x;
                    const unsigned long minY = rectangle.min.y;
                    const unsigned long maxX = rectangle.max.x;
                    const unsigned long maxY = rectangle.max.y;

                    const bool colMatch = minX > maxX ? (col >= minX || col <= maxX)
                                                      : (col >= minX && col <= maxX);
                    const bool rowMatch = minY > maxY ? (row >= minY || row <= maxY)
                                                      : (row >= minY && row <= maxY);

                    if (rowMatch && colMatch)
                    {
                        cellValue |= (1 << index);
                    }
                }

                switch (cellValue)
                {
                    case 1:
                    {
                        std::cout << "\033[33m" << " 0 " << "\033[0m";
                        break;
                    }
                    case 2:
                    {
                        std::cout << "\033[34m" << " $ " << "\033[0m";
                        break;
                    }
                    case 3:
                    {
                        std::cout << "\033[36m" << " # " << "\033[0m";
                        break;
                    }
                    default:
                    {
                        std::cout << " * ";
                        break;
                    }
                }
            }
            std::cout << "\n";
        }
    }

    /**
     * @brief 월드를 업데이트합니다.
     */
    void _Update() noexcept
    {
        while (!kbhit())
        {
            // 대기
        }

        switch (_getch())
        {
            case 'x':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                min.x = (min.x > 0) ? min.x - 1 : _width - 1;
                max.x = (max.x > 0) ? max.x - 1 : _width - 1;

                break;
            }
            case 'X':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                min.x = (min.x < _width - 1) ? min.x + 1 : 0;
                max.x = (max.x < _width - 1) ? max.x + 1 : 0;

                break;
            }
            case 'y':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                min.y = (min.y > 0) ? min.y - 1 : _height - 1;
                max.y = (max.y > 0) ? max.y - 1 : _height - 1;

                break;
            }
            case 'Y':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                min.y = (min.y < _height - 1) ? min.y + 1 : 0;
                max.y = (max.y < _height - 1) ? max.y + 1 : 0;

                break;
            }
            case 's':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x + 1 < max.x)
                {
                    min.x++;
                }
                if (min.y + 1 < max.y)
                {
                    min.y++;
                }
                if (max.x > min.x + 1)
                {
                    max.x--;
                }
                if (max.y > min.y + 1)
                {
                    max.y--;
                }

                break;
            }
            case 'S':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x > 0)
                {
                    min.x--;
                }
                if (min.y > 0)
                {
                    min.y--;
                }
                if (max.x + 1 < _width)
                {
                    max.x++;
                }
                if (max.y + 1 < _height)
                {
                    max.y--;
                }

                break;
            }
            case 'i':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x > 0)
                {
                    min.x--;
                }
                if (max.x + 1 < _width)
                {
                    max.x++;
                }
                break;
            }
            case 'I':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x + 1 < max.x)
                {
                    min.x++;
                }
                if (max.x > min.x + 1)
                {
                    max.x--;
                }
                break;
            }
            case 'j':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.y > 0)
                {
                    min.y--;
                }
                if (max.y + 1 < _height)
                {
                    max.y++;
                }
                break;
            }
            case 'J':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.y + 1 < max.y)
                {
                    min.y++;
                }
                if (max.y > min.y + 1)
                {
                    max.y--;
                }
                break;
            }
            case 'a':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x > 0)
                {
                    min.x--;
                }
                if (max.x + 1 < _width)
                {
                    max.x++;
                }
                if (min.y + 1 < max.y)
                {
                    min.y++;
                }
                if (max.y > min.y + 1)
                {
                    max.y--;
                }
                break;
            }
            case 'A':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                if (min.x + 1 < max.x)
                {
                    min.x++;
                }
                if (max.x > min.x + 1)
                {
                    max.x--;
                }
                if (min.y > 0)
                {
                    min.y--;
                }
                if (max.y + 1 < _height)
                {
                    max.y++;
                }
                break;
            }
            case 'b':
            case 'B':
            {
                _cursor = (_cursor + 1) % RECTANGLE_COUNTS;
                auto& [min, max] = _rectangle[_cursor];

                unsigned long width, height;

                if (max.x >= min.x)
                {
                    width = max.x - min.x + 1;
                }
                else
                {
                    width = (_width - min.x) + max.x + 1;
                }

                if (max.y >= min.y)
                {
                    height = max.y - min.y + 1;
                }
                else
                {
                    height = (_height - min.y) + max.y + 1;
                }

                std::size_t area = width * height;
                std::cout << std::format("\nRectangle No. {:d}: {:d} x {:d} = {:d}\n",
                    _cursor + 1, width, height, area);
                std::cout << "Press any key to continue...";
                _getch();
                break;
            }
            case 'c':
            case 'C':
            {
                const unsigned long oldWidth  = _width;
                const unsigned long oldHeight = _height;

                _width  = (_width < MAX_WIDTH_SIZE) ? _width + 1 : MAX_WIDTH_SIZE;
                _height = (_height < MAX_HEIGHT_SIZE) ? _height + 1 : MAX_HEIGHT_SIZE;

                for (auto& [min, max] : _rectangle)
                {
                    if (max.x < min.x)
                    {
                        min.x = _width - (oldWidth - min.x);
                    }
                    if (max.y < min.y)
                    {
                        min.y = _height - (oldHeight - min.y);
                    }
                }

                break;
            }
            case 'd':
            case 'D':
            {
                const unsigned long oldWidth  = _width;
                const unsigned long oldHeight = _height;

                _width  = (_width > MIN_WIDTH_SIZE) ? _width - 1 : MIN_WIDTH_SIZE;
                _height = (_height > MIN_HEIGHT_SIZE) ? _height - 1 : MIN_HEIGHT_SIZE;

                for (auto& [min, max] : _rectangle)
                {
                    if (max.x < min.x)
                    {
                        min.x = _width - (oldWidth - min.x);
                    }
                    else
                    {
                        min.x = std::min(min.x, _width - 1);
                        max.x = std::min(max.x, _width - 1);
                    }

                    if (max.y < min.y)
                    {
                        min.y = _height - (oldHeight - min.y);
                    }
                    else
                    {
                        min.y = std::min(min.y, _height - 1);
                        max.y = std::min(max.y, _height - 1);
                    }
                }
                break;
            }
            case 'r':
            case 'R':
            {
                _Init();
                break;
            }
            case 'q':
            {
                _isSimulating = false;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    /**
     * @brief 공간 기본 너비.
     */
    static constexpr std::size_t DEFAULT_WIDTH_SIZE = 30;

    /**
     * @brief 공간 기본 높이.
     */
    static constexpr std::size_t DEFAULT_HEIGHT_SIZE = 30;

    /**
     * @brief 공간 최대 너비.
     */
    static constexpr std::size_t MAX_WIDTH_SIZE = DEFAULT_WIDTH_SIZE + 10;

    /**
     * @brief 공간 최대 높이.
     */
    static constexpr std::size_t MAX_HEIGHT_SIZE = DEFAULT_HEIGHT_SIZE + 10;

    /**
     * @brief 공간 최소 너비.
     */
    static constexpr std::size_t MIN_WIDTH_SIZE = DEFAULT_WIDTH_SIZE - 10;

    /**
     * @brief 공간 최소 높이.
     */
    static constexpr std::size_t MIN_HEIGHT_SIZE = DEFAULT_HEIGHT_SIZE - 10;

    /**
     * @brief 공간 너비.
     */
    unsigned long _width;

    /**
     * @brief 공간 높이.
     */
    unsigned long _height;

    /**
     * @brief 월드 내 직사각형 개수.
     */
    static constexpr std::size_t RECTANGLE_COUNTS = 2;

    /**
     * @brief 필드 내 직사각형들.
     */
    std::array<Rectangle, RECTANGLE_COUNTS> _rectangle = { };

    /**
     * @brief 현재 가리키는 직사각형 인덱스.
     */
    std::size_t _cursor;

    /**
     * @brief 프로그램 실행 여부.
     */
    bool _isSimulating;
};

int main()
{
    World world;
    world.Simulate();
}