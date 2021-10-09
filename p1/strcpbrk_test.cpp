// https://stackoverflow.com/questions/29719999/testing-function-for-speed-performance-in-cpp/29720314
// https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
#include <cstring>
#include <cstdlib>

template<typename TimeT = std::chrono::milliseconds>
struct measure {
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F func, Args&&... args)
    {
        auto start = std::chrono::high_resolution_clock::now();
        func(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT>
                (std::chrono::high_resolution_clock::now() - start);
        return duration.count();
    }
};

int total = 0;

char *strcpbrk1(const char *s1, const char *s2) {
    while (*s1)
        if (!strchr(s2, *s1++))
            return (char *)--s1;
    return 0;
}

char *strcpbrk2(const char *s, const char *b) {
    s += strspn(s, b);
    return *s ? (char *)s : 0;
}

void test1(char *parsedln) {
    for (int i=0; i<100; ++i)
        strcpbrk1(parsedln, " \t\n");
}

void test2(char *parsedln) {
    for (int i=0; i<100; ++i)
        strcpbrk2(parsedln, " \t\n");
}

static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size) {
     char *s = (char *)malloc(sizeof(char) * size);
     if (s) {
         rand_string(s, size);
     }
     return s;
}

int main () {
    using namespace std::chrono;
    
    char *parsedln = rand_string_alloc(1000);
    for (int i = 0; i < 5; ++i) {
        total = 0;
        auto t = measure<std::chrono::nanoseconds>::execution(test2, parsedln);
        std::cout << "Calculated total = " << total << " in " << t << " ns." << std::endl;
    }
    free(parsedln);
}
