#ifndef UNIT_H
#define UNIT_H

#define pass(tname, fcall, err_fmt, ...) do {                               \
    bool passed = fcall;                                                    \
    u->total++;                                                             \
    if(passed) {                                                            \
        if(u->print_passed) printf("  [ pass ] " #tname "\n");              \
        u->passed++;                                                        \
    } else {                                                                \
        printf("  [ fail ] %s " #tname ": " err_fmt "\n",                   \
            u->name, ##__VA_ARGS__);                                        \
    }                                                                       \
} while(0)

#define unit_test(name) static void test_##name(struct unit_s *u)

#define unit_summary(u)                                             \
    printf("[%s %.0f%% ] %s (%lu of %lu)\n",                        \
    u.passed == u.total ? "" : " ",                                 \
    ((float) u.passed / (float) u.total) * 100, u.name, u.passed, u.total)

#define unit_new(n) (struct unit_s) \
    {.passed = 0, .total = 0, .name = #n, .print_passed = false }

#define test(name) do {                                     \
    struct unit_s name##_test_results = unit_new(name);     \
    test_##name(&name##_test_results);                      \
    unit_summary(name##_test_results);                      \
} while(0)

struct unit_s {
    size_t passed, total;
    bool print_passed;
    const char *name;
};

#endif
