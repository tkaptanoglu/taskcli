#include <string>
#include <vector>
#include <iostream>

#include "person_manager.hpp"
#include "person.hpp"
#include "task.hpp"
#include "print_options.hpp"

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

// --- Concepts for PersonManager-like API ---
template <typename PM, typename P>
concept PersonManagerLike = requires(PM pm, const PM cpm, const std::string &name) {
    { pm.add_person(name) } -> std::same_as<P*>;      // returns a Person pointer
    { cpm.find_person_by_name(name) } -> std::same_as<P*>;     // retrieves by name
    { pm.remove_person(name) };                       // removes by name
    { cpm.get_all_people() } -> std::same_as<std::vector<P*>>;
};

template <typename P>
concept PersonLike = requires(P p, const P cp) {
    { P(std::string{}) };
    { cp.get_tasks() } -> std::same_as<std::vector<Task*>>;
};

// Helper to create a valid Task
Task* make_task(int id, const std::string &name, const std::string &desc) {
    return new Task(id, name, desc); // adjust status if needed
}

// --- Tests ---
int test_add_and_find_person_by_name() {
#if __cpp_concepts
    using PM = PersonManager;
    using P  = Person;

    if constexpr (PersonManagerLike<PM, P> && PersonLike<P>) {
        std::string person_name = "Alice";

        PM pm;
        pm.add_person(person_name);
        P* p = pm.find_person_by_name(person_name);
        ASSERT_TRUE(p != nullptr);

        // Ensure the fetched person starts with 0 tasks
        ASSERT_EQ(p->get_tasks().size(), 0u);
    }
#endif
    return 0;
}

int test_remove_person() {
#if __cpp_concepts
    using PM = PersonManager;
    using P  = Person;

    if constexpr (PersonManagerLike<PM, P>) {
        PM pm;
        pm.add_person("Bob");
        ASSERT_TRUE(pm.find_person_by_name("Bob") != nullptr);

        pm.delete_person("Bob");
        ASSERT_TRUE(pm.find_person_by_name("Bob") == nullptr);
    }
#endif
    return 0;
}

int test_assign_task_via_manager() {
#if __cpp_concepts
    using PM = PersonManager;
    using P  = Person;

    if constexpr (PersonManagerLike<PM, P> && PersonLike<P>) {
        std::string person_name = "Dana";
        std::string task_name = "Task for " + person_name;
        std::string task_description = "Description for " + person_name;
        PM pm;
        pm.add_person(person_name);
        P* p = pm.find_person_by_name(person_name);
        Task* t = make_task(42, task_name, task_description);

        // Only run if Person supports assign_task
        if constexpr (requires(P* p, Task* t){ p->assign_task(t); }) {
            p->assign_task(t);
            std::vector<Task*> tasks = p->get_tasks();
            ASSERT_EQ(tasks.size(), 1u);
            ASSERT_EQ(tasks[0]->get_name(), task_name);
            ASSERT_EQ(tasks[0]->get_description(), task_description);
        }
    }
#endif
    return 0;
}

int test_person_task_list_management() {
#if __cpp_concepts
    using PM = PersonManager;
    using P  = Person;

    if constexpr (PersonManagerLike<PM, P> && PersonLike<P>) {
        std::string person_name = "Eve";
        int number_of_tasks = 4;

        PM pm;
        pm.add_person(person_name);
        P* p = pm.find_person_by_name(person_name);
        ASSERT_TRUE(p != nullptr);
        
        std::vector<Task*> tasks = p->get_tasks();

        for (int i = 0; i < number_of_tasks; ++i) {
            Task* t = make_task(i, "Task " + std::to_string(i), "Description " + std::to_string(i));
            p->assign_task(t);
            ASSERT_EQ(tasks.size(), i + 1);
        }

        pm.set_persons_all_tasks_as_done(person_name);
        for (Task* task : p->get_tasks()) {
            ASSERT_TRUE(task->is_done());
        }

        pm.delete_persons_all_tasks(person_name);
        ASSERT_TRUE(p->get_tasks().empty());
    }
#endif
    return 0;
}

// --- Main runner ---
int main() {
    int fails = 0;
    fails += test_add_and_find_person_by_name();
    fails += test_remove_person();
    fails += test_assign_task_via_manager();
    fails += test_person_task_list_management();

    if (fails == 0) {
        std::cout << "[person_manager_unit_test] All tests passed\n";
        return 0;
    } else {
        std::cout << "[person_manager_unit_test] " << fails << " tests failed\n";
        return 1;
    }
}
