#include <array>
#include <iostream>
#include <conio.h>
#include <cstddef>

struct Coordinate
{
    unsigned long long x;
    unsigned long long y;
};

class World
{
public:
    World()
        : isRunning(false)
    {
        Init();
    }

    void Simulate() noexcept
    {
        isRunning = true;
        while (isRunning)
        {
            Draw();
            Update();
        }
    }
private:
    void Init() noexcept
    {
        for (auto& field : fields)
        {
            for (int& col : field)
            {
                col = 0;
            }
        }

        for (int count = 0; count < 4; ++count)
        {
            int x = std::rand() % WIDTH_SIZE;
            int y = std::rand() % HEIGHT_SIZE;

            fields[y][x] = -1;
        }

        player = {0 ,0};
        fields[0][0] = 1;

    }

    void Draw() const noexcept
    {
        system("cls");

        for (std::size_t row = 0; row < HEIGHT_SIZE; ++row)
        {
            for (std::size_t col = 0; col < WIDTH_SIZE; ++col)
            {
                if (row == player.y && col == player.x)
                {
                    std::cout << "\033[31m " << "P" << " \033[0m";
                    continue;
                }

                if (fields[row][col] == -1)
                {
                    std::cout << "\033[32m " << "X" << " \033[0m";
                }
                else
                {
                    if (fields[row][col] == 0)
                    {
                        std::cout << "\033[37m " << fields[row][col] << " \033[0m";
                    }
                    else
                    {
                        std::cout << "\033[33m " << fields[row][col] << " \033[0m";
                    }
                }
            }
            std::cout << '\n';
        }
    }

    void Update() noexcept
    {
        switch (_getch())
        {
            case 'w':
            {
                if (player.y > 0 && fields[player.y - 1][player.x] != -1)
                {
                    player.y--;
                    fields[player.y][player.x]++;
                }

                break;
            }
            case 'a':
            {
                if (player.x > 0 && fields[player.y][player.x - 1] != -1)
                {
                    player.x--;
                    fields[player.y][player.x]++;
                }

                break;
            }
            case 's':
            {
                if (player.y < HEIGHT_SIZE && fields[player.y + 1][player.x] != -1)
                {
                    player.y++;
                    fields[player.y][player.x]++;
                }

                break;
            }
            case 'd':
            {
                if (player.x < WIDTH_SIZE && fields[player.y][player.x + 1] != -1)
                {
                    player.x++;
                    fields[player.y][player.x]++;
                }

                break;
            }
            case 13:
            {
                Init();
                break;
            }
            case 'q':
            {
                isRunning = false;
                break;
            }
            default:
            {
                break;
            }
        }
    }

    static constexpr std::size_t WIDTH_SIZE  = 30;
    static constexpr std::size_t HEIGHT_SIZE = 30;

    bool isRunning;

    int fields[WIDTH_SIZE][HEIGHT_SIZE];
    Coordinate player;
};

int main()
{
    World world;
    world.Simulate();
}