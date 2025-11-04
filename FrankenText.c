#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 50000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 10

char *book = NULL;

char *load_book(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Could not open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char *buffer = malloc(length + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        exit(1);
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

/// Array of tokens registered so far
/// No duplicates are allowed.
char *tokens[MAX_WORD_COUNT];
/// `tokens`'s current size
size_t tokens_size = 0;

/// Array of successor tokens
/// One token can have many successor tokens. `succs[x]` corresponds to
/// `token[x]`'s successors.
/// We store directly tokens instead of token_ids, because we will directly
/// print them. If we wanted to delete the book, then it would make more sense
/// to store `token_id`s
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
/// `succs`'s size
size_t succs_sizes[MAX_WORD_COUNT];

/// Overwrites non-printable characters in `book` with a space.
/// Non-printable characters may lead to duplicates like
/// `"\xefthe" != "the"` even both print `the`.
void replace_non_printable_chars_with_space() {
    for (size_t i = 0; book[i] != '\0'; i++) { 
        if (!isprint((unsigned char)book[i])) {
            book[i] = ' ';
        }
    }
}


/// Returns token id if token is known, otherwise creates a new entry and
/// returns the new token id
size_t token_id(char *token) {
    for (size_t i = 0; i < tokens_size; i++) {
        if (strcmp(tokens[i], token) == 0) return i;
    }
    // først her oprettes et nyt
    if (tokens_size >= MAX_WORD_COUNT) { fprintf(stderr, "Too many tokens\n"); exit(1); }
    tokens[tokens_size] = strdup(token);
    succs_sizes[tokens_size] = 0;
    return tokens_size++;
}


/// Appends a token to the successors list of a `token`
void append_to_succs(char *token, char *succ) {
    size_t id = token_id(token);

    succs[id][succs_sizes[id]++] = strdup(succ);
}

/// Creates tokens from the `book` and fills `tokens` and `succs` arrays using
/// the `token`s.
void tokenize_and_fill_succs(char *delimiters, char *str) {
    char *prev = NULL;
    char *token = strtok(str, delimiters);

    while (token) {
        token_id(token);
        if(prev)
            append_to_succs(prev, token);
        prev = token;
        token = strtok(NULL, delimiters);
    };
}

/// Returns last character of a string
char last_char(char *str) {
    size_t len = strlen(str);
    return len > 0 ? str[len-1] : '\0';
}   

/// Returns whether the token ends with `!`, `?` or `.`.
bool token_ends_a_sentence(char *token) {
    size_t len = strlen(token);

    if (len == 0) return false;

    return (token[len-1] == '!' || token[len-1] == '?' || token[len-1] == '.');
}

/// Returns a random `token_id` that corresponds to a `token` that starts with a
/// capital letter.
/// Uses `tokens`, `tokens_size.
size_t random_token_id_that_starts_a_sentence() {
    size_t ids[MAX_WORD_COUNT];
    size_t count = 0;

    for (size_t i = 0; i < tokens_size; i++) {
        if(isupper((unsigned char)tokens[i][0]))
            ids[count++] = i;
    }
    return ids[rand() % count];
}

/// Generates a random sentence using `tokens`, `succs`, and `succs_sizes`.
/// The sentence array will be filled up to `sentence_size-1` characters using
/// random tokens until:
/// - a token is found where `token_ends_a_sentence()`
/// - or more tokens cannot be concatenated to the `sentence` anymore.
///  Returns the filled sentence array.
char *generate_sentence(char *sentence, size_t sentence_size) {
    sentence[0] = '\0';                                  
    size_t current_token_id = random_token_id_that_starts_a_sentence();
    char *token = tokens[current_token_id];
    strncat(sentence, token, sentence_size - 1);

    if (token_ends_a_sentence(token)) return sentence;

  // Calculated sentence length for the next iteration.
  // Used to stop the loop if the length exceeds sentence size
  size_t sentence_len_next;
  // Concatenates random successors to the sentence as long as
  // `sentence` can hold them.
  do {
    if (succs_sizes[current_token_id] == 0) break;

    size_t next_idx = rand() % succs_sizes[current_token_id];
    char *next_token = succs[current_token_id][next_idx];

    sentence_len_next = strlen(sentence) + 1 + strlen(next_token);

    if(sentence_len_next >= sentence_size - 1) break;

    strcat(sentence, " ");
    strcat(sentence, next_token);

    if(token_ends_a_sentence(next_token)) break;

    current_token_id = token_id(next_token);


  } while (sentence_len_next < sentence_size - 1);
  return sentence;
}

int main(void) {

    book = load_book("C:/Users/jeppe/OneDrive/02002students/Basis_C/pg84.txt");

    replace_non_printable_chars_with_space();

    char *delimiters = " \n\r";
    tokenize_and_fill_succs(delimiters, book);

    char sentence[1000];
    srand(time(NULL));


    do {
        // YOUR CODE HERE
        generate_sentence(sentence, sizeof sentence);
    } while (last_char(sentence) != '?');
    puts(sentence);
    puts("");


    do {
        // YOUR CODE HERE
        generate_sentence(sentence, sizeof sentence);
    } while (last_char(sentence) != '!');
    puts(sentence);

    free(book);
    return 0;
}