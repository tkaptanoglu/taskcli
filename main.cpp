#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <span>
#include <algorithm>

#include "task_manager.hpp"
#include "person_manager.hpp"
#include "print_options.hpp"

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
    std::cout << "  help       Show this help message\n";
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
    std::cout << "  add <name>                                   Add a new person\n";
    std::cout << "  list [-v:verbose]                            List all people\n";
    std::cout << "  rename <old-name> <new-name>                 Rename a person\n";
    std::cout << "  delete <name>                                Delete a person\n";
    std::cout << "  delete-all                                   Delete all people\n";
    std::cout << "  delete-tasks                                 Delete all tasks assigned to a person\n";
    std::cout << "  assign-task <name> <task-id>                 Assign a task to a person\n";
    std::cout << "  set-all-tasks-done <name>                    Mark all tasks of a person as done\n";
    std::cout << "  list-one <name> [-v:verbose]                 List the details of one person";
    std::cout << "  list-tasks <name> [-v:verbose] [-n:nested]   List all tasks of a person\n";
    std::cout << "  list-tasks-count <name>                      List the count of tasks of a person\n";
}


int handle_task_command(std::span<const std::string> args) {
    if (args.empty()) {
        std::cerr << "Error: No command provided for 'task'. Use --help for usage.\n";
        return 1;
    }
    std::string command = args[0];
    if (command == "help") {
        print_task_help();
        return 0;
    }

    return 0;
}

int handle_person_command(std::span<const std::string> args) {
    if (args.empty()) {
        std::cerr << "Error: No command provided for 'person'. Use --help for usage.\n";
        return 1;
    }
    std::string command = args[0];
    if (command == "help") {
        print_person_help();
        return 0;
    }
    
    if (command == "add") {
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'add'. Use --help for usage.\n";
            return 1;
        }
        std::cout << "Adding a new person...\n";
        std::string name = args[1];
        if (name.empty()) {
            std::cerr << "Error: Name cannot be empty.\n";
            return 1;
        }
        get_person_manager().add_person(name); // Add person using PersonManager
        std::cout << "Person '" << name << "' added successfully.\n";
    } else if (command == "list") {
        PrintOptions options;
        options.verbose = std::find(args.begin(), args.end(), "-v") != args.end();
        std::cout << "Listing all people...\n";
        get_person_manager().print_all_people(options); // Print all people using PersonManager
    } else if (command == "rename") {
        if (args.size() < 4) {
            std::cerr << "Error: Not enough arguments for 'rename'. Use --help for usage.\n";
            return 1;
        }
        std::string old_name = args[2];
        std::string new_name = args[3];
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
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'delete'. Use --help for usage.\n";
            return 1;
        }
        std::string name = args[1];
        if (get_person_manager().delete_person(name)) {
            std::cout << "Person '" << name << "' deleted successfully.\n";
        } else {
            std::cerr << "Error: Failed to delete person '" << name << "'.\n";
        }
    } else if (command == "delete-all") {
        get_person_manager().delete_all_people();
        std::cout << "All people deleted successfully.\n";
    } else if (command == "delete-tasks") {
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'delete-tasks'. Use --help for usage.\n";
            return 1;
        }
        std::string name = args[1];
        get_person_manager().delete_persons_all_tasks(name);
    } else if (command == "assign-task") {
        if (args.size() < 3) {
            std::cerr << "Error: Not enough arguments for 'assign-task'. Use --help for usage.\n";
            return 1;
        }
        std::string name = args[1];
        int task_id = std::stoi(args[2]);
        Task* task = get_task_manager().get_task(task_id);
        if (task == nullptr) {
            std::cerr << "Error: Task with ID " << task_id << " not found.\n";
            return 1;
        }
        get_person_manager().assign_task(name, task);
    } else if (command == "set-all-tasks-done") {
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'set-all-tasks-done'. Use --help for usage.\n";
            return 1;
        }
        std::string name = args[1];
        get_person_manager().set_persons_all_tasks_as_done(name);
    } else if (command == "list-one") {
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'list-one'. Use --help for usage.\n";
            return 1;
        }
        PrintOptions options;
        options.verbose = std::find(args.begin(), args.end(), "-v") != args.end();
        std::string name = args[1];
        get_person_manager().print_person(name, options);
    } else if (command == "list-tasks") {
        if (args.size() < 2) {
            std::cerr << "Error: Not enough arguments for 'list-tasks'. Use --help for usage.\n";
            return 1;
        }
        PrintOptions options;
        options.verbose = std::find(args.begin(), args.end(), "-v") != args.end();
        options.nested = std::find(args.begin(), args.end(), "-n") != args.end();
        std::string name = args[1];
        get_person_manager().print_persons_tasks(name, options);
    } else if (command == "list-tasks-count") {
        bool nested = std::find(args.begin(), args.end(), "-n") != args.end();
        get_person_manager().print_all_peoples_task_counts(nested);
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
            handle_task_command(args);
        } else if (command == "person") {
            handle_person_command(args);
        } else {
            std::cerr << "Error: Unknown command '" << command << "'. Type 'help' for usage instructions.\n";
        }
    }

    std::cout << "Goodbye.\n";
    return 0;
}
