#include <iostream>
#include <string>

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
    std::cout << "  list      List all people\n";
    std::cout << "  rename     Rename a person\n";
    std::cout << "  delete     Delete a person\n";
}

int handle_task_command(int argc, char* argv[]) {
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

int handle_person_command(int argc, char* argv[]) {
    if (argc < 1) {
        std::cerr << "Error: No command provided for 'person'. Use --help for usage.\n";
        return 1;
    }
    std::string command = argv[1];
    if (command == "--help") {
        print_person_help();
        return 0;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Error: No arguments provided. Use --help for usage.\n";
        return 1;
    }

    std::string first_arg = argv[1];
    if (first_arg == "--help") {
        print_help();
        return 0;
    } else if (first_arg == "task") {
        return handle_task_command(argc - 1, argv + 1);
    } else if (first_arg == "person") {
        return handle_person_command(argc - 1, argv + 1);
    }   

    std::cerr << "Error: Unknown argument '" << first_arg << "'. Use --help for usage.\n";
    return 1;
}
