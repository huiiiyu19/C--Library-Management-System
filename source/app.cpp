#define _CRT_SECURE_NO_WARNINGS

#include    <iostream>
#include    <cstdlib>
#include    <cstdio>
#include    <fstream>
#include    <string>
#include    "List.h"
#include    "LibStudent.h"
#include    "LibBook.h"
#include    <iomanip>
#include    <cstring>

using namespace std;

bool ReadFile(string, List*);
bool DeleteRecord(List*, char*);
bool Display(List*, int, int);
bool InsertBook(string, List*);
bool SearchStudent(List*, char* id, LibStudent&);
bool computeAndDisplayStatistics(List*);
bool printStuWithSameBook(List*, char*);
bool displayWarnedStudent(List*, List*, List*);
int menu();
int JulianDay(int, int, int);
void calculateDate(const string&, int&, int&, int&);
bool checkEmpty(List*);
bool checkBookList(Node*);
bool checkNumber(string, int&);
const string currentDate = "29/3/2020";

int main() {
    int choice, source, detail;
    char id[10], callNum[20];
    List* studentList = new List;
    List* firstMisde = new List;
    List* secondMisde = new List;
    LibStudent stu;
    string input;
    Node* cur;

    do {
        choice = menu();

        switch (choice) {
        case 1: ReadFile("student.txt", studentList);
            break;
        case 2:
            DeleteRecord(studentList, id);
            break;
        case 3:
            if (SearchStudent(studentList, id, stu)) {
                cout << "\nName: " << stu.name;
                cout << "\nId: " << stu.id;
                cout << "\nCourse: " << stu.course;
                cout << "\nPhone No: " << stu.phone_no << "\n\n";
                system("pause");
                system("cls");
            }
            break;
        case 4: InsertBook("book.txt", studentList);
            break;
        case 5:
            cout << "\nDISPLAY OUTPUT\n";
            cout << "==============" << endl;

            if (!checkEmpty(studentList))
                continue;
            cur = studentList->head;
            if (!checkBookList(cur))
                continue;
            do {
                cout << "\nWhere do you want to display the output (1 - File / 2 - Screen): ";
                getline(cin, input);

                if (!checkNumber(input, source))
                    continue;

                if (source > 2 || source < 1) {
                    cout << "\nInvalid input! Must be 1 or 2! \n";
                    continue;
                }
                break;
            } while (true);

            do {
                cout << "\nDo you want to display book list for every student (1 - YES / 2 - NO): ";
                getline(cin, input);

                if (!checkNumber(input, detail))
                    continue;
                if (detail > 2 || detail < 1) {
                    cout << "\nInvalid input! Must be 1 or 2! \n";
                    continue;
                }
                break;
            } while (true);

            Display(studentList, source, detail);
            break;

        case 6: computeAndDisplayStatistics(studentList);
            break;
        case 7:
            printStuWithSameBook(studentList, callNum);
            break;

        case 8: displayWarnedStudent(studentList, firstMisde, secondMisde);
            break;
        case 9:
            cout << "\nExiting the program..... " << endl;
            cout << "Thanks for using.";
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n" << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 9);

    cout << "\n\n";
    system("pause");
    return 0;
}

int menu() {

    string choice;
    int input;
    do {
        cout << "======================= Menu =======================\n" << endl;
        cout << "       1.      Read file" << endl;
        cout << "       2.      Delete record" << endl;
        cout << "       3.      Search student" << endl;
        cout << "       4.      Insert book" << endl;
        cout << "       5.      Display output" << endl;
        cout << "       6.      Compute and Display Statistics" << endl;
        cout << "       7.      Student with Same Book" << endl;
        cout << "       8.      Display Warned Student" << endl;
        cout << "       9.      Exit" << endl;
        cout << "\n====================================================" << endl;
        cout << "\nEnter your choice: ";
        getline(cin, choice);

        // Validate numeric input; if invalid, pause and clear screen then restart loop
        if (!checkNumber(choice, input)) {
            cout << endl;
            system("pause");
            system("cls");
            continue;
        }
        break;
    } while (true);
    return input;
}

bool ReadFile(string filename, List* list) {
    cout << "\nREAD FILE" << endl;
    cout << "=========" << endl << endl;
    ifstream file;
    file.open(filename);
    if (!file.is_open())
        return false;

    string id_str, name_str, course_str, phone_str;

    // Read each record from file
    while (getline(file, id_str)) {
        if (id_str.empty())
            continue;

        getline(file, name_str);
        getline(file, course_str);
        getline(file, phone_str);

        id_str = id_str.substr(id_str.find('=') + 2);
        name_str = name_str.substr(name_str.find('=') + 2);
        course_str = course_str.substr(course_str.find('=') + 2);
        phone_str = phone_str.substr(phone_str.find('=') + 2);

        // Check for duplicate student ID in the list
        bool duplicateRecordFound = false;
        for (Node* cur = list->head; cur; cur = cur->next) {
            if (strcmp(cur->item.id, id_str.c_str()) == 0) {
                duplicateRecordFound = true;
                break;
            }
        }
        if (duplicateRecordFound) // Skip if duplicate found
            continue;

        // Create new student object and set details
        LibStudent student;
        strcpy(student.id, id_str.c_str());
        strcpy(student.name, name_str.c_str());
        strcpy(student.course, course_str.c_str());
        strcpy(student.phone_no, phone_str.c_str());
        student.total_fine = 0.0;
        student.totalbook = 0;

        list->insert(student);
    }

    cout << list->size() << " records have been succesfully read." << endl << endl;
    system("pause");
    system("cls");
    return true;
}

bool DeleteRecord(List* list, char* id) {
    cout << "\nDELETE RECORD" << endl;
    cout << "=============" << endl;
    if (!checkEmpty(list))
        return false;

    string id_str;

    cout << "\nEnter Student ID to delete: ";
    getline(cin, id_str);

    // Ensure ID is not empty
    if (id_str.empty()) {
        cout << "\nError: Please enter a id.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    // Copy string to char array
    strcpy(id, id_str.c_str());

    // Check if all characters are digits
    bool allDigits = true;
    for (int i = 0; i < strlen(id); i++) {
        if (!isdigit(id[i])) {
            allDigits = false;
            break;
        }
    }

    if (!allDigits) {
        cout << "\nError: Invalid input! Please enter number id.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    // Validate length of Student ID
    if (strlen(id) < 7 || strlen(id) > 7) {
        cout << "\nError: Invalid input! Student ID must be 7 digit numbers.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    Node* cur = list->head;
    Node* pre = NULL;
    int count = 1;
    bool found = false;

    while (cur != NULL) {
        if (strcmp(cur->item.id, id) == 0) { // match found
            list->remove(count);
            found = true;
            break;
        }
        cur = cur->next;
        count++;
    }

    // Handle if record not found
    if (!found) {
        cout << "\nStudent with ID " << id << " not found.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }
    cout << "\nThe record is successfully deleted.\n" << endl;
    system("pause");
    system("cls");
    return true;
}

bool SearchStudent(List* list, char* id, LibStudent& stu) {
    cout << "\nSEARCH STUDENT" << endl;
    cout << "==============" << endl;
    if (!checkEmpty(list))
        return false;
    string id_str;

    cout << "\nEnter Student ID to search: ";
    getline(cin, id_str);

    if (id_str.empty()) {
        cout << "\nError: Please enter a id.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    strcpy(id, id_str.c_str());

    bool allDigits = true;
    for (int i = 0; i < strlen(id); i++) {
        if (!isdigit(id[i])) {
            allDigits = false;
            break;
        }
    }

    if (!allDigits) {
        cout << "\nError: Invalid input! Please enter number id.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    if (strlen(id) < 7 || strlen(id) > 7) {
        cout << "\nError: Invalid input! Student ID must be 7 digit numbers.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    Node* cur = list->head; // start from first node

    while (cur != NULL) {
        if (strcmp(cur->item.id, id) == 0) {
            stu = cur->item; // copy student info
            return true;
        }
        cur = cur->next; // move to next node
    }

    // If we reach here, student was not found
    cout << "\nStudent not found.\n\n";
    system("pause");
    system("cls");
    return false;
}

bool InsertBook(string filename, List* list) {

    cout << "\nINSERT BOOK" << endl;
    cout << "===========" << endl;;
    if (!checkEmpty(list))
        return false;

    ifstream infile;
    infile.open(filename);

    char stu_id[10], authorLine[250], title[250], publisher[30], ISBN[11], callNum[20];
    string borrow, due; // Dates in string format
    int yearPublished;
    char tmpAuthor[250]; // Temporary buffer for author name

    Node* cur;

    if (!infile.is_open()) {
        cout << "\nError: Unable to open the file.\n\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    while (!infile.eof()) {

        infile >> stu_id >> authorLine >> title >> publisher >> ISBN
            >> yearPublished >> callNum >> borrow >> due;

        LibBook book;
        int authorIndex = 0, start = 0;

        // Parse authors separated by '/' in authorLine
        for (int i = 0; i <= strlen(authorLine); i++) {
            if (authorLine[i] == '/' || authorLine[i] == '\0') {
                int length = i - start;
                strncpy(tmpAuthor, authorLine + start, length);
                tmpAuthor[length] = '\0';

                // Allocate memory for author name and copy
                book.author[authorIndex] = new char[strlen(tmpAuthor) + 1];
                strcpy(book.author[authorIndex], tmpAuthor);

                authorIndex++;
                start = i + 1;
            }
        }

        strcpy(book.title, title);
        strcpy(book.publisher, publisher);
        strcpy(book.ISBN, ISBN);
        book.yearPublished = yearPublished;
        strcpy(book.callNum, callNum);

        // Convert borrow/due dates into integers
        int day, month, year;
        calculateDate(borrow, book.borrow.day, book.borrow.month, book.borrow.year);
        calculateDate(due, book.due.day, book.due.month, book.due.year);
        calculateDate(currentDate, day, month, year);

        // Calculate overdue days
        int dueJulian = JulianDay(book.due.day, book.due.month, book.due.year);
        int currentJulian = JulianDay(day, month, year);
        int overDue = currentJulian - dueJulian;

        // Apply fine if overdue
        if (overDue > 0)
            book.fine += overDue * 0.5;
        else
            book.fine += 0.0;

        // Find the student in the list to assign this book
        cur = list->head;
        for (; cur != NULL;) {
            if (strcmp(cur->item.id, stu_id) == 0) {

                // Check for duplicate callNum in the list
                bool duplicateRecordFound = false;
                for (int i = 0; i < cur->item.totalbook; i++) {
                    if (cur->item.book[i].compareCallNum(book)) { // compare call numbers
                        duplicateRecordFound = true;
                        break;
                    }
                }
                if (duplicateRecordFound) //break if found duplicate record
                    break;

                // Check if student has already borrowed max books
                if (cur->item.totalbook >= 15) {
                    cout << "\nError: Book array full for student\n\n" << stu_id << endl;
                    break;
                }

                int countbook = cur->item.totalbook;

                cur->item.book[countbook] = book;
                cur->item.totalbook++;
                break;
            }
            cur = cur->next;
        }

        if (infile.fail()) break;
    }

    infile.close();

    cur = list->head;
    for (; cur != NULL;) {
        cur->item.calculateTotalFine();
        cur = cur->next;
    }
    cout << "\nBooks inserted successfully to student list.\n\n";
    system("pause");
    system("cls");
    return true;
}

// Function to split date into day, month, year
void calculateDate(const string& date, int& day, int& month, int& year) {
    int firstSlash = date.find('/');
    int secondSlash = date.find('/', firstSlash + 1);

    day = stoi(date.substr(0, firstSlash));
    month = stoi(date.substr(firstSlash + 1, secondSlash - firstSlash - 1));
    year = stoi(date.substr(secondSlash + 1));
}

// Function to convert a date into Julian Day Number
int JulianDay(int day, int month, int year) {
    if (month <= 2) {
        year -= 1;
        month += 12;
    }

    int A = year / 100;
    int B = 2 - A + A / 4;

    int JD = day + int(365.25 * (year + 4716)) + int(30.6001 * (month + 1)) + B - 1524;

    return JD;
}

bool Display(List* list, int source, int detail) {
    Node* cur = list->head, * book;
    int stuCount = 1;

    // Case 1: Display student info only on screen
    if (source == 2 && detail == 2) {
        system("cls");
        // Display student info only on screen
        for (; cur != NULL;) {
            cout << "\nSTUDENT " << stuCount++ << "\n";
            cur->item.print(cout);
            cout << "\n*************************\n\n";
            cur = cur->next;
        }
    }
    // Case 2: Display student info + book list on screen
    else if (source == 2 && detail == 1) {
        system("cls");
        // Display student info + book list on screen
        for (; cur != NULL;) {
            cout << "\nSTUDENT " << stuCount++ << "\n";
            cur->item.print(cout);

            cout << "\nBOOK LIST: " << endl;
            book = cur;
            for (int i = 0; i < book->item.totalbook; i++) {
                cout << "\nBOOK " << i + 1 << endl;
                book->item.book[i].print(cout);
            }
            cur = cur->next;
            cout << "\n**********************************************************************************\n\n";
        }
    }
    // Case 3: Output student info + book list to file
    else if (source == 1 && detail == 1) {
        ofstream outfile("student_booklist.txt");
        if (!outfile.is_open()) {
            cout << "\nUnable to open output file.\n\n";
            return false;
        }
        for (; cur != NULL;) {
            outfile << "STUDENT " << stuCount++ << endl;
            cur->item.print(outfile);

            outfile << "\nBOOK LIST: " << endl;
            book = cur;
            for (int i = 0; i < book->item.totalbook; i++) {
                outfile << "\nBOOK " << i + 1 << "\n";
                book->item.book[i].print(outfile);
            }
            cur = cur->next;
            outfile << "\n**********************************************************************************\n\n";
        }
        outfile.close();
        cout << "\n\nSuccessfully display output to student_booklist.txt\n\n";
    }
    // Case 4: Output student info only to file
    else if (source == 1 && detail == 2) {
        ofstream outfile("student_info.txt");
        for (; cur != NULL;) {
            outfile << "STUDENT " << stuCount++ << "\n";
            cur->item.print(outfile);
            outfile << "\n***************************(\n\n";
            cur = cur->next;
        }
        outfile.close();
        cout << "\n\nSuccessfully display output to student_info.txt\n\n";
    }

    cout << "\n\nSuccessfully display output\n\n\n";
    system("pause");
    system("cls");
    return true;
}

bool computeAndDisplayStatistics(List* list) {
    cout << "\nDISPLAY STATISTICS" << endl;
    cout << "====================" << endl;

    if (!checkEmpty(list))
        return false;

    Node* cur = list->head, * bookcur;
    const char* course[5] = { "CS", "IA", "IB", "CN", "CT" };

    // Check if any student has a book list
    if (!checkBookList(cur))
        return false;

    cout << endl;
    cout << left << setw(8) << "Course"
        << setw(20) << "Number of Students"
        << setw(22) << "Total Books Borrowed"
        << setw(22) << "Total Overdue Books"
        << "Total Overdue Fine (RM)\n";
    cout << "-----------------------------------------------------------------------------------------------" << endl;

    // Loop through each course to compute statistics
    for (int i = 0; i < 5; i++) {
        int noStudent = 0, book = 0, overdue = 0;
        double totalFine = 0.0;

        cur = list->head;
        // Traverse the student list
        for (; cur != NULL;) {
            if (strcmp(cur->item.course, course[i]) == 0) {
                noStudent++;
                book += cur->item.totalbook;

                // Check each book for overdue fine
                bookcur = cur;
                for (int i = 0; i < bookcur->item.totalbook; i++) {
                    if (bookcur->item.book[i].fine != 0)
                        overdue++;
                }
                totalFine += cur->item.total_fine;
            }
            cur = cur->next;
        }
        cout << right << setw(4) << course[i] << setw(12) << noStudent << setw(22) << book << setw(22) << overdue << setw(26) << fixed << setprecision(2) << totalFine << endl;
    }
    cout << endl;
    system("pause");
    system("cls");
    return true;
}

bool printStuWithSameBook(List* list, char* callNum) {
    cout << "\nSTUDENT WITH SAME BOOK" << endl;
    cout << "=======================" << endl;

    if (!checkEmpty(list))
        return false;

    Node* cur = list->head;
    int count = 0;

    if (!checkBookList(cur))
        return false;

    cout << "\nEnter call number: ";
    cin.getline(callNum, 20);

    if (strlen(callNum) == 0) {
        cout << "\nError: Please enter a call Number.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    cur = list->head;
    //First pass - count matching students
    while (cur != NULL) {
        for (int i = 0; i < cur->item.totalbook; i++) {
            if (strcmp(cur->item.book[i].callNum, callNum) == 0) {
                count++;
                break;
            }
        }
        cur = cur->next;
    }

    if (count == 0) {
        cout << "\nNo students found borrowing the book with call number " << callNum << "\n\n";
        system("pause");
        system("cls");
        return true; //List not empty but no matches
    }

    system("cls");
    cout << "\nThere are " << count << " students that borrow the book with call number " << callNum << " as shown below:\n\n";

    //Second pass - print details
    cur = list->head;
    while (cur != NULL) {
        for (int i = 0; i < cur->item.totalbook; i++) {
            if (strcmp(cur->item.book[i].callNum, callNum) == 0) {
                cout << "Student Id = " << cur->item.id << endl;
                cout << "Name = " << cur->item.name << endl;
                cout << "Course = " << cur->item.course << endl;
                cout << "Phone Number = " << cur->item.phone_no << endl;
                cout << "Borrow Date: ";
                cur->item.book[i].borrow.print(cout);
                cout << "\nDue Date: ";
                cur->item.book[i].due.print(cout);
                cout << "\n\n\n";
            }
        }
        cur = cur->next;
    }
    system("pause");
    system("cls");
    return true;
}

bool displayWarnedStudent(List* list, List* type1, List* type2) {
    cout << "\nWARNED STUDENT" << endl;
    cout << "================" << endl;

    if (!checkEmpty(list))
        return false;

    Node* book, * cur = list->head;

    // If no books in the list, exit
    if (!checkBookList(cur))
        return false;

    // Clear all nodes in type1 list by repeatedly removing the first node
    while (!type1->empty()) {
        type1->remove(1);
    }
    // Clear all nodes in type2 list by repeatedly removing the first node
    while (!type2->empty()) {
        type2->remove(1);
    }

    cur = list->head;
    for (; cur != NULL;) {
        int noBook = 0;
        bool allBooksOverdue = true;

        // Count books with fine >= RM5.00
        for (int i = 0; i < cur->item.totalbook; i++) {
            if (cur->item.book[i].fine >= 5.0) {
                noBook++;
            }
        }
        if (noBook > 2) {
            type1->insert(cur->item);
        }

        // Check if every book is overdue
        for (int i = 0; i < cur->item.totalbook; i++) {
            if (cur->item.book[i].fine == 0.0) {
                allBooksOverdue = false;
                break;
            }
        }
        if (allBooksOverdue) {
            if (cur->item.total_fine > 50.0) {
                type2->insert(cur->item);
            }
        }
        cur = cur->next;
    }

    if (type1->empty() && type2->empty()) {
        cout << "\n*************************************************************\n" << endl;
        cout << "--- The First and Second Misdemeanors List is empty ---" << endl;
        cout << "\n*************************************************************\n" << endl;
        system("pause");
        system("cls");
        return false;
    }
    system("cls");

    if (type1->empty()) {
        cout << "\n*******************************************************************************************" << endl;
        cout << "\n--- First type of Misdemeanors: More than 2 books that are overdue for >= 10 days ---" << endl;
        cout << "\n*******************************************************************************************" << endl;
        cout << "\n                         --- No student in this list. --- \n\n" << endl;
    }

    else {
        cout << "\n*******************************************************************************************" << endl;
        cout << "\n--- First type of Misdemeanors: More than 2 books that are overdue for >= 10 days ---" << endl;
        cur = type1->head;
        int student = 1;
        for (; cur != NULL;) {
            cout << "\n*******************************************************************************************\n" << endl;
            cout << "\nSTUDENT " << student++;
            cur->item.print(cout);

            cout << "\nBOOK LIST: " << endl;
            book = cur;
            for (int i = 0; i < book->item.totalbook; i++) {
                cout << "\nBOOK " << i + 1 << endl;
                book->item.book[i].print(cout);
            }
            cout << "\n" << endl;
            cur = cur->next;
        }
        cout << endl;
    }

    if (!type2->empty()) {
        cout << "\n*******************************************************************************************" << endl;
        cout << "\n--- Second type of Misdemeanors: Total Fine > RM50.00 and Every Book is Overdue ---" << endl;
        int student = 1;
        cur = type2->head;
        for (; cur != NULL;) {
            cout << "\n*******************************************************************************************\n" << endl;
            cout << "\nSTUDENT " << student++;
            cur->item.print(cout);

            cout << "\nBOOK LIST: " << endl;
            book = cur;
            for (int i = 0; i < book->item.totalbook; i++) {
                cout << "\nBOOK " << i + 1 << endl;
                book->item.book[i].print(cout);
            }
            cout << "\n" << endl;
            cur = cur->next;
        }
        cout << "*******************************************************************************************\n" << endl;
    }
    else {
        cout << "\n**************************************************************************************" << endl;
        cout << "\n--- Second type of Misdemeanors: Total Fine > RM50.00 and Every Book is Overdue ---" << endl;
        cout << "\n**************************************************************************************" << endl;
        cout << "\n                         --- No student in this list. --- \n\n" << endl;
    }

    system("pause");
    system("cls");
    return true;
}

// Checks if the given list is empty
bool checkEmpty(List* list) {
    if (list->empty()) {
        cout << "\nError: No student in the list.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }
    return true;
}

// Checks if the given list contains at least one book
bool checkBookList(Node* cur) {
    bool found = false;

    for (; cur != NULL;) {
        if (cur->item.totalbook != 0)
            found = true;
        cur = cur->next;
    }

    if (!found) {
        cout << "\nError: The book list is empty, please insert the booklist first.\n" << endl;
        system("pause");
        system("cls");
        return false;
    }

    return true;
}

// Checks if the input is exactly one digit and converts it to integer
bool checkNumber(string input, int& ans) {
    bool isNumber = true;

    if (input.empty())
        isNumber = false;
    else if (input.length() != 1)
        isNumber = false;
    else if (input.length() == 1) {
        if (!isdigit(input[0]))
            isNumber = false;
    }

    if (!isNumber) {
        cout << "\nInvalid input. Please enter one digit only.\n";
        return false;
    }

    ans = stoi(input);

    return true;
}