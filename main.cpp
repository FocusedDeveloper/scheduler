//*****************************************************
//  Darnell Johnson 5100474
//  11/23/2015
// Student schedule program
// This program keeps track of student's schedules
// by storing them based on student IDs
//  Schedule is maintained with an linked list of structs
//
//
//*****************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#include "schedule.h"


int main()
{

   // string schedule;                        // Stores the entire student schedule
    string studentID;                       // Keeps track of the student ID#
    string mainMenu;                        // Variable used to navigate the menu
    string buffer;

    int totCourses = 0;                     // Total number or courses at any given time

    Course myCourse;                        // Course to be added from the user

    int ClassesPerDay[DaysInWeek];          // Keep track of how many courses are scheduled on each particular day

    //Course studentSchedule[MaxCourses];     // Contains all the courses in the student schedule

    initArrays(ClassesPerDay);     // initialize arrays before use

    NodePtr scheduleHeadPtr = NULL;
    NodePtr scheduleTailPtr = NULL;
    NodePtr newCoursePtr = NULL;


    cout<<"This software is designed to manage your schedule."<<endl;
    cout<<endl;

    studentID = getStudentID();                 // First get student ID and see if they exist in the system
    getStudentRecord(studentID, ClassesPerDay,  scheduleHeadPtr, scheduleTailPtr, totCourses);     // Next, retrieve the student's schedule from the text file


    do{                                         // Until user quits...

        cout<<"Type (1) to add classes. (2) to display your schedule."<<endl<<"(3) to display weekly schedule. (4) to drop a course (5) to quit: ";
        cin>>mainMenu;
        cin.ignore();       // this command used throughout this program to prepare my cursor by ridding whitespace
        cout<<endl;
        if(mainMenu== "1"|| mainMenu == "add"){

            if(totCourses<MaxCourses)           // If we are not at maximum course load...
            {

                newCoursePtr = userAddCourses();                                                // Retrieves a new courses from the user
                if(totCourses==0)                                                               // If the schedule list is empty...
                {
                    InitList(scheduleHeadPtr,scheduleTailPtr,newCoursePtr);                     // Initialize the list with the new course
                    updateClassesPerDay(ClassesPerDay, scheduleHeadPtr->component.dayString);
                    totCourses++;
                }
                else
                {
                    findConflict(totCourses,ClassesPerDay, scheduleHeadPtr, scheduleTailPtr, newCoursePtr);        // If no conflict is found in the schedule the new course is added
                }

            }
            else
            {
                cout<<endl<<"Your schedule is full with "<<MaxCourses<<" courses."<<endl;
                cout<<"You may not add more courses at this time."<<endl<<endl;
            }

        }
        else if(mainMenu == "2" || mainMenu == "display")
        {
           //displayFullSchedudule(totCourses, studentSchedule);      // displays full schedule to screen
           cout<<"Total Courses : "<<totCourses<<endl;
           PrintList(scheduleHeadPtr);
        }
        else if(mainMenu == "3")
        {
            displayWeeklySchedule(ClassesPerDay,  scheduleHeadPtr, totCourses);      // prints weekly schedule  **to be done** arrange daily courses in proper order**
        }
        else if(mainMenu == "4")
        {
            cout<<"Total Courses : "<<totCourses<<endl;
            if(totCourses>0)
            {
                dropCourse(ClassesPerDay, scheduleHeadPtr, scheduleTailPtr, totCourses);             // allows user to drop a course from their schedule
            }
            else
            {
                cout<<endl<<"Your current schedule is empty.  There are no courses to drop."<<endl;
            }
        }

    }while(mainMenu!="5"&&mainMenu!="quit");                    // Prepare to exit the program

    // only save IF there are courses to save
    if(totCourses >0)
        saveSchedule(studentID,  totCourses,  scheduleHeadPtr);                          // Saves student schedule to a text file

    return 0;
}
// Initializes the two main arrays of this program
// PRE: Arrays, ClassesPerDay and studentSchedule have been declared
// POST: The arrays have been initialized for use
void initArrays(int ClassesPerDay[])
{
    for(Days day=SUN; day<=SAT; day = Days(day+1))  // For every day of the week...
    {
        ClassesPerDay[day]=0;                       // The number of classes for this day are ZERO
    }

}

// Updates the ClassesPerDay array
// PRE: dayString contains the correct days that the course meets
// POST: classPerDay array has been updated to current values
void updateClassesPerDay(int ClassesPerDay[], string myDayString)
{
    string dayString;
    for(Days day = SUN; day<= SAT ; day = Days (day + 1) )
    {
        dayString = getDayString(day);

        if(myDayString.find(dayString)!=string::npos)                // If the day is inside the dayString, add the course to my schedule.  Ref: http://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
        {
            ClassesPerDay[day]++;                       // and increase the amount of classes this day by one
        }
    }
}

// Takes time from two courses and checks if they conflict with one another
// PRE: time from both courses must be defined
// POST: Return true if no conflict is found
bool checkTime(Time weekly, Time pending)
{
    bool timeAvailable = true;
    if((weekly.exactStart == pending.exactStart)||      // Classes can not start at the same time.
       (weekly.exactEnd == pending.exactStart))          // Or start the exact moment another class ends.
        timeAvailable = false;
    else if((weekly.exactEnd > pending.exactStart)&&    // Classes can not start if start time falls between...
            (pending.exactStart> weekly.exactStart) )   // when another class ends AND when that same class starts
            timeAvailable = false;
    else if((weekly.exactEnd > pending.exactEnd)&&    // Classes can not end if end time falls between...
            (pending.exactEnd> weekly.exactStart) )   // when another class ends AND when that same class starts
            timeAvailable = false;
    else if((weekly.exactStart == pending.exactEnd)||      // Classes can not end at the same time.
       (weekly.exactEnd== pending.exactEnd))          // Or end the exact moment another class ends.
            timeAvailable = false;
    else if(pending.exactStart< weekly.exactStart&&weekly.exactEnd<pending.exactEnd)        // Class can not start before a weekly class and end after that same class.
        timeAvailable = false;
    else
        timeAvailable = true;
    return timeAvailable;


}

// Prompts user for information to satisfy course code and name
// PRE: Pass this function a title to a Courses that needs to be set
// POST: Returning title will have name and course code filled by user
Title getName(Title title)
{
    //Title title;
    cout<<"Please enter the course code as follows"<<endl;
    cout<<"example: CSC201"<<endl;
   // cin>>title.code;
    getline(cin, title.code);
    cout<<"Please enter the course name as follows"<<endl;
    cout<<"example: Computer Science I"<<endl;
    //cin>>title.name;
    getline(cin, title.name);
    return title;
}

// Prompts user for information to satisfy course location
// PRE: Pass this function a Location to a Course that needs to be set
// POST: Returning Location will have building and room number filled by user
Location getLocation(Location location)
{
    //Location location;
    cout<<"Please enter the building name as follows"<<endl;
    cout<<"example: Lynnhaven"<<endl;
    getline(cin, location.building);
    cout<<"Please enter the room number as follows"<<endl;
    cout<<"example: E181"<<endl;
    getline(cin, location.room);
    return location;
}

// Returns a Days (day of week enum type) from given string
// PRE: string passed has been initialized, and first 2 letters the day are spelled correctly
// POST: returning (Days) will be valid day of week or ERR
Days getDay(string dayString)
{
    const char FirstLETTER = toupper(dayString[0]);     // Stores first letter of day string as capitol character
    const char SecondLETTER = toupper(dayString[1]);    // Stores second letter of day string as capitol character
    Days myDay;

    switch(FirstLETTER)                     // Checks the first character of day string
    {
        case 'S':                           // More than one day of the week starts wth S...
            if(SecondLETTER=='U')           // So check the 2nd character
                myDay=SUN;
            else if(SecondLETTER=='A')
                myDay=SAT;
            break;
        case 'M':
            myDay = MON;
            break;
        case 'T':
            if(SecondLETTER=='U')
                myDay=TUE;
            else if(SecondLETTER=='H')
                myDay=THU;
            break;
        case 'W':
            myDay = WED;
            break;
        case 'F':
            myDay = FRI;
            break;
        default :                       // If no valid Days was found return ERR
            myDay= ERR;
            break;
    }
    return myDay;
}

//Takes a Days and returns appropriate string
// PRE: myDay contains a valid Days type
// POST: Returning string contains name of a Day of the week - or Error code
string getDayString(Days myDay)
{
    string dayString;
    switch(myDay)
    {
        case SUN: dayString= "SUN";
        break;
        case MON: dayString= "MON";
        break;
        case TUE: dayString= "TUE";
        break;
        case WED: dayString= "WED";
        break;
        case THU: dayString= "THU";
        break;
        case FRI: dayString= "FRI";
        break;
        case SAT: dayString= "SAT";
        break;
        default : dayString= "ERROR";
        break;
    }
    return dayString;
}

// Prompts user for Days that a course has session
// PRE: course requires Days
// POST: Course dayString is returned with meetdays array also being filled correctly
string getDays(bool myDays[])
{

    Days day;
    string classDay;
    string classWeek = "";

    bool addNewDay= true;
    bool dayLooksGood= true;

    for(day = SUN; day<= SAT ; day = Days (day + 1) )       // Resets each day to false
        myDays[day] = false;

    do{         // While there is a new day to be added...

        do{     // If the user is not satisfied with the Day they input they may try again
            dayLooksGood = true;
            cout<<"Please enter the Day(s) the class meets, one at a time."<<endl;
            cout<<"example: Monday"<<endl;
            getline(cin, classDay);

            day = getDay(classDay);             // convert user input into type Days
            classDay = getDayString(day);       // convert type Days into a standardized string to be stored

            cout<<endl<<classDay<<" will be added to the course schedule.  Is this correct?"<<endl;
            cout<<"Enter '0' for No.  Enter '1' for Yes"<<endl<<endl;
            cin>>dayLooksGood;
            cin.ignore(1000, '\n');

        }while(!dayLooksGood);

        if(classWeek!="")
            classWeek = classWeek+"/";
        classWeek= classWeek+classDay;                                      // build my class week for display to the user.


        myDays[day] = true;                                                 // Once the day looks good to the user, change the day to true in the weekly schedule array


        cout<<endl<<"This the current class week: "<<classWeek<<endl;
        cout<<"Would you like to add another day for this course?"<<endl;
        cout<<"Enter '0' for No.  Enter '1' for Yes"<<endl<<endl;
        cin>>addNewDay;
        cin.ignore(1000, '\n');


    }while(addNewDay);

    classWeek = "";                                                         // reset class week to prepare for rebuilding

    for(day = SUN; day<= SAT ; day = Days (day + 1) )                       // re order my class week in the case that the user input days in out of order.
    {
        if(myDays[day]==true)
        {
             if(classWeek!="")
                classWeek = classWeek+"/";
            classDay = getDayString(day);
            classWeek= classWeek+classDay;
        }
    }
    return classWeek;

}

// converts hours to military time for accurate calculations
// PRE: 8 through 11 treated as AM hours.  12 to 7 treated as PM hours
// POST: 12 hours added to all PM hours except noon
int convertToMilitaryTime(int hour)
{

    if((hour >= 1) && (hour <= 7))  // if hour falls between 1 and 7
        hour=hour+12;               // add 12 to hour
    return hour;
}

// Prompts user for information to satisfy course Time
// PRE: Pass this function a Time to a Course that needs to be set
// POST: Returning Time will have starting and ending time filled by user
Time getTime(Time time)
{
    //Time time;
    int startMinute;
    int endMinute;
    int totalTime;
    const int minutesPerHour = 60;
    stringstream sstm;                  // http://stackoverflow.com/questions/191757/c-concatenate-string-and-int
                                        // used to format a string

    do{
        cout<<"Please enter the starting time as follows"<<endl;
        cout<<"example: 9:00"<<endl;
        cin>>time.startHr;
        cin.ignore();
        cin>>time.startMin;

        time.startHr=convertToMilitaryTime(time.startHr);                   // convert hour to military time for calculations
        startMinute = int(((time.startHr)*minutesPerHour)+time.startMin);   // get exact minute course starts
        time.exactStart = startMinute;

        cout<<"Please enter the ending time as follows"<<endl;
        cout<<"example: 10:50"<<endl;
        cin>>time.endHr;
        cin.ignore();
        cin>>time.endMin;

        time.endHr = convertToMilitaryTime(time.endHr);
        endMinute = int(((time.endHr)*minutesPerHour)+time.endMin);
        time.exactEnd = endMinute;

        if(startMinute>=endMinute)
            cout<<"Class cannot end before it begins.  Please try again"<<endl;
    }while(startMinute>=endMinute);

    cin.ignore();

    // Create an easy to access string to display time for course
    sstm << time.startHr << ":" << setw(2) << setfill('0') << time.startMin <<  " - " << time.endHr <<":" << setw(2) << setfill('0')<< time.endMin;

    time.timeString = sstm.str();
    totalTime = endMinute - startMinute;
    time.totMinutes = totalTime;

    return time;
}

// Prompts user for information to satisfy course Instructor name
// PRE: Pass this function an instructor string to a Course that needs to be set
// POST: Returning Instructor will have been filled by user
string getInstructor(string instructor)
{

    bool inputCorrect = true;
    do{                                             // Allows user to continue to input instructor name until they are satisfied
        cout<<"Please enter the name of the instructor as follows"<<endl;
        cout<<"example: Mr. Davis"<<endl;
        getline(cin, instructor);
        cout<<endl<<"The name you entered is "<<instructor<<".  Is this correct?"<<endl;
        cout<<"Enter '0' for No.  Enter '1' for Yes"<<endl;
        cin>>inputCorrect;
        cin.ignore();
    }while(!inputCorrect);
    return instructor;
}

// Builds easy to access string with all course information
// PRE: course must be fully defined with all attributes set.
// POST: Returning string will contain all course information in a single variable
string getCourseString(Course myCourse)
{
  //  string name, date, time, instructor;                        // data to be retrieved for each course
    string courseString;

    courseString = myCourse.title.code + " - " + myCourse.title.name + ", " + myCourse.dayString + ", " + myCourse.time.timeString +
    ", " + myCourse.location.building + ", " + myCourse.location.room + ", " + myCourse.instructor + '\n';
    return courseString;                                                      // return the new course

}


// Builds a complete course with user input
// PRE: myCourse is declared and ready to be defined
// POST: Returning Course will be fully defined by user, and returns updated to calling function
NodePtr userAddCourses()
{
    NodePtr newCoursePtr = new NodeType;
    newCoursePtr->component.title = getName(newCoursePtr->component.title);
    newCoursePtr->component.dayString = getDays(newCoursePtr->component.meetDays);
    newCoursePtr->component.time = getTime(newCoursePtr->component.time);
    newCoursePtr->component.location = getLocation(newCoursePtr->component.location);
    newCoursePtr->component.instructor = getInstructor(newCoursePtr->component.instructor);
    newCoursePtr->component.courseString = getCourseString(newCoursePtr->component);

    return newCoursePtr;

}

// saves student schedule to text file
// PRE: There is at least one class to be saved to the file
// POST: schedule saved in text file
void saveSchedule(string studentID, int totCourses, NodePtr listPtr)
{
    ofstream outFile;                                           // output file stream
    string studentFile;

    studentFile = getStudentFile(studentID);                    // Gets filename to be used based on student ID
    outFile.open(studentFile.c_str());                          // Creates or overwrites the new schedule into a text file
    while(listPtr!=NULL)
    {
        outFile<<listPtr->component.courseString;               // Writes the data to the new file
        listPtr = listPtr->next;
    }
    outFile.close();
}

// Creates name of file to save student schedule
// PRE: student ID already established
// POST: returns .txt with student ID
string getStudentFile(string studentID)
{
    string studentFile = "myCourses";                   // Base file name...
    studentFile.append(studentID);                      // appended with the student ID...
    studentFile.append(".txt");                         // finally at the file type (txt)
    return studentFile;
}

// Locates student file based on student ID
// PRE: Student File is in the same location as this program
// POST: IF not student file is found, a new one will be created
string getStudentID()
{
    ifstream inFile;                                    // In file stream
    bool newStudent = true;
    string studentID;
    string studentFile;                                 // Name of file may contain the schedule
    do{

        cout<<"Please enter your student id#"<<endl;
        cin>>studentID;
        cout<<endl;

        studentFile = getStudentFile(studentID);                // create file name based off student ID

        inFile.open(studentFile.c_str());
        if(inFile)                                              // if the student's file exist
        {
            cout<<"student "<<studentID<<" found."<<endl;
            cout<<endl;
            newStudent= false;                                  // This is not a new student
        }
        else
        {
        cout<<"student "<<studentID<<" not found."<<endl;
        cout<<"Would you like to retry? (1) or Create a new schedule? (0)"<<endl;
        cin>>newStudent;
        }

    }while(newStudent);                                         // stay inside loop until either student is found, or user creates a new account

    inFile.close();

    return studentID;

}

// Populates student Record with a information from a text file
// PRE: text file exist, arrays Declared, totCourses initialized to zero
// POST: Arrays have been populated with information from text file and totCourses has correct number of courses
void getStudentRecord(string studentID, int ClassesPerDay[], NodePtr& scheduleHeadPtr, NodePtr& scheduleTailPtr, int& totCourses)
{
    ifstream inFile;                                            // input file stream
    string studentFile;
    string currentSchedule = "";
    Course myCourse;
    NodePtr newCoursePtr = NULL;

    string dayString;
    stringstream sstm2;


    const int minutesPerHour = 60;
    string courseString;

    studentFile = getStudentFile(studentID);                    // creates file name based on student ID
    inFile.open(studentFile.c_str());

    while(inFile)                                               // while the file still has data
    {
        newCoursePtr = new NodeType;
        stringstream sstm;                                      // by placing in loop stringstream is reset on each iteration of the loop. (took me a while to figure this one out)
        string buffer="";

        getline(inFile, myCourse.title.code, '-');
        myCourse.title.code = myCourse.title.code.substr(0,myCourse.title.code.size()-1);       // because there is a space before the hyphen, i delete the last character (the space) from my course code

        inFile.ignore();
        getline(inFile, myCourse.title.name, ',');
        inFile.ignore();
        getline(inFile, myCourse.dayString, ',');

        for(Days day = SUN; day<= SAT ; day = Days (day + 1) )                      // Sets all days to new course to false
        myCourse.meetDays[day] = false;

        inFile>>myCourse.time.startHr;
        inFile.ignore();
        inFile>>myCourse.time.startMin;
        inFile.ignore();
        inFile.ignore();
        inFile>>myCourse.time.endHr;
        inFile.ignore();
        inFile>>myCourse.time.endMin;
        inFile.ignore();

        inFile.ignore();
        getline(inFile, myCourse.location.building, ',');
        inFile.ignore();
        getline(inFile, myCourse.location.room, ',');
        inFile.ignore();
        getline(inFile, myCourse.instructor);

        // calculate time variables...
        myCourse.time.endHr=convertToMilitaryTime(myCourse.time.endHr);
        int endMinute = int(((myCourse.time.endHr)*minutesPerHour)+myCourse.time.endMin);
        myCourse.time.exactEnd = endMinute;

        myCourse.time.startHr=convertToMilitaryTime(myCourse.time.startHr);
        int startMinute = int(((myCourse.time.startHr)*minutesPerHour)+myCourse.time.startMin);
        myCourse.time.exactStart = startMinute;

        // organize and place my time string into a string stream
        sstm << myCourse.time.startHr << ":" << setw(2) << setfill('0') << myCourse.time.startMin <<  " - " << myCourse.time.endHr <<":" << setw(2) << setfill('0')<< myCourse.time.endMin;

        myCourse.time.timeString = sstm.str();
        int totalTime = endMinute - startMinute;
        myCourse.time.totMinutes = totalTime;

        // Create my easy to access course string
        myCourse.courseString = myCourse.title.code + " - " + myCourse.title.name + ", " + myCourse.dayString + ", " + myCourse.time.timeString +
    ", " + myCourse.location.building + ", " + myCourse.location.room + ", " + myCourse.instructor + '\n';

        //currentSchedule = currentSchedule + myCourse.courseString;

        for(Days day = SUN; day<= SAT ; day = Days (day + 1) )
        {
            dayString = getDayString(day);

            if(myCourse.dayString.find(dayString)!=string::npos)                // If the day is inside the dayString, add the course to my schedule.  Ref: http://stackoverflow.com/questions/2340281/check-if-a-string-contains-a-string-in-c
            {

                myCourse.meetDays[day] = true;              // If course meets on this day turn to true
                ClassesPerDay[day]++;                       // and increase the amount of classes this day by one
            }
        }
        while(inFile.peek()=='\n'||inFile.peek()==' ')      // check if there is white space coming up...
        {
            //cout<<endl<<"Here!"<<endl;
            getline(inFile,buffer);                         // if so, eat it!
        }

        newCoursePtr->component = myCourse;

        if(scheduleHeadPtr==NULL)                            // if the schedule linked list is empty
        {
            InitList(scheduleHeadPtr, scheduleTailPtr, newCoursePtr);       // Initialize the linked list
        }
        else                                                                // If the list has already been initialized...
        {
            AddToBack(scheduleTailPtr, newCoursePtr);                       // Add the new course to the back of the list.
        }


        totCourses++;

      }
    inFile.close();
    //return currentSchedule;
}

// Initializes a linked list with a New Node Pointer
// PRE: The linked list is empty, and the new node has been defined
// Post: The new node will be both the Head and Tail of the Linked list
//       and will point to NULL
void InitList(NodePtr& listPtr, NodePtr& lastPtr, NodePtr newNodePtr)
{
    AddToFront(listPtr, newNodePtr);
    listPtr->next=NULL;
    lastPtr=listPtr;
}

// Adds a new node to the Head of a linked list
// PRE: Linked list has been initialized, and new node has been defined
// Post: The new node will be added to the front of the linked list
void AddToFront(NodePtr& listPtr, NodePtr newNodePtr)
{
        newNodePtr->next = listPtr;
        listPtr= newNodePtr;
}

// Adds a new node to the Head of a linked list
// PRE: Linked list has been initialized, and new node has been defined
// Post: The new node will be added to the back of the linked list
void AddToBack(NodePtr& lastPtr, NodePtr newNodePtr)
{
        newNodePtr->next = NULL;
        lastPtr->next = newNodePtr;
        lastPtr=newNodePtr;

}

// Prints the entire schedule linked list
// PRE: Linked list is populated with the student schedule
// POST: No data changes are made
void PrintList(NodePtr listPtr)
{
    NodePtr currPtr = listPtr;
    cout<<endl;
    cout<<endl<<"Printing list"<<endl<<endl;
    while(currPtr!=NULL)
    {
        cout<<currPtr->component.courseString;
        currPtr=currPtr->next;
    }
    cout<<endl;
}

// Prints partial schedule to screen grouped by days of the week
// PRE: The schedule has been populated
// POST: No changes are made to data
void displayWeeklySchedule(int ClassesPerDay[], NodePtr scheduleHeadPtr,int totCourses)
{

    string dayString;
    NodePtr currPtr = scheduleHeadPtr;

     for(Days day = SUN; day<=SAT; day = Days(day +1))         // For every day of the week...
    {
        dayString = getDayString(day);
        cout<<"***"<<endl<<dayString<<endl<<"***"<<endl<<endl;
        if(ClassesPerDay[day]!=0)
        {
            currPtr = scheduleHeadPtr;
            while(currPtr!=NULL)                                // For every course in the schedule...
            {
                if(currPtr->component.meetDays[day]==true)      // If the class meets this day, display it.
                {
                    cout<<currPtr->component.title.code<<endl;
                    cout<<currPtr->component.time.timeString<<endl;
                    cout<<currPtr->component.location.building<<" - "<<currPtr->component.location.room<<endl;
                    cout<<"____________"<<endl;
                    cout<<endl;
                }
                currPtr = currPtr->next;
            }
        }
        else
        {
            cout<<"NO CLASSES ON THIS DAY"<<endl<<endl;
        }
    }

}

// Prints full schedule to screen
// PRE: Schedule is populated
// POST: No data changes.
void displayFullSchedudule(int totCourses, Course courseSchedule[])
{
    cout<<endl;
    for(int i=0; i<totCourses; i++)
    {
        cout<<courseSchedule[i].courseString;                   // Display the current schedule to the screen
    }
    cout<<endl;
}

//  Finds conflicts in scheduling before adding a new course
// PRE: myCourse is a fully defined course, total courses is accurate and up to date, arrays have been populated with current student schedule
// POST: If no conflict in schedule is found course is added
void findConflict(int & totCourses, int ClassesPerDay[], NodePtr& scheduleHeadPtr, NodePtr& scheduleTailPtr, NodePtr& newCoursePtr)
{
    NodePtr currPtr = scheduleHeadPtr;
    NodePtr conflictPtr = NULL;
    bool noConflict = true;
    bool makeRoom = false;
    string conflict;
    string conflict1 = "";

    int conflictClass;
    string conflictDayString;
    string conflict2 = "";
    int conflictCounter = 0;
    for(Days day = SUN; day<=SAT; day = Days(day +1))         // For every day of the week...
    {
        if(newCoursePtr->component.meetDays[day]==true)                        // If the pending class is scheduled for this day...
        {
            if(ClassesPerDay[day]>0)                            // and we already have class this day...
            {
                           // int totalClasses = ClassesPerDay[day];
                //for(int classes = 0; classes <= totCourses; classes++)     // Go through every class...
                while(currPtr!=NULL)
                {
                    //if(studentSchedule[classes].meetDays[day]==true)        // if the current class is scheduled for the current day...
                    if(currPtr->component.meetDays[day]==true)                // if the current class is scheduled for the current day...
                    {
                        //if(!checkTime(weeklySchedule[day][classes].time, myCourse.time))
                        //if(!checkTime(studentSchedule[classes].time, myCourse.time))        // check for time conflicts on this day
                        if(!checkTime(currPtr->component.time, newCoursePtr->component.time))                  // check for time conflicts on this day
                        {

                            //conflict = studentSchedule[classes].title.code;
                            conflict = currPtr->component.title.code;
                            if(conflict1 == "")                                             // the first time a conflict is found save that courses information for later use
                            {
                            //conflict1 = weeklySchedule[day][classes].title.code;
                                conflict1 = conflict;

                                            //conflictDay = int(day);
                                //conflictClass = int(classes);
                                conflictPtr = currPtr;
                                conflictDayString = getDayString(day);
                                conflictCounter++;

                            }
                            else if(conflict1 == conflict)                                  // if you find a conflict with the same course, ignore it
                            {
                            // conflict already found
                            }
                            else if(conflict2=="")                                          // if we find a new course conflict document it
                            {
                            //conflict2 == weeklySchedule[day][classes].title.code;
                                conflict2 = conflict;

                                conflictCounter++;
                                break;                                                      // and there is no need to do further checking at this point because the course will not be added to the schedule.
                            }
                        }
                    }
                    currPtr=currPtr->next;
                }
            }
        }
    }

    if(conflictCounter == 0)                    // no conflict found, add the course!
    {

        cout<<"Your new course has been successfully added to your schedule!"<<endl;
        //schedule.append(buffer);
        noConflict = true;
        for(Days day = SUN; day<=SAT; day = Days(day +1))         // For every day of the week...
        {
            if(newCoursePtr->component.meetDays[day]==true)
            {
                ClassesPerDay[day]++;
            }
        }

        AddToBack(scheduleTailPtr, newCoursePtr);
        totCourses++;
    }
    else if(conflictCounter == 1)       // One conflict found...
    {
        cout<<"There is a time conflict with your schedule and the class you are trying to add."<<endl;
        cout<<"You are currently enrolled in: "<<conflictPtr->component.title.code<<" on "<<conflictDayString<<" at: "<<conflictPtr->component.time.timeString<<endl;
        cout<<"Your are trying to add: "<<newCoursePtr->component.title.code<<" on "<<conflictDayString<<" at: "<<newCoursePtr->component.time.timeString<<endl;

        /* My final touch then I'm going to sleep!
            For my final trick... I'm going to make a course disappear!
        */

        //if the user wishes, they may remove the conflicting course to make room for the pending course

        cout<<endl<<"Would you like to remove "<<conflictPtr->component.title.code<<" to make room in your schedule for the new course?"<<endl;
        cout<<"(0) No    (1) Yes "<<endl;
        cin>>makeRoom;
        if(makeRoom)
        {
            makeRoom = false;
            cout<<"Warning!  This will drop "<<conflictPtr->component.title.code<<" from your schedule."<<endl;
            cout<<"Are you sure you want to drop this course to add "<<newCoursePtr->component.title.code<<"?"<<endl;
            cout<<"(0) No    (1) Yes "<<endl;
            cin>>makeRoom;
            if(makeRoom)        // if the user confirms class swap
            {
                removeCourse(conflictPtr, scheduleHeadPtr, scheduleTailPtr, ClassesPerDay, totCourses);        // delete the conflicting course from the schedule
                cout<<"Your new course has been successfully added to your schedule!"<<endl;

                noConflict = true;
                for(Days day = SUN; day<=SAT; day = Days(day +1))         // For every day of the week...
                {
                    if(newCoursePtr->component.meetDays[day]==true)                      // And add the new course to the schedule
                    {
                        ClassesPerDay[day]++;
                    }
                }

                AddToBack(scheduleTailPtr, newCoursePtr);
                totCourses++;
            }
        }
    }
    else            //more than one conflicting course
    {
        cout<<"Because your pending course conflicts with more than one class on your schedule it cannot be added at this time."<<endl;
        cout<<"If you would like to add this enroll in "<<newCoursePtr->component.title.code<<" you will first have to drop at least one of the following classes."<<endl;
        cout<<conflict1<<", "<<conflict2<<endl;
    }
}

// Allows the user to drop a course from their schedule
// PRE: There are courses in the student schedule
// POST: If the user confirms which course to remove, the course will be completely removed from the schedule
void dropCourse(int ClassesPerDay[], NodePtr& scheduleHeadPtr, NodePtr& scheduleTailPtr, int & totCourses)
{


    NodePtr dropThisCourse = NULL;
    NodePtr currPtr = scheduleHeadPtr;
    PrintList(scheduleHeadPtr);     // show user their current courses

    string courseCode;
    int index = -1;                                         // index of the course to be dropped
    bool dropConfirm = false;
    bool dropMenu = false;
    do
    {
        cout<<"Enter the course code of the class you would like to drop"<<endl;
        cout<<"example: "<<scheduleHeadPtr->component.title.code<<endl;
        cin>>courseCode;
        while(currPtr!=NULL)
        {
            if(currPtr->component.title.code==courseCode)       // search all courses to find matching course
            {
                dropThisCourse = currPtr;                                        // once found, save a pointer to that course
                break;
            }
            currPtr= currPtr->next;
        }
        if(dropThisCourse!=NULL)                                            // index found!
        {
            cout<<"Course found: "<<endl;
            cout<<dropThisCourse->component.courseString<<endl;
            cout<<"Do you wish to drop this course?"<<endl;
            cout<<"Enter (0) = No  (1) Yes: "<<endl;
            cin>> dropConfirm;
            cout<<endl;
            if(dropConfirm)                                                         // confirm drop?
                removeCourse(dropThisCourse, scheduleHeadPtr, scheduleTailPtr, ClassesPerDay, totCourses);    // drop course
            else
            {
                cout<<"Course drop canceled.  Your schedule remains the same."<<endl;
                cout<<"Do you still wish to drop a course?"<<endl;
                cout<<"Enter (0) = No  (1) Yes: "<<endl;
                cin>>dropMenu;
            }

        }
        else    // Course not found... give chance to correct.
        {
            cout<<courseCode<<" not found.  Would you like to try again?"<<endl;
            cout<<"Enter (0) = No  (1) Yes: "<<endl;
            cin>>dropMenu;
            cout<<endl;
        }

    }while(dropMenu);
}

// Removes Course from schedule
// PRE: index contains the index of a course to be removed from the studentSchedule array
// POST: Course removed.  ClassesPerDay updated, total courses updated.
void removeCourse(NodePtr& dropThisCourse, NodePtr& scheduleHeadPtr, NodePtr& scheduleTailPtr,int ClassesPerDay[],int& totCourses)
{
    NodePtr currPtr = scheduleHeadPtr;
    NodePtr prevPtr = NULL;
    bool dropped = false;

    for(Days day = SUN; day<= SAT ; day = Days (day + 1) )  // For every day of the week...
    {
        if(dropThisCourse->component.meetDays[day]==true)      // that this course had a meeting...
            ClassesPerDay[day]--;                           // removing one class from that day
        dropThisCourse->component.meetDays[day]=false;         // and set that day to false inside the course itself
    }
    if(dropThisCourse == scheduleHeadPtr)                   // if the course we are dropping is the head
    {
        scheduleHeadPtr = scheduleHeadPtr->next;            // set head pointer to the next pointer
        delete dropThisCourse;                              // delete the course to be dropped
    }
    else
    {
        while(currPtr!=NULL)                                // exit loop if we reach the end
        {
            if(dropThisCourse == currPtr)                   // Once the current pointer is pointing to the course to be dropped...
            {
                if(dropThisCourse == scheduleTailPtr)       // Check to see if course to be dropped is the tail pointer
                    scheduleTailPtr = prevPtr;              // If so move the Tail pointer back one
                prevPtr->next = dropThisCourse->next;       // previous pointer skips over the dropped course, to the next proceeding in the list
                delete dropThisCourse;                      // delete the course to be dropped
                break;
            }
            prevPtr = currPtr;
            currPtr = currPtr->next;
        }
    }
    totCourses--;                                           // Finally - update the total number of courses
}
