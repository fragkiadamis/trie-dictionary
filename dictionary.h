#ifndef DICTIONARY_H
#define DICTIONARY_H

/* TRIE tree node */
typedef struct Trie {
    char *word;                 // If it's leaf, the word is stored here
    unsigned leaf: 1;           // Is leaf?
    struct Trie *children[26];     // The children array
} TRIE;

TRIE *dictionary_from_file(FILE *);

void dictionary_insert(TRIE *, char *);

void dictionary_display(TRIE *);

unsigned dictionary_search(TRIE *, char *);

void dictionary_update(TRIE *, FILE *);

#endif
