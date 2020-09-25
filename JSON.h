#ifndef JSON_H
#define JSON_H

#include "MeteoData.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Util.h"

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

void JSON_AddRecord(char * buff, size_t* BUFF_SIZE, MeteoRecord* row, Units temp_units, int last_comma)
{

    /*
     ,{...}

     */
    const char * COMMA = ",";
    const char * UNITS_STR = temp_units == CELSIUS ? "Celsius" : "Fahrenheit";

    char* aux = (char *)malloc(2048);


    float minT = row->min_temp;
    float maxT = row->max_temp;
    if (temp_units == FAHRENHEIT)
    {
        celsius_to_fahrenheit(&minT);
        celsius_to_fahrenheit(&maxT);
    }

    sprintf(aux, "{\"Date\":\"%d-%d-%d\", \"MinTemp\": %.2f, \"MaxTemp\": %.2f, \"TempUnits\":\"%s\", \"Precipitation\": %.2f, \"Cloudiness\": %d}%s",
            row->date.year, row->date.month, row->date.day,
            minT, maxT, UNITS_STR,
            row->precipitation, row->cloudiness,
            last_comma ? COMMA : " ");

    JSON_Concat(buff, BUFF_SIZE, aux);

    free(aux);
}

void JSON_Start(char * buff, size_t* BUFF_SIZE, const char * city)
{
    /*
     {
        "City": city,
        "Records": [
    */

    char* aux = (char *)malloc(256);
    snprintf(aux, 256, "{ \"City\":\"%s\", \"Records\":[", city);
    strcpy(buff, aux);
    free(aux);
}

void JSON_End(char * buff, size_t* BUFF_SIZE)
{
    /*

      ]}

     */
    JSON_Concat(buff, BUFF_SIZE, "]}");

}


#endif // JSON_H
