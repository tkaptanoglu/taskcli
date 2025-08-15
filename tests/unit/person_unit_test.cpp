#include <string>
#include <vector>
#include <iostream>

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

// --- Concepts to adapt to your API ---
template <typename P>
concept PersonLike = requires(P p, const P cp) {
    { P(std::string{}) };
    { cp.get_tasks() } -> std::same_as<std::vector<Task*>>;
};

template <typename P>
concept HasAssignTask = requires(P p, Task* t) {
    { p.assign_task(t) };
};

template <typename P>
concept HasRemoveTask = requires(P p, Task* t) {
    { p.remove_task(t) };
};

template <typename P>
concept HasRemoveAllTasks = requires(P p) {
    { p.remove_all_tasks() };
};

template <typename P>
concept HasReturnNumberOfTasks = requires(const P p) {
    { p.return_number_of_tasks() } -> std::same_as<int>;
};

// --- Actual tests ---
int test_assigns_and_removes() {
#if __cpp_concepts
    using P = Person;
    using T = Task;

    if constexpr (PersonLike<P>) {
        P alice("Alice");
        ASSERT_EQ(alice.get_tasks().size(), 0u);

        if constexpr (HasAssignTask<P>) {
            T* t1 = new T(1, "Task 1", "Description 1");
            T* t2 = new T(2, "Task 2", "Description 2");
            alice.assign_task(t1);
            alice.assign_task(t2);
            ASSERT_EQ(alice.get_tasks().size(), 2u);

            if constexpr (HasRemoveTask<P>) {
                alice.remove_task(t1);
                ASSERT_EQ(alice.get_tasks().size(), 1u);
            }

            if constexpr (HasRemoveAllTasks<P>) {
                alice.remove_all_tasks();
                ASSERT_EQ(alice.get_tasks().size(), 0u);
            } else {
                delete t1;
                delete t2;
            }
        }
    }
#endif
    return 0;
}

int test_reports_number_of_tasks() {
#if __cpp_concepts
    using P = Person;
    using T = Task;

    if constexpr (PersonLike<P>) {
        P bob("Bob");
        if constexpr (HasAssignTask<P>) {
            T* t = new T(1, "Task 1", "Description 1");
            bob.assign_task(t);
        }
        if constexpr (HasReturnNumberOfTasks<P>) {
            int n = bob.return_number_of_tasks();
            ASSERT_EQ(n, 1);
        }
    }
#endif
    return 0;
}

int tests_set_and_get_name() {
#if __cpp_concepts
    using P = Person;

    if constexpr (PersonLike<P>) {
        P alice("Alice");
        ASSERT_EQ(alice.get_name(), "Alice");
        alice.set_name("Alicia");
        ASSERT_EQ(alice.get_name(), "Alicia");
    }
#endif
    return 0;
}

int tests_set_all_tasks_to_done() {
#if __cpp_concepts
    using P = Person;
    using T = Task;

    if constexpr (PersonLike<P>) {
        P alice("Alice");
        if constexpr (HasAssignTask<P>) {
            T* t1 = new T(1, "Task 1", "Description 1");
            T* t2 = new T(2, "Task 2", "Description 2");
            T* t3 = new T(3, "Task 3", "Description 3");
            alice.assign_task(t1);
            alice.assign_task(t2);
            alice.assign_task(t3);

            std::vector<Task*> tasks = alice.get_tasks();
            for (Task* task : tasks) {
                task->mark_as_done();
            }
            for (Task* task : tasks) {
                ASSERT_TRUE(task->is_done());
            }
        }
        if constexpr (HasRemoveAllTasks<P>) {
            alice.remove_all_tasks();
        }
    }
#endif
    return 0;
}

// --- Main runner ---
int main() {
    int fails = 0;
    fails += test_assigns_and_removes();
    fails += test_reports_number_of_tasks();
    fails += tests_set_and_get_name();

    if (fails == 0) {
        std::cout << "[person_unit_test] All tests passed\n";
        return 0;
    } else {
        std::cout << "[person_unit_test] " << fails << " tests failed\n";
        return 1;
    }
}