#include "calculate.h"
#include "data.h"

float calculateScore(int indexOfCourse, GradeRecord recordsToCalculate, char component)
{
    float totalScore = 0.0;

    if (component == 'a')
    {
        for (int i = 0; i < recordsToCalculate.performanceTask.size(); i++)
        {
            totalScore += recordsToCalculate.performanceTask[i];
        }
    }
    else if (component == 'b')
    {
        for (int i = 0; i < recordsToCalculate.writtenTask.size(); i++)
        {
            totalScore += recordsToCalculate.writtenTask[i];
        }
    }
    else if (component == 'c')
    {
        for (int i = 0; i < recordsToCalculate.performanceTaskOver.size(); i++)
        {
            totalScore += recordsToCalculate.performanceTaskOver[i];
        }
    }
    else if (component == 'd')
    {
        for (int i = 0; i < recordsToCalculate.writtenTaskOver.size(); i++)
        {
            totalScore += recordsToCalculate.writtenTaskOver[i];
        }
    }
    
    return totalScore;
}

float calculateGrade(int indexOfCourse, float performanceGrade, float writtenGrade, float majorGrade)
{
    float partialGrade = (performanceGrade * courses[indexOfCourse].performanceTaskPercentage) + (writtenGrade * courses[indexOfCourse].writtenTaskPercentage) + (majorGrade * courses[indexOfCourse].majorExamPercentage);
    float withBase = ((partialGrade / 100.00) * (100.00 - courses[indexOfCourse].base)) + courses[indexOfCourse].base;
    float systemGrade;
    if (withBase >= 99.0f)
    {
        systemGrade = 1.00;
    }
    else if(withBase >= 96.0f)
    {
        systemGrade = 1.25;
    }
    else if(withBase >= 93)
    {
        systemGrade = 1.50;
    }
    else if(withBase >= 90)
    {
        systemGrade = 1.75;
    }
    else if(withBase >= 87)
    {
        systemGrade = 2.00;
    }
    else if(withBase >= 84)
    {
        systemGrade = 2.25;
    }
    else if(withBase >= 81)
    {
        systemGrade = 2.50;
    }
    else if(withBase >= 78)
    {
        systemGrade = 2.75;
    }
    else if(withBase >= 75)
    {
        systemGrade = 3.00;
    }
    else
    {
        systemGrade = 5.00;
    }
    
    return systemGrade;
}

float calculateGPA()
{
    for (int i = 0; i < logInCredential.size(); i++)
    {
        if (logInCredential[i].role == "student")
        {
            
        }
    }
    
}