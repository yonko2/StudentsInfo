#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const short GROUP_LOWER_RANGE = 1;
const short GROUP_UPPER_RANGE = 8;
const short GRADE_LOWER_RANGE = 2;
const short GRADE_UPPER_RANGE = 6;

struct course
{
	std::string name;
	std::string grade;
};

struct student
{
	std::string name;
	std::string fn;
	std::vector<course> courses;
};

struct studentGroup
{
	short group;
	std::vector<student> students;
};

void printOptions()
{
	std::cout << "\nPlease choose an option:\n"
		"1) Insert student in a group\n"
		"2) Remove student from a group\n"
		"3) Print all students from a group\n"
		"4) Sort student group(s)\n"
		"5) Exit\n"
		"\nPlease choose an option: ";
}

short handleInputGroup()
{
	short group = 0;
	std::cout << "Please choose a group from "
		<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << ": ";
	std::cin >> group;

	while (group<GROUP_LOWER_RANGE || group>GROUP_UPPER_RANGE)
	{
		std::cout << "This group does not exist.\n";
		std::cout << "Please choose a group from "
			<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << ": ";
		std::cin >> group;
	}

	return group;
}

std::vector<short> handleInputGroups()
{
	std::vector<short> groups;

	short groupCount = 0;
	std::cout << "How many groups do you wish to process? ";
	std::cin >> groupCount;

	for (short i = 0; i < groupCount; i++)
	{
		short group = 0;

		std::cout << "Please choose group No. " << i + 1 << " from "
			<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << ": ";
		std::cin >> group;

		while (group<GROUP_LOWER_RANGE || group>GROUP_UPPER_RANGE)
		{
			std::cout << "This group does not exist.\n";
			std::cout << "Please choose a group from "
				<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << ": ";
			std::cin >> group;
		}

		groups.push_back(group);
	}

	return groups;
}

void handleFileNotOpenError()
{
	std::cout << "Error while opening the file.";
	exit(-1);
}

std::string getFilenameFromGroup(const short group)
{
	std::string filename = "csv/group";
	filename += group + '0';
	filename += ".csv";
	return filename;
}

std::string getCoursesString(const std::vector<course> courses)
{
	std::string output;

	for (const course course : courses)
	{
		output += course.name + '/' + course.grade + ';';
	}

	output.erase(output.length() - 1);

	return output;
}

std::string getCoursesOutputString(const std::vector<course> courses)
{
	std::string output;

	for (const course course : courses)
	{
		output += course.name + '/' + course.grade + ' ';
	}

	output.erase(output.length() - 1);

	return output;
}

student parseStudentString(const std::string line)
{
	size_t lineIndex = 0;
	student student;

	while (line[lineIndex] != ',') {
		student.name += line[lineIndex];
		lineIndex++;
	}
	lineIndex++;

	while (line[lineIndex] != ',') {
		student.fn += line[lineIndex];
		lineIndex++;
	}
	lineIndex++;

	std::vector<course> courses;
	bool appendToName = true;
	course course;
	while (line[lineIndex] != '\0') {
		if (line[lineIndex] == '/')
		{
			appendToName = false;
		}
		else if (line[lineIndex] == ';')
		{
			courses.push_back(course);
			appendToName = true;

			course.name = "";
			course.grade = "";
		}
		else
		{
			if (appendToName)
			{
				course.name += line[lineIndex];
			}
			else
			{
				course.grade += line[lineIndex];
			}
		}

		lineIndex++;
	}
	courses.push_back(course);
	student.courses = courses;

	return student;
}

studentGroup getStudentsFromGroup(const short group)
{
	const std::string filename = getFilenameFromGroup(group);
	std::vector<student> students;

	std::ifstream file(filename);
	if (!file.is_open()) {
		handleFileNotOpenError();
	}

	bool headerLine = true;
	std::string line;
	while (std::getline(file, line)) {
		// Skip reading header
		if (headerLine == true) {
			headerLine = false;
			continue;
		}

		students.push_back(parseStudentString(line));
	}
	file.close();

	studentGroup studentGroup = { group, students };

	return studentGroup;
}

void saveGroupToFile(const studentGroup studentGroup)
{
	const std::string filename = getFilenameFromGroup(studentGroup.group);

	std::ofstream file(filename);
	if (!file.is_open()) {
		handleFileNotOpenError();
	}

	std::string fileOutputString = "Name,Fn,Courses\n";

	const size_t studentsCount = studentGroup.students.size();
	for (size_t i = 0; i < studentsCount; i++)
	{
		student student = studentGroup.students[i];

		fileOutputString += student.name + "," + student.fn +
			"," + getCoursesString(student.courses);

		if (i != studentsCount - 1)
		{
			fileOutputString += "\n";
		}
	}
	file << fileOutputString;

	file.close();
}

bool fnExists(const std::string fn)
{
	for (short i = 1; i <= 8; i++)
	{
		studentGroup studentsGroup = getStudentsFromGroup(i);
		for (const student student : studentsGroup.students)
		{
			if (fn == student.fn)
			{
				return true;
			}
		}
	}
	return false;
}

bool isGradeValid(const std::string grade)
{
	if (grade.length() != 4)
	{
		return false;
	}
	if (grade[0] < GRADE_LOWER_RANGE + '0' || grade[0]> GRADE_UPPER_RANGE + '0' ||
		grade[1] != '.' ||
		grade[2] < '0' || grade[2]>'9' ||
		grade[3] < '0' || grade[3]>'9')
	{
		return false;
	}
	return true;
}

double getGPA(const student student)
{
	double gpa = 0;

	for (const course course : student.courses)
	{
		gpa += std::stod(course.grade);
	}

	return gpa / student.courses.size();
}

void insertStudentInGroup(const short group, const student student)
{
	studentGroup studentGroup = getStudentsFromGroup(group);

	studentGroup.students.push_back(student);

	saveGroupToFile(studentGroup);
}

void removeStudentFromGroup(const short group, const std::string fn)
{
	studentGroup studentGroup = getStudentsFromGroup(group);

	for (size_t i = 0; i < studentGroup.students.size(); i++)
	{
		student student = studentGroup.students[i];
		if (fn == student.fn)
		{
			studentGroup.students.erase(studentGroup.students.begin() + i);
			i--;
		}
	}

	saveGroupToFile(studentGroup);
}

void printStudentGroup(const studentGroup studentGroup)
{
	std::cout << "\nName\t\t\tFN\t\tCourses\n";
	for (const student student : studentGroup.students)
	{
		std::cout << student.name << "\t" << student.fn
			<< "\t" << getCoursesOutputString(student.courses) << "\n";
	}
}

void sortStudentGroup(studentGroup& studentGroup, bool isSortingByGPA, bool isAscending)
{
	const size_t groupSize = studentGroup.students.size() - 1;

	// Bubble sort
	if (isSortingByGPA && isAscending)
	{
		for (size_t step = 0; step < groupSize; step++)
		{
			for (size_t i = 0; i < groupSize - step; i++)
			{
				if (getGPA(studentGroup.students[i]) > getGPA(studentGroup.students[i + 1]))
				{
					std::swap(studentGroup.students[i], studentGroup.students[i + 1]);
				}
			}
		}
	}
	else if (isSortingByGPA && !isAscending)
	{
		for (size_t step = 0; step < groupSize; step++)
		{
			for (size_t i = 0; i < groupSize - step; i++)
			{
				if (getGPA(studentGroup.students[i]) < getGPA(studentGroup.students[i + 1]))
				{
					std::swap(studentGroup.students[i], studentGroup.students[i + 1]);
				}
			}
		}
	}
	else if (!isSortingByGPA && isAscending)
	{
		for (size_t step = 0; step < groupSize; step++)
		{
			for (size_t i = 0; i < groupSize - step; i++)
			{
				if (studentGroup.students[i].fn > studentGroup.students[i + 1].fn)
				{
					std::swap(studentGroup.students[i], studentGroup.students[i + 1]);
				}
			}
		}
	}
	else
	{
		for (size_t step = 0; step < groupSize; step++)
		{
			for (size_t i = 0; i < groupSize - step; i++)
			{
				if (studentGroup.students[i].fn < studentGroup.students[i + 1].fn)
				{
					std::swap(studentGroup.students[i], studentGroup.students[i + 1]);
				}
			}
		}
	}
}

std::vector<studentGroup> getStudentGroups(const std::vector<short> groups)
{
	std::vector<studentGroup> studentGroups;

	for (const short group : groups)
	{
		studentGroups.push_back(getStudentsFromGroup(group));
	}

	return studentGroups;
}

std::vector<studentGroup> getSortedStudentGroups(const std::vector<short> groups, bool isSortingByGPA, bool isAscending)
{
	std::vector<studentGroup> studentGroupsSorted = getStudentGroups(groups);

	for (studentGroup& studentGroup : studentGroupsSorted)
	{
		sortStudentGroup(studentGroup, isSortingByGPA, isAscending);
	}

	return studentGroupsSorted;
}

studentGroup getStudentGroupAggregate(const std::vector<studentGroup> studentGroups)
{
	studentGroup studentGroupAggregate;
	studentGroupAggregate.group = 0;

	for (const studentGroup studentGroup : studentGroups)
	{
		for (const student student : studentGroup.students)
		{
			studentGroupAggregate.students.push_back(student);
		}
	}

	return studentGroupAggregate;
}

void handleInsertStudentInGroup()
{
	const short group = handleInputGroup();
	std::string name, fn;
	std::vector<course> courses;

	std::cout << "Please input the student's name: ";
	std::cin >> name;

	std::cout << "Please input a faculty number: ";
	std::cin >> fn;
	while (fnExists(fn))
	{
		std::cout << "This faculty number already exists. Please input another faculty number: ";
		std::cin >> fn;
	}

	short coursesNumber = 0;
	do
	{
		std::cout << "Please input the number of student courses (from 1 to 10): ";
		std::cin >> coursesNumber;
	} while (coursesNumber < 1 || coursesNumber > 10);

	for (short i = 0; i < coursesNumber; i++)
	{
		std::string currentCourse, currentGrade;
		std::cout << "Insert course No. " << (char)(i + 1 + '0') << " ";
		std::cin >> currentCourse;
		std::cout << "With grade (between "
			<< GRADE_LOWER_RANGE
			<< " and "
			<< GRADE_UPPER_RANGE
			<< " with two decimal digits): ";
		std::cin >> currentGrade;
		while (!isGradeValid(currentGrade))
		{
			std::cout << "Please input a grade in the correct format: ";
			std::cin >> currentGrade;
		}

		course course = { currentCourse, currentGrade };
		courses.push_back(course);
	}

	const student student = { name, fn, courses };
	insertStudentInGroup(group, student);
	std::cout << "Student successfully added.\n";
}

void handleRemoveStudentFromGroup()
{
	const short group = handleInputGroup();
	std::string fn;

	std::cout << "Please input the faculty number of the student: ";
	std::cin >> fn;
	while (!fnExists(fn))
	{
		std::cout << "This faculty number doesn't exist, please input another: ";
		std::cin >> fn;
	}
	removeStudentFromGroup(group, fn);
	std::cout << "Student removed successfully.\n";
}

void handlePrintStudentsInGroup()
{
	const short group = handleInputGroup();
	printStudentGroup(getStudentsFromGroup(group));
}

void handleSortStudentGroups()
{
	bool isSortingByGPA = true;
	std::cout << "Sort by GPA or FN? ";
	std::string sortByOption;
	std::cin >> sortByOption;
	while (sortByOption != "GPA" && sortByOption != "FN")
	{
		std::cout << "Please choose a valid sort type (GPA/FN): ";
		std::cin >> sortByOption;
	}
	isSortingByGPA = sortByOption == "GPA";

	bool isAscending = true;
	std::cout << "Sort ascending or descending? ";
	std::string sortOrder;
	std::cin >> sortOrder;
	while (sortOrder != "ascending" && sortOrder != "descending")
	{
		std::cout << "Please choose a valid sort order (ascending/descending): ";
		std::cin >> sortOrder;
	}
	isAscending = sortOrder == "ascending";

	const std::vector<short> groups = handleInputGroups();

	studentGroup studentGroupAggregate = getStudentGroupAggregate(getStudentGroups(groups));

	sortStudentGroup(studentGroupAggregate, isSortingByGPA, isAscending);
	printStudentGroup(studentGroupAggregate);

	bool saveSortedGroups = false;
	std::cout << "Do you wish to save the sorted groups? (Y/N) ";
	std::string saveGroups;
	std::cin >> saveGroups;
	while (saveGroups != "Y" && saveGroups != "N")
	{
		std::cout << "Please choose a valid answer (Y/N): ";
		std::cin >> saveGroups;
	}
	saveSortedGroups = saveGroups == "Y";

	if (saveSortedGroups)
	{
		const std::vector<studentGroup> studentGroupsSorted = getSortedStudentGroups(groups, isSortingByGPA, isAscending);
		for (const studentGroup studentGroupSorted : studentGroupsSorted)
		{
			saveGroupToFile(studentGroupSorted);
		}
		std::cout << "Groups saved successfully to the corresponding files.\n";
	}
}

int main()
{
	std::cout << "Welcome to Students Info!\n";
	printOptions();

	short actionNumber = 0;
	do
	{
		std::cin >> actionNumber;
		switch (actionNumber)
		{
		case 1:
			handleInsertStudentInGroup();
			break;
		case 2:
			handleRemoveStudentFromGroup();
			break;
		case 3:
			handlePrintStudentsInGroup();
			break;
		case 4:
			handleSortStudentGroups();
			break;
		case 5:
			std::cout << "\nThank you for using our app!\n";
			return 0; // Exit program
		default:
			std::cout << "Please choose an option between 1 and 5: ";
		}

		printOptions();
	} while (actionNumber != 5);

	return 0;
}

