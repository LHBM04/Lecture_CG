#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#include <windows.h>

int main()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return -1;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return -1;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    std::vector<std::string> lines;

    std::string input;
    std::getline(std::cin, input);
    std::ifstream file(input);

    if (!file.is_open())
    {
        return 1;
    }

    std::string casheLine;
    while (std::getline(file, casheLine))
    {
        lines.push_back(casheLine);
    }

    // Add newlines when displaying file contents
    for (auto iter : lines)
    {
        std::cout << iter << '\n';
    }
    std::cout.flush();

    bool isRunning = true;
    while (isRunning)
    {
        switch (getchar())
        {
            case 'a':
            {
                static bool trigger = false;
                trigger = !trigger;

                for (const auto& iter1 : lines)
                {
                    for (const auto iter2 : iter1)
                    {
                        if (trigger)
                        {
                            std::cout << static_cast<char>(std::toupper(iter2));
                            continue;
                        }

                        std::cout << iter2;
                    }
                    std::cout << '\n'; // Add newline after each line
                }
                std::cout.flush();
                break;
            }
            case 'b':
            {
                std::size_t wordCount = 0;
                for (const auto& iter1 : lines)
                {
                    for (const auto iter2 : iter1)
                    {
                        if (iter2 == ' ')
                        {
                            ++wordCount;
                        }
                    }
                }

                std::cout << wordCount << '\n';

                break;
            }
            case 'c':
            {
                for (const auto& line : lines)
                {
                    std::stringstream ss(line);
                    std::string word;

                    while (ss >> word)
                    {
                        if (!word.empty() && std::isupper(word[0]))
                        {
                            constexpr const char* RESET  = "\033[0m";
                            constexpr const char* YELLOW = "\033[33m";

                            std::cout << YELLOW << word << RESET << " ";
                        }
                        else
                        {
                            std::cout << word << " ";
                        }
                    }
                    std::cout << '\n';
                }
                break;
            }
            case 'd':
            {
                for (auto& iter1 : lines)
                {
                    auto temp = iter1;
                    std::ranges::reverse(temp);
                    std::cout << temp << '\n';
                }

                break;
            }
            case 'e':
            {
                static bool trigger = false;
                trigger = !trigger;

                for (std::string& r : lines)
                {
                    std::ranges::replace(r, trigger ? ' ' : '/', trigger ? '/' : ' ');
                    std::cout << r << '\n'; // 수정된 내용 출력
                }

                break;
            }
            case 'f':
            {
                static bool trigger = false;
                trigger = !trigger;

                for (auto& iter1 : lines)
                {
                    bool trigger2 = false;
                    for (auto& iter2 : iter1)
                    {
                        if (iter2 == ' ' || iter2 == '/')
                        {
                            trigger2 = true;
                        }
                    }

                    if (trigger2)
                    {
                        std::ranges::reverse(iter1);
                    }

                    std::cout << iter1 << '\n';
                }

                break;
            }
            case 'g':
            {
                static bool trigger= false;
                trigger = !trigger;

                if (trigger)
                {
                    char targetChar, replacementChar;
                    std::cin >> targetChar >> replacementChar;

                    for (auto& line : lines)
                    {
                        std::string temp = line;

                        for (auto& character : temp)
                        {
                            if (std::isupper(character) && std::tolower(character) == std::tolower(targetChar))
                            {
                                std::cout << static_cast<char>(std::toupper(replacementChar));
                            }
                            else if (std::islower(character) && std::tolower(character) == std::tolower(targetChar))
                            {
                                std::cout << static_cast<char>(std::tolower(replacementChar));
                            }
                            else
                            {
                                std::cout << character;
                            }
                        }
                        std::cout << '\n';
                    }
                }
                else
                {
                    for (auto& line : lines)
                    {
                        std::cout << line << '\n';
                    }
                }

                break;
            }
            case 'h':
            {
                static bool trigger = false;
                trigger = !trigger;

                for (auto& line : lines)
                {
                    for (auto& character : line)
                    {
                        std::cout << character;

                        if (trigger && std::isdigit(character))
                        {
                            std::cout << '\n';
                        }
                    }
                    std::cout << '\n';
                }

                break;
            }
            case 'i':
            {
                enum SortType
                {
                    None,
                    Ascending,
                    Descending,
                };

                static SortType sortType = None;
                sortType = static_cast<SortType>((sortType + 1) % 3);

                struct Test
                {
                    std::string content;
                    std::size_t alphabetCount;
                };

                std::vector<Test> tests;

                for (auto& line : lines)
                {
                    for (auto character : line)
                    {
                        if (std::isalpha(character))
                        {
                            if (tests.empty() || tests.back().content != line)
                            {
                                tests.push_back({ line, 1 });
                            }
                            else
                            {
                                ++tests.back().alphabetCount;
                            }
                        }
                    }

                    switch (sortType)
                    {
                        case Ascending:
                        {
                            std::ranges::sort(tests.begin(), tests.end(),
                                [](const Test& a, const Test& b) {
                                    return a.alphabetCount < b.alphabetCount;
                                });
                            break;
                        }
                        case Descending:
                        {
                            std::ranges::sort(tests.begin(), tests.end(),
                                [](const Test& a, const Test& b) {
                                    return a.alphabetCount > b.alphabetCount;
                                });
                            break;
                        }
                        case None:
                        {
                            break;
                        }
                    }

                    // Display results
                    for (const auto& [content, alphabetCount] : tests)
                    {
                        std::cout << content << " (" << alphabetCount << ")\n";
                    }
                }

                break;
            }
            case 'j':
            {
                std::string targetWord = "";
                std::size_t wordCount  = 0;
                std::cin >> targetWord;

                for (const auto& line : lines)
                {
                    std::stringstream ss(line);
                    std::string word;

                    while (ss >> word)
                    {
                        std::string temp1 = word;
                        std::ranges::transform(temp1 , temp1.begin(), [](const unsigned char c)
                        {
                            return std::tolower(c);
                        });

                        std::string temp2 = targetWord;
                        std::ranges::transform(temp2, temp2.begin(), [](const unsigned char c)
                        {
                            return std::tolower(c);
                        });

                        if (temp1 == temp2)
                        {
                            std::cout << "\033[32m" << word << "\033[0m ";
                            wordCount++;
                            continue;
                        }

                        std::cout << word << " ";
                    }
                    std::cout << '\n';
                }

                std::cout << "targetWord: " << wordCount << '\n';

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

    file.close();
    return 0;
}