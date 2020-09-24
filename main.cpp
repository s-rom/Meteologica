#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "HashTable.h"
#include "MeteoData.h"
#include "Util.h"
#include "CSVParser.h"



int get_closest_record(MeteoData data, Date date, const char * city, Units units)
{
    for (int i = 0; i < data.rows - 1; i++)
    {
        MeteoRecord * row = &data.records[i];
//        if (strcmp(city, row->city) != 0)
//            continue;

    }

    return 0;
}


int retrieve_row_index_by_date(MeteoData* city, Date date)
{
    Date* mid_date;
    int start = 0, end = city->next_idx - 1;
    int mid = (start + end) / 2;


    while (start <= end)
    {
        mid_date = &city->records[mid].date;
        int compare = Date_Compare(mid_date, &date);

        if (compare < 0)
            start = mid + 1;
        else if (compare > 0)
            end = mid - 1;
        else
            break;

        mid = (start + end) / 2;
    }


    if (start > end)
        return -1;
    else
        return mid;
}


char * get_next_days_prevision(HashTable* cities, const char * city, Date date, Units units, int days_forward)
{
    HashNode* node = HashTable_GetNode(cities, city);
    if (node == NULL)
    {
        fprintf(stderr, "The city \"%s\" does not has any records\n", city);
        return NULL;
    }


    MeteoData* data = (MeteoData*) node->data;
    int idx = retrieve_row_index_by_date(data, date);
    if (idx == -1)
    {
        // TODO
        return NULL;
    }



    printf("-> Buscando previsión para %s, %d/%d: Próximos %d dias...\n",
           city, date.day, date.month, days_forward);
    for (int i = idx; i <= idx + days_forward && i < data->next_idx; i++)
    {
        MeteoRecord* row = &data->records[i];

        float minT = row->min_temp;
        float maxT = row->max_temp;
        if (units == FAHRENHEIT)
        {
            minT = celsius_to_fahrenheit(minT);
            maxT = celsius_to_fahrenheit(maxT);
        }

        printf("\t[%d/%d] Temperatura: [%.2f hasta %.2f]%s, precipitaciones: %.2fmm, nubosidad: %d\n",
               row->date.day, row->date.month,
               row->min_temp, row->max_temp,
               units == FAHRENHEIT ? "F" : "C",
               row->precipitation,
               row->cloudiness);
    }
}


void test_hash_table()
{
    HashTable* cities = HashTable_Make(5);

    const char * filePath = "data.csv";
    FILE * f = fopen(filePath, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filePath);
        return;
    }

    parse_csv_file(cities, f);

    Date date;
    date.day = 24;
    date.month = 9;
    date.year = 2020;
    get_next_days_prevision(cities, "Madrid", date, CELSIUS, 3);
}

int main(int argc, char *argv[])
{
    test_hash_table();

    return 0;
}
