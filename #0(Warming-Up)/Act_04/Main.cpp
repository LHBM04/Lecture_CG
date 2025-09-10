#include <cctype>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <windows.h>

std::string_view GetColorCode(int code_ = -1)
{
    if (code_ == -1)
    {
        return "\033[0m";
    }

    return std::format("\033[3{:d}m", code_);
}

struct Coordinate
{
    bool isChecked;
    char value;

    [[nodiscard]]
    constexpr bool operator==(const Coordinate& other_) const noexcept
    {
        return value == other_.value;
    }

    [[nodiscard]]
    constexpr bool operator!=(const Coordinate& other_) const noexcept
    {
        return !(*this == other_);
    }
};

struct Game
{
    void Init() noexcept
    {
        InitBoard();
        InitScore();
        InitCount();
    }

    void Draw() const noexcept
    {
        system("cls");
        DrawBoard();
        DrawScore();
    }

    void InitBoard() noexcept
    {
        for (size_t row = 0; row < Game::ROW_SIZE; ++row)
        {
            for (size_t col = 0; col < Game::COLUMN_SIZE; ++col)
            {
                board[row][col].value = '\0';
                board[row][col].isChecked = false;
            }
        }

        for (std::size_t letterIdx = 0; letterIdx < 12; ++letterIdx)
        {
            for (std::size_t count = 0; count < 2; ++count)
            {
                std::size_t row, col;
                do
                {
                    row = std::rand() % Game::ROW_SIZE;
                    col = std::rand() % Game::COLUMN_SIZE;
                }
                while (board[row][col].value != '\0');

                board[row][col].value = static_cast<char>(letterIdx + 'a');
            }
        }

        for (size_t row = 0; row < Game::ROW_SIZE; ++row)
        {
            for (size_t col = 0; col < Game::COLUMN_SIZE; ++col)
            {
                if (board[row][col].value == '\0')
                {
                    board[row][col].value = '@';
                    return;
                }
            }
        }
    }

    void InitScore(int score_ = 0) noexcept
    {
        score = score_;
    }

    void InitCount(int count_ = 20) noexcept
    {
        count = count_;
    }

    void DrawBoard() const noexcept
    {
        std::cout << "  ";
        for (std::size_t count = 0; count < Game::COLUMN_SIZE; ++count)
        {
            std::cout << static_cast<char>(count + 'a') << ' ';
        }
        std::cout << '\n';

        for (std::size_t row = 0; row < Game::ROW_SIZE; ++row)
        {
            std::cout << row << ' ';
            for (std::size_t col = 0; col < Game::COLUMN_SIZE; ++col)
            {
                if (!shouldHint && !board[row][col].isChecked)
                {
                    std::cout << "* ";
                    continue;
                }

                if (shouldHint || board[row][col].isChecked)
                {
                    std::cout << GetColorCode(board[row][col].value - 'a');
                }
                else
                {
                    std::cout << GetColorCode();
                }

                std::cout << board[row][col].value << ' ';
                std::cout << GetColorCode();
            }
            std::cout << '\n';
        }
    }

    void DrawScore() const noexcept
    {
        std::cout << std::format("Score: {:3d} Count: {:3d}\n", score, count);
    }

    /**
    * @brief 보드판 세로 최대 크기.
    */
    static constexpr std::size_t COLUMN_SIZE = 5;

    /**
    * @brief 보드판 가로 최대 크기.
    */
    static constexpr std::size_t ROW_SIZE = 5;

    /**
    * @brief 보드판.
    */
    Coordinate board[COLUMN_SIZE][ROW_SIZE];

    /**
    * @brief 힌트 적용 여부.
    */
    bool shouldHint = false;

    /**
    * @brief 점수.
    */
    unsigned int score;

    unsigned int count;
};

Game game;

bool CheckKan(char lhs_[2], char rhs_[2])
{
    auto firstCol = std::tolower(lhs_[0]) - 'a';
    auto firstRow = lhs_[1] - '0';

    auto secondCol = std::tolower(rhs_[0]) - 'a';
    auto secondRow = rhs_[1] - '0';

    if (game.board[firstRow][firstCol].isChecked ||
        game.board[secondRow][secondCol].isChecked)
    {
        return false;
    }

    if (game.board[firstRow][firstCol].value == '@' ||
        game.board[secondRow][secondCol].value == '@')
    {
        game.board[firstRow][firstCol].isChecked = true;
        game.board[secondRow][secondCol].isChecked = true;

        char temp = (game.board[firstRow][firstCol].value == '@')
                        ? game.board[secondRow][secondCol].value
                        : game.board[firstRow][firstCol].value;

        for (int i = 0; i < Game::ROW_SIZE; ++i)
        {
            for (int j = 0; j < Game::COLUMN_SIZE; ++j)
            {
                if (game.board[i][j].value == temp)
                {
                    game.board[i][j].isChecked = true;
                }
            }
        }
        return true;
    }

    if (game.board[firstRow][firstCol] == game.board[secondRow][secondCol])
    {
        game.board[firstRow][firstCol].isChecked = true;
        game.board[secondRow][secondCol].isChecked = true;
        return true;
    }

    return false;
}

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    game.Init();

    bool isRunning = true;
    while (isRunning)
    {
        game.Draw();

        if (game.count <= 0)
        {
            std::cout << "Game Over!\n";
            std::cout << "Your Score: " << game.score << '\n';
            system("pause");
            break;
        }

        std::cout << "Enter command: ";
        std::string line;
        std::getline(std::cin, line);

        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string first_token;
        ss >> first_token;

        if (first_token.length() == 1)
        {
            char cmd = std::toupper(first_token[0]);
            if (cmd == 'R' ||
                cmd == 'H' ||
                cmd == 'Q')
            {
                switch (cmd)
                {
                    case 'R':
                        game.Init();
                        continue;
                    case 'H':
                        game.shouldHint = true;
                        game.Draw();
                        std::cout << "Showing Hint...\n";
                        Sleep(1000);
                        game.shouldHint = false;
                        game.count--; // Using a hint costs one turn
                        continue;
                    case 'Q':
                        isRunning = false;
                        continue;
                }
            }
        }

        std::string second_token;
        ss >> second_token;

        if (!first_token.empty() && !second_token.empty() && first_token.length() >= 2 && second_token.length() >= 2)
        {
            char first[3] = {first_token[0], first_token[1], 0};
            char second[3] = {second_token[0], second_token[1], 0};

            if (CheckKan(first, second))
            {
                game.score += 10;
            }
        }
        else
        {
            std::cout << "Invalid command or coordinate format.\n";
            Sleep(1000);
        }

        game.count--;
    }
    return 0;

}