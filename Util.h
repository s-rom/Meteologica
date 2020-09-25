#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "MeteoData.h"

/**
 * Removes the whitespace surrounding a string (left and right).
 *
 * @param str pointer to allocated string, it will be modified by
 * placing a null terminator next to the last non space character
 * @return pointer to first non space character of str
 */
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

/**
 * Counts all the rows (lines) in a file.
 * @param f pointer to an opened file
 * @return total number of lines in the file
 */
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

/**
 * Checks if the string contains a comma character (',')
 * @param str_float
 * @return 1 if parameter contains a comma, 0 otherwise
 */
int contains_comma(char * str_float)
{
    return (strchr(str_float, ',') == NULL)? 0 : 1;
}

/**
 * Computes the float value of a given string formated with a comma
 * (ex: 0,23) instead of a point (ex: 0.23)
 * @param str_float
 * @return float value
 */
float atof_comma(char * str_float)
{
    char * point_ptr = strchr(str_float, ',');
    if (point_ptr == NULL)
    {
        fprintf(stderr, "Error parsing float with comma %s." \
                        " The string doesn't contains a comma (\',\') character. Use atof instead\n",
                str_float);
        return -1;
    }
    *point_ptr = '.';
    return (float) atof(str_float);
}

/**
 * Computes the float value of a give string formated with a comma
 * (Ex: 0,23) or with a point (Ex: 0.23)
 * @param str_float
 * @return float value
 */
float parse_float(char * str_float)
{
    if (contains_comma(str_float))
        return atof_comma(str_float);
    else
        return atof(str_float);
}

/**
 * Parses a date given as a string formated as: YYYY/MM/DD
 * @param str
 * @param day
 * @param month
 * @param year
 */
void parse_date(char * str, int* day, int* month, int* year)
{
    char * tk = strtok(str, "/");
    *year = atoi(tk);

    tk = strtok(NULL, "/");
    *month = atoi(tk);

    tk = strtok(NULL, "/");
    *day = atoi(tk);
}

/**
 * Celsius to Fahrenheit. Function version.
 * @param celsius value
 * @return fahrenheit value
 */
float celsius_to_fahrenheit(float celsius)
{
    return celsius * (9.0f/5.0f) + 32;
}


/**
 * Celsius to Fahrenheit. Procedure version.
 * @param t temperature in celsius
 */
void celsius_to_fahrenheit(float *t)
{
    *t = (*t) * (9.0f/5.0f) + 32;
}


/**
 * Compares two numbers in a similar way to strcmp
 * @param a first number
 * @param b second number
 * @return a > b : 1
 *         a < b: -1
 *         a = b: 0
 */
int compare_number(int a, int b)
{
    if (a > b) return 1;
    else if (a < b) return -1;
    else return 0;
}

/**
 * Compares two dates in a similar way to strcmp
 * @param a first date
 * @param b second date
 * @return a > b : 1
 *         a < b: -1
 *         a = b: 0
 */
int Date_Compare(Date* a, Date* b)
{
    int year = compare_number(a->year, b->year);
    if (year != 0) return year;

    int month = compare_number(a->month, b->month);
    if (month != 0) return month;

    compare_number(a->day, b->day);
}

#endif // UTIL_H
