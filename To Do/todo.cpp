#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void readFile(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "File can not be opened. You can create a new file!\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
}

void writeFile(const std::string& filename, std::vector<std::string>& lines) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "File can not be wrote!\n";
        return;
    }
    for (const auto& line : lines) {
        file << line << '\n';
    }
    file.close();
    std::cout << "Changes saved!\n";
}

void editFile(std::vector<std::string>& lines) {
    int choose;
    std::string newTask;

    do {
        std::cout << "\n1. Show all tasks\n";
        std::cout << "2. Add a new task\n";
        std::cout << "3. Edit a task\n";
        std::cout << "4. Delete a task\n";
        std::cout << "0. Quit\n\n";
        std::cout << "Choose: ";
        std::cin >> choose;
        std::cin.ignore();

        switch (choose) {
            case 1:
                if (lines.size() == 0) {
                    std::cout << "\nNo Tasks Found!\n";
                }
                else {
                    std::cout << "\nTasks:\n";
                    
                    for (size_t i = 0; i < lines.size(); i++) {
                        std::cout << i + 1 << ". " << lines[i] << "\n";
                    }
                }
                
                break;
            case 2:
                std::cout << "\nWrite the task: ";
                std::getline(std::cin, newTask);
                lines.push_back(newTask);
                std::cout << "\nTask added!\n";
                break;
            case 3:
                std::cout << "\nWrite the task number you want to edit: ";
                size_t lineNo;
                std::cin >> lineNo;
                std::cin.ignore();
                if (lineNo > 0 && lineNo <= lines.size()) {
                    std::cout << "Write new task: ";
                    std::getline(std::cin, lines[lineNo - 1]);
                }
                else {
                    std::cout << "Invalid task number!\n";
                }
                break;
            case 4:
                std::cout << "\nWrite the task number you want to delete: ";
                std::cin >> lineNo;
                std::cin.ignore();
                if (lineNo > 0 && lineNo <= lines.size()) {
                    lines.erase(lines.begin() + lineNo - 1);
                    std::cout << "Task removed.\n";
                }
                else {
                    std::cout << "Invalid task number!\n";
                }
                break;
            case 0:
                std::cout << "Editing done!\n";
                break;
            default:
                std::cout << "Invalid selection!\n";
                break;
        }
    }
    while (choose != 0);
}

int main() {
    const std::string filename = "notes.txt";
    std::vector<std::string> lines;

    readFile(filename, lines);

    std::cout << "Welcome to To Do App!\n";
    
    editFile(lines);

    writeFile(filename, lines);

    return 0;
}