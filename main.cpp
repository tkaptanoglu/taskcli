#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "task_manager.hpp"
#include "person_manager.hpp"

// Singleton accessor for TaskManager
TaskManager& get_task_manager() {
    static TaskManager instance;
    return instance;
}

// Singleton accessor for PersonManager
PersonManager& get_person_manager() {
    static PersonManager instance;
    return instance;
}

void print_help() {
    std::cout << "taskcli - Task Manager CLI Tool\n\n";
    std::cout << "Usage:\n";
    std::cout << "  taskcli <entity> <command> [options]\n\n";
    std::cout << "Entities:\n";
    std::cout << "  task       Manage tasks (add, list, delete, etc.)\n";
    std::cout << "  person     Manage people (add, list, rename, etc.)\n\n";
    std::cout << "Global Options:\n";
    std::cout << "  --help     Show this help message\n";
}

void print_task_help() {
    std::cout << "taskcli task - Manage tasks\n\n";
    std::cout << "Usage:\n";
    std::cout << "  taskcli task <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  add        Add a new task\n";
    std::cout << "  list      List all tasks\n";
    std::cout << "  delete     Delete a task\n";
    std::cout << "  complete   Mark a task as complete\n";
}

void print_person_help() {
    std::cout << "taskcli person - Manage people\n\n";
    std::cout << "Usage:\n";
    std::cout << "  taskcli person <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  add        Add a new person\n";
    std::cout << "  list       List all people\n";
    std::cout << "  rename     Rename a person\n";
    std::cout << "  delete     Delete a person\n";
}


int handle_task_command(int argc, const char* argv[]) {
    if (argc < 1) {
        std::cerr << "Error: No command provided for 'task'. Use --help for usage.\n";
        return 1;
    }
    std::string command = argv[1];
    if (command == "--help") {
        print_task_help();
        return 0;
    }

    return 0;
}

int handle_person_command(int argc, const char* argv[]) {
    if (argc < 1) {
        std::cerr << "Error: No command provided for 'person'. Use --help for usage.\n";
        return 1;
    }
    std::string command = argv[1];
    if (command == "--help") {
        print_person_help();
        return 0;
    }
    
    if (command == "add") {
        std::cout << "Adding a new person...\n";
        std::string name = argv[2];
        if (name.empty()) {
            std::cerr << "Error: Name cannot be empty.\n";
            return 1;
        }
        get_person_manager().add_person(name); // Add person using PersonManager
        std::cout << "Person '" << name << "' added successfully.\n";
    } else if (command == "list") {
        std::cout << "Listing all people...\n";
        get_person_manager().print_all_people(); // Print all people using PersonManager
    } else if (command == "rename") {
        if (argc < 4) {
            std::cerr << "Error: Not enough arguments for 'rename'. Use --help for usage.\n";
            return 1;
        }
        std::string old_name = argv[2];
        std::string new_name = argv[3];
        if (new_name.empty()) {
            std::cerr << "Error: New name cannot be empty.\n";
            return 1;
        }
        if (get_person_manager().change_name(old_name, new_name)) {
            std::cout << "Person '" << old_name << "' renamed to '" << new_name << "' successfully.\n";
        } else {
            std::cerr << "Error: Failed to rename person '" << old_name << "'.\n";
        }
    } else if (command == "delete") {
        if (argc < 3) {
            std::cerr << "Error: Not enough arguments for 'delete'. Use --help for usage.\n";
            return 1;
        }
        std::string name = argv[2];
        if (get_person_manager().delete_person(name)) {
            std::cout << "Person '" << name << "' deleted successfully.\n";
        } else {
            std::cerr << "Error: Failed to delete person '" << name << "'.\n";
        }
    }

    return 0;
}

int main() {
    std::string line;

    std::cout << "Welcome to the Task Manager CLI Tool!\n";
    std::cout << "Type 'help' for usage instructions.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line.empty()) continue;

        // Exit command
        if (line == "exit" || line == "quit") {
            std::cout << "Exiting the Task Manager CLI Tool. Goodbye!\n";
            break;
        }

         // Parse and handle command
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        // Collect remaining tokens into a vector
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        std::vector<const char*> argv_like;
        for (const auto& a : args) {
            argv_like.push_back(a.c_str());
        }
        int argc = argv_like.size(); // +1 for the command itself

        if (command == "help") {
            print_help();
            continue;
        }
        if (command == "task") {
            handle_task_command(argc, argv_like.data());
        } else if (command == "person") {
            handle_person_command(argc, argv_like.data());
        } else {
            std::cerr << "Error: Unknown command '" << command << "'. Type 'help' for usage instructions.\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
