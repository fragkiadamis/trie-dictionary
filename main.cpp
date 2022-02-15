#include <stdio.h>
#include "dictionary.h"

int main() {
    init_dictionary();
    if(!dictionary_from_file("diction.txt"))
        return 1;

    return 0;
}
