/**
 * @file main.cpp
 * @brief Demo for Student Course Registration System
 *
 * This file demonstrates STL containers, tuples, strings, and type traits.
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "StudentRecord.hpp"
#include "CourseManager.hpp"
#include "StringUtils.hpp"
#include "TypeTraitsUtils.hpp"

using namespace registration;

int main() {
    std::cout << "===== Student Course Registration System Demo =====\n\n";

    // ---- Section 1: Tuples (Student Records) ----
    std::cout << "--- Student Records (Tuples) ---\n";

    StudentRecord alice = createRecord("Alice", 1001, 92.5);
    StudentRecord bob = createRecord("Bob", 1002, 85.0);
    StudentRecord charlie = createRecord("Charlie", 1003, 97.3);
    StudentRecord diana = createRecord("Diana", 1004, 78.8);

    std::cout << formatRecord(alice) << "\n";
    // Expected: Alice (ID: 1001, Grade: 92.5)

    std::cout << formatRecord(bob) << "\n";
    // Expected: Bob (ID: 1002, Grade: 85.0)

    // Unpack with tie
    std::string name;
    int id;
    double grade;
    unpackRecord(charlie, name, id, grade);
    std::cout << "Unpacked: " << name << ", ID=" << id
              << ", Grade=" << grade << "\n";
    // Expected: Unpacked: Charlie, ID=1003, Grade=97.3

    // Vector of records
    std::vector<StudentRecord> records = {alice, bob, charlie, diana};

    std::cout << "Average grade: " << std::fixed << std::setprecision(1)
              << averageGrade(records) << "\n";
    // Expected: Average grade: 88.4

    StudentRecord best = bestStudent(records);
    std::cout << "Best student: " << formatRecord(best) << "\n";
    // Expected: Best student: Charlie (ID: 1003, Grade: 97.3)

    std::cout << "\nSorted by grade (descending):\n";
    auto byGrade = sortByGrade(records);
    for (const auto& r : byGrade) {
        std::cout << "  " << formatRecord(r) << "\n";
    }
    // Expected:
    //   Charlie (ID: 1003, Grade: 97.3)
    //   Alice (ID: 1001, Grade: 92.5)
    //   Bob (ID: 1002, Grade: 85.0)
    //   Diana (ID: 1004, Grade: 78.8)

    std::cout << "\nSorted by name:\n";
    auto byName = sortByName(records);
    for (const auto& r : byName) {
        std::cout << "  " << formatRecord(r) << "\n";
    }
    // Expected:
    //   Alice (ID: 1001, Grade: 92.5)
    //   Bob (ID: 1002, Grade: 85.0)
    //   Charlie (ID: 1003, Grade: 97.3)
    //   Diana (ID: 1004, Grade: 78.8)

    std::cout << "\nFiltered (grade > 85):\n";
    auto filtered = filterByGrade(records, 85.0);
    for (const auto& r : filtered) {
        std::cout << "  " << formatRecord(r) << "\n";
    }
    // Expected:
    //   Alice (ID: 1001, Grade: 92.5)
    //   Charlie (ID: 1003, Grade: 97.3)

    // ---- Section 2: Course Manager (Maps, Sets, Vectors) ----
    std::cout << "\n--- Course Manager ---\n";

    CourseManager cm;
    cm.addStudent(alice);
    cm.addStudent(bob);
    cm.addStudent(charlie);
    cm.addStudent(diana);

    cm.createCourse("CPP");
    cm.createCourse("Algorithms");
    cm.createCourse("Linear Algebra");

    cm.enrollStudent(1001, "CPP");
    cm.enrollStudent(1002, "CPP");
    cm.enrollStudent(1003, "CPP");
    cm.enrollStudent(1001, "Algorithms");
    cm.enrollStudent(1003, "Algorithms");
    cm.enrollStudent(1004, "Linear Algebra");
    cm.enrollStudent(1002, "Linear Algebra");

    std::cout << "CPP enrollment: " << cm.getEnrollmentCount("CPP") << "\n";
    // Expected: CPP enrollment: 3

    std::cout << "Alice enrolled in CPP? " << cm.isEnrolled(1001, "CPP") << "\n";
    // Expected: Alice enrolled in CPP? 1

    std::cout << "Diana enrolled in CPP? " << cm.isEnrolled(1004, "CPP") << "\n";
    // Expected: Diana enrolled in CPP? 0

    std::cout << "\nStudents in CPP: ";
    auto cppStudents = cm.getEnrolledStudents("CPP");
    for (int sid : cppStudents) {
        std::cout << sid << " ";
    }
    std::cout << "\n";
    // Expected: Students in CPP: 1001 1002 1003

    std::cout << "Alice's courses: ";
    auto aliceCourses = cm.getStudentCourses(1001);
    for (const auto& c : aliceCourses) {
        std::cout << c << " ";
    }
    std::cout << "\n";
    // Expected: Alice's courses: Algorithms CPP
    // (sorted alphabetically since we iterate over the map)

    // Intersection: students in both CPP and Algorithms
    auto both = cm.studentsInBothCourses("CPP", "Algorithms");
    std::cout << "In both CPP and Algorithms: ";
    for (int sid : both) {
        std::cout << sid << " ";
    }
    std::cout << "\n";
    // Expected: In both CPP and Algorithms: 1001 1003

    // Union: students in CPP or Algorithms
    auto either = cm.studentsInEitherCourse("CPP", "Algorithms");
    std::cout << "In CPP or Algorithms: ";
    for (int sid : either) {
        std::cout << sid << " ";
    }
    std::cout << "\n";
    // Expected: In CPP or Algorithms: 1001 1002 1003

    // Grades
    cm.assignGrade(1001, "CPP", 95.0);
    cm.assignGrade(1002, "CPP", 82.0);
    cm.assignGrade(1003, "CPP", 91.0);

    std::cout << "CPP average: " << std::fixed << std::setprecision(1)
              << cm.getCourseAverage("CPP") << "\n";
    // Expected: CPP average: 89.3

    std::cout << "Total students: " << cm.getTotalStudents() << "\n";
    // Expected: Total students: 4

    std::cout << "Total courses: " << cm.getTotalCourses() << "\n";
    // Expected: Total courses: 3

    // ---- Section 3: String Utils ----
    std::cout << "\n--- String Utilities ---\n";
    std::cout << std::defaultfloat;

    std::cout << "toUpper: " << toUpper("Hello World") << "\n";
    // Expected: toUpper: HELLO WORLD

    std::cout << "toLower: " << toLower("Hello World") << "\n";
    // Expected: toLower: hello world

    std::cout << "trim: '" << trim("  hello  ") << "'\n";
    // Expected: trim: 'hello'

    auto parts = split("apple,banana,cherry", ',');
    std::cout << "split: ";
    for (const auto& p : parts) {
        std::cout << "[" << p << "] ";
    }
    std::cout << "\n";
    // Expected: split: [apple] [banana] [cherry]

    std::cout << "join: " << join({"a", "b", "c"}, " - ") << "\n";
    // Expected: join: a - b - c

    std::cout << "startsWith: " << startsWith("Hello World", "Hello") << "\n";
    // Expected: startsWith: 1

    std::cout << "endsWith: " << endsWith("Hello World", "World") << "\n";
    // Expected: endsWith: 1

    std::cout << "replaceAll: " << replaceAll("aabbcc", "bb", "XX") << "\n";
    // Expected: replaceAll: aaXXcc

    std::cout << "countChar: " << countChar("hello world", 'l') << "\n";
    // Expected: countChar: 3

    std::cout << "isPalindrome: " << isPalindrome("Race Car") << "\n";
    // Expected: isPalindrome: 1

    std::cout << "reverse: " << reverseString("Hello") << "\n";
    // Expected: reverse: olleH

    // ---- Section 4: Type Traits ----
    std::cout << "\n--- Type Traits ---\n";

    std::cout << "int: " << describeType<int>() << "\n";
    // Expected: int: integral

    std::cout << "double: " << describeType<double>() << "\n";
    // Expected: double: floating-point

    std::cout << "string: " << describeType<std::string>() << "\n";
    // Expected: string: string

    std::cout << "int*: " << describeType<int*>() << "\n";
    // Expected: int*: pointer

    std::cout << "isNumeric<int>: " << isNumeric<int>() << "\n";
    // Expected: isNumeric<int>: 1

    std::cout << "isNumeric<string>: " << isNumeric<std::string>() << "\n";
    // Expected: isNumeric<string>: 0

    std::cout << "areSameType<int,int>: " << areSameType<int, int>() << "\n";
    // Expected: areSameType<int,int>: 1

    std::cout << "areSameType<int,double>: " << areSameType<int, double>() << "\n";
    // Expected: areSameType<int,double>: 0

    std::cout << "numericToString(42): " << numericToString(42) << "\n";
    // Expected: numericToString(42): 42

    std::cout << "numericToString(3.14): " << numericToString(3.14) << "\n";
    // Expected: numericToString(3.14): 3.14

    auto sum1 = safeAdd(3, 4);
    std::cout << "safeAdd(3, 4): " << sum1 << "\n";
    // Expected: safeAdd(3, 4): 7

    auto sum2 = safeAdd(3, 4.5);
    std::cout << "safeAdd(3, 4.5): " << sum2 << "\n";
    // Expected: safeAdd(3, 4.5): 7.5

    std::cout << "\n===== Demo Complete =====\n";

    return 0;
}
