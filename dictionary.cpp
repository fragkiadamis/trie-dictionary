#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

/* Create a new trie node and return it */
TRIE *create_trie_node() {
    TRIE *node = (TRIE *) malloc(sizeof(TRIE));
    node->word = NULL;
    node->leaf = 0;
    return node;
}

/* Insert a word in the trie tree */
void dictionary_insert(TRIE *node, char *word) {
    char *word_parser = (char *) malloc(sizeof(strlen(word)));
    strcpy(word_parser, word);

    /* Traverse string character by character */
    while (*word_parser) {
        unsigned ab_index = tolower(*word_parser) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
        if (!node->children[ab_index]) // Create node if not exists
            node->children[ab_index] = create_trie_node();
        node = node->children[ab_index]; // Go to next node
        word_parser++; // Go to next character
    }

    /* If the word already exists just return */
    if (node->word && node->leaf) {
        puts("The word already exists");
        return;
    }

    node->word = (char *) malloc(strlen(word) + 1); // Allocate required space for word
    strcpy(node->word, word); // Assign word
    node->leaf = 1; // The node is a leaf
}

/* Initializes the dictionary */
TRIE *dictionary_from_file(FILE *file) {
    unsigned bytes_allocated = 0;
    char *word = (char *) malloc(bytes_allocated * sizeof(char));
    TRIE *root = create_trie_node(); // Create trie root

    while (fscanf(file, "%s", word) > 0)// Ensure that at least a word is parsed
        dictionary_insert(root, word); // Insert the word in the trie node

    return root;
}

/* Display all the words in the trie structure leaves */
void dictionary_display(TRIE *node) {
    // If it's a leaf, display the word
    if (node->leaf)
        puts(node->word);

    // Start recursion for each position of the child node
    for (unsigned i = 0; i < 26; i++) {
        if (node->children[i])
            dictionary_display(node->children[i]);
    }
}

/* Search the dictionary for a word */
unsigned dictionary_search(TRIE *node, char *word) {
    char *word_parser = (char *) malloc(sizeof(strlen(word)));
    strcpy(word_parser, word);

    /* Traverse string character by character */
    while (*word_parser) {
        unsigned ab_index = tolower(*word_parser) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
        if (!node->children[ab_index])
            return 0;
        node = node->children[ab_index];
        word_parser++; // Go to next character
    }

    return 1;
}

/* Update the dictionary file */
void dictionary_update(TRIE *node, FILE *file) {
    // If it's a leaf, write the word with a newline character
    if (node->leaf) {
        fputs(node->word, file);
        fputc('\n', file);
    }

    // Start recursion for each position of the child node
    for (unsigned i = 0; i < 26; i++) {
        if (node->children[i])
            dictionary_update(node->children[i], file);
    }
}
