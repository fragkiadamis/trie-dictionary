#ifndef DICTIONARY_H
#define DICTIONARY_H

/* TRIE tree node */
typedef struct Trie {
    char *word;                 // If it's leaf, the word is stored here
    unsigned leaf: 1;           // Is leaf?
    struct Trie *children[26];     // The children array
} TRIE;

TRIE *dictionary_from_file(FILE *);

TRIE *dictionary_insert(TRIE *, char *);

TRIE *dictionary_search(TRIE *, char *);

void find_children(TRIE ***, TRIE *, unsigned *);

void dictionary_update(TRIE *, FILE *);

void dictionary_display(TRIE *);

TRIE *dictionary_delete(TRIE *, char *, unsigned depth = 0);

#endif
