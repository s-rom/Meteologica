#ifndef JSON_H
#define JSON_H

#include "MeteoData.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void JSON_AddRecord(char * buff, size_t* BUFF_SIZE, MeteoRecord* row, int last_comma)
{

    /*
     ,{...}

     */

    char* aux = (char *)malloc(1024);
    sprintf(aux, "{ \"MinTemp\": %.2f, \"MaxTemp\": %.2f, \"TempUnits\":\"%s\", \"Precipitation\": %.2f, \"Cloudiness\": %d}",
             row->min_temp, row->max_temp, "celsius", row->precipitation, row->cloudiness);

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

void JSON_End(char * buff)
{

    /*

      ]}

     */
}


#endif // JSON_H
