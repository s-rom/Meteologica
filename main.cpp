#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "HashTable.h"
#include "MeteoData.h"
#include "Util.h"
#include "CSVParser.h"
#include "JSON.h"


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

    size_t BUFF_SIZE = 2048; // initial size
    char * json_buff = (char*) malloc(BUFF_SIZE);
    JSON_Start(json_buff, &BUFF_SIZE, city);


    const int MAX_ROWS = idx + days_forward;
    for (int i = idx; i <= MAX_ROWS && i < data->next_idx; i++)
    {
        MeteoRecord* row = &data->records[i];
        JSON_AddRecord(json_buff, &BUFF_SIZE, row, units,
                       (i+1 <= MAX_ROWS && i+1 < data->next_idx));
    }

    JSON_End(json_buff, &BUFF_SIZE);
    return json_buff;
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
    char * json = get_next_days_prevision(cities, "Valencia", date, FAHRENHEIT, 10);
    printf("%s\n",json);
    free(json);
}

int main(int argc, char *argv[])
{
    test_hash_table();
    return 0;
}
