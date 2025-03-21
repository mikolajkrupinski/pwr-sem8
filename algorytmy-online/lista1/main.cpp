#include <iostream>
#include <forward_list>

enum Ordering {
    NONE,
    MOVE_TO_FRONT,
    TRANSPOSE,
    COUNT,
};

enum Distribution {
    UNIFORM,
    HARMONIC,
    BIHARMONIC,
    GEOMETRIC,
};

size_t access(std::forward_list<uint16_t>& list, uint16_t i, Ordering order);

uint16_t randomFromDist(Distribution dist);

int main() {
    std::cout << "Hello, world!" << std::endl;

    return 0;
}

size_t access(std::forward_list<uint16_t>& list, uint16_t i, Ordering order) {
    size_t cost = 0;

    auto prev = list.before_begin();
    auto curr = list.begin();
    while (curr != list.end()) {
        cost++;

        if (*curr == i) {
            break;
        }

        prev++;
        curr++;
    }

    list.insert_after(prev, i);

    switch (order) {
    case MOVE_TO_FRONT:
        list.erase_after(prev);
        list.insert_after(list.before_begin(), i);
        break;

    case TRANSPOSE:
        std::swap(*prev, *(prev++));
        break;

    case COUNT:
        // TODO
        break;
    }

    return cost;
}

uint16_t randomFromDist(Distribution dist) {
    return 0;
}
