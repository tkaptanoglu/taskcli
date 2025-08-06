#include "person.hpp"
#include "task.hpp"

Person::Person(const std::string& name) : name(name) {}

Person::~Person() {
    remove_all_tasks();
}

std::string Person::get_name() const {
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

void Person::print_all_tasks(const PrintOptions& options) const {
   
}

void Person::set_all_tasks_to_done() {
    for (Task* task : tasks) {
        task->mark_as_done();
    }
}

int Person::return_number_of_tasks(const TaskOptions& options) const {
    int count = 0;
    for (const Task* task : tasks) {
        if (options.include_done && task->is_done()) {
            count++;
        }
        if (options.nested && task->get_parent()) {
            count++;
        }
    }
    return count;
}

void Person::assign_task(Task* task) {
    tasks.push_back(task);
}
