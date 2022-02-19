#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

char *read_word() {
    char ch, *word = (char*) malloc(sizeof(char));
    unsigned bytes_allocated = 0;

    while((ch = getchar()) != '\n') {
        word = (char*)realloc(word, bytes_allocated + 2);
        word[bytes_allocated++] = ch;
    }
    word[bytes_allocated] = '\0';
    getchar(); // Clear newline from the buffer
    return word;
}

int main() {
    FILE *file;
    unsigned choice = 0, exists;
    char *word, ans;

    // Open file
    if (!(file = fopen("diction.txt", "r"))) {
        puts("Could not find/open dictionary file");
        exit(EXIT_FAILURE);
    }

    /* Initialize dictionary from file */
    TRIE *root = dictionary_from_file(file);
    fclose(file); // Close file

    while (1) {
        /* Print menu */
        printf(
                "Menu\n"
                "1: Insert a word\n"
                "2: Search a word\n"
                "3: Update dictionary\n"
                "4: Display dictionary\n"
                "5: Delete word\n"
                "6: Auto complete\n"
                "7: Exit\n"
        );

        scanf("%d",&choice);
        getchar(); // Clear newline from the buffer

        if (choice == 7) // Exit the program
            exit(EXIT_SUCCESS);

        /* Process user choice */
        switch (choice) {
            case 1:
                puts("Write new word and press enter");
                word = read_word(); // Read word from keyboard
                exists = dictionary_insert(root, word); // Insert word in dictionary
                if (exists)
                    printf("Word %s already exists in the dictionary\n", word);
                break;
            case 2:
                puts("Write search string and press enter");
                word = read_word(); // Read word from keyboard
                exists = dictionary_search(root, word);
                if (exists)
                    printf("Word: %s already exists in the dictionary\n", word);
                else {
                    printf("Word %s does not exists in the dictionary, would you like to add it? (y/n): ", word);
                    scanf("%c", &ans); // Read answer from keyboard
                    if (ans == 'y') {
                        dictionary_insert(root, word); // Insert word in dictionary
                        printf("%s added to dictionary\n", word);
                    }
                }
                break;
            case 3:
                file = fopen("diction.txt", "w+"); // Open file with write permissions
                dictionary_update(root, file); // Update file
                fclose(file); // Close file
                break;
            case 4:
                dictionary_display(root); // Display list
                break;
            case 5:
                puts("Write the word to be deleted and press enter");
                word = read_word(); // Read word from keyboard
                dictionary_delete(root, word);
                printf("Word %s deleted from the dictionary", word);
                break;
            default:
                puts("Enter a valid choice");
        }
    }

    return 0;
}
