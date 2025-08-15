#include <string>
#include <vector>
#include <iostream>

#include "task.hpp"
#include "person.hpp"

// --- Tiny assert helpers ---
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

int test_task_creation_and_accessors() {
    std::string name = "Test task";
    std::string description = "Description for test task";
    Task t(1, name, description);
    ASSERT_EQ(t.get_id(), 1);
    ASSERT_EQ(t.get_name(), name);
    ASSERT_EQ(t.get_description(), description);
    ASSERT_EQ(static_cast<int>(t.get_status()), static_cast<int>(Task::Status::Todo));
    ASSERT_EQ(t.get_level(), 1);
    return 0;
}

int test_task_status_change() {
    Task t(2, "Change status", "Description for change status");
    t.set_status(Task::Status::Done);
    ASSERT_EQ(static_cast<int>(t.get_status()), static_cast<int>(Task::Status::Done));

    t.set_status(Task::Status::Todo);
    ASSERT_EQ(static_cast<int>(t.get_status()), static_cast<int>(Task::Status::Todo));

    t.advance_status();
    ASSERT_EQ(static_cast<int>(t.get_status()), static_cast<int>(Task::Status::InProgress));

    t.mark_as_done();
    ASSERT_EQ(static_cast<int>(t.get_status()), static_cast<int>(Task::Status::Done));
    return 0;
}

int test_parent_child_relationships() {
    Task parent(3, "Parent", "Description for parent");
    Task child(4, "Child", "Description for child");

    child.set_parent(&parent);
    ASSERT_EQ(child.get_parent(), &parent);

    parent.add_child(&child);
    const auto &children = parent.get_children();
    ASSERT_EQ(children.size(), 1u);
    ASSERT_TRUE(children[0] == &child);
    ASSERT_TRUE(child.get_level() == 2);

    // Prevent self-parent
    Task self(5, "Self", "Description for self");
    self.set_parent(&self);
    ASSERT_TRUE(self.get_parent() != &self);

    return 0;
}

int test_owner_operations() {
    Person owner("Alice");
    Task t(1, "Test task", "Description for test task");
    ASSERT_TRUE(t.get_owner() == nullptr);

    t.set_owner(&owner);
    ASSERT_TRUE(t.get_owner() == &owner);

    t.unown();
    ASSERT_TRUE(t.get_owner() == nullptr);
    return 0;
}

int main() {
    int fails = 0;
    fails += test_task_creation_and_accessors();
    fails += test_task_status_change();
    fails += test_parent_child_relationships();
    fails += test_owner_operations();

    if (fails == 0) {
        std::cout << "[task_unit_test] All tests passed\n";
        return 0;
    } else {
        std::cout << "[task_unit_test] " << fails << " tests failed\n";
        return 1;
    }
}
