#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define ALPHABET_SIZE 26

/* Create a new trie node and return it */
TRIE *create_trie_node() {
    TRIE *node = (TRIE *) malloc(sizeof(TRIE));
    node->word = NULL;
    node->leaf = 0;
    return node;
}

/* Check if node has children nodes */
unsigned has_children(TRIE *node) {
    for (unsigned i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i])
            return 1;
    return 0;
}

/* Insert a word in the trie tree */
TRIE *dictionary_insert(TRIE *node, char *word) {
    char *word_parser = (char *) malloc(sizeof(strlen(word)));
    strcpy(word_parser, word);

    // Traverse string character by character
    while (*word_parser) {
        unsigned ab_index = tolower(*word_parser) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
        if (!node->children[ab_index]) // Create node if not exists
            node->children[ab_index] = create_trie_node();
        node = node->children[ab_index]; // Go to next node
        word_parser++; // Go to next character
    }

    /* If the word already exists just return */
    if (node->word && node->leaf)
        return node;

    node->word = (char *) malloc(strlen(word) + 1); // Allocate required space for word
    strcpy(node->word, word); // Assign word
    node->leaf = 1; // The node is a leaf

    return NULL;
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

/* Search the dictionary for a string */
TRIE *dictionary_search(TRIE *node, char *word) {
    char *word_parser = (char *) malloc(sizeof(strlen(word)));
    strcpy(word_parser, word);

    // Traverse string character by character, stop if word ends with '$'
    while (*word_parser && *word_parser != '$') {
        unsigned ab_index = tolower(*word_parser) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
        if (!node->children[ab_index]) // If there is no continuity, then the word does not exist
            return NULL;
        node = node->children[ab_index];
        word_parser++; // Go to next character
    }

    return node;
}

/* Find all leaves of an intermediate node */
void find_children(TRIE ***results, TRIE *node, unsigned *size) {
    if (node->leaf) {
        (*size)++;
        *results = (TRIE**) realloc(*results, (*size) * sizeof(TRIE*));
        (*results)[(*size) - 1] = node;
    }

    for (unsigned i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i])
            find_children(results, node->children[i], size);
    }
}

/* Update the dictionary file */
void dictionary_update(TRIE *node, FILE *file) {
    // If it's a leaf, write the word with a newline character
    if (node->leaf) {
        fputs(node->word, file);
        fputc('\n', file);
    }

    // Start recursion for each position of the child node
    for (unsigned i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i])
            dictionary_update(node->children[i], file);
}

/* Display all the words in the trie structure leaves */
void dictionary_display(TRIE *node) {
    // If it's a leaf, display the word
    if (node->leaf)
        puts(node->word);

    // Start recursion for each position of the child node
    for (unsigned i = 0; i < ALPHABET_SIZE; i++)
        if (node->children[i])
            dictionary_display(node->children[i]);
}

/* Delete a word from the dictionary */
TRIE *dictionary_delete(TRIE *node, char *word, unsigned depth) {
    // If the node is empty return
    if (!node)
        return NULL;

    // If processing the last character of the word
    if (depth == strlen(word)) {
        // Delete leaf
        if (node->leaf && !has_children(node)) {
            node->leaf = 0;
            free(node);
        }

        return node;
    }

    // Not a leaf, recur for the leaf
    unsigned ab_index = tolower(word[depth]) - 'a'; // Get order, (alphabetical order --- a-z --> 0-25)
    node->children[ab_index] = dictionary_delete(node->children[ab_index], word, depth + 1);

    // If node is empty after child deletion, delete it
    if (!has_children(node) && !node->leaf)
        free(node);

    return node;
}
