#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>

constexpr std::size_t MAT_SIZE = 4;

int matrix1[MAT_SIZE][MAT_SIZE] = {};
int matrix2[MAT_SIZE][MAT_SIZE] = {};

void Init(int mat_[MAT_SIZE][MAT_SIZE]) noexcept
{
    static bool isInitialized = false;
    if (!isInitialized)
    {
        std::srand(std::time(nullptr));
        isInitialized = true;
    }

    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            mat_[row][col] = std::rand() % 9 + 1;
        }
    }
}

void Print(const int mat_[MAT_SIZE][MAT_SIZE], const int target_ = -1) noexcept
{
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            if (target_ != -1)
            {
                if (mat_[row][col] % target_ == 0)
                {
                    std::cout << mat_[row][col] << ' ';
                }
                else
                {
                    std::cout << "[]" << ' ';
                }

                continue;
            }

            std::cout << mat_[row][col] << ' ';
        }

        std::cout << '\n';
    }
}

void Add(const int lhs_[MAT_SIZE][MAT_SIZE],
         const int rhs_[MAT_SIZE][MAT_SIZE],
         int       res_[MAT_SIZE][MAT_SIZE]) noexcept
{
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            res_[row][col] = lhs_[row][col] + rhs_[row][col];
        }
    }
}

void Subtract(const int lhs_[MAT_SIZE][MAT_SIZE],
              const int rhs_[MAT_SIZE][MAT_SIZE],
              int       res_[MAT_SIZE][MAT_SIZE]) noexcept
{
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            res_[row][col] = lhs_[row][col] - rhs_[row][col];
        }
    }
}

void Multiply(const int lhs_[MAT_SIZE][MAT_SIZE],
              const int rhs_[MAT_SIZE][MAT_SIZE],
              int       res_[MAT_SIZE][MAT_SIZE]) noexcept
{
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            res_[row][col] = 0;

            for (std::size_t k = 0; k < MAT_SIZE; ++k)
            {
                res_[row][col] += lhs_[row][k] * rhs_[k][col];
            }
        }
    }
}

int Determinant(const int mat_[MAT_SIZE][MAT_SIZE]) noexcept
{
    return mat_[0][0] * (mat_[1][1] * (mat_[2][2] * mat_[3][3] - mat_[2][3] * mat_[3][2]) -
                         mat_[1][2] * (mat_[2][1] * mat_[3][3] - mat_[2][3] * mat_[3][1]) +
                         mat_[1][3] * (mat_[2][1] * mat_[3][2] - mat_[2][2] * mat_[3][1]))

         - mat_[0][1] * (mat_[1][0] * (mat_[2][2] * mat_[3][3] - mat_[2][3] * mat_[3][2]) -
                         mat_[1][2] * (mat_[2][0] * mat_[3][3] - mat_[2][3] * mat_[3][0]) +
                         mat_[1][3] * (mat_[2][0] * mat_[3][2] - mat_[2][2] * mat_[3][0]))

         + mat_[0][2] * (mat_[1][0] * (mat_[2][1] * mat_[3][3] - mat_[2][3] * mat_[3][1]) -
                         mat_[1][1] * (mat_[2][0] * mat_[3][3] - mat_[2][3] * mat_[3][0]) +
                         mat_[1][3] * (mat_[2][0] * mat_[3][1] - mat_[2][1] * mat_[3][0]))

         - mat_[0][3] * (mat_[1][0] * (mat_[2][1] * mat_[3][2] - mat_[2][2] * mat_[3][1]) -
                         mat_[1][1] * (mat_[2][0] * mat_[3][2] - mat_[2][2] * mat_[3][0]) +
                         mat_[1][2] * (mat_[2][0] * mat_[3][1] - mat_[2][1] * mat_[3][0]));
}

void Transpose(const int mat_[MAT_SIZE][MAT_SIZE],
               int       res_[MAT_SIZE][MAT_SIZE]) noexcept
{
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            res_[col][row] = mat_[row][col];
        }
    }
}

[[nodiscard]]
int FindMin(const int mat_[MAT_SIZE][MAT_SIZE]) noexcept
{
    int res = mat_[0][0];
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            if (res > mat_[row][col])
            {
                res = mat_[row][col];
            }
        }
    }
    return res;
}

[[nodiscard]]
int FindMax(const int mat_[MAT_SIZE][MAT_SIZE]) noexcept
{
    int res = mat_[0][0];
    for (std::size_t row = 0; row < MAT_SIZE; ++row)
    {
        for (std::size_t col = 0; col < MAT_SIZE; ++col)
        {
            if (res < mat_[row][col])
            {
                res = mat_[row][col];
            }
        }
    }
    return res;
}

int main()
{
    std::cout << "====[1st Matrix]====\n";
    Init(matrix1);
    Print(matrix1);

    std::cout << "====[2nd Matrix]====\n";
    Init(matrix2);
    Print(matrix2);

    bool isRunning = true;
    while (isRunning)
    {
        std::cout << "Enter command: ";

        char command = '\0';
        std::cin >> command;

        if (std::isdigit(command))
        {
            static bool trigger = false;
            trigger = !trigger;

            if (trigger)
            {
                const int target = command - '0';

                std::cout << "====[1st Matrix - Filtered]====\n";
                Print(matrix1, target);

                std::cout << "====[2nd Matrix - Filtered]====\n";
                Print(matrix2, target);
            }
            else
            {
                std::cout << "====[1st Matrix]====\n";
                Print(matrix1);

                std::cout << "====[2nd Matrix]====\n";
                Print(matrix2);
            }

            continue;
        }

        switch (command)
        {
            case 'm':
            {
                int res[MAT_SIZE][MAT_SIZE] = {};
                Multiply(matrix1, matrix2, res);

                std::cout << "====[Result Matrix]====\n";
                Print(res);

                break;
            }
            case 'a':
            {
                int res[MAT_SIZE][MAT_SIZE] = {};
                Add(matrix1, matrix2, res);

                std::cout << "====[Result Matrix]====\n";
                Print(res);

                break;
            }
            case 'd':
            {
                int res[MAT_SIZE][MAT_SIZE] = {};
                Subtract(matrix1, matrix2, res);

                std::cout << "====[Result Matrix]====\n";
                Print(res);

                break;
            }
            case 'r':
            {
                const int det1 = Determinant(matrix1);
                const int det2 = Determinant(matrix2);

                std::cout << "====[Determinants]====\n";
                std::cout << "det(mat1): " << det1 << '\n';
                std::cout << "det(mat2): " << det2 << '\n';

                break;
            }
            case 't':
            {
                {
                    int res1[MAT_SIZE][MAT_SIZE] = {};
                    int res2[MAT_SIZE][MAT_SIZE] = {};

                    Transpose(matrix1, res1);
                    Transpose(matrix2, res2);

                    std::cout << "====[Transposed Matrix 1]====\n";
                    Print(res1);

                    std::cout << "====[Transposed Matrix 2]====\n";
                    Print(res2);
                }
                {
                    const int det1 = Determinant(matrix1);
                    const int det2 = Determinant(matrix2);

                    std::cout << "====[Determinants]====\n";
                    std::cout << "det(mat1): " << det1 << '\n';
                    std::cout << "det(mat2): " << det2 << '\n';
                }

                break;
            }
            case 'e':
            {
                static bool trigger = false;
                trigger = !trigger;

                static int subMatrix1[MAT_SIZE][MAT_SIZE] = {};
                static int subMatrix2[MAT_SIZE][MAT_SIZE] = {};

                if (trigger)
                {
                    std::memcpy(subMatrix1, matrix1, sizeof(int) * MAT_SIZE * MAT_SIZE);
                    std::memcpy(subMatrix2, matrix2, sizeof(int) * MAT_SIZE * MAT_SIZE);

                    const int matrix1Max = FindMin(matrix1);
                    const int matrix2Max = FindMin(matrix2);

                    for (std::size_t row = 0; row < MAT_SIZE; ++row)
                    {
                        for (std::size_t col = 0; col < MAT_SIZE; ++col)
                        {
                            matrix1[row][col] -= matrix1Max;
                            matrix2[row][col] -= matrix2Max;
                        }
                    }
                    std::cout << "====[1st Matrix - Min Extracted]====\n";
                    Print(matrix1);

                    std::cout << "====[2nd Matrix - Min Extracted]====\n";
                    Print(matrix2);
                }
                else
                {
                    std::memcpy(matrix1, subMatrix1, sizeof(int) * MAT_SIZE * MAT_SIZE);
                    std::memcpy(matrix2, subMatrix2, sizeof(int) * MAT_SIZE * MAT_SIZE);

                    std::cout << "====[1st Matrix]====\n";
                    Print(matrix1);

                    std::cout << "====[2nd Matrix]====\n";
                    Print(matrix2);
                }

                break;
            }
            case 'f':
            {
                static bool trigger = false;
                trigger = !trigger;

                static int subMatrix1[MAT_SIZE][MAT_SIZE] = {};
                static int subMatrix2[MAT_SIZE][MAT_SIZE] = {};

                if (trigger)
                {
                    std::memcpy(subMatrix1, matrix1, sizeof(int) * MAT_SIZE * MAT_SIZE);
                    std::memcpy(subMatrix2, matrix2, sizeof(int) * MAT_SIZE * MAT_SIZE);

                    const int mat1Max = FindMax(matrix1);
                    const int mat2Max = FindMax(matrix2);

                    for (std::size_t row = 0; row < MAT_SIZE; ++row)
                    {
                        for (std::size_t col = 0; col < MAT_SIZE; ++col)
                        {
                            matrix1[row][col] += mat1Max;
                            matrix2[row][col] += mat2Max;
                        }
                    }

                    std::cout << "====[1st Matrix - Max Extracted]====\n";
                    Print(matrix1);

                    std::cout << "====[2nd Matrix - Max Extracted]====\n";
                    Print(matrix2);
                }
                else
                {
                    std::memcpy(matrix1, subMatrix1, sizeof(int) * MAT_SIZE * MAT_SIZE);
                    std::memcpy(matrix2, subMatrix2, sizeof(int) * MAT_SIZE * MAT_SIZE);

                    std::cout << "====[1st Matrix]====\n";
                    Print(matrix1);

                    std::cout << "====[2nd Matrix]====\n";
                    Print(matrix2);
                }

                break;
            }
            case 's':
            {
                Init(matrix1);
                Init(matrix2);

                std::cout << "====[1st Matrix]====\n";
                Print(matrix1);

                std::cout << "====[2nd Matrix]====\n";
                Print(matrix2);

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
}