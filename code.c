#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000
#define MAXLEN 8
#define MAX_INPUT 100

typedef struct TrieNode {
    struct TrieNode *child[26];
    int *ids;
    int count, capacity;
} TrieNode;

char catalog[MAXN][MAXLEN];
int N = 0;

TrieNode *prefixRoot, *suffixRoot;

/* Create Trie node */
TrieNode* createNode() {
    return (TrieNode*)calloc(1, sizeof(TrieNode));
}

/* Add ID to node */
int addID(TrieNode *node, int id) {

    if (node->count == node->capacity) {

        int size = (node->capacity == 0) ?
                   4 : node->capacity * 2;

        int *temp = realloc(node->ids, size * sizeof(int));

        if (temp == NULL)
            return 0;

        node->ids = temp;
        node->capacity = size;
    }

    node->ids[node->count++] = id;

    return 1;
}

/* Insert word into Trie */
int insertTrie(TrieNode *root, char *word, int id) {

    TrieNode *cur = root;

    for (int i = 0; word[i] != '\0'; i++) {

        int index = word[i] - 'a';

        if (index < 0 || index >= 26)
            return 0;

        if (cur->child[index] == NULL) {

            cur->child[index] = createNode();

            if (cur->child[index] == NULL)
                return 0;
        }

        cur = cur->child[index];

        if (!addID(cur, id))
            return 0;
    }

    return 1;
}

/* Reverse string */
void reverseString(char *src, char *dest) {

    int len = strlen(src);

    for (int i = 0; i < len; i++)
        dest[i] = src[len - 1 - i];

    dest[len] = '\0';
}

/* Insert into prefix Trie */
int insertPrefix(char *word, int id) {
    return insertTrie(prefixRoot, word, id);
}

/* Insert into suffix Trie */
int insertSuffix(char *word, int id) {

    char rev[MAXLEN];

    reverseString(word, rev);

    return insertTrie(suffixRoot, rev, id);
}

/* Remove ID from Trie */
void removeIDFromTrie(TrieNode *node, int id) {

    if (node == NULL)
        return;

    for (int i = 0; i < node->count; i++) {

        if (node->ids[i] == id) {

            for (int j = i; j < node->count - 1; j++)
                node->ids[j] = node->ids[j + 1];

            node->count--;
            i--;
        }
    }

    for (int i = 0; i < 26; i++)
        removeIDFromTrie(node->child[i], id);
}

/* Insert record into both Tries safely */
int insertRecord(char *word, int id) {

    if (!insertPrefix(word, id) ||
        !insertSuffix(word, id)) {

        removeIDFromTrie(prefixRoot, id);
        removeIDFromTrie(suffixRoot, id);

        return 0;
    }

    return 1;
}

/* Search Trie */
TrieNode* searchTrie(TrieNode *root, char *key) {

    TrieNode *cur = root;

    for (int i = 0; key[i] != '\0'; i++) {

        int index = key[i] - 'a';

        if (index < 0 || index >= 26)
            return NULL;

        if (cur->child[index] == NULL)
            return NULL;

        cur = cur->child[index];
    }

    return cur;
}

/* Find prefix node */
TrieNode* searchPrefix(char *prefix) {
    return searchTrie(prefixRoot, prefix);
}

/* Find suffix node */
TrieNode* searchSuffix(char *suffix) {

    char rev[MAXLEN];

    reverseString(suffix, rev);

    return searchTrie(suffixRoot, rev);
}

/* Find latest matching record */
int latestMatch(char *prefix, char *suffix) {

    TrieNode *p = searchPrefix(prefix);
    TrieNode *s = searchSuffix(suffix);

    if (p == NULL || s == NULL)
        return -1;

    int i = p->count - 1;
    int j = s->count - 1;

    while (i >= 0 && j >= 0) {

        if (p->ids[i] == s->ids[j])
            return p->ids[i];

        if (p->ids[i] > s->ids[j])
            i--;
        else
            j--;
    }

    return -1;
}

/* Validate input word */
int validWord(char *word) {

    int len = strlen(word);

    if (len < 1 || len > 8)
        return 0;

    for (int i = 0; word[i] != '\0'; i++) {

        if (word[i] < 'a' || word[i] > 'z')
            return 0;
    }

    return 1;
}

/* Add new title */
int addNewTitle(char *word) {

    if (N >= MAXN) {
        printf("Catalog is full!\n");
        return 0;
    }

    if (!validWord(word)) {
        printf("Invalid title! Use 1-8 lowercase letters.\n");
        return 0;
    }

    int id = N + 1;

    if (!insertRecord(word, id)) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    strcpy(catalog[N], word);

    N++;

    printf("Title added successfully!\n");
    printf("Record ID: %d\n", id);

    return 1;
}

/* Display catalog */
void displayCatalog() {

    printf("\n===== LIBRARY CATALOG =====\n");

    for (int i = 0; i < N; i++)
        printf("ID %d : %s\n", i + 1, catalog[i]);
}

/* Free memory */
void freeTrie(TrieNode *node) {

    if (node == NULL)
        return;

    for (int i = 0; i < 26; i++)
        freeTrie(node->child[i]);

    free(node->ids);
    free(node);
}

/* Main function */
int main() {

    prefixRoot = createNode();
    suffixRoot = createNode();

    if (prefixRoot == NULL || suffixRoot == NULL) {

        printf("Memory allocation failed!\n");

        freeTrie(prefixRoot);
        freeTrie(suffixRoot);

        return 1;
    }

    int initialN;

    printf("Enter number of titles: ");

    if (scanf("%d", &initialN) != 1 ||
        initialN < 0 || initialN > MAXN) {

        printf("Invalid catalog!\n");

        freeTrie(prefixRoot);
        freeTrie(suffixRoot);

        return 1;
    }

    /* Build initial catalog */
    for (int i = 0; i < initialN; i++) {

        char word[MAX_INPUT];

        while (1) {

            printf("Enter title %d: ", N + 1);

            if (scanf("%99s", word) != 1) {

                printf("Input error!\n");

                freeTrie(prefixRoot);
                freeTrie(suffixRoot);

                return 1;
            }

            if (!validWord(word)) {

                printf("Invalid title! Use 1-8 lowercase letters.\n");
                printf("Please enter the title again.\n");

                continue;
            }

            if (!insertRecord(word, N + 1)) {

                printf("Memory allocation failed!\n");

                freeTrie(prefixRoot);
                freeTrie(suffixRoot);

                return 1;
            }

            strcpy(catalog[N], word);

            N++;

            break;
        }
    }

    int choice;

    /* Main menu */
    while (1) {

        printf("\n===== LIBRARY SEARCH SYSTEM =====\n");
        printf("1. Search by prefix and suffix\n");
        printf("2. Add new title\n");
        printf("3. Display catalog\n");
        printf("4. Exit\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {

            printf("Invalid menu input!\n");
            break;
        }

        if (choice == 1) {

            char prefix[MAX_INPUT];
            char suffix[MAX_INPUT];

            printf("Enter prefix: ");

            if (scanf("%99s", prefix) != 1)
                break;

            printf("Enter suffix: ");

            if (scanf("%99s", suffix) != 1)
                break;

            if (!validWord(prefix) || !validWord(suffix)) {

                printf("Invalid input! Use 1-8 lowercase letters.\n");
                continue;
            }

            int id = latestMatch(prefix, suffix);

            if (id == -1) {

                printf("No Match Found!\n");
            }
            else {

                printf("\nMatched Record: %s\n", catalog[id - 1]);
                printf("Record ID: %d\n", id);
            }
        }

        else if (choice == 2) {

            char word[MAX_INPUT];

            printf("Enter new title: ");

            if (scanf("%99s", word) != 1)
                break;

            int result = addNewTitle(word);

            if (result == -1) {

                printf("Memory allocation failed. Exiting.\n");
                break;
            }
        }

        else if (choice == 3) {

            displayCatalog();
        }

        else if (choice == 4) {

            printf("Exiting system...\n");
            break;
        }

        else {

            printf("Invalid choice! Select 1-4.\n");
        }
    }

    freeTrie(prefixRoot);
    freeTrie(suffixRoot);

    return 0;
}
