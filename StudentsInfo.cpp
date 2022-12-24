#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const short GROUP_LOWER_RANGE = 1;
const short GROUP_UPPER_RANGE = 8;

void printOptions()
{
	std::cout << "Please choose an option:\n"
		"1) Insert student in a group\n"
		"2) Remove student from a group\n"
		"3) Print all students from a group\n"
		"4) Sort a student group\n"
		/*"- asc / desc\n"
		"- by gpa / fn\n"
		"4.1) Save the sorted group \n?"
		"Y / N\n"*/
		"5) Print sorted student groups\n"
		/*"5.1) Please input desired groups(1 to 8), separated by commas\n"
		"Example - 1, 2, 3, 7\n"
		"5.2) Sort ascending or descending ?\n"
		"5.3) Sort by GPA or faculty number ?\n"*/
		"6) Exit\n";
}

short handleInputGroup()
{
	short group = 0;
	std::cout << "Please choose a group from "
		<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << "\n";
	std::cin >> group;

	while (group<GROUP_LOWER_RANGE || group>GROUP_UPPER_RANGE)
	{
		std::cout << "This group does not exist.\n";
		std::cout << "Please choose a group from "
			<< GROUP_LOWER_RANGE << " to " << GROUP_UPPER_RANGE << "\n";
		std::cin >> group;
	}

	return group;
}

void handleFileNotOpenError()
{
	std::cout << "Error while opening the file.";
	exit(-1);
}

std::string getFilenameFromGroup(short group)
{
	std::string filename = "csv/group";
	filename += group + '0';
	filename += ".csv";
	return filename;
}

std::vector<std::string> parseStudentString(std::string line)
{
	size_t lineIndex = 0;
	std::vector<std::string> student = { "","","" };

	for (size_t i = 0; i < 3; i++)
	{
		while (line[lineIndex] != ',' && line[lineIndex] != '\0') {
			student[i] += line[lineIndex];
			lineIndex++;
		}
		//student[i] += '\0';
		lineIndex++;
	}

	return student;
}

std::vector<std::vector<std::string>> getStudentsFromGroup(short group)
{
	// Each student is a string vector
	// [0] - Name, [1] - FN, [2] Subjects composite string

	const std::string filename = getFilenameFromGroup(group);
	std::vector<std::vector<std::string>> students;

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

	return students;
}

void insertStudentInGroup(short group)
{
	const std::string filename = getFilenameFromGroup(group);

	/*std::ifstream file(filename);
	if (!file.is_open()) {
		handleFileNotOpenError();
	}*/
}

void removeStudentFromGroup(short group, const std::string fn)
{
	const std::string filename = getFilenameFromGroup(group);

	const std::vector<std::vector<std::string>> students = getStudentsFromGroup(group);

	std::ofstream file(filename);
	if (!file.is_open()) {
		handleFileNotOpenError();
	}

	std::string fileOutputString = "Name,Fn,Courses\n";

	const size_t studentsCount = students.size();
	for (size_t i = 0; i < studentsCount; i++)
	{
		std::vector<std::string> student = students[i];
		if (fn == student[1])
		{
			continue;
		}

		fileOutputString += student[0] + "," + student[1] + "," + student[2];

		if (i != studentsCount - 1)
		{
			fileOutputString += "\n";
		}

	}
	file << fileOutputString;

	file.close();
}

void printStudentsInGroup(const short group)
{
	const std::vector<std::vector<std::string>> students = getStudentsFromGroup(group);

	std::cout << "Name\t\t\tFN\t\tSubjects&Grades\n";
	for (std::vector<std::string> student : students)
	{
		std::cout << student[0] << "\t" << student[1] << "\t" << student[2] << "\n";
	}
}

void handleInsertStudentInGroup()
{
	const short group = handleInputGroup();
	insertStudentInGroup(group);
}

void handleRemoveStudentFromGroup()
{
	const short group = handleInputGroup();
	std::string fn;

	std::cout << "Please input the faculty number of the student\n";
	std::cin >> fn;
	removeStudentFromGroup(group, fn);
}

void handlePrintStudentsInGroup()
{
	const short group = handleInputGroup();
	printStudentsInGroup(group);
}

void handleSortStudentGroup()
{

}

void handlePrintSortedStudentGroups()
{

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
			handleSortStudentGroup();
			break;
		case 5:
			handlePrintSortedStudentGroups();
			break;
		case 6:
			return 0; // Exit program
		default:
			std::cout << "Please choose an option between 1 and 6.\n";

		}

		//system("cls"); // Clear console output for windows
		printOptions();
	} while (actionNumber != 6);

	return 0;
}

