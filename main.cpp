#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

char *read_string() {
    char ch, *word = (char*) malloc(sizeof(char));
    unsigned bytes_allocated = 0;

    while((ch = getchar()) != '\n') {
        word = (char*)realloc(word, bytes_allocated + 2);
        word[bytes_allocated++] = ch;
    }

    word[bytes_allocated] = '\0';
    return word;
}

void prompt_to_insert(TRIE *root, char *word) {
    char ans;
    printf("Word '%s' does not exists in the dictionary, would you like to add it? (y/n): ", word);
    scanf("%c", &ans); // Read answer from keyboard
    if (ans == 'y') {
        dictionary_insert(root, word); // Insert word in dictionary
        printf("%s added to dictionary\n", word);
    }
}

void handle_results(TRIE *root, TRIE *node, char *word, unsigned  auto_complete) {
    if (auto_complete) { // If it's an auto-complete request
        TRIE **results = (TRIE**) malloc(sizeof(TRIE*));
        unsigned size = 0;
        find_children(&results, node, &size); // Find all the leaves of the node
        puts("Choose word");
        for (unsigned i = 0; i < size; i++)
            printf("%d. %s\t", i + 1, results[i]->word);
        putchar('\n');
        char *ans = read_string();
        if (strlen(ans) == 1 && isdigit(ans[0])) { // If the user input is a digit
            unsigned digit = atoi(ans);
            if (0 < digit <= size) // If the input is in the correct range according to arrays size
                printf("Word: '%s' exists in the dictionary\n", results[digit - 1]->word);
            else
                puts("Not a valid option. Abort.");
        } else { // The user types more characters
            word = (char*) realloc(word, (strlen(word) - 1) * sizeof(char)); // Remove last character (null-terminator)
            word[strlen(word) - 1] = '\0'; // Add null terminator where the '$' is
            strcat(word, ans); // Concatenate the stings to create the new word
            prompt_to_insert(root, word); // Ask to insert the word
        }
    } else if (!node || !node->leaf) // Node NOT found, or it is not a leaf, NOT an auto-complete request
        prompt_to_insert(root, word);
    else // Node found, NOT an auto-complete request
        printf("Word: %s exists in the dictionary\n", node->word);
}

int main() {
    FILE *file;
    unsigned choice = 0, auto_complete = 0;
    char *word;
    TRIE *root, *node;

    // Open file
    if (!(file = fopen("diction.txt", "r"))) {
        puts("Could not find/open dictionary file");
        exit(EXIT_FAILURE);
    }
    // Initialize dictionary from file
    root = dictionary_from_file(file);
    fclose(file); // Close file

    while (1) {
        // Print menu
        printf(
                "Menu\n"
                "1: Insert a word\n"
                "2: Search a word\n"
                "3: Update dictionary\n"
                "4: Display dictionary\n"
                "5: Delete word\n"
                "6: Exit\n"
        );

        scanf("%d",&choice);
        getchar(); // Clear newline from the buffer

        if (choice == 6) // Exit the program
            exit(EXIT_SUCCESS);

        // Process user choice
        switch (choice) {
            case 1:
                puts("Write new word and press enter");
                word = read_string(); // Read word from keyboard
                node = dictionary_insert(root, word); // Insert word in dictionary
                if (node)
                    printf("Word %s already exists in the dictionary\n", node->word);
                break;
            case 2:
                puts("Write search string and press enter");
                word = read_string(); // Read word from keyboard
                auto_complete = (word[strlen(word) - 1] == '$') ? 1 : 0; // Is it an auto-complete request?
                node = dictionary_search(root, word); // Search for a node according to the string input
                handle_results(root, node, word, auto_complete); // Handle the results of the search according to the inputs
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
                word = read_string(); // Read word from keyboard
                dictionary_delete(root, word); // Delete word from dictionary
                file = fopen("diction.txt", "w+"); // Open file with write permissions
                dictionary_update(root, file); // Update file
                fclose(file); // Close file
                printf("Word %s deleted from the dictionary", word);
                break;
            default:
                puts("Enter a valid choice");
        }
    }
}
