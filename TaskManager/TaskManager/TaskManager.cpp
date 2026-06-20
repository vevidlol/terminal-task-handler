#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

// ============================================================================
// TASK CLASS: Encapsulation and core attributes
// ============================================================================
class Task {
private:
    int id;
    std::string description;
    bool isCompleted;

public:
    // Constructor
    Task(int id, const std::string& description, bool isCompleted = false)
        : id(id), description(description), isCompleted(isCompleted) {
    }

    // Getters displaying clean encapsulation
    int getId() const { return id; }
    std::string getDescription() const { return description; }
    bool getIsCompleted() const { return isCompleted; }

    // State modifier
    void markComplete() {
        isCompleted = true;
    }
};

// ============================================================================
// TASKMANAGER CLASS: Operations, vector storage, and File I/O persistence
// ============================================================================
class TaskManager {
private:
    std::vector<Task> tasks;
    int nextId;
    const std::string filename = "tasks.txt";

    // Saves tasks instantly to flat file tasks.txt with safe '|' boundaries
    void saveToFile() const {
        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return;
        }
        for (const auto& task : tasks) {
            outFile << task.getId() << "|"
                << (task.getIsCompleted() ? "1" : "0") << "|"
                << task.getDescription() << "\n";
        }
    }

    // Loads tasks from tasks.txt on manager initiation and syncs auto-increment ID
    void loadFromFile() {
        std::ifstream inFile(filename);
        if (!inFile) {
            // File does not exist yet; gracefully fallback to default state
            return;
        }
        std::string line;
        int maxId = 0;
        tasks.clear();

        while (std::getline(inFile, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string idStr, completedStr, desc;

            // Safely parse data delimited by the pipe operator
            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, completedStr, '|') &&
                std::getline(ss, desc)) {
                try {
                    int id = std::stoi(idStr);
                    bool completed = (completedStr == "1");
                    tasks.emplace_back(id, desc, completed);
                    if (id > maxId) {
                        maxId = id;
                    }
                }
                catch (...) {
                    // Ignore malformed rows of data to prevent system crashes
                }
            }
        }
        nextId = maxId + 1;
    }

public:
    // Constructor initiating local file reading
    TaskManager() : nextId(1) {
        loadFromFile();
    }

    // Creates task with empty input validation
    bool addTask(const std::string& description) {
        std::string trimmed = description;

        // Trim leading and trailing whitespace
        trimmed.erase(0, trimmed.find_first_not_of(" \t\r\n"));
        trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);

        if (trimmed.empty()) {
            std::cout << "\nError: Task description cannot be empty!\n";
            return false;
        }

        Task newTask(nextId++, trimmed);
        tasks.push_back(newTask);
        saveToFile();

        std::cout << "\nTask added successfully with ID: " << newTask.getId() << "\n";
        return true;
    }

    // Prints all tasks in formatted tabular grid using std::iomanip manipulators
    void viewTasks() const {
        if (tasks.empty()) {
            std::cout << "\n=============================================\n";
            std::cout << "                 NO TASKS FOUND              \n";
            std::cout << "=============================================\n";
            return;
        }

        // Draw header grid
        std::cout << "\n" << std::setfill('=') << std::setw(60) << "" << "\n";
        std::cout << std::setfill(' ');
        std::cout << " " << std::left << std::setw(8) << "ID"
            << " | " << std::left << std::setw(35) << "Description"
            << " | " << std::left << std::setw(10) << "Status" << "\n";
        std::cout << std::setfill('-') << std::setw(60) << "" << "\n";
        std::cout << std::setfill(' ');

        // Draw each grid list row
        for (const auto& task : tasks) {
            std::string status = task.getIsCompleted() ? "[Completed]" : "[Pending]";
            std::string displayDesc = task.getDescription();

            // Truncate long strings elegantly with ellipsis to preserve column boundaries
            if (displayDesc.length() > 32) {
                displayDesc = displayDesc.substr(0, 29) + "...";
            }

            std::cout << " " << std::left << std::setw(8) << task.getId()
                << " | " << std::left << std::setw(35) << displayDesc
                << " | " << std::left << std::setw(10) << status << "\n";
        }
        std::cout << std::setfill('=') << std::setw(60) << "" << "\n";
        std::cout << std::setfill(' '); // Reset padding stream filler
    }

    // Marks task as completed after locating ID in tasks vector
    bool completeTask(int id) {
        for (auto& task : tasks) {
            if (task.getId() == id) {
                if (task.getIsCompleted()) {
                    std::cout << "\nTask with ID " << id << " is already completed!\n";
                    return false;
                }
                task.markComplete();
                saveToFile();
                std::cout << "\nTask ID " << id << " marked as COMPLETED!\n";
                return true;
            }
        }
        std::cout << "\nError: Task with ID " << id << " not found!\n";
        return false;
    }

    // Deletes task from list via standard robust STL iterator erasing
    bool deleteTask(int id) {
        auto it = std::remove_if(tasks.begin(), tasks.end(), [id](const Task& t) {
            return t.getId() == id;
            });

        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            saveToFile();
            std::cout << "\nTask ID " << id << " deleted successfully!\n";
            return true;
        }
        std::cout << "\nError: Task with ID " << id << " not found!\n";
        return false;
    }
};

// ============================================================================
// HELPER: Prints program title and navigation menu options
// ============================================================================
void displayMainMenu() {
    std::cout << "\n=============================================\n";
    std::cout << "      C++ COMMAND-LINE TASK MANAGER v1.0     \n";
    std::cout << "=============================================\n";
    std::cout << "  1. Add New Task                            \n";
    std::cout << "  2. View Active & Completed Tasks           \n";
    std::cout << "  3. Mark Task as Completed                  \n";
    std::cout << "  4. Delete Task                             \n";
    std::cout << "  5. Exit Program                            \n";
    std::cout << "=============================================\n";
    std::cout << "Enter your choice (1-5): ";
}

// ============================================================================
// MAIN RUNTIME ENVIRONMENT: Infinite navigation loop with stream validation
// ============================================================================
int main() {
    TaskManager manager;
    int choice = 0;

    while (true) {
        displayMainMenu();

        // Handles alphabetic strings gracefully prevents infinite output loops
        if (!(std::cin >> choice)) {
            std::cin.clear(); // Clears standard error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Flushes stream input buffer
            std::cout << "\nInvalid choice! Please enter a number between 1 and 5.\n";
            continue;
        }

        // Clears any remnants in buffer up to newline to safeguard getline runs
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string desc;
            std::cout << "Enter task description: ";
            std::getline(std::cin, desc);
            manager.addTask(desc);
        }
        else if (choice == 2) {
            manager.viewTasks();
        }
        else if (choice == 3) {
            int targetId;
            std::cout << "Enter Task ID to mark complete: ";
            if (!(std::cin >> targetId)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid ID! Must be a numeric value.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            manager.completeTask(targetId);
        }
        else if (choice == 4) {
            int targetId;
            std::cout << "Enter Task ID to delete: ";
            if (!(std::cin >> targetId)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nInvalid ID! Must be a numeric value.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            manager.deleteTask(targetId);
        }
        else if (choice == 5) {
            std::cout << "\nThank you for using C++ CLI Task Manager. Goodbye!\n\n";
            break;
        }
        else {
            std::cout << "\nInvalid input! Choose an option between 1 and 5.\n";
        }
    }
    return 0;
}
