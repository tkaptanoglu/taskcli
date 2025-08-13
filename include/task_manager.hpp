#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <memory>
#include <string>
#include <vector>
#include "task.hpp"
#include "print_options.hpp"

class TaskManager {
public:

    TaskManager() = default;
    ~TaskManager() = default;

    void delete_all_tasks();

    void print_all_tasks(const PrintOptions& options) const;
    void print_task(Task* task, const PrintOptions& options) const;
    void print_task(int id, const PrintOptions& options) const;

    int create_task(const std::string& name, const std::string& description, Person* owner = nullptr);
    int delete_task(int id);

    void assign_task(int id, Person* person);
    void unown_task(int id);
    void unown_all_tasks();

    void set_task_name(int id, const std::string& name);
    const std::string get_task_name(int id) const;

    void set_task_description(int id, const std::string& description);
    const std::string get_task_description(int id) const;

    int advance_task_status(int id);
    int mark_task_as_done(int id);

    void make_child_task(int parent_id, int child_id);

    void print_all_task_owners(const PrintOptions& options) const;

    Task* get_task(int id) const;
private:
    std::vector<std::unique_ptr<Task>> tasks;

    void print_line_indentations(int level) const;
    Task* find_task_by_id(int id) const;
};

#endif
