#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <stdexcept>

struct Coordinate
{
    [[nodiscard]]
    constexpr unsigned long LengthSq() const noexcept
    {
        return x * x + y * y + z * z;
    }

    [[nodiscard]]
    constexpr double Length() const noexcept
    {
        return std::sqrt(static_cast<double>(LengthSq()));
    }

    friend std::istream& operator>>(std::istream& is_, Coordinate& point_)
    {
        is_ >> point_.x >> point_.y >> point_.z;
        return is_;
    }

    friend std::ostream& operator<<(std::ostream& os_, const Coordinate& point_)
    {
        os_ << "[" << point_.x << " " << point_.y << " " << point_.z << "]";
        return os_;
    }

    unsigned long x, y, z;
};

class PointQueue
{
public:
    void Reset()
    {
        points_.clear();
    }

    void Print() const
    {
        if (points_.empty())
        {
            std::cout << "[empty]\n";
            return;
        }

        for (std::size_t i = 0; i < points_.size(); ++i)
        {
            std::cout << (i + 1) << ". " << points_[i] << "\n";
        }
    }

    void PushBack(const Coordinate& point)
    {
        if (points_.size() >= MAX_POINTS_COUNT)
        {
            std::cerr << "Overflow\n";
            return;
        }

        points_.push_back(point);
    }

    void PushFront(const Coordinate& point)
    {
        if (points_.size() >= MAX_POINTS_COUNT)
        {
            std::cerr << "Overflow\n";
            return;
        }

        points_.insert(points_.begin(), point);
    }

    void PopBack()
    {
        if (points_.empty())
        {
            std::cerr << "Underflow\n";
            return;
        }

        points_.pop_back();
    }

    void PopFront()
    {
        if (points_.empty())
        {
            std::cerr << "Underflow\n";
            return;
        }

        points_.erase(points_.begin());
    }

    [[nodiscard]]
    std::size_t GetSize() const
    {
        return points_.size();
    }

    void Move()
    {
        if (points_.size() < 2)
        {
            return;
        }
        std::rotate(points_.begin(), points_.begin() + 1, points_.end());
    }

    void Sort()
    {
        std::sort(points_.begin(), points_.end(), [](const Coordinate& a, const Coordinate& b)
        {
            return a.LengthSq() < b.LengthSq();
        });
    }

private:
    static constexpr std::size_t MAX_POINTS_COUNT = 10;
    std::vector<Coordinate> points_;
};

int main()
{
    PointQueue queue;
    queue.Print();

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
                std::cin >> newPoint;
                queue.PushBack(newPoint);
                break;
            }
            case '-':
            {
                queue.PopBack();
                break;
            }
            case 'e':
            {
                Coordinate newPoint = { };
                std::cin >> newPoint;
                queue.PushFront(newPoint);
                break;
            }
            case 'd':
            {
                queue.PopFront();
                break;
            }
            case 'a':
            {
                std::cout << "Size: " << queue.GetSize() << '\n';
                break;
            }
            case 'b':
            {
                queue.Move();
                break;
            }
            case 'c':
            {
                queue.Reset();
                break;
            }
            case 'f':
            {
                queue.Sort();
                break;
            }
            case 'q':
            {
                isRunning = false;
                break;
            }
            default:
            {
                // Ignore unknown commands
                break;
            }
        }

        if (isRunning)
        {
            queue.Print();
        }
    }
}