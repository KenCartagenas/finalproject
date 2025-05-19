// jsonhelper.cpp
#include "jsonhelper.h"
#include "data.h"
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

// Grade
void to_json(json& j, const Grade& g) {
    j = json{{"courseName", g.courseName}, {"grade", g.grade}};
}
void from_json(const json& j, Grade& g) {
    j.at("courseName").get_to(g.courseName);
    j.at("grade").get_to(g.grade);
}

// GradeRecord
void to_json(json& j, const GradeRecord& gr) {
    j = json{
        {"studentID", gr.studentID},
        {"finalGrade", gr.finalGrade},
        {"majorExamName", gr.majorExamName},
        {"performanceTaskName", gr.performanceTaskName},
        {"writtenTaskName", gr.writtenTaskName},
        {"majorExam", gr.majorExam},
        {"performanceTask", gr.performanceTask},
        {"writtenTask", gr.writtenTask},
        {"majorExamOver", gr.majorExamOver},
        {"performanceTaskOver", gr.performanceTaskOver},
        {"writtenTaskOver", gr.writtenTaskOver},
        {"majorExamPercent", gr.majorExamPercent},
        {"performanceTaskPercent", gr.performanceTaskPercent},
        {"writtenTaskPercent", gr.writtenTaskPercent}
    };
}
void from_json(const json& j, GradeRecord& gr) {
    j.at("studentID").get_to(gr.studentID);
    j.at("finalGrade").get_to(gr.finalGrade);
    j.at("majorExamName").get_to(gr.majorExamName);
    j.at("performanceTaskName").get_to(gr.performanceTaskName);
    j.at("writtenTaskName").get_to(gr.writtenTaskName);
    j.at("majorExam").get_to(gr.majorExam);
    j.at("performanceTask").get_to(gr.performanceTask);
    j.at("writtenTask").get_to(gr.writtenTask);
    j.at("majorExamOver").get_to(gr.majorExamOver);
    j.at("performanceTaskOver").get_to(gr.performanceTaskOver);
    j.at("writtenTaskOver").get_to(gr.writtenTaskOver);
    j.at("majorExamPercent").get_to(gr.majorExamPercent);
    j.at("performanceTaskPercent").get_to(gr.performanceTaskPercent);
    j.at("writtenTaskPercent").get_to(gr.writtenTaskPercent);
}

// Course
void to_json(json& j, const Course& c) {
    j = json{
        {"courseID", c.courseID},
        {"courseName", c.courseName},
        {"teacher", c.teacher},
        {"teacherID", c.teacherID},
        {"enrolledStudentID", c.enrolledStudentID},
        {"base", c.base},
        {"majorExamPercentage", c.majorExamPercentage},
        {"performanceTaskPercentage", c.performanceTaskPercentage},
        {"writtenTaskPercentage", c.writtenTaskPercentage},
        {"studentRecords", c.studentRecords}
    };
}
void from_json(const json& j, Course& c) {
    j.at("courseID").get_to(c.courseID);
    j.at("courseName").get_to(c.courseName);
    j.at("teacher").get_to(c.teacher);
    j.at("teacherID").get_to(c.teacherID);
    j.at("enrolledStudentID").get_to(c.enrolledStudentID);
    j.at("base").get_to(c.base);
    j.at("majorExamPercentage").get_to(c.majorExamPercentage);
    j.at("performanceTaskPercentage").get_to(c.performanceTaskPercentage);
    j.at("writtenTaskPercentage").get_to(c.writtenTaskPercentage);
    j.at("studentRecords").get_to(c.studentRecords);
}

// User
void to_json(json& j, const User& u) {
    j = json{
        {"role", u.role},
        {"id", u.id},
        {"space", u.space},
        {"GPA", u.GPA},
        {"username", u.username},
        {"password", u.password},
        {"section", u.section},
        {"program", u.program},
        {"finalGrades", u.finalGrades},
        {"coursesEnrolled", u.coursesEnrolled},
        {"coursesHandled", u.coursesHandled}
    };
}
void from_json(const json& j, User& u) {
    j.at("role").get_to(u.role);
    j.at("id").get_to(u.id);
    j.at("space").get_to(u.space);
    j.at("GPA").get_to(u.GPA);
    j.at("username").get_to(u.username);
    j.at("password").get_to(u.password);
    j.at("section").get_to(u.section);
    j.at("program").get_to(u.program);
    j.at("finalGrades").get_to(u.finalGrades);
    j.at("coursesEnrolled").get_to(u.coursesEnrolled);
    j.at("coursesHandled").get_to(u.coursesHandled);
}
