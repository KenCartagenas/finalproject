#include "calculate.h"
#include "data.h"
#include "utils.h"
//
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
    else if (component == 'e')
    {
        for (int i = 0; i < recordsToCalculate.majorExam.size(); i++)
        {
            totalScore += recordsToCalculate.majorExam[i];
        }
    }
    else if (component == 'f')
    {
        for (int i = 0; i < recordsToCalculate.majorExamOver.size(); i++)
        {
            totalScore += recordsToCalculate.majorExamOver[i];
        }
    }
    
    return totalScore;
}
//
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

float calculateGradeRaw(int indexOfCourse, float performanceGrade, float writtenGrade, float majorGrade)
{
    float partialGrade = (performanceGrade * courses[indexOfCourse].performanceTaskPercentage) + (writtenGrade * courses[indexOfCourse].writtenTaskPercentage) + (majorGrade * courses[indexOfCourse].majorExamPercentage);
    float withBase = ((partialGrade / 100.00) * (100.00 - courses[indexOfCourse].base)) + courses[indexOfCourse].base;
    return withBase;
}

void calculateGPA()
{
    float gpa = 0.0;
    int gpaCount = 0;
    for (int i = 0; i < logInCredential.size(); i++)
    {
        gpa = 0.0;
        gpaCount = 0.0;
        if (logInCredential[i].role == "student")
        {
            for (int j = 0; j < logInCredential[i].finalGrades.size(); j++)
            {
                gpa += logInCredential[i].finalGrades[j].grade;
                gpaCount++;
            }
            gpa /= (float) gpaCount;
            logInCredential[i].GPA = gpa;
        }
    }
}
//
float getFinal(int indexOfCourse, int j)
{
  float final;
    if (courses[indexOfCourse].studentRecords[j].performanceTask.size() > 0)
    {
      courses[indexOfCourse].studentRecords[j].performanceTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'a') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'c');
    }
    else
    {
      courses[indexOfCourse].studentRecords[j].performanceTaskPercent = 0.0;
    }

    if (courses[indexOfCourse].studentRecords[j].writtenTask.size() > 0)
    {
      courses[indexOfCourse].studentRecords[j].writtenTaskPercent = calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'b') / calculateScore(indexOfCourse, courses[indexOfCourse].studentRecords[j], 'd');
    }
    else
    {
      courses[indexOfCourse].studentRecords[j].writtenTaskPercent = 0.0;
    }
    if (courses[indexOfCourse].studentRecords[j].majorExam.size() > 0)
    {
      courses[indexOfCourse].studentRecords[j].majorExamPercent = courses[indexOfCourse].studentRecords[j].majorExam[0] / courses[indexOfCourse].studentRecords[j].majorExamOver[0];
      final = calculateGrade(indexOfCourse, courses[indexOfCourse].studentRecords[j].performanceTaskPercent, courses[indexOfCourse].studentRecords[j].writtenTaskPercent, courses[indexOfCourse].studentRecords[j].majorExamPercent);
    }
  return final;
}

int calculateTotalstudents()
{
    int students = 0;
    for(int i = 0; i < courses.size(); i++)
    {
        if(courses[i].teacherID == loggedInID)
        {
            for(int j = 0; j < courses[i].enrolledStudentID.size(); j++)
            {
                students++;
            }
        }
    }
    return students;
}

float calculateAverageGrade()
{
    float grade = 0.0;
    int count = 0;
    int index;
    for(int i = 0; i < courses.size(); i++)
    {
        if(courses[i].teacherID == loggedInID)
        {
            for(int j = 0; j < courses[i].enrolledStudentID.size(); j++)
            {
                index = indexFind(courses[i].enrolledStudentID[j]);
                for (int k = 0; k < logInCredential[index].finalGrades.size(); k++)
                {
                    if (logInCredential[index].finalGrades[k].courseName == courses[i].courseName)
                    {
                        grade += logInCredential[index].finalGrades[k].grade;
                        count++;
                    }
                    
                }
                
            }
        }
    }
    return grade / (float) count;
}
