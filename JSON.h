#ifndef JSON_H
#define JSON_H

#include "MeteoData.h"

void JSON_AddDate(char * buff, Date* date)
{
    /*
     "Date": date
     */
}

void JSON_AddRecord(char * buff, MeteoRecord* row)
{

    /*
     ,{...}

     */

}

void JSON_Start(char * buff, char * city)
{
    /*
     {
        "City": city,
        "Records": [


     */
}

void JSON_End(char * buff)
{

    /*

      ]}

     */
}


#endif // JSON_H
