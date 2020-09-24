#ifndef METEODATA_H
#define METEODATA_H

#include <stdlib.h>

struct Date
{
    int day, month, year;
};


#define DATE_MSK  0x10
#define MAXT_MSK  0x08
#define MINT_MSK  0x04
#define PREC_MSK  0x02
#define CLOUD_MSK 0x01

struct MeteoRecord
{
    Date date;                  // Date
    float max_temp, min_temp;   // Celsius
    float precipitation;        // mm
    int cloudiness;             // %

    unsigned int valid = 0x00;  // Bitmap
};

struct MeteoData
{
    MeteoRecord * records;      // Array of records
    int next_idx = 0;           // Next index to push back
    int rows = 0;               // Number of records available
};



MeteoData * MeteoData_Make(int initial_rows)
{
    MeteoData* data = (MeteoData*) malloc(sizeof(MeteoData));
    data->records = (MeteoRecord*) malloc(sizeof(MeteoRecord) * initial_rows);
    data->rows = initial_rows;
    data->next_idx = 0;
    return data;
}

void MeteoData_Free(MeteoData * data)
{
    free(data->records);
    free(data);
}

MeteoData * MeteoData_AllocExtraRows(MeteoData * data, int extra_rows)
{
    int rows = data->rows;
    data->rows = rows + extra_rows;
    data->records = (MeteoRecord*) realloc(data->records, sizeof(MeteoRecord) * data->rows);
    return data;
}


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
