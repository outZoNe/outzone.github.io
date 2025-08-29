#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>

// ANSI escape codes for colors
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"

// ANSI escape codes for font size
#define BOLD    "\033[1m"  // Bold font without changing color

int main(int argc, char *argv[]) {
    // Set locale for UTF-8 support
    setlocale(LC_ALL, "");
    
    // Calculate total length needed
    size_t total_len = 0;
    for (int i = 1; i < argc; i++) {
        total_len += strlen(argv[i]);
        if (i < argc - 1) total_len++; // Space between words
    }
    
    // Allocate memory for combined string
    char *combined_input = malloc(total_len + 1);
    if (!combined_input) {
        printf("Ошибка: не удалось выделить память\n");
        return 1;
    }
    
    // Combine all arguments with spaces
    size_t pos = 0;
    for (int i = 1; i < argc; i++) {
        strcpy(combined_input + pos, argv[i]);
        pos += strlen(argv[i]);
        if (i < argc - 1) {
            combined_input[pos] = ' ';
            pos++;
        }
    }
    combined_input[pos] = '\0';
    
    // Convert to wide string
    size_t len = mbstowcs(NULL, combined_input, 0);
    if (len == (size_t)-1) {
        printf("Ошибка: неверная кодировка\n");
        free(combined_input);
        return 1;
    }
    
    wchar_t *word = malloc((len + 1) * sizeof(wchar_t));
    mbstowcs(word, combined_input, len + 1);
    
    // Colors array for rainbow effect
    char *colors[] = {RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN};
    int num_colors = 6;
    
    printf("\n\n\n\n     ");
    
    int current_line_length = 0;
    int word_start = 0;
    
    for (size_t i = 0; i < len; i++) {
        int color_index = i % num_colors;
        
        // Check if this is a space or end of string
        if (word[i] == L' ' || i == len - 1) {
            int word_length = i - word_start + (i == len - 1 ? 1 : 0);
            
            // If adding this word would exceed line limit, start new line
            if (current_line_length + word_length > 20 && current_line_length > 0) {
                printf("%s\n     ", RESET);
                current_line_length = 0;
            }
            
            // Print the word
            for (int j = word_start; j <= i; j++) {
                int word_color_index = j % num_colors;
                printf("%s%lc", colors[word_color_index], word[j]);
            }
            
            current_line_length += word_length;
            word_start = i + 1;
        }
    }
    
    printf("%s\n\n\n\n\n", RESET);
    
    free(word);
    free(combined_input);
    return 0;
}
