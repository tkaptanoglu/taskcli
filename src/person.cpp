#include "person.hpp"
#include "task.hpp"

#include <algorithm>
#include <iostream>

Person::Person(const std::string& name) : name(name) {}

Person::~Person() {
    remove_all_tasks();
}

const std::string Person::get_name() const {
    return name;
}

void Person::set_name(const std::string& new_name) {
    name = new_name;
}

void Person::remove_all_tasks() {
    for (Task* task : tasks) {
        delete task;
    }
    tasks.clear();
}

void Person::remove_task(Task* task) {
    auto it = std::find(tasks.begin(), tasks.end(), task);
    if (it != tasks.end()) {
        tasks.erase(it);
    }
}

void Person::set_all_tasks_to_done() {
    for (Task* task : tasks) {
        task->mark_as_done();
    }
}

int Person::return_number_of_tasks(const PrintOptions& options) const {
    int count = 0;
    for (const Task* task : tasks) {
        if (options.nested || !task->get_parent()) {
            count++;
        }
    }
    return count;
}

void Person::assign_task(Task* task) {
    tasks.push_back(task);
}

std::vector<Task*> Person::get_tasks() const {
    return tasks;
}

void Person::print_all_tasks(const PrintOptions& options) const {
    for (const Task* task : tasks) {
        if (options.verbose) {
            std::cout << "Task ID: " << task->get_id() << ", Name: " << task->get_name() << std::endl;
        }
        if (options.nested || !task->get_parent()) {
            std::cout << " - " << task->get_name() << std::endl;
        }
    }
}