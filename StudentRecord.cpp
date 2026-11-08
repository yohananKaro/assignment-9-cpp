#include "StudentRecord.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace registration {

namespace {
// Tuple field indices
constexpr int kNameIndex  = 0;
constexpr int kIdIndex    = 1;
constexpr int kGradeIndex = 2;

// Valid grade bounds
constexpr double kMinGrade = 0.0;
constexpr double kMaxGrade = 100.0;

// Minimum valid student id
constexpr int kMinId = 1;

// Precision for formatting
constexpr int kPrecisionOne = 1;
} // anonymous namespace

StudentRecord createRecord(const std::string& name, int studentId, double grade) {
    if (studentId < kMinId) {
        throw std::invalid_argument("Student ID must be positive");
    }
    if ((grade < kMinGrade) || (grade > kMaxGrade)) {
        throw std::invalid_argument("Grade must be between 0 and 100");
    }
    return std::make_tuple(name, studentId, grade);
}

std::string getName(const StudentRecord& rec) {
    return std::get<kNameIndex>(rec);
}

int getId(const StudentRecord& rec) {
    return std::get<kIdIndex>(rec);
}

double getGrade(const StudentRecord& rec) {
    return std::get<kGradeIndex>(rec);
}

void unpackRecord(const StudentRecord& rec,
                  std::string& outName,
                  int& outId,
                  double& outGrade) {
    std::tie(outName, outId, outGrade) = rec;
}

std::string formatRecord(const StudentRecord& rec) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(kPrecisionOne);
    oss << getName(rec)
        << " (ID: " << getId(rec)
        << ", Grade: " << getGrade(rec) << ")";
    return oss.str();
}

double averageGrade(const std::vector<StudentRecord>& records) {
    if (records.empty()) {
        throw std::invalid_argument("Cannot average an empty record list");
    }
    double total = 0.0;
    for (const auto& rec : records) {
        total += getGrade(rec);
    }
    return (total / static_cast<double>(records.size()));
}

StudentRecord bestStudent(const std::vector<StudentRecord>& records) {
    if (records.empty()) {
        throw std::invalid_argument("Cannot find best student in an empty list");
    }
    auto best = records.begin();
    for (auto iter = records.begin(); iter != records.end(); ++iter) {
        if (getGrade(*iter) > getGrade(*best)) {
            best = iter;
        }
    }
    return *best;
}

std::vector<StudentRecord> sortByGrade(const std::vector<StudentRecord>& records) {
    auto copy = records;
    std::sort(copy.begin(), copy.end(), [](const StudentRecord& lhs,
                                           const StudentRecord& rhs) {
        return (getGrade(lhs) > getGrade(rhs));
    });
    return copy;
}

std::vector<StudentRecord> sortByName(const std::vector<StudentRecord>& records) {
    auto copy = records;
    std::sort(copy.begin(), copy.end(), [](const StudentRecord& lhs,
                                           const StudentRecord& rhs) {
        return (getName(lhs) < getName(rhs));
    });
    return copy;
}

std::vector<StudentRecord> filterByGrade(const std::vector<StudentRecord>& records,
                                         double minGrade) {
    std::vector<StudentRecord> result;
    for (const auto& rec : records) {
        if (getGrade(rec) > minGrade) {
            result.push_back(rec);
        }
    }
    return result;
}

} // namespace registration
