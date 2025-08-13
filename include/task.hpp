#ifndef TASK_HPP
#define TASK_HPP

#include <memory>
#include <string>
#include <vector>

class Person;

class Task {
public:
    enum class Status {
        Todo,
        InProgress,
        Blocked,
        Cancelled,
        Done
    };

    Task(int id, const std::string& name, const std::string& description, Person* owner = nullptr, Status status = Status::Todo, Task* parent = nullptr)
        : id(id), name(name), description(description), owner(owner), status(status), parent(parent), level(1) {}
    ~Task() = default;

    const std::string get_name() const;
    void set_name(const std::string& name);

    const std::string get_description() const;
    void set_description(const std::string& description);

    int get_id() const;

    void set_level(int level);
    int get_level() const;

    Person* get_owner() const;
    void set_owner(Person* person = nullptr);
    void unown();

    void set_status(Status status);
    Status get_status() const;
    int advance_status();
    void mark_as_done();
    bool is_done() const;

    void set_parent(Task* parent);
    Task* get_parent() const;
    void add_child(Task* child);
    const std::vector<Task*> get_children() const;
    
private:
    int id, level;
    std::string name, description;
    Person* owner;
    Status status;
    Task* parent;
    std::vector<Task*> children;
};

#endif // TASK_HPP