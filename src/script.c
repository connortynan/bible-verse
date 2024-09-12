#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 66
#define MAX_LINE_LENGTH 1024
#define MAX_BOOK_NAME_LENGTH 32
#define BASE_DIR "/usr/share/games/bible-verses" // Base directory for data files

const char *BOOK_NAMES[MAX_BOOKS] = {
    "Genesis", "Exodus", "Leviticus", "Numbers", "Deuteronomy",
    "Joshua", "Judges", "Ruth", "1 Samuel", "2 Samuel", "1 Kings",
    "2 Kings", "1 Chronicles", "2 Chronicles", "Ezra", "Nehemiah",
    "Esther", "Job", "Psalms", "Proverbs", "Ecclesiastes", "Song of Solomon",
    "Isaiah", "Jeremiah", "Lamentations", "Ezekiel", "Daniel", "Hosea",
    "Joel", "Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk",
    "Zephaniah", "Haggai", "Zechariah", "Malachi", "Matthew", "Mark",
    "Luke", "John", "Acts", "Romans", "1 Corinthians", "2 Corinthians",
    "Galatians", "Ephesians", "Philippians", "Colossians", "1 Thessalonians",
    "2 Thessalonians", "1 Timothy", "2 Timothy", "Titus", "Philemon",
    "Hebrews", "James", "1 Peter", "2 Peter", "1 John", "2 John",
    "3 John", "Jude", "Revelation"};

// Function to get the book number
int get_book_number(const char *book_name)
{
    for (int i = 1; i < MAX_BOOKS; i++)
    {
        if (strcmp(BOOK_NAMES[i], book_name) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Function to get a line from a file
char *get_nth_line(const char *filename, int n)
{
    static char line[MAX_LINE_LENGTH];
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    int current_line = 0;
    while (fgets(line, sizeof(line), file))
    {
        if (current_line == n)
        {
            fclose(file);
            return line;
        }
        current_line++;
    }

    fclose(file);
    return NULL;
}

// Function to get a random verse
char *get_random_verse(const char *filename, char **valid_books, int valid_books_count)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    int total_verses = 0;
    int line_numbers[MAX_BOOKS] = {0};
    int book_counts[MAX_BOOKS] = {0};
    int current_line = 0;

    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '%')
        {
            char chapter_name[MAX_BOOK_NAME_LENGTH];
            int num_of_verses;
            if (sscanf(line, "%%\"%[^\"]\":%d", chapter_name, &num_of_verses) == 2)
            {
                int book_number = get_book_number(chapter_name);
                if (book_number > 0)
                {
                    int is_valid = 0;
                    for (int i = 0; i < valid_books_count; i++)
                    {
                        if (strcmp(BOOK_NAMES[book_number], valid_books[i]) == 0)
                        {
                            is_valid = 1;
                            break;
                        }
                    }
                    if (valid_books_count == 0 || is_valid)
                    {
                        book_counts[book_number] = num_of_verses;
                        total_verses += num_of_verses;
                        line_numbers[book_number] = current_line;
                    }
                }
            }
        }
        current_line++;
    }

    fclose(file);

    if (total_verses == 0)
    {
        return NULL;
    }

    int verse_num = rand() % total_verses + 1;
    for (int i = 1; i < MAX_BOOKS; i++)
    {
        if (book_counts[i] > 0)
        {
            if (verse_num <= book_counts[i])
            {
                int line_num = line_numbers[i] + verse_num;
                return get_nth_line(filename, line_num);
            }
            verse_num -= book_counts[i];
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    // Check for help flag first
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("Usage: %s [--version <version>] [--whitelist <book1> <book2> ...] [--blacklist <book1> <book2> ...]\n", argv[0]);
            printf("Options:\n");
            printf("  --version <version>    Specify the Bible version (default: kjv). Choices are [kjv, asv].\n");
            printf("  --whitelist <book1> <book2> ...\n");
            printf("                        List of book names to include (whitelist).\n");
            printf("  --blacklist <book1> <book2> ...\n");
            printf("                        List of book names to exclude (blacklist).\n");
            printf("  -l                    List all valid book names.\n");
            printf("  -h, --help             Show this help message and exit.\n");
            return EXIT_SUCCESS;
        }
    }

    // Check for list books option
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
        {
            for (int j = 0; j < MAX_BOOKS; j++)
            {
                printf("\"%s\"", BOOK_NAMES[j]);
                if (j + 1 < MAX_BOOKS)
                    printf(", ");
            }
            return EXIT_SUCCESS;
        }
    }

    srand(time(NULL));

    char *version = "kjv"; // Default translation
    char *whitelist[MAX_BOOKS] = {0};
    int whitelist_count = 0;
    char *blacklist[MAX_BOOKS] = {0};
    int blacklist_count = 0;

    int argi = 1;
    while (argi < argc)
    {
        if (strcmp(argv[argi], "--version") == 0 && argi + 1 < argc)
        {
            version = argv[++argi];
        }
        else if (strcmp(argv[argi], "--whitelist") == 0)
        {
            while (argi + 1 < argc && argv[argi + 1][0] != '-')
            {
                char *book = argv[++argi];
                if (get_book_number(book) != -1) // Is valid book name
                {
                    whitelist[whitelist_count++] = book;
                }
                else
                {
                    fprintf(stderr, "Error: Unknown book %s\n", book);
                    return EXIT_FAILURE;
                }
            }
        }
        else if (strcmp(argv[argi], "--blacklist") == 0)
        {
            while (argi + 1 < argc && argv[argi + 1][0] != '-')
            {
                char *book = argv[++argi];
                if (get_book_number(book) != -1) // Is valid book name
                {
                    blacklist[blacklist_count++] = book;
                }
                else
                {
                    fprintf(stderr, "Error: Unknown book %s\n", book);
                    return EXIT_FAILURE;
                }
            }
        }
        else
        {
            fprintf(stderr, "Error: Unknown argument %s\n", argv[argi]);
            return EXIT_FAILURE;
        }
        ++argi;
    }

    // Check for mutually exclusive arguments
    if (whitelist_count > 0 && blacklist_count > 0)
    {
        fprintf(stderr, "Error: You cannot use both whitelist and blacklist. Choose one.\n");
        return EXIT_FAILURE;
    }

    // Determine valid books based on whitelist or blacklist
    char *valid_books[MAX_BOOKS];
    int valid_books_count = 0;

    if (whitelist_count > 0)
    {
        for (int i = 0; i < whitelist_count; i++)
        {
            valid_books[valid_books_count++] = whitelist[i];
        }
    }
    else if (blacklist_count > 0)
    {
        for (int i = 1; i < MAX_BOOKS; i++)
        {
            int found = 0;
            for (int j = 0; j < blacklist_count; j++)
            {
                if (strcmp(BOOK_NAMES[i], blacklist[j]) == 0)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                valid_books[valid_books_count++] = (char *)BOOK_NAMES[i];
            }
        }
    }
    else
    {
        for (int i = 1; i < MAX_BOOKS; i++)
        {
            valid_books[valid_books_count++] = (char *)BOOK_NAMES[i];
        }
    }

    char filename[MAX_LINE_LENGTH];
    snprintf(filename, sizeof(filename), "%s/%s.txt", BASE_DIR, version);

    char *verse = get_random_verse(filename, valid_books, valid_books_count);

    if (verse != NULL)
    {
        printf("%s", verse);
    }
    else
    {
        fprintf(stderr, "No verse found.\n");
    }

    return EXIT_SUCCESS;
}
