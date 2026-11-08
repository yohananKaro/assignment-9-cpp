#include "CourseManager.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>

namespace registration {

namespace {
constexpr double kMinGrade = 0.0;
constexpr double kMaxGrade = 100.0;
} // anonymous namespace

void CourseManager::addStudent(const StudentRecord& rec) {
    int studentId = getId(rec);
    if (mStudents.contains(studentId)) {
        throw std::invalid_argument("Student ID already registered");
    }
    mStudents[studentId] = rec;
}

StudentRecord CourseManager::getStudent(int studentId) const {
    auto iter = mStudents.find(studentId);
    if (iter == mStudents.end()) {
        throw std::invalid_argument("Student not found");
    }
    return iter->second;
}

void CourseManager::removeStudent(int studentId,
                                  const std::string& courseName) {
    auto courseIter = mCourseEnrollments.find(courseName);
    if (courseIter == mCourseEnrollments.end() ||
        (!courseIter->second.contains(studentId))) {
        throw std::invalid_argument("Student not enrolled in course");
    }
    courseIter->second.erase(studentId);
    mGrades.erase({studentId, courseName});
}

void CourseManager::createCourse(const std::string& courseName) {
    if (mCourseEnrollments.contains(courseName)) {
        throw std::invalid_argument("Course already exists");
    }
    mCourseEnrollments[courseName] = std::set<int>{};
}

void CourseManager::enrollStudent(int studentId,
                                  const std::string& courseName) {
    if (!mStudents.contains(studentId)) {
        throw std::invalid_argument("Student not found");
    }
    auto courseIter = mCourseEnrollments.find(courseName);
    if (courseIter == mCourseEnrollments.end()) {
        throw std::invalid_argument("Course not found");
    }
    if (courseIter->second.contains(studentId)) {
        throw std::invalid_argument("Student already enrolled in course");
    }
    courseIter->second.insert(studentId);
}

bool CourseManager::isEnrolled(int studentId,
                               const std::string& courseName) const {
    auto courseIter = mCourseEnrollments.find(courseName);
    if (courseIter == mCourseEnrollments.end()) {
        return false;
    }
    return courseIter->second.contains(studentId);
}

int CourseManager::getEnrollmentCount(const std::string& courseName) const {
    auto courseIter = mCourseEnrollments.find(courseName);
    if (courseIter == mCourseEnrollments.end()) {
        return 0;
    }
    return static_cast<int>(courseIter->second.size());
}

std::vector<int> CourseManager::getEnrolledStudents(
    const std::string& courseName) const {
    auto courseIter = mCourseEnrollments.find(courseName);
    if (courseIter == mCourseEnrollments.end()) {
        return {};
    }
    return std::vector<int>(courseIter->second.begin(),
                            courseIter->second.end());
}

std::vector<std::string> CourseManager::getStudentCourses(int studentId) const {
    std::vector<std::string> courses;
    for (const auto& [name, enrolled] : mCourseEnrollments) {
        if (enrolled.contains(studentId)) {
            courses.push_back(name);
        }
    }
    return courses;
}

std::vector<int> CourseManager::studentsInBothCourses(
    const std::string& firstCourse,
    const std::string& secondCourse) const {
    std::vector<int> result;
    auto firstIter  = mCourseEnrollments.find(firstCourse);
    auto secondIter = mCourseEnrollments.find(secondCourse);
    if (firstIter == mCourseEnrollments.end() ||
        secondIter == mCourseEnrollments.end()) {
        return result;
    }
    std::set_intersection(firstIter->second.begin(), firstIter->second.end(),
                          secondIter->second.begin(), secondIter->second.end(),
                          std::back_inserter(result));
    return result;
}

std::vector<int> CourseManager::studentsInEitherCourse(
    const std::string& firstCourse,
    const std::string& secondCourse) const {
    std::vector<int> result;
    auto firstIter  = mCourseEnrollments.find(firstCourse);
    auto secondIter = mCourseEnrollments.find(secondCourse);
    if (firstIter == mCourseEnrollments.end() &&
        secondIter == mCourseEnrollments.end()) {
        return result;
    }
    std::set<int> unionSet;
    if (firstIter != mCourseEnrollments.end()) {
        unionSet.insert(firstIter->second.begin(), firstIter->second.end());
    }
    if (secondIter != mCourseEnrollments.end()) {
        unionSet.insert(secondIter->second.begin(), secondIter->second.end());
    }
    result.assign(unionSet.begin(), unionSet.end());
    return result;
}

void CourseManager::assignGrade(int studentId,
                                const std::string& courseName,
                                double grade) {
    if (grade < kMinGrade || grade > kMaxGrade) {
        throw std::invalid_argument("Grade must be between 0 and 100");
    }
    if (!isEnrolled(studentId, courseName)) {
        throw std::invalid_argument("Student not enrolled in course");
    }
    mGrades[{studentId, courseName}] = grade;
}

double CourseManager::getCourseAverage(const std::string& courseName) const {
    double total = 0.0;
    int    count = 0;
    for (const auto& [key, grade] : mGrades) {
        if (key.second == courseName) {
            total += grade;
            ++count;
        }
    }
    if (count == 0) {
        throw std::invalid_argument("No grades assigned for this course");
    }
    return (total / static_cast<double>(count));
}

int CourseManager::getTotalStudents() const {
    return static_cast<int>(mStudents.size());
}

int CourseManager::getTotalCourses() const {
    return static_cast<int>(mCourseEnrollments.size());
}

std::vector<std::string> CourseManager::getAllCourses() const {
    std::vector<std::string> courses;
    courses.reserve(mCourseEnrollments.size());
    for (const auto& [name, enrolled] : mCourseEnrollments) {
        courses.push_back(name);
    }
    return courses;
}

std::vector<int> CourseManager::getAllStudentIds() const {
    std::vector<int> ids;
    ids.reserve(mStudents.size());
    for (const auto& [studentId, rec] : mStudents) {
        ids.push_back(studentId);
    }
    return ids;
}

} // namespace registration
