#include <iostream>
#include <optional>
#include <array>
#include <format>
#include <algorithm>
#include <cmath>

struct Point
{
    [[nodiscard]]
    constexpr std::size_t Length() const noexcept
    {
        return static_cast<std::size_t>(std::sqrt(x * x + y * y + z * z));
    }

    int x, y, z;
};

constexpr std::size_t MAX_POINTS_COUNT = 10;
std::array<std::optional<Point>, MAX_POINTS_COUNT> points{ };

std::size_t currentHead;
std::size_t currentTail;

void Reset()
{
    points.fill(std::nullopt);

    currentHead = 0;
    currentTail = 0;
}

void Print()
{
    if (currentHead == currentTail)
    {
        std::cout << "[empty]\n";
        return;
    }

    for (std::size_t index = currentTail; index > currentHead; --index)
    {
        std::size_t subIndex = index - 1;

        if (!points.at(subIndex).has_value())
        {
            std::cout << "[empty]\n";
            continue;
        }

        const Point& point = points.at(subIndex).value();
        std::cout << std::format("{:d}. [{:d} {:d} {:d}]\n", index, point.x, point.y, point.z);
    }
}

void PushBack(const Point& point_)
{
    if (currentTail >= MAX_POINTS_COUNT)
    {
        std::cerr << "Overflow\n";
        return;
    }

    auto begin = points.begin() + currentHead;
    auto end   = points.begin() + currentTail;

    auto result = std::find(begin, end, std::nullopt);
    if (result != end)
    {
        std::size_t index = std::distance(points.begin(), result);
        points.at(index) = point_;

        return;
    }

    points.at(currentTail++) = point_;
}

void PushFront(const Point& point_)
{
    if (currentTail >= MAX_POINTS_COUNT)
    {
        std::cerr << "Overflow\n";
        return;
    }

    for (std::size_t index = currentTail; index > currentHead; index--)
    {
        points.at(index) = points.at(index - 1);
    }

    points.at(currentHead) = point_;
    ++currentTail;
}

void PopBack()
{
    if (currentHead == currentTail)
    {
        std::cerr << "Underflow\n";
        return;
    }

    points.at(--currentTail) = std::nullopt;
}

void PopFront()
{
    if (currentHead == currentTail)
    {
        std::cerr << "Underflow\n";
        return;
    }

    points.at(currentHead++) = std::nullopt;
}

void Move()
{
    const auto first = points.at(currentHead);

    for (std::size_t index = currentHead; index < currentTail - 1; ++index)
    {
        points.at(index) = points.at(index + 1);
    }

    points.at(currentTail - 1) = first;
}

void Sort()
{
    const auto subBegin  = points.begin() + currentHead;
    const auto subEnd    = points.begin() + currentTail;

    auto predicate = [](const std::optional<Point>& a, const std::optional<Point>& b)
    {
        if (!a.has_value())
        {
            return false;
        }
        if (!b.has_value())
        {
            return true;
        }

        return a->Length() < b->Length();
    };

    std::sort(subBegin, subEnd, predicate);
}

int main()
{
    Reset();
    Print();

    bool isRunning = true;
    while (isRunning)
    {
        char command = '\0';
        std::cin >> command;

        switch (command)
        {
            case '+':
            {
                Point newPoint = { };
                std::cin >> newPoint.x >> newPoint.y >> newPoint.z;

                PushBack(newPoint);

                break;
            }
            case '-':
            {
                PopBack();
                break;
            }
            case 'e':
            {
                Point newPoint = { };
                std::cin >> newPoint.x >> newPoint.y >> newPoint.z;

                PushFront(newPoint);

                break;
            }
            case 'd':
            {
                PopFront();
                break;
            }
            case 'b':
            {
                Move();
                break;
            }
            case 'c':
            {
                Reset();
            }
            case 'f':
            {
                Sort();
                break;
            }
            case 'q':
            {
                isRunning = false;
            }
            default:
            {
                break;
            }
        }

        Print();
    }
}