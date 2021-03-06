#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "HashTable.h"
#include "MeteoData.h"
#include "Util.h"
#include <stdio.h>


/**
 * Parses the next token from the csv file.
 * @param f must be opened.
 * @param token
 * @param token_size
 * @param separator
 * @return
 */
int get_next_token(FILE * f, char * token, size_t token_size, char separator)
{
    const size_t BUFFER_SIZE = 256;
    const char NEWLINE = '\n';
    const char CR = '\r';

    char buffer[BUFFER_SIZE];
    int buff_idx = 0;
    memset(buffer, 0x00, BUFFER_SIZE);

    char c = fgetc(f);
    while(c != NEWLINE && c != CR && c != separator && c != EOF)
    {
        buffer[buff_idx++] = c;
        c = fgetc(f);
    }

    strcpy(token, buffer);
    return c != EOF;
}

/**
 * Parses a csv file and stores the content on a HashTable indexed by city.
 * File format must be:
 *  HEADER
 *  YYYY/MM/DD;City;MaxTemp;MinTemp;Precipitation;Cloudiness
 * @param cities
 * @param f must be opened
 */
void parse_csv_file(struct HashTable* cities, FILE* f)
{
    // Skip first
    fscanf(f, "%*[^\n\r][\n\r]");
    fgetc(f);

    const size_t TK_SIZE = 256;
    char token[TK_SIZE];

    const char SEP = ';';
    int next_available = 1;
    int row = 0;
    while(next_available)
    {
        // Read date
        struct Date date;
        get_next_token(f, token, TK_SIZE, SEP);
        parse_date(token, &date.day, &date.month, &date.year);

        // Read city
        get_next_token(f, token, TK_SIZE, SEP);
        char * trimmed_tk = trim_space(token);

        // Tries to get the node associated to that city
        struct HashNode* city_node = HashTable_GetNode(cities, trimmed_tk);

        // If not found, insert a new node with a new MeteoData for that city
        if (city_node == NULL)
        {
            struct MeteoData* city_data = MeteoData_Make(30);
            city_node = HashTable_InsertNode(cities, trimmed_tk, city_data);
        }

        struct MeteoRecord record;
        record.date = date;

        get_next_token(f, token, TK_SIZE, SEP);
        record.max_temp = parse_float(token);

        get_next_token(f, token, TK_SIZE, SEP);
        record.min_temp = parse_float(token);

        get_next_token(f, token, TK_SIZE, SEP);
        record.precipitation = parse_float(token);

        next_available = get_next_token(f, token, TK_SIZE, SEP);
        record.cloudiness = atoi(token);

        MeteoData_InsertRecord((struct MeteoData*)city_node->data, record);

        row++;
    }
}


#endif // CSVPARSER_H
