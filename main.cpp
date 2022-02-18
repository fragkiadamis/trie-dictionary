#include <stdio.h>
#include "dictionary.h"

int main() {
    TRIE *root = dictionary_from_file("diction.txt");
    if(!root) {
        puts("Could not find/open dictionary file");
        return 1;
    }

    display_trie_structure(root);

    return 0;
}
