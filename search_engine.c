#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct wordinfo
{
    char word[50];
    int count;
};

void clean_text(char text[], int size)
{
    printf("Insert the text: \n");
    fgets(text, size, stdin);

    for (char *p = text; *p; p++)
        *p = tolower((unsigned char)*p);

    char *read = text, *write = text;
    while (*read)
    {
        if (isalnum((unsigned char)*read) || isspace((unsigned char)*read))
        {
            *write++ = *read;
        }
        else if (*read == '\'')
        {
            if (isalpha((unsigned char)*(read - 1)) && isalpha((unsigned char)*(read + 1)))
                *write++ = *read;
        }
        read++;
    }
    *write = '\0';

    while (isspace((unsigned char)*text))
        text++;

    char *end = text + strlen(text) - 1;
    while (end > text && isspace((unsigned char)*end))
        *end-- = '\0';
}

void split_words(char *text, struct wordinfo dictionary[], int *total)
{
    char *token = strtok(text, " ");
    while (token != NULL)
    {
        if (strlen(token) > 0)
        {
            int found = 0;
            for (int i = 0; i < *total; i++)
            {
                if (strcmp(dictionary[i].word, token) == 0)
                {
                    dictionary[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                strcpy(dictionary[*total].word, token);
                dictionary[*total].count = 1;
                (*total)++;
            }
        }
        token = strtok(NULL, " ");
    }
}

void display_dictionary(struct wordinfo dictionary[], int total)
{
    for (int i = 0; i < total; i++)
    {
        if (dictionary[i].count == 1)
            printf("The word '%s' appeared one time\n", dictionary[i].word);
        else
            printf("The word '%s' appeared %d times\n", dictionary[i].word, dictionary[i].count);
    }
}

void search_word_exact(struct wordinfo dictionary[], int total)
{
    char search[50];
    printf("Enter word to search: ");
    scanf("%s", search);

    int found = 0;
    for (int i = 0; i < total; i++)
    {
        if (strcmp(dictionary[i].word, search) == 0)
        {
            printf("The word '%s' appeared %d time/s.\n", dictionary[i].word, dictionary[i].count);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("The word '%s' was not found.\n", search);
}

void search_word_partial(struct wordinfo dictionary[], int total)
{
    char search[50];
    printf("Enter partial word to search: ");
    scanf("%s", search);

    int found = 0;
    for (int i = 0; i < total; i++)
    {
        if (strstr(dictionary[i].word, search) != NULL)
        {
            printf("The word '%s' appeared %d time/s.\n", dictionary[i].word, dictionary[i].count);
            found = 1;
        }
    }

    if (!found)
        printf("No words containing '%s' were found.\n", search);
}

void sort_words_alph(struct wordinfo dictionary[], int total)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (strcmp(dictionary[j].word, dictionary[j + 1].word) > 0)
            {
                struct wordinfo temp = dictionary[j];
                dictionary[j] = dictionary[j + 1];
                dictionary[j + 1] = temp;
            }
        }
    }
    printf("\nWords sorted alphabetically:\n");
    for (int i = 0; i < total; i++)
        printf("%s : %d\n", dictionary[i].word, dictionary[i].count);
}

void sort_words_by_count(struct wordinfo dictionary[], int total, int ascending)
{
    for (int i = 0; i < total - 1; i++)
    {
        for (int j = 0; j < total - i - 1; j++)
        {
            if (ascending ? dictionary[j].count > dictionary[j + 1].count
                          : dictionary[j].count < dictionary[j + 1].count)
            {
                struct wordinfo temp = dictionary[j];
                dictionary[j] = dictionary[j + 1];
                dictionary[j + 1] = temp;
            }
        }
    }

    printf("\nWords sorted by frequency %s:\n", ascending ? "(ascending)" : "(descending)");
    for (int i = 0; i < total; i++)
        printf("%s : %d\n", dictionary[i].word, dictionary[i].count);
}

void statistics(struct wordinfo dictionary[], int total)
{
    if (total == 0)
    {
        printf("No words entered yet.\n");
        return;
    }

    int maxCount = dictionary[0].count;
    int minCount = dictionary[0].count;
    char *mostFrequent = dictionary[0].word;
    char *leastFrequent = dictionary[0].word;
    int sum = 0;

    for (int i = 0; i < total; i++)
    {
        sum += dictionary[i].count;
        if (dictionary[i].count > maxCount)
        {
            maxCount = dictionary[i].count;
            mostFrequent = dictionary[i].word;
        }
        if (dictionary[i].count < minCount)
        {
            minCount = dictionary[i].count;
            leastFrequent = dictionary[i].word;
        }
    }

    printf("Total words: %d\n", sum);
    printf("Unique words: %d\n", total);
    printf("Most frequent word: %s (%d times)\n", mostFrequent, maxCount);
    printf("Least frequent word: %s (%d times)\n", leastFrequent, minCount);
}

int are_anagrams(char a[], char b[])
{
    if (strlen(a) != strlen(b))
        return 0;

    int count[256] = {0};
    for (int i = 0; a[i]; i++)
    {
        count[(unsigned char)a[i]]++;
        count[(unsigned char)b[i]]--;
    }
    for (int i = 0; i < 256; i++)
    {
        if (count[i] != 0)
            return 0;
    }
    return 1;
}

void show_anagrams(struct wordinfo dictionary[], int total)
{
    printf("Anagram pairs in the text:\n");
    int found = 0;
    for (int i = 0; i < total; i++)
    {
        for (int j = i + 1; j < total; j++)
        {
            if (are_anagrams(dictionary[i].word, dictionary[j].word))
            {
                printf("%s <--> %s\n", dictionary[i].word, dictionary[j].word);
                found = 1;
            }
        }
    }
    if (!found)
        printf("No anagrams found.\n");
}

int is_palindrome(char word[])
{
    int left = 0;
    int right = strlen(word) - 1;
    while (left < right)
    {
        if (word[left] != word[right])
            return 0;
        left++;
        right--;
    }
    return 1;
}

void show_palindromes(struct wordinfo dictionary[], int total)
{
    printf("Palindromes in the text:\n");
    int found = 0;
    for (int i = 0; i < total; i++)
    {
        if (is_palindrome(dictionary[i].word))
        {
            printf("%s (%d times)\n", dictionary[i].word, dictionary[i].count);
            found = 1;
        }
    }
    if (!found)
        printf("No palindromes found.\n");
}

int main()
{
    char text[2000];
    struct wordinfo dictionary[2000];
    int total = 0, choice;

    do
    {
        printf("\n--- Menu ---\n");
        printf("1. Enter text\n2. Display words\n3. Search word (exact)\n");
        printf("4. Search word (partial)\n5. Sort words\n6. Text analyses \n0. Quit\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            clean_text(text, sizeof(text));
            split_words(text, dictionary, &total);
            break;
        case 2:
            display_dictionary(dictionary, total);
            break;
        case 3:
            search_word_exact(dictionary, total);
            break;
        case 4:
            search_word_partial(dictionary, total);
            break;
        case 5:
            printf("Sort by:\n1- Alphabetical\n2- Ascending Frequency\n3- Descending Frequency\nChoice: ");
            int sort_choice;
            scanf("%d", &sort_choice);
            getchar();
            if (sort_choice == 1)
                sort_words_alph(dictionary, total);
            else if (sort_choice == 2)
                sort_words_by_count(dictionary, total, 1);
            else if (sort_choice == 3)
                sort_words_by_count(dictionary, total, 0);
            else
                printf("Invalid choice!\n");
            break;

        case 6:
            printf("\nText analyses:\n");
            printf("1. Statistics\n2. Palindromes\n3. Anagrams\nChoice: ");
            int analysis_choice;
            scanf("%d", &analysis_choice);
            getchar();

            if (analysis_choice == 1)
                statistics(dictionary, total);
            else if (analysis_choice == 2)
                show_palindromes(dictionary, total);
            else if (analysis_choice == 3)
                show_anagrams(dictionary, total);
            else
                printf("Invalid choice!\n");
            break;

        case 0:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice! Try again.\n");
            break;
        }

    } while (choice != 0);

    return 0;
}
