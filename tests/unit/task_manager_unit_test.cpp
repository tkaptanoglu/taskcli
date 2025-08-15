#include <iostream>
#include <string>

// Project headers
#include "task_manager.hpp"
#include "task.hpp"
#include "print_options.hpp"
#include "person.hpp"

// --- Tiny assert helpers (no external frameworks) ---
#define ASSERT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " ASSERT_TRUE(" << #cond << ")\n"; \
        return 1; \
    } \
} while(0)

#define ASSERT_EQ(a,b) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " ASSERT_EQ(" << #a << "," << #b << ") got (" \
                  << (a) << "," << (b) << ")\n"; \
        return 1; \
    } \
} while(0)

// -- Tests --


// Simple task creation and deletion
static int test_create_and_delete_task() {
    TaskManager tm;
    std::string task_name = "Test Task";
    std::string task_description = "This is a test task";

    // Create a task
    int task_id = tm.create_task(task_name, task_description, nullptr);
    ASSERT_TRUE(task_id != 0);

    // Lookup the task
    Task* task = tm.get_task(task_id);
    ASSERT_TRUE(task != nullptr);
    ASSERT_EQ(task->get_name(), task_name);
    ASSERT_EQ(task->get_description(), task_description);

    std::string new_task_name = "Updated Task Name";
    std::string new_task_description = "Updated task description";

    tm.set_task_name(task_id, new_task_name);
    ASSERT_EQ(tm.get_task_name(task_id), new_task_name);

    tm.set_task_description(task_id, new_task_description);
    ASSERT_EQ(tm.get_task_description(task_id), new_task_description);

    // Delete the task
    int delete_result = tm.delete_task(task_id);
    ASSERT_EQ(delete_result, 1);

    // Lookup the task again
    ASSERT_TRUE(tm.get_task(task_id) == nullptr);

    return 0;
}

static int test_get_task_on_empty_manager_returns_null() {
    TaskManager tm;
    ASSERT_TRUE(tm.get_task(0) == nullptr);
    ASSERT_TRUE(tm.get_task(42) == nullptr);
    ASSERT_TRUE(tm.get_task(-7) == nullptr); // negative IDs should also be “not found”
    return 0;
}

// Making a parent/child link with unknown IDs should fail with a non-zero error code.
static int test_make_child_task_fails_when_ids_missing() {
    TaskManager tm;

    int rc1 = tm.make_child_task(1, 2);    // neither exists
    ASSERT_EQ(rc1, 0);

    int rc2 = tm.make_child_task(0, 0);    // same unknown id
    ASSERT_EQ(rc2, 0);

    int rc3 = tm.make_child_task(-1, 99);  // mixed unknown ids
    ASSERT_EQ(rc3, 0);

    tm.create_task("Test Task", "This is a test task", nullptr);
    int rc4 = tm.make_child_task(1, 2);    // now one exists, but the other does not
    ASSERT_EQ(rc4, 0);

    int rc5 = tm.make_child_task(2, 1);    // the other way around now
    ASSERT_EQ(rc5, 0);

    return 0;
}

// Deleting all tasks on an empty manager should be safe and keep it empty.
static int test_delete_all_tasks_is_idempotent_and_safe() {
    TaskManager tm;
    tm.delete_all_tasks(); // should not crash

    // Still empty: any id should be not found
    ASSERT_TRUE(tm.get_task(1) == nullptr);
    ASSERT_TRUE(tm.get_task(123456) == nullptr);

    // Call again to check idempotency
    tm.delete_all_tasks();
    ASSERT_TRUE(tm.get_task(1) == nullptr);
    return 0;
}

// Printing should not throw or crash on empty state (we don't assert output text here).
static int test_print_all_task_owners_does_not_throw() {
    TaskManager tm;
    PrintOptions opts{};
    opts.verbose = true;
    opts.nested  = true;

    // Just ensure this is callable without exceptions.
    tm.print_all_task_owners(opts);
    return 0;
}

static int test_ownership_operations() {
    TaskManager tm;
    std::string person_name = "John Doe";
    Person person(person_name);
    int number_of_tasks = 4;

    // Create a task and assign it to a person
    int task_id = tm.create_task("Test Task", "This is a test task", &person);
    ASSERT_TRUE(task_id != 0);
    ASSERT_EQ(tm.get_task(task_id)->get_owner()->get_name(), person_name);

    // Unassign the task
    tm.unown_task(task_id);
    ASSERT_TRUE(tm.get_task(task_id)->get_owner() == nullptr);

    for (int i = 1; i <= number_of_tasks; ++i) {
        tm.create_task("Task " + std::to_string(i), "Description for task " + std::to_string(i), &person);
        tm.assign_task(i, &person);
    }
    for (int i = 1; i <= number_of_tasks; ++i) {
        ASSERT_TRUE(tm.get_task(i) != nullptr);
        ASSERT_EQ(tm.get_task(i)->get_owner()->get_name(), person_name);
    }

    tm.unown_all_tasks();
    for (int i = 1; i <= number_of_tasks; ++i) {
        ASSERT_TRUE(tm.get_task(i) != nullptr);
        ASSERT_EQ(tm.get_task(i)->get_owner(), nullptr);
    }

    return 0;
}

static int test_task_status_operations() {
    TaskManager tm;
    std::string person_name = "John Doe";
    Person person(person_name);

    // Create a task and set its status
    std::string task_name = "Test Task";
    std::string task_description = "This is a test task";
    int task_id = tm.create_task(task_name, task_description, &person);

    ASSERT_TRUE(tm.get_task(task_id)->get_status() == Task::Status::Todo);

    tm.advance_task_status(task_id);
    ASSERT_TRUE(tm.get_task(task_id)->get_status() == Task::Status::InProgress);

    // Update the task status
    tm.mark_task_as_done(task_id);
    ASSERT_TRUE(tm.get_task(task_id)->get_status() == Task::Status::Done);

    return 0;
}

static int test_parent_child_operations() {
    TaskManager tm;
    std::string person_name = "John Doe";
    Person person(person_name);

    // Create a parent task
    int parent_task_id = tm.create_task("Parent Task", "This is a parent task", &person);
    ASSERT_TRUE(parent_task_id != 0);

    // Create a child task
    int child_task_id = tm.create_task("Child Task", "This is a child task", &person);
    ASSERT_TRUE(child_task_id != 0);

    // Make the child task a subtask of the parent task
    tm.make_child_task(parent_task_id, child_task_id);
    ASSERT_EQ(tm.get_task(child_task_id)->get_parent()->get_id(), parent_task_id);
    ASSERT_EQ(tm.get_task(parent_task_id)->get_children()[0]->get_id(), child_task_id);

    // Clean up
    tm.delete_task(child_task_id);
    tm.delete_task(parent_task_id);

    return 0;
}

// --- Main runner ---
int main() {
    int fails = 0;
    fails += test_create_and_delete_task();
    fails += test_get_task_on_empty_manager_returns_null();
    fails += test_make_child_task_fails_when_ids_missing();
    fails += test_delete_all_tasks_is_idempotent_and_safe();
    fails += test_print_all_task_owners_does_not_throw();
    fails += test_ownership_operations();
    fails += test_task_status_operations();
    fails += test_parent_child_operations();

    if (fails == 0) {
        std::cout << "[task_manager_unit_test] All tests passed\n";
        return 0;
    } else {
        std::cout << "[task_manager_unit_test] " << fails << " tests failed\n";
        return 1;
    }
}
