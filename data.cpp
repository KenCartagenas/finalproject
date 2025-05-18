#include "data.h"
#include "jsonhelper.h" // Required for to_json/from_json
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip> // for setw
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// Global Variables
vector<Course> courses;
vector<User> logInCredential;
bool isLoggedIn = false;
bool isRunning = true;
int loggedInID = 0;
const int BASE_ID = 2500000;
int coursesCount = courses.size();
int userIndex = 0; // This should be computed dynamically, not once here

// Save to JSON file
void saveData()
{
    json j;
    j["users"] = logInCredential;
    j["courses"] = courses;
    j["isLoggedIn"] = isLoggedIn;
    j["loggedInID"] = loggedInID;
    j["userIndex"] = userIndex;

    ofstream file("data.json");
    if (file.is_open())
    {
        file << setw(2) << j << endl;
        file.close();
    }
    else
    {
        cerr << "❌ Failed to write to data.json\n";
    }
}

// Load from JSON file
void loadData()
{
    ifstream file("data.json");
    if (!file.is_open())
    {
        cerr << "⚠️ No data.json file found. Starting fresh.\n";
        return;
    }

    json j;
    try
    {
        file >> j;
        if (j.contains("users"))
            j["users"].get_to(logInCredential);
        if (j.contains("courses"))
            j["courses"].get_to(courses);
        if (j.contains("isLoggedIn"))
            j["isLoggedIn"].get_to(isLoggedIn);
        if (j.contains("loggedInID"))
            j["loggedInID"].get_to(loggedInID);
        if (j.contains("userIndex"))
            j["userIndex"].get_to(userIndex);
    }
    catch (const json::exception &e)
    {
        cerr << "❌ Failed to parse data.json: " << e.what() << endl;
    }

    file.close();
}
