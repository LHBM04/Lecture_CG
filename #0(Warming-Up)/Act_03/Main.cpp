#include <iostream>
#include <optional>
#include <array>
#include <format>
#include <algorithm>
#include <cmath>

struct Coordinate
{
    [[nodiscard]]
    constexpr std::size_t Length() const noexcept
    {
        return static_cast<std::size_t>(std::sqrt(x * x + y * y + z * z));
    }

    [[nodiscard]]
    friend std::istream& operator>>(std::istream& is_, Coordinate& point_)
    {
        is_ >> point_.x
            >> point_.y
            >> point_.z;
        return is_;
    }

    [[nodiscard]]
    friend std::ostream& operator<<(std::ostream& os_, const Coordinate& point_)
    {
        os_ << std::format("[{:d} {:d} {:d}]", point_.x, point_.y, point_.z);
        return os_;
    }

    unsigned long x, y, z;
};

constexpr std::size_t MAX_POINTS_COUNT = 10;
std::array<std::optional<Coordinate>, MAX_POINTS_COUNT> points{ };

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

    for (std::size_t index = currentTail; index > 0; --index)
    {
        std::size_t subIndex = index - 1;
        if (!points.at(subIndex).has_value())
        {
            continue;
        }

        const auto& [x, y, z] = points.at(subIndex).value();
        std::cout << std::format("{:d}. [{:d} {:d} {:d}]\n", index, x, y, z);
    }
}

void PushBack(const Coordinate& point_)
{
    if (currentTail >= MAX_POINTS_COUNT)
    {
        for (auto& iter : points)
        {
            if (iter == std::nullopt)
            {
                iter = point_;
                return;
            }
        }

        std::cerr << "Overflow\n";
        return;
    }

    points.at(currentTail++) = point_;
}

void PushFront(const Coordinate& point_)
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

[[nodiscard]]
std::size_t GetSize()
{
    return (currentTail - currentHead) + 1;
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

    auto predicate = [](const std::optional<Coordinate>& a, const std::optional<Coordinate>& b)
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
                Coordinate newPoint = { };
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
                Coordinate newPoint = { };
                std::cin >> newPoint.x >> newPoint.y >> newPoint.z;

                PushFront(newPoint);

                break;
            }
            case 'd':
            {
                PopFront();
                break;
            }
            case 'a':
            {
                std::cout << "Size: " << GetSize() << '\n';
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