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

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "Error: No arguments provided. Use --help for usage.\n";
        return 1;
    }

    std::string first_arg = argv[1];
    if (first_arg == "--help") {
        print_help();
        return 0;
    }

    std::cerr << "Error: Unknown argument '" << first_arg << "'. Use --help for usage.\n";
    return 1;
}
