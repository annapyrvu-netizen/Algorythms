#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// коды цвета и сброса цвета
#define CL_GREEN "\033[92m"
#define CL_RESET "\033[0m"

// размер буфера строки
#define LINE_SIZE 256

// нужно, чтобы проверить, что символ перед словом static
// разрешенный (не было int loop_static, где оно в имени переменной)
bool prev_symbol_allowed(char c)
{
    return c == '\0' || c == '\t' || c == ' ';
}

// обработка одной строки посимвольно
void process_line_symbolwise(char* line)
{
    // флаги, означающие, находимся ли мы 
    // сейчас в комментарии или строковом литерале
    bool in_com1 = false;
    bool in_str = false;
    // это делаем static, чтобы переменная сохраняла значение в разные вызовы
    // так как комментарий /* может быть открыт или закрыт на предыдущей строке
    static bool in_com2 = false;

    int i = 0;
    char prev = '\0';
    while (line[i] != '\0')
    {
        if (prev == '/' && line[i] == '/')
            in_com1 = true;

        if (prev == '/' && line[i] == '*')
            in_com2 = true;

        if (prev == '*' && line[i] == '/')
            in_com2 = false;

        if (line[i] == '"')
            in_str = !in_str;

        if (in_com1 || in_com2 || in_str)
        {
            printf("%c", line[i]);
            prev = line[i];
            i++;
            continue;
        }

        // strncmp сравнивает первые N символов у строк
        // "стоит ли на позиции i слово static с пробелом?"
        if (prev_symbol_allowed(prev) && strncmp(line + i, "static ", 7) == 0)
        {
            // начало цветной области
            printf(CL_GREEN);
            // идем до 2го пробела: static int abc; подсветим static int
            int k_spaces = 0;
            while (line[i] != '\0' && k_spaces < 2)
            {
                if (line[i] == ' ')
                {
                    k_spaces += 1;
                }
                printf("%c", line[i]);
                i++;
            }
            // отключаем подсветку
            printf(CL_RESET);
        }

        printf("%c", line[i]);
        prev = line[i];
        i++;
    }
}

int main()
{
    FILE* fp = fopen("static.c", "r");

    char line[LINE_SIZE];
    // [!] fgets кладет в конец строки символ \n!
    while (fgets(line, LINE_SIZE, fp) != NULL)
    {
        // вызов для каждой строки
        process_line_symbolwise(line);
    }
}
