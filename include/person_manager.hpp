#ifndef PERSON_MANAGER_HPP
#define PERSON_MANAGER_HPP

#include <string>
#include <memory>
#include <vector>
#include "person.hpp"
#include "task.hpp"

class PersonManager {
public:
    PersonManager();
    ~PersonManager();

    // CRUD operations
    int change_name(const std::string& old_name, const std::string& new_name);

    void delete_all_people();

    int delete_persons_all_tasks(const std::string& name);
    int set_persons_all_tasks_as_done(const std::string& name);
    void assign_task(const Task& task);

    // Print operations
    void print_all_people() const;
    void print_person(const std::string& name) const;
    void print_persons_tasks(const std::string& name, bool nested) const;
    void print_all_peoples_task_counts(bool nested) const;
private:
    std::vector<std::unique_ptr<Person>> people;
};

#endif