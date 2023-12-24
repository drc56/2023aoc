#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>
#include <limits>
#include <regex>

int convertCharNumToIntNum(char const intChar)
{
    return static_cast<int>(intChar - '0');
}

bool checkColorValue(char const color, int const val, int const blue, int const red, int const green)
{
    if(color == 'b')
        return val <= blue;
    else if(color == 'r')
        return val <= red;
    else if (color == 'g')
        return val <= green;
    return false;
}

bool extractColorNum(std::string const& section, int& convertedNum, char& extractedColor)
{
    const std::regex blue("([0-9])+\\s([blue]).+");
    const std::regex red("([0-9]+)\\s([red]).+");
    const std::regex green("([0-9]+)\\s([green]).+");
    std::smatch regexMatch;
    if(std::regex_match(section, regexMatch, blue))
    {
        convertedNum = std::stoi(regexMatch[0]);
        extractedColor = 'b';
        return true;
    }
    else if(std::regex_match(section, regexMatch, red))
    {
        convertedNum = std::stoi(regexMatch[0]);
        extractedColor = 'r';
        return true;
    }
    else if(std::regex_match(section, regexMatch, green))
    {
        convertedNum = std::stoi(regexMatch[0]);
        extractedColor = 'g';
        return true;
    }
    else{
        return false;
    }
}

bool checkValidRound(std::string const& round, int const blue, int const red, int const green)
{
    bool roundValid = true;
    size_t pos = 0;
    for(;;)
    {
        auto nextPos = round.find(",", pos);
        int val;
        char color;
        if(nextPos == std::string::npos)
        {

            if(extractColorNum(round.substr(pos, round.size()-pos), val, color))
            {
                roundValid = roundValid && checkColorValue(color, val, blue, red, green);
                return roundValid;
            }
            return false;
        }
        else
        {
            if(extractColorNum(round.substr(pos, nextPos - pos), val, color))
            {
                roundValid = roundValid && checkColorValue(color, val, blue, red, green);
                pos = nextPos + 2;
                continue;
            }
            return false;
        }
    }
}

bool checkValidGame(std::string const& game, int const blue, int const red, int const green)
{
    bool gameValid = true;
    size_t pos = 0;
    for(;;)
    {
        auto nextPos = game.find(";", pos);
        if(nextPos == std::string::npos)
        {
            gameValid = gameValid && checkValidRound(game.substr(pos, (game.size()-pos)), blue, red, green);
            return gameValid;
        }
        else
        {
            gameValid = gameValid && checkValidRound(game.substr(pos, (nextPos-pos)), blue, red, green);
            pos = nextPos + 2;
        }
    }
}

void getMinMaxRound(std::string const& round, int& minBlue, int& minRed, int& minGreen)
{
    size_t pos = 0;
    for(;;)
    {
        auto nextPos = round.find(",", pos);
        int val;
        char color;
        if(nextPos == std::string::npos)
        {
            if(extractColorNum(round.substr(pos, (round.size()) - pos), val, color))
            {
                if(color == 'r')
                    minRed < val ? minRed = val : minRed = minRed;
                else if(color == 'g')
                    minGreen < val ? minGreen = val : minGreen = minGreen;
                else if (color == 'b')
                    minBlue < val ? minBlue = val : minBlue = minBlue;
                else
                    return;
            }
            return;
        }
        else
        {
            if(extractColorNum(round.substr(pos, nextPos - pos), val, color))
            {
                if(color == 'r')
                    minRed < val ? minRed = val : minRed = minRed;
                else if(color == 'g')
                    minGreen < val ? minGreen = val : minGreen = minGreen;
                else if (color == 'b')
                    minBlue < val ? minBlue = val : minBlue = minBlue;
                else
                    return;
                pos = nextPos + 2;
                continue;
            }
        }
    }
}

int getMinPowerGame(std::string const& game)
{
    bool gameValid = true;
    int minBlue = 0, minRed = 0, minGreen = 0;
    size_t pos = 0;
    for(;;)
    {
        int blue = 0, red = 0, green = 0;
        auto nextPos = game.find(";", pos);
        if(nextPos == std::string::npos)
        {
            getMinMaxRound(game.substr(pos, (game.size()) - pos), blue, red, green);
            minBlue < blue ? minBlue = blue : minBlue = minBlue;
            minGreen < green ? minGreen = green : minGreen = minGreen;
            minRed < red ? minRed = red : minRed = minRed;
            return minBlue * minGreen * minRed;
        }
        else
        {
            getMinMaxRound(game.substr(pos, (nextPos-pos)), blue, red, green);
            minBlue < blue ? minBlue = blue : minBlue = minBlue;
            minGreen < green ? minGreen = green : minGreen = minGreen;
            minRed < red ? minRed = red : minRed = minRed;
            pos = nextPos + 2;
        }
    }
}

int validGameCheck(std::string const& filePath, bool powerCheck)
{
    std::ifstream dataFile(filePath);
    if(!dataFile.is_open())
    {
        std::cerr << "failed to open file" << std::endl;
    }

    int rSum = 0;
    int gameNum = 1;
    for(std::string line; std::getline(dataFile, line);)
    {
        if(line.size() == 0)
        {
            break;
        }
        // Skip Invalid Lines
        auto pos = line.find(':');
        if(pos == std::string::npos)
        {
            continue;
        }

        pos++;
        if(powerCheck)
        {
            rSum += getMinPowerGame(line.substr(pos+1, line.size()-1));
        }
        else
        {
            bool gameValid = true;
            gameValid = checkValidGame(line.substr(pos+1, line.size()-1), 14, 12, 13);
            if(gameValid)
            {
                rSum += gameNum;
            }
        }
        gameNum++;
    }
    return rSum;
}

int main() 
{ 
    std::cout << "Result is: " << validGameCheck("aoc2/sample_input.txt", false) << std::endl;
    std::cout << "Result is: " << validGameCheck("aoc2/sample_input.txt", true) << std::endl;
    return 0;
}