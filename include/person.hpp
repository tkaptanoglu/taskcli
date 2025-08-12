#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector>
#include "task.hpp"
#include "print_options.hpp"

class Person {
public:
    Person(const std::string& name);
    ~Person();

    const std::string get_name() const;
    void set_name(const std::string& new_name);

    void remove_all_tasks();
    void remove_task(Task* task);
    void print_all_tasks(const PrintOptions& options = PrintOptions()) const;
    void set_all_tasks_to_done();
    int return_number_of_tasks(const PrintOptions& options = PrintOptions()) const;
    void assign_task(Task* task);
    std::vector<Task*> get_tasks() const;
private:
    std::string name;
    std::vector<Task*> tasks;
};

#endif