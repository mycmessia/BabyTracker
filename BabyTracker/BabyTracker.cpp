// BabyTracker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

std::string FileNamePrefix = "BabyLog ";
std::string FileNamePostfix = "2021.txt";
std::string FileName;
std::vector<std::string> FileVector;

void ReadFileIntoVector()
{
    if (FileVector.empty())
    {
        std::ifstream InputFileStream(FileName);

        std::string Line;
        while (std::getline(InputFileStream, Line))
        {
            FileVector.push_back(Line);
        }
    }
}

void WriteVectorToFile(const std::string& Keywords, const std::string& OutputStr)
{
    for (int Index = 0; Index < FileVector.size() - 1; Index++)
    {
        if (FileVector[Index].find(Keywords) != std::string::npos &&
            FileVector[Index + 1].find(Keywords) == std::string::npos)
        {
            std::vector<std::string>::iterator it = FileVector.begin();
            FileVector.insert(it + Index + 1, OutputStr);
            break;
        }
    }

    std::ofstream OutputFile;

    OutputFile.open(FileName, std::ios_base::trunc);

    for (int Index = 0; Index < FileVector.size(); Index++)
    {
        OutputFile << FileVector[Index] << std::endl;
    }

    OutputFile.close();
}

template <typename Out>
void Split(const std::string& s, char delim, Out result) 
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) 
    {
        *result++ = item;
    }
}

std::vector<std::string> Split(const std::string& s, char delim) 
{
    std::vector<std::string> elems;
    Split(s, delim, std::back_inserter(elems));
    return elems;
}

void GetCurrentTime(char* OutTime, int size)
{
    auto Now = std::chrono::system_clock::now();

    std::time_t NowTime = std::chrono::system_clock::to_time_t(Now);

    ctime_s(OutTime, size, &NowTime);
}

void GetCurrentHourMin(std::string& OutHourMin)
{
    char CurrentTime[30];
    GetCurrentTime(CurrentTime, 30);

    std::string CurrentTimeStr(CurrentTime);
    CurrentTimeStr.pop_back();  // We don't need \n at the end.

    std::vector<std::string> SplitedDate = Split(CurrentTimeStr, ' ');

    std::string CurrentHourMinSec;

    for (int Index = 0; Index < SplitedDate.size(); Index++)
    {
        if (Index == SplitedDate.size() - 2)
        {
            CurrentHourMinSec = SplitedDate[Index];
        }
    }

    std::vector<std::string> SplitedTime = Split(CurrentHourMinSec, ':');

    for (int Index = 0; Index < SplitedTime.size() - 1; Index++)
    {
        OutHourMin += (SplitedTime[Index] + ":");
    }

    OutHourMin.pop_back();
}

void AddBottleFeed()
{
    ReadFileIntoVector();

    std::cout << "Please input bottle feed amount:" << std::endl;

    int FeedAmount = 0;

    std::cin >> FeedAmount;

    std::string CurrentHourMinStr;

    GetCurrentHourMin(CurrentHourMinStr);

    std::string OutputStr = CurrentHourMinStr + " | Bottle Feed Amount: " + std::to_string(FeedAmount) + "ml";

    WriteVectorToFile("Bottle Feed", OutputStr);
}

int CountExistingNappyChangeTimes()
{
    int NappyChangeTimes = 0;

    std::ifstream InputFileStream(FileName);
    std::string NappyChangeStr = "Nappy Change Reason";
    std::string Line;
    while (std::getline(InputFileStream, Line))
    {
        if (Line.find(NappyChangeStr) != std::string::npos)
        {
            NappyChangeTimes++;
        }
    }

    return NappyChangeTimes;
}

void AddNappyChange()
{
    ReadFileIntoVector();

    std::cout << "Please input nappy reason: 1.Poop 2.Pee 3.Both" << std::endl;

    int Type = 0;

    std::cin >> Type;

    std::string TypeStr;

    switch (Type)
    {
    case 1:
        TypeStr = "Poop";
        break;
    case 2:
        TypeStr = "Pee";
        break;
    case 3:
        TypeStr = "Poop & Pee";
        break;
    default:
        TypeStr = "Wrong Nappy Type";
        break;
    }

    std::string CurrentHourMinStr;

    GetCurrentHourMin(CurrentHourMinStr);

    int NappyChangeTimes = CountExistingNappyChangeTimes();
    NappyChangeTimes++;

    std::string OutputStr = CurrentHourMinStr + " | Nappy Change Reason: " + TypeStr + " | Times " + std::to_string(NappyChangeTimes);

    WriteVectorToFile("Nappy Change", OutputStr);
}

void AddBreastFeed()
{
    ReadFileIntoVector();

    std::cout << "Please input breast feed duration:" << std::endl;

    int FeedDuration = 0;

    std::cin >> FeedDuration;

    std::cout << "Please input breast feed side: 1.Left 2.Right" << std::endl;

    int FeedSide = 0;

    std::cin >> FeedSide;

    std::string CurrentHourMinStr;

    GetCurrentHourMin(CurrentHourMinStr);

    std::string FeedSideStr = FeedSide == 1 ? "Left" : "Right";
    std::string OutputStr = CurrentHourMinStr + " | Breast Feed: " + std::to_string(FeedDuration) + " mins " + FeedSideStr;

    WriteVectorToFile("Breast Feed", OutputStr);
}

void AddSleep()
{
    ReadFileIntoVector();

    int SleepType = 0;

    std::cout << "Please input: 1.Fall asleep 2.Wake up" << std::endl;

    std::cin >> SleepType;

    std::string CurrentHourMinStr;

    GetCurrentHourMin(CurrentHourMinStr);

    std::string SleepTypeStr;

    if (SleepType == 1)
    {
        SleepTypeStr = "Fall asleep";
    }
    else if (SleepType == 2)
    {
        SleepTypeStr = "Wake up";
    }

    std::string OutputStr = CurrentHourMinStr + " | Sleep: " + SleepTypeStr;

    WriteVectorToFile("Sleep", OutputStr);
}

void AddBoilWater()
{
    ReadFileIntoVector();


    std::string CurrentHourMinStr;

    GetCurrentHourMin(CurrentHourMinStr);

    std::string OutputStr = CurrentHourMinStr + " | Boil Water";

    WriteVectorToFile("Boil Water", OutputStr);
}

void AddAction()
{
    bool Stop = false;

    do
    {
        std::cout << "What action do you want to add? 1. Sleep 2. Breast Feed 3. Bottle Feed 4. Nappy Change 5. Boil Water" 
                  << std::endl;

        int ActionType = -1;

        std::cin >> ActionType;

        switch (ActionType)
        {
        case 1:
            AddSleep();
            break;
        case 2:
            AddBreastFeed();
            break;
        case 3:
            AddBottleFeed();
            break;
        case 4:
            AddNappyChange();
            break;
        case 5:
            AddBoilWater();
            break;
        default:
            std::cout << "Wrong Action Number!" << std::endl;
            break;
        }

        std::cout << "Do you want to add more actions? 1. Yes 2. No" << std::endl;

        int Continue = 0;

        std::cin >> Continue;

        if (Continue != 1)
        {
            Stop = true;
        }

    } while (!Stop);
}

void QueryAction()
{

}

void CreateTodaysFile()
{
    char CurrentTime[30];
    GetCurrentTime(CurrentTime, 30);

    std::string CurrentTimeStr(CurrentTime);

    std::vector<std::string> SplitedTime = Split(CurrentTimeStr, ' ');

    std::string ReportTitle;

    for (int Index = 0; Index < SplitedTime.size(); Index++)
    {
        if (Index < SplitedTime.size() - 2)
        {
            FileName += SplitedTime[Index] + " ";
        }

        if (Index != SplitedTime.size() - 2)
        {
            ReportTitle += SplitedTime[Index] + " ";
        }
    }

    ReportTitle.pop_back();
    ReportTitle.pop_back();
    ReportTitle += " Report";

    FileName = FileNamePrefix + FileName;
    FileName = FileName + FileNamePostfix;

    if (!std::ifstream(FileName))
    {
        std::ofstream OutputFile(FileName);

        if (!OutputFile)
        {
            std::cout << "Today's log file could not be created." << std::endl;
        }

        CurrentTimeStr.pop_back();

        OutputFile << ReportTitle;
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "Sleep Session";
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "Breast Feed Session";
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "Bottle Feed Session";
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "Nappy Change Session";
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "Boil Water Session";
        OutputFile << std::endl;
        OutputFile << std::endl;
        OutputFile << "The End";

        OutputFile.close();
    }
}

int main()
{
    CreateTodaysFile();

    bool Quit = false;

    std::cout << "Which option do you want to do? 1. Add Action 2. Query Action" << std::endl;

    int Action = -1;

    std::cin >> Action;

    if (Action != 1 && Action != 2 && Action != 3)
    {
        std::cout << "Please input a valid Action 1,2 or 3." << std::endl;
    }
    else
    {
        switch (Action)
        {
        case 1:
            AddAction();
            break;
        case 2:
            QueryAction();
            break;
        default:
            std::cout << "Wrong Action Input!" << std::endl;
            break;
        }
    }

    std::cout << "Have a nice day!" << std::endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
