#ifndef JSON_H
#define JSON_H

/*
=== JSON generator for MeteoData===
@author Sergi

    -> Implements a system that generates JSON data for MeteoData.
       Uses string reallocation.

    -> Example of use:

        An initial buffer must be allocated (with an arbitrary initial size).

        size_t BUFF_SIZE = 1024;
        char * json_buffer = (char *) malloc(BUFF_SIZE);

        JSON_Start(json_buffer, &BUFF_SIZE, "Madrid");
        JSON_AddRecord(json_buffer, &BUFF_SIZE, row_ptr1, CELSIUS, 1);
        JSON_AddRecord(json_buffer, &BUFF_SIZE, row_ptr2, CELSIUS, 1);
        JSON_AddRecord(json_buffer, &BUFF_SIZE, row_ptr3, CELSIUS, 0);
        ...
        JSON_End(json_buffer, &BUFF_SIZE);

        >> Use json_buffer

        free(json_buffer);
*/


#include "MeteoData.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

/**
 * Concatenates a string to a buffer. If the buffer cannot hold
 * the entire final string, the buffer will be assigned with extra
 * memory.
 * @param buff must be pre-allocated
 * @param BUFF_SIZE current size of buff
 * @param src string to be concatenated
 */
void JSON_Concat(char * buff, size_t* BUFF_SIZE, const char * src)
{
    int src_len = strlen(src);
    int buff_len = strlen(buff);
    if (buff_len + src_len >= *BUFF_SIZE)
    {
        *BUFF_SIZE = buff_len + src_len;
        buff = (char*) realloc(buff, *BUFF_SIZE);
    }

    strcat(buff, src);
}


/**
 * Concatenates to a buffer the information corresponding to a MeteoRecord
 * @param buff must be pre-allocated
 * @param BUFF_SIZE current size of buff
 * @param row pointer to the record
 * @param temp_units desired units for temperature limits
 * @param last_comma whether a comma should be appended or not
 */
void JSON_AddRecord(char * buff, size_t* BUFF_SIZE, struct MeteoRecord* row, enum Units temp_units, int last_comma)
{

    const char * COMMA = ",";
    char* aux = (char *)malloc(2048);


    float minT = row->min_temp;
    float maxT = row->max_temp;
    if (temp_units == FAHRENHEIT)
    {
        celsius_to_fahrenheit(&minT);
        celsius_to_fahrenheit(&maxT);
    }

    sprintf(aux, "{\"Date\":\"%d-%d-%d\", \"MinTemp\": %.2f, \"MaxTemp\": %.2f, \"Precipitation\": %.2f, \"Cloudiness\": %d}%s",
            row->date.year, row->date.month, row->date.day,
            minT, maxT,
            row->precipitation, row->cloudiness,
            last_comma ? COMMA : " ");

    JSON_Concat(buff, BUFF_SIZE, aux);

    free(aux);
}

/**
 * Concatenates to a buffer the initial json data associated to a city.
 * @param buff must be pre-allocated
 * @param BUFF_SIZE current size of buff
 * @param city
 */
void JSON_Start(char * buff, size_t* BUFF_SIZE, const char * city, enum Units units)
{
    /*
     {
        "City": city,
        "Records": [
    */
    const char *units_str = units == FAHRENHEIT ? "Fahrenheit" : "Celsius";
    char* aux = (char *)malloc(256);
    snprintf(aux, 256, "{ \"City\":\"%s\", \"TemperatureUnits\":\"%s\", \"Records\":[",
             city, units_str);
    strcpy(buff, aux);
    free(aux);
}

/**
 * Concatenates to a buffer the final necessary json data.
 * @param buff must be pre-allocated
 * @param BUFF_SIZE current size of buff
 */
void JSON_End(char * buff, size_t* BUFF_SIZE)
{
    JSON_Concat(buff, BUFF_SIZE, "]}");
}


#endif // JSON_H
