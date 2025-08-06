#include "person_manager.hpp"
#include "person.hpp"
#include "task.hpp"

#include <iostream>
#include <algorithm>

// PersonManager class implementation
PersonManager::PersonManager() = default;  
PersonManager::~PersonManager() {
    delete_all_people();
}

std::unique_ptr<Person> PersonManager::find_person_by_name(const std::string& name) const {
    for (const auto& person : people) {
        if (person->get_name() == name) {
            return std::make_unique<Person>(*person);
        }
    }
    return nullptr;
}

// CRUD operations

// Change a person's name
int PersonManager::change_name(const std::string& old_name, const std::string& new_name) {
    for (const auto& person : people) {
        if (person->get_name() == old_name) {
            person->set_name(new_name);
            return 0;  // Success
        }
    }
    return -1;  // Person not found
}

// Delete all people
void PersonManager::delete_all_people() {
    for (auto& person : people) {
        person.reset();  // Automatically calls Person's destructor
    }
    people.clear();
}

// Add a new person
void PersonManager::add_person(const std::string& name) {
    people.emplace_back(std::make_unique<Person>(name));
}

// Delete a person's all tasks
int PersonManager::delete_persons_all_tasks(const std::string& name) {
    auto person = find_person_by_name(name);
    if (person) {
        person->remove_all_tasks();
        return 0;  // Success
    }
    return -1;  // Person not found
}

// Set a person's all tasks as done
int PersonManager::set_persons_all_tasks_as_done(const std::string& name) {
    auto person = find_person_by_name(name);
    if (person) {
        person->set_all_tasks_to_done();
        return 0;  // Success
    }
    return -1;  // Person not found
}

// Assign a task to a person
void PersonManager::assign_task(Person* person, Task* task) {
    if (person) {
        person->assign_task(task);
    } else {
        std::cerr << "Error: Person is null." << std::endl;
    }
}

// Print operations

// Print all people
void PersonManager::print_all_people() const {
    for (const auto& person : people) {
        std::cout << person->get_name() << std::endl;
    }
}

// Print a specific person
void PersonManager::print_person(const std::string& name) const {
    auto person = find_person_by_name(name);
    if (person) {
        std::cout << person->get_name() << std::endl;
        return;
    }
    std::cerr << "Error: Person '" << name << "' not found." << std::endl;
}

// Print a person's tasks
void PersonManager::print_persons_tasks(const std::string& name, bool nested) const {
    auto person = find_person_by_name(name);
    if (person) {
        for(const auto& task : person->get_tasks()) {
            std::cout << " - " << task->get_name() << std::endl; // Extend later with nested printing
        }
        return;
    }
    std::cerr << "Error: Person '" << name << "' not found." << std::endl;
}

// Print all people's task counts
void PersonManager::print_all_peoples_task_counts(bool nested) const {
    for (const auto& person : people) {
        std::cout << person->get_name() << ": " << person->return_number_of_tasks(TaskOptions{true, nested}) << std::endl;
    }
}

// Delete a person by name
int PersonManager::delete_person(const std::string& name) {
    auto it = std::remove_if(people.begin(), people.end(),
        [&name](const std::unique_ptr<Person>& person) {
            return person->get_name() == name;
        });
    if (it != people.end()) {
        people.erase(it, people.end());
        return 0;  // Success
    }
    return -1;  // Person not found
}