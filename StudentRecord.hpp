#ifndef STUDENTRECORD_HPP
#define STUDENTRECORD_HPP

#include <string>
#include <tuple>
#include <vector>

namespace registration {

// A student record is a tuple of (name, id, grade)
using StudentRecord = std::tuple<std::string, int, double>;

// Create a validated student record
StudentRecord createRecord(const std::string& name, int studentId, double grade);

// Accessors
std::string getName(const StudentRecord& rec);
int         getId(const StudentRecord& rec);
double      getGrade(const StudentRecord& rec);

// Unpack the record into separate variables
void unpackRecord(const StudentRecord& rec,
                  std::string& outName,
                  int& outId,
                  double& outGrade);

// Format as "Name (ID: X, Grade: Y)"
std::string formatRecord(const StudentRecord& rec);

// Compute average grade; throws if vector is empty
double averageGrade(const std::vector<StudentRecord>& records);

// Return the record with the highest grade; throws if vector is empty
StudentRecord bestStudent(const std::vector<StudentRecord>& records);

// Return a copy sorted by grade descending
std::vector<StudentRecord> sortByGrade(const std::vector<StudentRecord>& records);

// Return a copy sorted by name ascending
std::vector<StudentRecord> sortByName(const std::vector<StudentRecord>& records);

// Return records whose grade is strictly greater than minGrade
std::vector<StudentRecord> filterByGrade(const std::vector<StudentRecord>& records,
                                         double minGrade);

} // namespace registration

#endif // STUDENTRECORD_HPP
