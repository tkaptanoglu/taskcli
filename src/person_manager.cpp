#include "person_manager.hpp"
#include "person.hpp"
#include "task.hpp"

PersonManager::PersonManager() = default;  
PersonManager::~PersonManager() {
    delete_all_people();
}

void PersonManager::delete_all_people() {
    for (auto& person : people) {
        person.reset();  // Automatically calls Person's destructor
    }
    people.clear();
}

int PersonManager::change_name(const std::string& old_name, const std::string& new_name) {
    for (const auto& person : people) {
        if (person->get_name() == old_name) {
            person->set_name(new_name);
            return 0;  // Success
        }
    }
    return -1;  // Person not found
}

int PersonManager::delete_persons_all_tasks(const std::string& name) {
    for (const auto& person : people) {
        if (person->get_name() == name) {
            person->remove_all_tasks();
            return 0;  // Success
        }
    }
    return -1;  // Person not found
}

int PersonManager::set_persons_all_tasks_as_done(const std::string& name) {
    for (const auto& person : people) {
        if (person->get_name() == name) {
            person->set_all_tasks_to_done();
            return 0;  // Success
        }
    }
    return -1;  // Person not found
}