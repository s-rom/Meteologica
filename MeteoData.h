/*
=== MeteoData library ===
@author Sergi

    -> Implements a data structure to store and fast retrieve meteo data
*/

#ifndef METEODATA_H
#define METEODATA_H

#include <stdlib.h>

struct Date
{
    int day, month, year;
};


struct MeteoRecord
{
    struct Date date;           // Date
    float max_temp, min_temp;   // Celsius
    float precipitation;        // mm
    int cloudiness;             // %

};


struct MeteoData
{
    struct MeteoRecord * records; // Array of records
    int next_idx;                 // Next index to push back
    int rows;                     // Number of records available
};


/**
 * Creates a MeteoData struct with an initial capacity.
 * @param initial_rows
 * @return the pointer to the created struct.
 */
struct MeteoData * MeteoData_Make(int initial_rows)
{
    struct MeteoData* data = (struct MeteoData*) malloc(sizeof(struct MeteoData));
    data->records = (struct MeteoRecord*) malloc(sizeof(struct MeteoRecord) * initial_rows);
    data->rows = initial_rows;
    data->next_idx = 0;
    return data;
}

/**
 * Releases memory associated to a MeteoData pointer.
 * @param data
 */
void MeteoData_Free(struct MeteoData * data)
{
    free(data->records);
    free(data);
}

/**
 * Allocates extra memory for the MeteoData struct.
 * @param data pointer to initialized MeteoData
 * @param extra_rows
 * @return pointer to the data parameter
 */
struct MeteoData * MeteoData_AllocExtraRows(struct MeteoData * data, int extra_rows)
{
    int rows = data->rows;
    data->rows = rows + extra_rows;
    data->records = (struct MeteoRecord*) realloc(data->records, sizeof(struct MeteoRecord) * data->rows);
    return data;
}


/**
 * Inserts back a new record to a MeteoData struct.
 * @param data pointer to initialized MeteoData
 * @param record
 */
void MeteoData_InsertRecord(struct MeteoData* data, struct MeteoRecord record)
{
    if (data->next_idx >= data->rows)
    {
        MeteoData_AllocExtraRows(data, 10);
    }

     data->records[data->next_idx++] = record;

}

enum Units
{
    CELSIUS, FAHRENHEIT
};




#endif // METEODATA_H
