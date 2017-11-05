#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

const int ADDITIONAL_CODE_BASE = 16;    // 16 binary digits

string reverseCopy(string::const_iterator begin, string::const_iterator end)
{
    auto temp = string(begin, end);
    auto left = temp.begin();
    auto right = temp.end() - 1;
    while (left < right)
    {
        iter_swap(left++, right--);
    }
    return temp;
}

string reverseCopy(const std::string &outString)
{
    return reverseCopy(outString.begin(), outString.end());
}

int convertToAdditionalCode(int binaryNumber)
{
    return ((1 << ADDITIONAL_CODE_BASE) + binaryNumber);
}

string decToBin(int number)
{
    string result = "";

    if (number < 0)
    {
        number = convertToAdditionalCode(number);
    }

    while (number > 0)
    {
        result += (number % 2 == 0 ? '0' : '1');
        number /= 2;
    }

    while (result.size() < ADDITIONAL_CODE_BASE)
    {
        result += '0';
    }

    return result;
}

int binToDec(const string &binaryNumber)
{
    int result = 0;
    int powerOfTwo = 1;
    for (auto e : binaryNumber)
    {
        result += powerOfTwo * (e - '0');
        powerOfTwo *= 2;
    }

    // If result >= 2 ^ (ADDITIONAL_CODE_BASE - 1) => negative
    if (result >= powerOfTwo / 2)
    {
        // 2 ^ ADDITIONAL_CODE_BASE == powerOfTwo due to size of the string
        result = -(powerOfTwo - result);
    }

    return result;
}

string sumBinaryNumbers(const string &a, const string &b)
{
    string tempString(ADDITIONAL_CODE_BASE, '0');

    short carry = 0;
    for (int i = 0; i < ADDITIONAL_CODE_BASE; i++)
    {
        short currentDigit = (a[i] - '0') + (b[i] - '0') + carry;

        tempString[i] = currentDigit % 2 + '0';
        carry = currentDigit / 2;
    }

    return tempString;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    int firstNumber = 0;
    int secondNumber = 0;
    cout << "Введите два числа: ";
    cin >> firstNumber >> secondNumber;

    auto firstNumberBin = decToBin(firstNumber);
    auto secondNumberBin = decToBin(secondNumber);

    cout << "Двоичное представление этих чисел в дополнительном коде: " << endl;
    cout << reverseCopy(decToBin(firstNumber)) << ' ';
    cout << reverseCopy(decToBin(secondNumber)) << endl;

    bool isNeedReverse = false;
    if (firstNumber < 0 && secondNumber < 0)
    {
        firstNumberBin = decToBin(-firstNumber);
        secondNumberBin = decToBin(-secondNumber);

        isNeedReverse = true;
    }

    string sumBinary = "0";
    sumBinary = sumBinaryNumbers(firstNumberBin, secondNumberBin);

    int sumDecimal = binToDec(sumBinary);
    if (isNeedReverse)
    {
        sumDecimal = -sumDecimal;
        sumBinary = decToBin(convertToAdditionalCode(sumDecimal));
    }

    cout << "Сумма в двоичном формате: ";
    cout << reverseCopy(sumBinary.begin(), sumBinary.end()) << endl;

    cout << "Сумма в десятичном формате: ";
    cout << sumDecimal << endl;
}