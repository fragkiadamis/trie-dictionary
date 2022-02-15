#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

/* TRIE tree node */
typedef struct trie {
    unsigned leaf: 1;
    char name[20];
    struct trie *pin[26];
} TRIE;

/* Insert a trie node */
TRIE *new_node() {
    TRIE *node;
    node = (TRIE *) malloc(sizeof(TRIE));
    return node;
}

TRIE dictionary;

/* Initialize the dictionary */
void init_dictionary() {
    dictionary = (struct trie) {};
}

/* Initializes the dictionary */
unsigned dictionary_from_file(const char *filename) {
    FILE *file;
    char *word;
    char ch;
    unsigned bytes_allocated = 0;

    /* Read file */
    file = fopen(filename, "r");
    if (!file) {
        printf("Could not find/open file: %s\n", filename);
        return 0;
    }

    /* Initialize word */
    word = (char *) malloc(2 * sizeof(char));
    /* Read file character by character */
    while ((ch = fgetc(file)) != EOF) {
        /* While on the same word (ENTER character terminates the word) */
        if (ch != '\n') {
            /* Change word size */
            word = (char*)realloc(word, bytes_allocated + 2);
            /* Add character to word */
            word[bytes_allocated] = ch;
            bytes_allocated++;
            continue;
        }

        /* Once the word is over, add null terminator */
        word[bytes_allocated] = '\0';
        /* Reset byte counter to 0 */
        bytes_allocated = 0;
        puts(word);
    }
    return 1;
}