// jsonhelper.h
#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "data.h"
#include "json.hpp"
using json = nlohmann::json;

// Grade
void to_json(json& j, const Grade& g);
void from_json(const json& j, Grade& g);

// GradeRecord
void to_json(json& j, const GradeRecord& gr);
void from_json(const json& j, GradeRecord& gr);

// Course
void to_json(json& j, const Course& c);
void from_json(const json& j, Course& c);

// User
void to_json(json& j, const User& u);
void from_json(const json& j, User& u);

#endif
