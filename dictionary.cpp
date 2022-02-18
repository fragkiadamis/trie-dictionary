#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

/* Create a new trie node and return it */
TRIE *create_trie_node() {
    TRIE *node = (TRIE*) malloc(sizeof(TRIE));
    node->word = NULL;
    node->leaf = 0;
    return node;
}

/* Insert a word in the trie tree */
void trie_insert_node(TRIE *node, char *word) {
    char *word_parser = word;

    while (*word_parser) {
        unsigned ab_index = tolower(*word_parser) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
        if (!node->children[ab_index])
            node->children[ab_index] = create_trie_node();
        node = node->children[ab_index];
        word_parser++;
    }

    node->word = (char*) malloc(strlen(word) + 1); // Allocate required space for word
    strcpy(node->word, word); // Assign word
    node->leaf = 1; // The node is a leaf
}

/* Initializes the dictionary */
TRIE *dictionary_from_file(const char *filename) {
    FILE *file = NULL;
    unsigned bytes_allocated = 0;
    char *word = (char *) malloc(bytes_allocated * sizeof(char)), ch;
    TRIE *root = create_trie_node();

    // Open file
    if (!(file = fopen(filename, "r")))
        return NULL;

    while (fscanf(file, "%s", word) > 0) { // Ensure that at least a word is parsed
//        printf("%s | length: %d | alphabet number: %d\n", word, strlen(word), tolower(*word) - 'a');
        trie_insert_node(root, word); // Insert the word in the trie node
    }

    fclose(file); // Close file

    return root;
}

/* Display all the words in the trie structure leaves */
void display_trie_structure(TRIE *node) {
    if(node->leaf)
        puts(node->word);

    for (unsigned i = 0; i < 26; i++) {
        if (node->children[i])
            display_trie_structure(node->children[i]);
    }
}