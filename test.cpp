/**
 * @file test.cpp
 * @brief Unit tests for Student Course Registration System using doctest
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "StudentRecord.hpp"
#include "CourseManager.hpp"
#include "StringUtils.hpp"
#include "TypeTraitsUtils.hpp"
#include <string>
#include <vector>
#include <fstream>

using namespace registration;

// ==================== STUDENT RECORD (TUPLE) TESTS ====================

TEST_CASE("createRecord - valid") {
    auto r = createRecord("Alice", 1001, 92.5);
    CHECK(getName(r) == "Alice");
    CHECK(getId(r) == 1001);
    CHECK(getGrade(r) == doctest::Approx(92.5));
}

TEST_CASE("createRecord - invalid id throws") {
    CHECK_THROWS_AS(createRecord("Bad", 0, 50.0), std::invalid_argument);
    CHECK_THROWS_AS(createRecord("Bad", -1, 50.0), std::invalid_argument);
}

TEST_CASE("createRecord - invalid grade throws") {
    CHECK_THROWS_AS(createRecord("Bad", 1, -1.0), std::invalid_argument);
    CHECK_THROWS_AS(createRecord("Bad", 1, 101.0), std::invalid_argument);
}

TEST_CASE("unpackRecord") {
    auto r = createRecord("Bob", 1002, 85.0);
    std::string name;
    int id;
    double grade;
    unpackRecord(r, name, id, grade);
    CHECK(name == "Bob");
    CHECK(id == 1002);
    CHECK(grade == doctest::Approx(85.0));
}

TEST_CASE("formatRecord") {
    auto r = createRecord("Alice", 1001, 92.5);
    CHECK(formatRecord(r) == "Alice (ID: 1001, Grade: 92.5)");
}

TEST_CASE("averageGrade") {
    std::vector<StudentRecord> records = {
        createRecord("A", 1, 80.0),
        createRecord("B", 2, 90.0),
        createRecord("C", 3, 100.0)
    };
    CHECK(averageGrade(records) == doctest::Approx(90.0));
}

TEST_CASE("averageGrade - empty throws") {
    std::vector<StudentRecord> empty;
    CHECK_THROWS_AS(averageGrade(empty), std::invalid_argument);
}

TEST_CASE("bestStudent") {
    std::vector<StudentRecord> records = {
        createRecord("A", 1, 80.0),
        createRecord("B", 2, 95.0),
        createRecord("C", 3, 90.0)
    };
    auto best = bestStudent(records);
    CHECK(getName(best) == "B");
    CHECK(getGrade(best) == doctest::Approx(95.0));
}

TEST_CASE("bestStudent - empty throws") {
    std::vector<StudentRecord> empty;
    CHECK_THROWS_AS(bestStudent(empty), std::invalid_argument);
}

TEST_CASE("sortByGrade - descending") {
    std::vector<StudentRecord> records = {
        createRecord("A", 1, 70.0),
        createRecord("B", 2, 90.0),
        createRecord("C", 3, 80.0)
    };
    auto sorted = sortByGrade(records);
    CHECK(getName(sorted[0]) == "B");
    CHECK(getName(sorted[1]) == "C");
    CHECK(getName(sorted[2]) == "A");
}

TEST_CASE("sortByName - ascending") {
    std::vector<StudentRecord> records = {
        createRecord("Charlie", 3, 70.0),
        createRecord("Alice", 1, 90.0),
        createRecord("Bob", 2, 80.0)
    };
    auto sorted = sortByName(records);
    CHECK(getName(sorted[0]) == "Alice");
    CHECK(getName(sorted[1]) == "Bob");
    CHECK(getName(sorted[2]) == "Charlie");
}

TEST_CASE("filterByGrade") {
    std::vector<StudentRecord> records = {
        createRecord("A", 1, 70.0),
        createRecord("B", 2, 90.0),
        createRecord("C", 3, 85.0),
        createRecord("D", 4, 60.0)
    };
    auto filtered = filterByGrade(records, 80.0);
    CHECK(filtered.size() == 2);
    CHECK(getName(filtered[0]) == "B");
    CHECK(getName(filtered[1]) == "C");
}

TEST_CASE("filterByGrade - none pass") {
    std::vector<StudentRecord> records = {
        createRecord("A", 1, 50.0),
        createRecord("B", 2, 60.0)
    };
    auto filtered = filterByGrade(records, 100.0);
    CHECK(filtered.empty());
}

// ==================== COURSE MANAGER TESTS ====================

TEST_CASE("CourseManager - addStudent and getStudent") {
    CourseManager cm;
    auto r = createRecord("Alice", 1001, 92.5);
    cm.addStudent(r);
    auto fetched = cm.getStudent(1001);
    CHECK(getName(fetched) == "Alice");
}

TEST_CASE("CourseManager - addStudent duplicate throws") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    CHECK_THROWS_AS(cm.addStudent(createRecord("Alice2", 1001, 80.0)), std::invalid_argument);
}

TEST_CASE("CourseManager - createCourse duplicate throws") {
    CourseManager cm;
    cm.createCourse("CPP");
    CHECK_THROWS_AS(cm.createCourse("CPP"), std::invalid_argument);
}

TEST_CASE("CourseManager - enroll and check") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    cm.createCourse("CPP");
    cm.enrollStudent(1001, "CPP");
    CHECK(cm.isEnrolled(1001, "CPP") == true);
    CHECK(cm.getEnrollmentCount("CPP") == 1);
}

TEST_CASE("CourseManager - enroll nonexistent student throws") {
    CourseManager cm;
    cm.createCourse("CPP");
    CHECK_THROWS_AS(cm.enrollStudent(9999, "CPP"), std::invalid_argument);
}

TEST_CASE("CourseManager - enroll nonexistent course throws") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    CHECK_THROWS_AS(cm.enrollStudent(1001, "Nonexistent"), std::invalid_argument);
}

TEST_CASE("CourseManager - double enrollment throws") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    cm.createCourse("CPP");
    cm.enrollStudent(1001, "CPP");
    CHECK_THROWS_AS(cm.enrollStudent(1001, "CPP"), std::invalid_argument);
}

TEST_CASE("CourseManager - removeStudent") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    cm.createCourse("CPP");
    cm.enrollStudent(1001, "CPP");
    cm.removeStudent(1001, "CPP");
    CHECK(cm.isEnrolled(1001, "CPP") == false);
    CHECK(cm.getEnrollmentCount("CPP") == 0);
}

TEST_CASE("CourseManager - removeStudent not enrolled throws") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    cm.createCourse("CPP");
    CHECK_THROWS_AS(cm.removeStudent(1001, "CPP"), std::invalid_argument);
}

TEST_CASE("CourseManager - getEnrolledStudents sorted") {
    CourseManager cm;
    cm.addStudent(createRecord("C", 3, 70.0));
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.addStudent(createRecord("B", 2, 80.0));
    cm.createCourse("CPP");
    cm.enrollStudent(3, "CPP");
    cm.enrollStudent(1, "CPP");
    cm.enrollStudent(2, "CPP");
    auto enrolled = cm.getEnrolledStudents("CPP");
    CHECK(enrolled.size() == 3);
    CHECK(enrolled[0] == 1);
    CHECK(enrolled[1] == 2);
    CHECK(enrolled[2] == 3);
}

TEST_CASE("CourseManager - getStudentCourses") {
    CourseManager cm;
    cm.addStudent(createRecord("Alice", 1001, 92.5));
    cm.createCourse("CPP");
    cm.createCourse("Algorithms");
    cm.enrollStudent(1001, "CPP");
    cm.enrollStudent(1001, "Algorithms");
    auto courses = cm.getStudentCourses(1001);
    CHECK(courses.size() == 2);
}

TEST_CASE("CourseManager - studentsInBothCourses") {
    CourseManager cm;
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.addStudent(createRecord("B", 2, 80.0));
    cm.addStudent(createRecord("C", 3, 70.0));
    cm.createCourse("CPP");
    cm.createCourse("Algo");
    cm.enrollStudent(1, "CPP");
    cm.enrollStudent(2, "CPP");
    cm.enrollStudent(1, "Algo");
    cm.enrollStudent(3, "Algo");

    auto both = cm.studentsInBothCourses("CPP", "Algo");
    CHECK(both.size() == 1);
    CHECK(both[0] == 1);
}

TEST_CASE("CourseManager - studentsInEitherCourse") {
    CourseManager cm;
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.addStudent(createRecord("B", 2, 80.0));
    cm.addStudent(createRecord("C", 3, 70.0));
    cm.createCourse("CPP");
    cm.createCourse("Algo");
    cm.enrollStudent(1, "CPP");
    cm.enrollStudent(2, "CPP");
    cm.enrollStudent(1, "Algo");
    cm.enrollStudent(3, "Algo");

    auto either = cm.studentsInEitherCourse("CPP", "Algo");
    CHECK(either.size() == 3);
}

TEST_CASE("CourseManager - assignGrade and getCourseAverage") {
    CourseManager cm;
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.addStudent(createRecord("B", 2, 80.0));
    cm.createCourse("CPP");
    cm.enrollStudent(1, "CPP");
    cm.enrollStudent(2, "CPP");
    cm.assignGrade(1, "CPP", 95.0);
    cm.assignGrade(2, "CPP", 85.0);
    CHECK(cm.getCourseAverage("CPP") == doctest::Approx(90.0));
}

TEST_CASE("CourseManager - assignGrade invalid range throws") {
    CourseManager cm;
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.createCourse("CPP");
    cm.enrollStudent(1, "CPP");
    CHECK_THROWS_AS(cm.assignGrade(1, "CPP", -1.0), std::invalid_argument);
    CHECK_THROWS_AS(cm.assignGrade(1, "CPP", 101.0), std::invalid_argument);
}

TEST_CASE("CourseManager - assignGrade not enrolled throws") {
    CourseManager cm;
    cm.addStudent(createRecord("A", 1, 90.0));
    cm.createCourse("CPP");
    CHECK_THROWS_AS(cm.assignGrade(1, "CPP", 90.0), std::invalid_argument);
}

TEST_CASE("CourseManager - getAllCourses") {
    CourseManager cm;
    cm.createCourse("CPP");
    cm.createCourse("Algorithms");
    cm.createCourse("Algebra");
    auto courses = cm.getAllCourses();
    CHECK(courses.size() == 3);
    CHECK(courses[0] == "Algebra");
    CHECK(courses[1] == "Algorithms");
    CHECK(courses[2] == "CPP");
}

TEST_CASE("CourseManager - getAllStudentIds") {
    CourseManager cm;
    cm.addStudent(createRecord("C", 3, 70.0));
    cm.addStudent(createRecord("A", 1, 90.0));
    auto ids = cm.getAllStudentIds();
    CHECK(ids.size() == 2);
    CHECK(ids[0] == 1);
    CHECK(ids[1] == 3);
}

// ==================== STRING UTILS TESTS ====================

TEST_CASE("toUpper") {
    CHECK(toUpper("hello") == "HELLO");
    CHECK(toUpper("Hello World") == "HELLO WORLD");
    CHECK(toUpper("") == "");
    CHECK(toUpper("123abc") == "123ABC");
}

TEST_CASE("toLower") {
    CHECK(toLower("HELLO") == "hello");
    CHECK(toLower("Hello World") == "hello world");
    CHECK(toLower("") == "");
}

TEST_CASE("trim") {
    CHECK(trim("  hello  ") == "hello");
    CHECK(trim("hello") == "hello");
    CHECK(trim("   ") == "");
    CHECK(trim("") == "");
    CHECK(trim("\t hello \n") == "hello");
}

TEST_CASE("split") {
    auto parts = split("a,b,c", ',');
    CHECK(parts.size() == 3);
    CHECK(parts[0] == "a");
    CHECK(parts[1] == "b");
    CHECK(parts[2] == "c");
}

TEST_CASE("split - no delimiter") {
    auto parts = split("hello", ',');
    CHECK(parts.size() == 1);
    CHECK(parts[0] == "hello");
}

TEST_CASE("split - empty string") {
    auto parts = split("", ',');
    CHECK(parts.size() == 1);
    CHECK(parts[0] == "");
}

TEST_CASE("split - trailing delimiter") {
    auto parts = split("a,b,", ',');
    CHECK(parts.size() == 3);
    CHECK(parts[2] == "");
}

TEST_CASE("join") {
    CHECK(join({"a", "b", "c"}, ", ") == "a, b, c");
    CHECK(join({"hello"}, "-") == "hello");
    CHECK(join({}, ", ") == "");
}

TEST_CASE("startsWith") {
    CHECK(startsWith("Hello World", "Hello") == true);
    CHECK(startsWith("Hello World", "World") == false);
    CHECK(startsWith("Hello", "Hello World") == false);
    CHECK(startsWith("", "") == true);
    CHECK(startsWith("Hello", "") == true);
}

TEST_CASE("endsWith") {
    CHECK(endsWith("Hello World", "World") == true);
    CHECK(endsWith("Hello World", "Hello") == false);
    CHECK(endsWith("Hello", "Hello World") == false);
    CHECK(endsWith("", "") == true);
}

TEST_CASE("replaceAll") {
    CHECK(replaceAll("aabbcc", "bb", "XX") == "aaXXcc");
    CHECK(replaceAll("hello", "ll", "r") == "hero");
    CHECK(replaceAll("aaa", "a", "bb") == "bbbbbb");
    CHECK(replaceAll("hello", "xyz", "abc") == "hello");
    CHECK(replaceAll("hello", "", "abc") == "hello");  // empty 'from' returns original
}

TEST_CASE("countChar") {
    CHECK(countChar("hello world", 'l') == 3);
    CHECK(countChar("hello", 'z') == 0);
    CHECK(countChar("", 'a') == 0);
}

TEST_CASE("isPalindrome") {
    CHECK(isPalindrome("racecar") == true);
    CHECK(isPalindrome("Race Car") == true);
    CHECK(isPalindrome("hello") == false);
    CHECK(isPalindrome("A man a plan a canal Panama") == true);
    CHECK(isPalindrome("") == true);
    CHECK(isPalindrome("a") == true);
}

TEST_CASE("reverseString") {
    CHECK(reverseString("Hello") == "olleH");
    CHECK(reverseString("") == "");
    CHECK(reverseString("a") == "a");
    CHECK(reverseString("abcd") == "dcba");
}

// ==================== TYPE TRAITS TESTS ====================

TEST_CASE("describeType") {
    CHECK(describeType<int>() == "integral");
    CHECK(describeType<long>() == "integral");
    CHECK(describeType<char>() == "integral");
    CHECK(describeType<bool>() == "integral");
    CHECK(describeType<double>() == "floating-point");
    CHECK(describeType<float>() == "floating-point");
    CHECK(describeType<std::string>() == "string");
    CHECK(describeType<int*>() == "pointer");
    CHECK(describeType<double*>() == "pointer");
}

TEST_CASE("isNumeric") {
    CHECK(isNumeric<int>() == true);
    CHECK(isNumeric<double>() == true);
    CHECK(isNumeric<float>() == true);
    CHECK(isNumeric<std::string>() == false);
}

TEST_CASE("areSameType") {
    CHECK(areSameType<int, int>() == true);
    CHECK(areSameType<int, double>() == false);
    CHECK(areSameType<std::string, std::string>() == true);
}

TEST_CASE("numericToString - integral") {
    CHECK(numericToString(42) == "42");
    CHECK(numericToString(-5) == "-5");
    CHECK(numericToString(0) == "0");
}

TEST_CASE("numericToString - floating point") {
    CHECK(numericToString(3.14) == "3.14");
    CHECK(numericToString(0.0) == "0.00");
}

TEST_CASE("safeAdd - int + int returns long long") {
    auto result = safeAdd(3, 4);
    CHECK(result == 7);
    static_assert(std::is_same<decltype(result), long long>::value,
                  "int + int should return long long");
}

TEST_CASE("safeAdd - int + double returns double") {
    auto result = safeAdd(3, 4.5);
    CHECK(result == doctest::Approx(7.5));
    static_assert(std::is_same<decltype(result), double>::value,
                  "int + double should return double");
}

TEST_CASE("getZero") {
    CHECK(getZero<int>() == 0);
    CHECK(getZero<double>() == doctest::Approx(0.0));
    CHECK(getZero<std::string>() == "");
}

// ==================== STUDENT TEST FILE CHECK ====================

TEST_CASE("StudentTest - File exists") {
    std::ifstream f("StudentTest.cpp");
    CHECK_MESSAGE(f.good(), "StudentTest.cpp not found. You must create StudentTest.cpp with at least 20 test cases.");
}