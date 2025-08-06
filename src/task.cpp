#include "task.hpp"
#include "person.hpp"

std::string Task::get_name() const {
    return name;
}

void Task::set_name(const std::string& name) {
    this->name = name;
}

std::string Task::get_description() const {
    return description;
}

void Task::set_description(const std::string& description) {
    this->description = description;
}

Person* Task::get_owner() const {
    return owner;
}

void Task::set_owner(Person* person) {
    owner = person;
}

int Task::get_id() const {
    return id;
}

int Task::get_level() const {
    return level;
}

void Task::unown() {
    owner = nullptr;
}

void Task::set_status(Status status) {
    this->status = status;
}

Task::Status Task::get_status() const {
    return status;
}

int Task::advance_status() {
    if (status == Status::Todo) {
        status = Status::InProgress;
    } else if (status == Status::InProgress) {
        status = Status::Blocked;
    } else if (status == Status::Blocked) {
        status = Status::Cancelled;
    } else if (status == Status::Cancelled) {
        status = Status::Done;
    }
    return static_cast<int>(status);
}

void Task::mark_as_done() {
    status = Status::Done;
}

bool Task::is_done() const {
    return status == Status::Done;
}

void Task::set_parent(Task* parent) {
    this->parent = parent;
}

Task* Task::get_parent() const {
    return parent;
}

void Task::add_child(Task* child) {
    children.push_back(child);
}

const std::vector<Task*> Task::get_children() const {
    return children;
}
