#include "task_manager.hpp"
#include "person.hpp"

#include <iostream>

void TaskManager::delete_all_tasks() {
    tasks.clear();
}

void TaskManager::print_all_tasks(const PrintOptions& options) const {
    for (const auto& task : tasks) {
        if (!task->get_parent()) {
            print_task(task.get(), options);
        }
    }
}

void TaskManager::print_task(int id, const PrintOptions& options) const {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            print_task(task.get(), options);
            return;
        }
    }
}

void TaskManager::print_task(Task* task, const PrintOptions& options) const {
    if (!task) return;
    int task_level = task->get_level();
    for (int i = 0; i < task_level - 1; ++i)
        std::cout << "--"; // Indentation for task level
    std::cout << "Task ID: " << task->get_id() << "\n";
    for (int i = 0; i < task_level - 1; ++i)
        std::cout << "--"; // Indentation for task level
    std::cout << "Name: " << task->get_name() << "\n";
    for (int i = 0; i < task_level - 1; ++i)
        std::cout << "--"; // Indentation for task level
    std::cout << "Status: " << static_cast<int>(task->get_status()) << "\n";
    if (options.verbose) {
        for (int i = 0; i < task_level - 1; ++i)
            std::cout << "--"; // Indentation for task level
        std::cout << "Description: " << task->get_description() << "\n";
        Person* owner = task->get_owner();
        if (owner) {
            std::cout << "Owner: " << owner->get_name() << "\n";
        } else {
            std::cout << "Owner: None\n";
        }
    }
    if (options.nested) {
        for (const auto& child : task->get_children()) {
            print_task(child.get(), options);
        }
    }
}

int TaskManager::create_task(const std::string& name, const std::string& description, Person* owner, Task* parent) {
    return -1; // Placeholder for task creation logic
}

int TaskManager::delete_task(int id) {
    return -1; // Placeholder for task deletion logic
}

void TaskManager::assign_task(int id, Person* person) {

}

void TaskManager::unown_task(int id) {
}

void TaskManager::unown_all_tasks() {
    for (auto& task : tasks) {
        task->unown();
    }
}

void TaskManager::set_task_name(int id, const std::string& name) {
}

std::string TaskManager::get_task_name(int id) const {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            return task->get_name();
        }
    }
    return {};
}

void TaskManager::set_task_description(int id, const std::string& description) {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            task->set_description(description);
            return;
        }
    }
}

std::string TaskManager::get_task_description(int id) const {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            return task->get_description();
        }
    }
    return {};
}

int TaskManager::advance_task_status(int id) {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            return task->advance_status();
        }
    }
    return -1; // Task not found
}

int TaskManager::mark_task_as_done(int id) {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            task->mark_as_done();
            return 0; // Success
        }
    }
    return -1; // Task not found
}

void TaskManager::make_child_task(int parent_id, int child_id) {
    
}

void TaskManager::print_all_task_owners(const PrintOptions& options) const {
    for (const auto& task : tasks) {
        Person* owner = task->get_owner();
        if (owner) {
            std::cout << "Task ID: " << task->get_id() << ", Owner: " << owner->get_name() << std::endl;
        } else {
            std::cout << "Task ID: " << task->get_id() << " has no owner." << std::endl;
        }
    }
}
