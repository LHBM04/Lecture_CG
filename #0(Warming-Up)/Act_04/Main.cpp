#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <random>       // <random> 헤더 사용
#include <chrono>       // 시간 기반 시드 및 sleep에 사용
#include <thread>       // std::this_thread::sleep_for 사용
#include <algorithm>    // std::shuffle 사용
#include <cctype>       // std::tolower 사용

#ifdef _WIN32
#include <windows.h>
#endif

// 플랫폼 종속적인 기능을 위한 함수
void ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // For Linux/Mac
    system("clear");
#endif
}

void SleepFor(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

// ANSI 색상 코드를 생성하는 유틸리티 함수
std::string GetColorCode(int code = -1) {
    if (code == -1) {
        return "\033[0m"; // Reset
    }
    return std::format("\033[38;5;{:d}m", code % 232); // 256 색상 모드 사용
}

// 좌표를 표현하는 구조체 (기존과 유사)
struct Coordinate {
    char value = '\0';
    bool isChecked = false;

    [[nodiscard]] constexpr bool operator==(const Coordinate& other) const noexcept {
        return value == other.value;
    }
};

// Point 구조체를 추가하여 row, col을 명확하게 관리
struct Point {
    std::size_t row = 0;
    std::size_t col = 0;
};

class Game {
public:
    // 생성자에서 초기화 및 난수 엔진 시드 설정
    Game() : m_rng(std::chrono::steady_clock::now().time_since_epoch().count()) {
        Init();
    }

    void Run() {
        m_isRunning = true;
        while (m_isRunning) {
            Draw();
            ProcessInput();
            Update();
        }
    }

private:
    // ### 게임 로직 및 상태 관련 함수들을 멤버 함수로 편입 ###
    void Init() noexcept {
        InitBoard();
        m_score = 0;
        m_tryCount = 20;
        m_isRunning = true;
    }

    void InitBoard() noexcept {
        // 1. 모든 칸을 비움
        for (auto& row : m_board) {
            for (auto& cell : row) {
                cell.value = '\0';
                cell.isChecked = false;
            }
        }

        // 2. 배치할 문자들을 미리 생성
        std::vector<char> tiles;
        for (std::size_t i = 0; i < (ROW_SIZE * COLUMN_SIZE - 1) / 2; ++i) {
            char letter = static_cast<char>('a' + i);
            tiles.push_back(letter);
            tiles.push_back(letter);
        }
        tiles.push_back('@'); // 조커 추가

        // 3. 문자들을 무작위로 섞음
        std::shuffle(tiles.begin(), tiles.end(), m_rng);

        // 4. 보드에 순서대로 배치
        for (std::size_t row = 0; row < ROW_SIZE; ++row) {
            for (std::size_t col = 0; col < COLUMN_SIZE; ++col) {
                if (!tiles.empty()) {
                    m_board[row][col].value = tiles.back();
                    tiles.pop_back();
                }
            }
        }
    }

    void Draw() const noexcept {
        ClearScreen();
        std::cout << "  ";
        for (std::size_t i = 0; i < COLUMN_SIZE; ++i) {
            std::cout << static_cast<char>('a' + i) << ' ';
        }
        std::cout << "\n";

        for (std::size_t row = 0; row < ROW_SIZE; ++row) {
            std::cout << row << ' ';
            for (std::size_t col = 0; col < COLUMN_SIZE; ++col) {
                const auto& cell = m_board[row][col];
                if (m_shouldHint || cell.isChecked) {
                    std::cout << GetColorCode(cell.value) << cell.value << ' ' << GetColorCode();
                } else {
                    std::cout << "* ";
                }
            }
            std::cout << '\n';
        }
        std::cout << std::format("\nScore: {:3d} Tries Left: {:3d}\n", m_score, m_tryCount);
    }

    void ProcessInput() {
        std::cout << "Enter command (e.g., a0 b1), or H(int), R(eset), Q(uit): ";
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) return;

        char command = std::toupper(line[0]);
        switch (command) {
            case 'R':
                Init();
                break;
            case 'H':
                ShowHint();
                break;
            case 'Q':
                m_isRunning = false;
                break;
            default:
                // ### 입력 파싱 로직 개선 ###
                if (auto points = ParseCoordinates(line)) {
                    ProcessTurn(points->first, points->second);
                    m_tryCount--;
                } else {
                    std::cout << "Invalid input. Please use format like 'a0 b1'.\n";
                    SleepFor(1000);
                }
                break;
        }
    }

    // 게임 상태 업데이트 (턴 종료 후)
    void Update() {
        if (m_tryCount <= 0 || IsGameFinished()) {
            Draw(); // 마지막 상태 보여주기
            std::cout << "Game Over!\n";
            std::cout << "Final Score: " << m_score << '\n';
            system("pause");
            m_isRunning = false; // 게임 루프 종료
        }
    }

    // ### CheckKan 함수를 클래스 내부로 가져와 역할에 맞게 분리 ###
    void ProcessTurn(Point p1, Point p2) {
        if (p1.row == p2.row && p1.col == p2.col) return; // 같은 칸 선택 방지

        auto& cell1 = m_board[p1.row][p1.col];
        auto& cell2 = m_board[p2.row][p2.col];

        if (cell1.isChecked || cell2.isChecked) return; // 이미 맞춘 칸 선택 방지

        // 선택한 두 칸을 잠시 보여줌
        cell1.isChecked = true;
        cell2.isChecked = true;
        Draw();
        SleepFor(1000);

        bool isMatch = false;
        if (cell1.value == '@' || cell2.value == '@') { // 조커 로직
            isMatch = true;
            char target = (cell1.value == '@') ? cell2.value : cell1.value;
            // 나머지 한 쌍도 찾아서 체크
            for (auto& row : m_board) {
                for (auto& cell : row) {
                    if (cell.value == target) cell.isChecked = true;
                }
            }
        } else if (cell1.value == cell2.value) { // 일반 매치 로직
            isMatch = true;
        }

        if (isMatch) {
            m_score += 10;
        } else {
            // 매치 실패 시 다시 뒤집음
            cell1.isChecked = false;
            cell2.isChecked = false;
        }
    }

    void ShowHint() {
        m_shouldHint = true;
        Draw();
        std::cout << "Showing Hint...\n";
        SleepFor(1000);
        m_shouldHint = false;
    }

    // ### 입력 파싱을 별도 함수로 분리 ###
    std::optional<std::pair<Point, Point>> ParseCoordinates(const std::string& input) {
        if (input.length() < 5) return std::nullopt;

        char c1, c2;
        int r1, r2;
        // C++20 std::format을 활용한 파싱 (C++17 이하라면 sscanf_s 또는 stringstream 사용)
        // 여기서는 간단한 직접 파싱 사용
        c1 = std::tolower(input[0]);
        r1 = input[1] - '0';
        c2 = std::tolower(input[3]);
        r2 = input[4] - '0';

        if (c1 < 'a' || c1 >= 'a' + COLUMN_SIZE || r1 < 0 || r1 >= ROW_SIZE ||
            c2 < 'a' || c2 >= 'a' + COLUMN_SIZE || r2 < 0 || r2 >= ROW_SIZE) {
            return std::nullopt;
        }

        return {{ { (size_t)r1, (size_t)(c1 - 'a') }, { (size_t)r2, (size_t)(c2 - 'a') } }};
    }

    bool IsGameFinished() const {
        for(const auto& row : m_board) {
            for(const auto& cell : row) {
                if (!cell.isChecked) return false;
            }
        }
        return true;
    }


private:
    // ### 멤버 변수(상태) ###
    static constexpr std::size_t ROW_SIZE = 5;
    static constexpr std::size_t COLUMN_SIZE = 5;

    // 버그 수정: [행][열] 순서로 변경
    Coordinate m_board[ROW_SIZE][COLUMN_SIZE];

    unsigned int m_score = 0;
    int m_tryCount = 0;
    bool m_shouldHint = false;
    bool m_isRunning = true;

    // ### 현대적인 난수 생성기 ###
    std::mt19937 m_rng;
};

int main() {
#ifdef _WIN32
    // Windows에서 ANSI 이스케이프 코드 활성화
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif

    // Game 객체를 main 내부에 생성
    Game game;
    game.Run(); // 게임 루프 실행

    return 0;
}