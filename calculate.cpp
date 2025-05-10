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
