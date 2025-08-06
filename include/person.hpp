#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <vector>
#include "task.hpp"

struct PrintOptions {
    bool verbose = false;
    bool nested = false;
};
struct TaskOptions {
    bool include_done = false;
    bool nested = true;
};
class Person {
public:
    Person(const std::string& name);
    ~Person();

    std::string get_name() const;
    void set_name(const std::string& new_name);

    void remove_all_tasks();
    void print_all_tasks(const PrintOptions& options = PrintOptions()) const;
    void set_all_tasks_to_done();
    int return_number_of_tasks(const TaskOptions& options = TaskOptions()) const;
    void assign_task(Task* task);
private:
    std::string name;
    std::vector<Task*> tasks;
};

#endif