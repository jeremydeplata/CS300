#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Course {
    string courseNumber;
    string courseName;
    vector<string> prerequisites;
};


struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course newCourse) {
        course = newCourse;
        left = nullptr;
        right = nullptr;
    }
};

class BinarySearchTree {
private:
    Node* root = nullptr;
    void PrintInOrder(Node* node);
public:
    void Insert(Course course);
    void PrintAll();
    Course Search(string courseNumber);
};

// insert course into the tree
void BinarySearchTree::Insert(Course course) {
    Node* newNode = new Node(course);

    if (root == nullptr) {
        root = newNode;
        return;
    }

    Node* current = root;

    while (true) {
        if (course.courseNumber < current->course.courseNumber) {
            if (current->left == nullptr) {
                current->left = newNode;
                return;
            }
            current = current->left;
        }
        else {
            if (current->right == nullptr) {
                current->right = newNode;
                return;
            }
            current = current->right;
        }
    }
}


// search for a course by its course number
Course BinarySearchTree::Search(string courseNumber) {
    Node* current = root;

    while (current != nullptr) {
        if (courseNumber == current->course.courseNumber) {
            return current->course;
        }
        else if (courseNumber < current->course.courseNumber) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }

    return Course();
}


// print courses in alphanumeric order
void BinarySearchTree::PrintInOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    PrintInOrder(node->left);
    cout << node->course.courseNumber << ", "
        << node->course.courseName << endl;
    PrintInOrder(node->right);
}

void BinarySearchTree::PrintAll() {
    if (root == nullptr) {
        cout << "No courses loaded." << endl;
        return;
    }

    PrintInOrder(root);
}


// load courses from the CSV file
bool loadCourses(string fileName, BinarySearchTree& tree) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Course course;
        string prerequisite;

        getline(ss, course.courseNumber, ',');
        getline(ss, course.courseName, ',');

        while (getline(ss, prerequisite, ',')) {
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }

        tree.Insert(course);
    }

    file.close();
    return true;
}


// display a course and its prerequisites
void printCourse(BinarySearchTree& tree, string courseNumber) {
    Course course = tree.Search(courseNumber);

    if (course.courseNumber.empty()) {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseNumber << ", " << course.courseName << endl;
    cout << "Prerequisites: ";

    if (course.prerequisites.empty()) {
        cout << "None";
    }
    else {
        for (int i = 0; i < course.prerequisites.size(); i++) {
            Course prerequisite = tree.Search(course.prerequisites[i]);

            if (i > 0) {
                cout << ", ";
            }

            cout << prerequisite.courseNumber << " - "
                << prerequisite.courseName;
        }
    }

    cout << endl;
}


int main() {
    BinarySearchTree tree;
    int choice = 0;
    bool fileLoaded = false;
    string fileName;
    string courseNumber;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "\nWhat would you like to do? ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input." << endl;
            continue;
        }

        switch (choice) {
        case 1:
            cout << "Enter file name: ";
            getline(cin >> ws, fileName);
            fileLoaded = loadCourses(fileName, tree);
            break;

        case 2:
            if (fileLoaded) {
                cout << "\nHere is a sample schedule:\n" << endl;
                tree.PrintAll();
            }
            else {
                cout << "Please load a file first." << endl;
            }
            break;

        case 3:
            if (fileLoaded) {
                cout << "What course do you want to know about? ";
                cin >> courseNumber;

                // allow lowercase course numbers
                transform(courseNumber.begin(), courseNumber.end(),
                    courseNumber.begin(), ::toupper);

                printCourse(tree, courseNumber);
            }
            else {
                cout << "Please load a file first." << endl;
            }
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}
