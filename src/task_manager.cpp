#include "task_manager.hpp"
#include "person.hpp"

#include <iostream>
#include <algorithm>

void TaskManager::delete_all_tasks() {
    tasks.clear();
}

Task* TaskManager::find_task_by_id(int id) const {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            return task.get();
        }
    }
    return nullptr;
}

void TaskManager::print_all_tasks(const PrintOptions& options) const {
    for (const auto& task : tasks) {
        if (!task->get_parent()) {
            print_task(task.get(), options);
        }
    }
}

void TaskManager::print_task(int id, const PrintOptions& options) const {
    Task* task = find_task_by_id(id);
    if (!task) {
        std::cerr << "Task with ID " << id << " not found." << std::endl;
        return;
    }
    print_task(task, options);
}

void TaskManager::print_task(Task* task, const PrintOptions& options) const {
    if (!task) return;

    int task_level = task->get_level();
    
    print_line_indentations(task_level);
    std::cout << "Task ID: " << task->get_id() << "\n";

    print_line_indentations(task_level);
    std::cout << "Name: " << task->get_name() << "\n";
    
    print_line_indentations(task_level);
    std::cout << "Status: " << static_cast<int>(task->get_status()) << "\n";
    if (options.verbose) {
        print_line_indentations(task_level);
        std::cout << "Description: " << task->get_description() << "\n";
        Person* owner = task->get_owner();
        if (owner) {
            std::cout << "Owner: " << owner->get_name() << "\n";
        } else {
            std::cout << "Owner: None\n";
        }
    }
    if (options.nested && !task->get_parent()) {
        for (const auto& child : task->get_children()) {
            print_task(child, options);
        }
    }
}

void TaskManager::print_line_indentations(int level) const {
    for (int i = 0; i < level - 1; ++i) {
        std::cout << "-"; // Indentation for task level
    }
}

int TaskManager::create_task(const std::string& name, const std::string& description, Person* owner) {
    int new_id = tasks.size() + 1; // Simple ID generation
    auto new_task = std::make_unique<Task>(new_id, name, description, owner);
    tasks.push_back(std::move(new_task));
    return new_id;
}

int TaskManager::delete_task(int id) {
    Task* task = find_task_by_id(id);
    if (!task) {
        std::cerr << "Task with ID " << id << " not found." << std::endl;
        return -1; // Task not found
    }
    tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
        [task](const std::unique_ptr<Task>& t) { return t.get() == task; }), tasks.end());
    return 0; // Success
}

void TaskManager::assign_task(int id, Person* person) {
    Task* task = find_task_by_id(id);
    if (task && person) {
        task->set_owner(person);
        person->assign_task(task);
    } else if (!task) {
        std::cerr << "Task with ID " << id << " not found." << std::endl;
    } else if (!person) {
        std::cerr << "Person not found." << std::endl;  
    }
}

void TaskManager::unown_task(int id) {
    Task* task = find_task_by_id(id);
    if (task) {
        task->unown();
        Person* owner = task->get_owner();
        if (owner) {
            owner->remove_task(task);
        }
    } else {
        std::cerr << "Task with ID " << id << " not found." << std::endl;
    }
}

void TaskManager::unown_all_tasks() {
    for (auto& task : tasks) {
        task->unown();
    }
}

void TaskManager::set_task_name(int id, const std::string& name) {
    Task* task = find_task_by_id(id);
    if (task) {
        task->set_name(name);
    } else {
        std::cerr << "Task with ID " << id << " not found." << std::endl;
    }
}

const std::string TaskManager::get_task_name(int id) const {
    Task* task = find_task_by_id(id);
    if (!task) {
        return "Task with id " + std::to_string(id) + " not found";;
    }
    return task->get_name();
}

void TaskManager::set_task_description(int id, const std::string& description) {
    Task* task = find_task_by_id(id);
    if (task) {
        task->set_description(description);
    }
}

const std::string TaskManager::get_task_description(int id) const {
    Task* task = find_task_by_id(id);
    if (!task) {
        return "Task with id " + std::to_string(id) + " not found";
    }
    return task->get_description();
}

int TaskManager::advance_task_status(int id) {
    Task* task = find_task_by_id(id);
    if (task) {
        return task->advance_status();
    }
    return -1; // Task not found
}

int TaskManager::mark_task_as_done(int id) {
    Task* task = find_task_by_id(id);
    if (task) {
        task->mark_as_done();
        return 0; // Success
    }
    return -1; // Task not found
}

void TaskManager::make_child_task(int parent_id, int child_id) {
    Task* parent = find_task_by_id(parent_id);
    Task* child = find_task_by_id(child_id);
    if (parent && child) {
        parent->add_child(child);
        child->set_parent(parent);
    }
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

Task* TaskManager::get_task(int id) const {
    for (const auto& task : tasks) {
        if (task->get_id() == id) {
            return task.get();
        }
    }
    return nullptr;
}
