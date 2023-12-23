#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <set>

const std::set<char> VALID_NUMBER_CHARACTERS = {'1','2','3','4','5','6','7','8','9'};

const std::set<std::string> VALID_NUMBER_PHRASES_ONE = {"o", "t", "f", "s", "e", "n"};
const std::set<std::string> VALID_NUMBER_PHRASES_TWO = {"on", "tw", "th", "fo", "fi", "si", "se", "ei", "ni"};
const std::set<std::string> VALID_NUMBER_PHRASES_THREE = {"one", "two", "thr", "fou", "fiv", "six", "sev", "eig", "nin"};
const std::set<std::string> VALID_NUMBER_PHRASES_FOUR = {"four", "five", "nine", "thre", "eigh", "seve"};
const std::set<std::string> VALID_NUMBER_PHRASES_FIVE = {"three", "seven", "eight"};



int convertStrToNum(std::string const& stringName)
{
    if(stringName == "one")
        return 1;
    else if(stringName == "two")
        return 2;
    else if(stringName == "three")
        return 3;
    else if(stringName == "four")
        return 4;
    else if(stringName == "five")
        return 5;
    else if(stringName == "six")
        return 6;
    else if(stringName == "seven")
        return 7;
    else if(stringName == "eight")
        return 8;
    else if(stringName == "nine")
        return 9;
    else
        return -1;
}

std::string convertQueueToStr(std::deque<char> const& charQueue)
{
    return std::string(charQueue.begin(), charQueue.end());
}

bool checkMatch(const std::set<std::string>& set, const std::string& val)
{
    return set.count(val) == 1;
}

int computeCalibrationValuePt2(std::string const& filePath)
{
    std::ifstream dataFile(filePath);
    int rSum = 0;
    if(!dataFile.is_open())
    {
        std::cerr << "failed to open file" << std::endl;
    }
    for(std::string line; std::getline(dataFile, line);)
    {
        int firstDigit = -1;
        int lastDigit = -1;
        std::deque<char> runningNumQueue;
        for(char const& c : line)
        {
            if(runningNumQueue.size() > 5)
            {
                runningNumQueue.pop_front();
            }

            runningNumQueue.push_back(c);
            // std::cout << convertQueueToStr(runningNumQueue) << std::endl;
            // Handle old number case
            if(VALID_NUMBER_CHARACTERS.count(c) == 1)
            {
                if(firstDigit == -1)
                    firstDigit = static_cast<int>(c - '0');
                else
                    lastDigit = static_cast<int>(c - '0');
                runningNumQueue.clear();
                continue;
            }
            else
            {
                if(runningNumQueue.size() == 5)
                {
                    if(checkMatch(VALID_NUMBER_PHRASES_FIVE, convertQueueToStr(runningNumQueue)))
                    {
                        firstDigit == -1 ? firstDigit = convertStrToNum(convertQueueToStr(runningNumQueue)) : lastDigit = convertStrToNum(convertQueueToStr(runningNumQueue));
                        runningNumQueue.pop_front();
                        continue;
                    }
                    else
                    {
                        runningNumQueue.pop_front();
                    }
                }
                if(runningNumQueue.size() == 4)
                {
                    if(checkMatch(VALID_NUMBER_PHRASES_FOUR, convertQueueToStr(runningNumQueue)))
                    {   
                        auto rVal =  convertStrToNum(convertQueueToStr(runningNumQueue));
                        if(rVal != -1)
                        {
                            firstDigit == -1 ? firstDigit = rVal : lastDigit = rVal;
                            runningNumQueue.pop_front();
                            continue;
                        }
                    }
                    else{
                        runningNumQueue.pop_front();
                    }
                }
                if(runningNumQueue.size() == 3)
                {
                    if(checkMatch(VALID_NUMBER_PHRASES_THREE, convertQueueToStr(runningNumQueue)))
                    {   
                        auto rVal =  convertStrToNum(convertQueueToStr(runningNumQueue));
                        if(rVal != -1)
                        {
                            firstDigit == -1 ? firstDigit = rVal : lastDigit = rVal;
                            runningNumQueue.pop_front();
                            continue;
                        }
                    }
                    else{
                        runningNumQueue.pop_front();
                    }
                }
                if(runningNumQueue.size() == 2)
                {
                    if(checkMatch(VALID_NUMBER_PHRASES_TWO, convertQueueToStr(runningNumQueue)) == false)
                    {   
                        runningNumQueue.pop_front();
                    }
                }
                if(runningNumQueue.size() == 1)
                {
                    if(checkMatch(VALID_NUMBER_PHRASES_ONE, convertQueueToStr(runningNumQueue)) == false)
                    {   
                        runningNumQueue.pop_front();
                    }
                }
            }
        }
        if(lastDigit == -1 && firstDigit != -1)
        {
            lastDigit = firstDigit;
        }
        else if(lastDigit == -1 && firstDigit == -1)
        {
            continue;
        }
        std::cout << firstDigit << lastDigit << std::endl;
        rSum += (firstDigit * 10) + lastDigit;
    }
    return rSum;
}

int computeCalibrationValue(std::string const& filePath)
{
    std::ifstream dataFile(filePath);
    int rSum = 0;
    if(!dataFile.is_open())
    {
        std::cerr << "failed to open file" << std::endl;
    }
    for(std::string line; std::getline(dataFile, line);)
    {
        int firstDigit = -1;
        int lastDigit = -1;

        for(char const& c : line)
        {
            if(VALID_NUMBER_CHARACTERS.count(c) == 1)
            {
                firstDigit = static_cast<int>(c - '0');
                break;
            }
        }
        for(auto c = line.rbegin(); c < line.rend(); c++)
        {
            if(VALID_NUMBER_CHARACTERS.count(*c) == 1)
            {
                lastDigit = static_cast<int>(*c - '0');
                break;
            }
        }
        if( firstDigit == -1 || lastDigit == -1)
        {
            continue;
        }
        rSum += (firstDigit * 10) + lastDigit;
    }
    return rSum;
}

int main() 
{ 
    std::cout << "Result is: " << computeCalibrationValue("aoc1/sample_input.txt") << std::endl;
    std::cout << "Result is: " << computeCalibrationValuePt2("aoc1/sample_input.txt") << std::endl;
    return 0;
}