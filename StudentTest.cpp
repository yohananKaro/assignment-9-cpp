/**
 * @file StudentTest.cpp
 * @brief Personal unit tests for the Student Course Registration System.
 *        Contains 20+ test cases covering StudentRecord, CourseManager,
 *        StringUtils, and TypeTraitsUtils.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// clang + GCC-15 libstdc++ in C++23 mode warns about <ciso646> being
// removed.  Suppress just that category around the doctest include.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-W#warnings"
#include "doctest.h"
#pragma clang diagnostic pop
#include "StudentRecord.hpp"
#include "CourseManager.hpp"
#include "StringUtils.hpp"
#include "TypeTraitsUtils.hpp"

#include <string>
#include <vector>

using namespace registration;

// ---- StudentRecord ----

TEST_CASE("ST01: createRecord stores correct name") {
    auto rec = createRecord("Eve", 2001, 77.0);
    CHECK(getName(rec) == "Eve");
}

TEST_CASE("ST02: createRecord stores correct id") {
    auto rec = createRecord("Eve", 2001, 77.0);
    CHECK(getId(rec) == 2001);
}

TEST_CASE("ST03: createRecord stores correct grade") {
    auto rec = createRecord("Eve", 2001, 77.0);
    CHECK(getGrade(rec) == doctest::Approx(77.0));
}

TEST_CASE("ST04: createRecord rejects zero id") {
    CHECK_THROWS_AS(createRecord("X", 0, 50.0), std::invalid_argument);
}

TEST_CASE("ST05: createRecord rejects grade above 100") {
    CHECK_THROWS_AS(createRecord("X", 1, 100.1), std::invalid_argument);
}

TEST_CASE("ST06: createRecord accepts boundary grade 0") {
    auto rec = createRecord("Zero", 5, 0.0);
    CHECK(getGrade(rec) == doctest::Approx(0.0));
}

TEST_CASE("ST07: createRecord accepts boundary grade 100") {
    auto rec = createRecord("Perfect", 6, 100.0);
    CHECK(getGrade(rec) == doctest::Approx(100.0));
}

TEST_CASE("ST08: formatRecord produces expected string") {
    auto rec = createRecord("Bob", 42, 88.0);
    CHECK(formatRecord(rec) == "Bob (ID: 42, Grade: 88.0)");
}

TEST_CASE("ST09: sortByGrade returns descending order") {
    std::vector<StudentRecord> recs = {
        createRecord("Low",  1, 55.0),
        createRecord("High", 2, 95.0),
        createRecord("Mid",  3, 75.0)
    };
    auto sorted = sortByGrade(recs);
    CHECK(getName(sorted[0]) == "High");
    CHECK(getName(sorted[2]) == "Low");
}

TEST_CASE("ST10: sortByName returns alphabetical order") {
    std::vector<StudentRecord> recs = {
        createRecord("Zara", 1, 70.0),
        createRecord("Anna", 2, 80.0),
        createRecord("Mike", 3, 60.0)
    };
    auto sorted = sortByName(recs);
    CHECK(getName(sorted[0]) == "Anna");
    CHECK(getName(sorted[1]) == "Mike");
    CHECK(getName(sorted[2]) == "Zara");
}

TEST_CASE("ST11: filterByGrade strict greater-than") {
    std::vector<StudentRecord> recs = {
        createRecord("Pass", 1, 80.0),
        createRecord("Fail", 2, 60.0)
    };
    auto filtered = filterByGrade(recs, 80.0); // strictly > 80
    CHECK(filtered.empty());
}

TEST_CASE("ST12: averageGrade throws on empty vector") {
    std::vector<StudentRecord> empty;
    CHECK_THROWS_AS(averageGrade(empty), std::invalid_argument);
}

// ---- CourseManager ----

TEST_CASE("ST13: addStudent and getTotalStudents") {
    CourseManager mgr;
    mgr.addStudent(createRecord("Ann", 101, 90.0));
    mgr.addStudent(createRecord("Ben", 102, 85.0));
    CHECK(mgr.getTotalStudents() == 2);
}

TEST_CASE("ST14: createCourse and getTotalCourses") {
    CourseManager mgr;
    mgr.createCourse("Math");
    mgr.createCourse("Physics");
    CHECK(mgr.getTotalCourses() == 2);
}

TEST_CASE("ST15: enrollStudent and isEnrolled") {
    CourseManager mgr;
    mgr.addStudent(createRecord("Ann", 101, 90.0));
    mgr.createCourse("Math");
    mgr.enrollStudent(101, "Math");
    CHECK(mgr.isEnrolled(101, "Math") == true);
}

TEST_CASE("ST16: removeStudent reduces enrollment count") {
    CourseManager mgr;
    mgr.addStudent(createRecord("Ann", 101, 90.0));
    mgr.createCourse("Math");
    mgr.enrollStudent(101, "Math");
    mgr.removeStudent(101, "Math");
    CHECK(mgr.getEnrollmentCount("Math") == 0);
}

TEST_CASE("ST17: studentsInBothCourses intersection") {
    CourseManager mgr;
    mgr.addStudent(createRecord("A", 1, 90.0));
    mgr.addStudent(createRecord("B", 2, 80.0));
    mgr.createCourse("X");
    mgr.createCourse("Y");
    mgr.enrollStudent(1, "X");
    mgr.enrollStudent(2, "X");
    mgr.enrollStudent(1, "Y");
    auto both = mgr.studentsInBothCourses("X", "Y");
    CHECK(both.size() == 1);
    CHECK(both[0] == 1);
}

TEST_CASE("ST18: studentsInEitherCourse union") {
    CourseManager mgr;
    mgr.addStudent(createRecord("A", 1, 90.0));
    mgr.addStudent(createRecord("B", 2, 80.0));
    mgr.createCourse("X");
    mgr.createCourse("Y");
    mgr.enrollStudent(1, "X");
    mgr.enrollStudent(2, "Y");
    auto either = mgr.studentsInEitherCourse("X", "Y");
    CHECK(either.size() == 2);
}

TEST_CASE("ST19: assignGrade and getCourseAverage") {
    CourseManager mgr;
    mgr.addStudent(createRecord("A", 1, 90.0));
    mgr.addStudent(createRecord("B", 2, 70.0));
    mgr.createCourse("CS");
    mgr.enrollStudent(1, "CS");
    mgr.enrollStudent(2, "CS");
    mgr.assignGrade(1, "CS", 100.0);
    mgr.assignGrade(2, "CS", 80.0);
    CHECK(mgr.getCourseAverage("CS") == doctest::Approx(90.0));
}

TEST_CASE("ST20: getAllCourses sorted alphabetically") {
    CourseManager mgr;
    mgr.createCourse("Zoo");
    mgr.createCourse("Art");
    mgr.createCourse("Math");
    auto courses = mgr.getAllCourses();
    CHECK(courses[0] == "Art");
    CHECK(courses[1] == "Math");
    CHECK(courses[2] == "Zoo");
}

// ---- StringUtils ----

TEST_CASE("ST21: trim removes tabs and newlines") {
    CHECK(trim("\t hello \n") == "hello");
}

TEST_CASE("ST22: replaceAll handles overlapping-free replacements") {
    CHECK(replaceAll("aaa", "a", "bb") == "bbbbbb");
}

TEST_CASE("ST23: isPalindrome ignores spaces and case") {
    CHECK(isPalindrome("A man a plan a canal Panama") == true);
    CHECK(isPalindrome("hello") == false);
}

// ---- TypeTraitsUtils ----

TEST_CASE("ST24: describeType for char returns integral") {
    CHECK(describeType<char>() == "integral");
}

TEST_CASE("ST25: getZero for string returns empty string") {
    CHECK(getZero<std::string>() == "");
}

TEST_CASE("ST26: safeAdd int+int returns long long") {
    auto result = safeAdd(10, 20);
    static_assert(std::is_same<decltype(result), long long>::value,
                  "int+int should give long long");
    CHECK(result == 30LL);
}
