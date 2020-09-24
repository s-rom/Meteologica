#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "MeteoData.h"

char * trim_space(char *str) {
    char * ptr = str;
    char * end;

    while (isspace(*ptr))
       ptr++;

    end = ptr + strlen(ptr) - 1;
    while (end > ptr && isspace(*end))
        end--;

    *(end+1) = '\0';
    return ptr;
}

int count_rows(FILE* f)
{
    rewind(f);
    int lines = 0;
    char c;
    do
    {
        c = fgetc(f);
        if (c == '\n' || c == '\r' || c == EOF) lines++;
    }
    while(c != EOF);

    rewind(f);
    return lines;
}

int contains_comma(char * str_float)
{
    return (strchr(str_float, ',') == NULL)? 0 : 1;
}

float atof_comma(char * str_float)
{
    char * point_ptr = strchr(str_float, ',');
    if (point_ptr == NULL)
    {
        fprintf(stderr, "Error parsing float with comma %s." \
                        " The string doesn't contains a comma (\',\') character\n",
                str_float);
        return -1;
    }
    *point_ptr = '.';
    return (float) atof(str_float);
}

float parse_float(char * str_float)
{
    if (contains_comma(str_float))
        return atof_comma(str_float);
    else
        return atof(str_float);
}

void parse_date(char * str, int* day, int* month, int* year)
{
    char * tk = strtok(str, "/");
    *year = atoi(tk);

    tk = strtok(NULL, "/");
    *month = atoi(tk);

    tk = strtok(NULL, "/");
    *day = atoi(tk);
}

float celsius_to_fahrenheit(float celsius)
{
    return celsius * (9.0f/5.0f) + 32;
}



int compare_number(int a, int b)
{
    if (a > b) return 1;
    else if (a < b) return -1;
    else return 0;
}

int Date_Compare(Date* a, Date* b)
{
    int year = compare_number(a->year, b->year);
    if (year != 0) return year;

    int month = compare_number(a->month, b->month);
    if (month != 0) return month;

    compare_number(a->day, b->day);
}

#endif // UTIL_H
