#ifndef SCHEDULE_H_INCLUDED
#define SCHEDULE_H_INCLUDED

using namespace std;


// Contains the course code and name of each course
struct Title
{
    string code;
    string name;

};

// contains starting time and ending time of each course.
// Along with other variables for quick calculations
struct Time
{
    int startHr;
    int startMin;
    int endHr;
    int endMin;
    int totMinutes;
    int exactStart;
    int exactEnd;
    string timeString;
};

// Contains the location of each course
struct Location
{
    string building;
    string room;
};

// The Course container
struct Course
{
    Title title;
    bool meetDays[7];   // for each day of the week the class meets this bool returns true
    string dayString;
    Time time;
    Location location;
    string instructor;
    string courseString;        // contains the complete course string for easy access
};

// There will be a struct called NodeType
struct NodeType;

typedef NodeType* NodePtr;

// struct used to organized my linked list of Courses
struct NodeType
{
    Course component;       // The current course
    NodePtr next;           // Points to the next node in the list
};

// For the days of the week - ERR used for errors and testing
enum Days{SUN, MON, TUE, WED, THU, FRI, SAT, ERR};

// Creates file name based off student ID
string getStudentFile(string);

// Retrieves any previously stored courses from file
void getStudentRecord(string, int [], NodePtr& , NodePtr& , int &);


// Initializes a linked list with a New Node Pointer
void InitList(NodePtr& ,NodePtr& , NodePtr);


//void SortList(NodePtr&, NodePtr&);

// Adds new node to the front of the linked list
void AddToFront(NodePtr&, NodePtr );

// Adds new node to the back of the linked list
void AddToBack(NodePtr&, NodePtr );

// Prints the linked list to the console
void PrintList(NodePtr);

// Retrieves the student ID and checks if the file already exist
string getStudentID();

// Adds a new course to the schedule
NodePtr userAddCourses();

// Returns the course string to be stored in the Course
string getCourseString(Course );

// Uses user input to create the Course title info
Title getName();

//Takes a Days type and returns a string
string getDayString(Days );

// Saves student schedule to a text file
void saveSchedule(string,int , NodePtr);

//Takes a string and returns a Days (day of the week)
Days getDay(string );

// Uses user input to create the Course Time info
Time getTime();

// Checks if there is a time conflict between two courses
bool checkTime(Time , Time );               //

// Allows user to remove a Course from their schedule
void dropCourse(int [],NodePtr& , NodePtr& ,int &);

// Removes course from schedule
void removeCourse(NodePtr&, NodePtr&, NodePtr& ,int [],int& totCourses);

const int DaysInWeek = 7;   // Number of Days in a school week
const int MaxCourses = 6;   // Maximum number of courses a student may enroll in

// Initializes Arrays  studentSchedule[] and ClassesPerDay[] before use
void initArrays(int []);

// Displays that just includes Course code, Meeting day/time, Building, and Room number
void displayWeeklySchedule(int [], NodePtr,int );

// Displays the Full Student Schedule
//void displayFullSchedudule(int, Course []);

// Find conflicts in schedule when user attempts to add a course
void findConflict(int &, int [],  NodePtr&, NodePtr&, NodePtr&);

void updateClassesPerDay(int [], string );






#endif // SCHEDULE_H_INCLUDED
