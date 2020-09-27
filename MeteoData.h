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
    Date date;                  // Date
    float max_temp, min_temp;   // Celsius
    float precipitation;        // mm
    int cloudiness;             // %

};


struct MeteoData
{
    MeteoRecord * records;      // Array of records
    int next_idx = 0;           // Next index to push back
    int rows = 0;               // Number of records available
};


/**
 * Creates a MeteoData struct with an initial capacity.
 * @param initial_rows
 * @return the pointer to the created struct.
 */
MeteoData * MeteoData_Make(int initial_rows)
{
    MeteoData* data = (MeteoData*) malloc(sizeof(MeteoData));
    data->records = (MeteoRecord*) malloc(sizeof(MeteoRecord) * initial_rows);
    data->rows = initial_rows;
    data->next_idx = 0;
    return data;
}

/**
 * Releases memory associated to a MeteoData pointer.
 * @param data
 */
void MeteoData_Free(MeteoData * data)
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
MeteoData * MeteoData_AllocExtraRows(MeteoData * data, int extra_rows)
{
    int rows = data->rows;
    data->rows = rows + extra_rows;
    data->records = (MeteoRecord*) realloc(data->records, sizeof(MeteoRecord) * data->rows);
    return data;
}


/**
 * Inserts back a new record to a MeteoData struct.
 * @param data pointer to initialized MeteoData
 * @param record
 */
void MeteoData_InsertRecord(MeteoData* data, MeteoRecord record)
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
