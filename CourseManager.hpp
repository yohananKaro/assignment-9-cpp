#ifndef COURSEMANAGER_HPP
#define COURSEMANAGER_HPP

#include "StudentRecord.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace registration {

class CourseManager {
public:
    // Add a student to the global registry; throws if ID already exists
    void addStudent(const StudentRecord& rec);

    // Look up a student by ID; throws if not found
    StudentRecord getStudent(int studentId) const;

    // Remove a student by ID; throws if not found
    void removeStudent(int studentId, const std::string& courseName);

    // Create a new course; throws if course name already exists
    void createCourse(const std::string& courseName);

    // Enroll a student in a course; throws if either not found or already enrolled
    void enrollStudent(int studentId, const std::string& courseName);

    // Check if a student is enrolled in a course
    bool isEnrolled(int studentId, const std::string& courseName) const;

    // Number of students enrolled in a course
    int getEnrollmentCount(const std::string& courseName) const;

    // Sorted list of student IDs enrolled in a course
    std::vector<int> getEnrolledStudents(const std::string& courseName) const;

    // All courses a student is enrolled in (sorted alphabetically)
    std::vector<std::string> getStudentCourses(int studentId) const;

    // IDs of students enrolled in both courses
    std::vector<int> studentsInBothCourses(const std::string& firstCourse,
                                           const std::string& secondCourse) const;

    // IDs of students enrolled in at least one of the two courses
    std::vector<int> studentsInEitherCourse(const std::string& firstCourse,
                                            const std::string& secondCourse) const;

    // Assign a grade to a student for a specific course
    void assignGrade(int studentId, const std::string& courseName, double grade);

    // Average of all assigned course grades; throws if no grades assigned
    double getCourseAverage(const std::string& courseName) const;

    // Total number of registered students
    int getTotalStudents() const;

    // Total number of courses
    int getTotalCourses() const;

    // All course names sorted alphabetically
    std::vector<std::string> getAllCourses() const;

    // All student IDs sorted ascending
    std::vector<int> getAllStudentIds() const;

private:
    // studentId -> StudentRecord
    std::map<int, StudentRecord> mStudents;

    // courseName -> set of enrolled student IDs
    std::map<std::string, std::set<int>> mCourseEnrollments;

    // (studentId, courseName) -> grade
    std::map<std::pair<int, std::string>, double> mGrades;
};

} // namespace registration

#endif // COURSEMANAGER_HPP
