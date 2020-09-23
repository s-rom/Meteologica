#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"


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



MeteoData * MeteoData_Make(int initial_rows)
{
    MeteoData* data = (MeteoData*) malloc(sizeof(MeteoData) * initial_rows);
    data->rows = initial_rows;
    return data;
}

MeteoData * MeteoData_AllocExtraRows(MeteoData * data, int extra_rows)
{
    int rows = data->rows;
    data = (MeteoData*) malloc(sizeof(MeteoData));
    data->rows = rows + extra_rows;
    data->records = (MeteoRecord*) malloc(sizeof(MeteoRecord) * data->rows);
    return data;
}


void MeteoData_InsertRecord(MeteoData* data, MeteoRecord record)
{
    if (data->next_idx >= data->rows)
    {
        MeteoData_AllocExtraRows(data, 10);
    }
    else
    {
        data->records[data->next_idx++] = record;
    }
}

enum TemperatureUnits
{
    CELSIUS, FAHRENHEIT
};



int count_rows(FILE* f)
{
    rewind(f);
    int lines = 0;
    char c;
    do
    {
        c = fgetc(f);
        if (c == '\n' || c == '\r' || c == EOF) lines++;
    }
    while(c != EOF);

    rewind(f);
    return lines;
}

int contains_comma(char * str_float)
{
    return (strchr(str_float, ',') == NULL)? 0 : 1;
}

float atof_comma(char * str_float)
{
    char * point_ptr = strchr(str_float, ',');
    if (point_ptr == NULL)
    {
        fprintf(stderr, "Error parsing float with comma %s." \
                        " The string doesn't contains a comma (\',\') character\n",
                str_float);
        return -1;
    }
    *point_ptr = '.';
    return (float) atof(str_float);
}

float parse_float(char * str_float)
{
    if (contains_comma(str_float))
        return atof_comma(str_float);
    else
        return atof(str_float);
}

void parse_date(char * str, int* day, int* month, int* year)
{
    char * tk = strtok(str, "/");
    *year = atoi(tk);

    tk = strtok(NULL, "/");
    *month = atoi(tk);

    tk = strtok(NULL, "/");
    *day = atoi(tk);
}

int get_next_token(FILE * f, char * token, size_t token_size, char separator)
{
    const int BUFFER_SIZE = token_size;
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

void print_token(char * token)
{
    if (token[0] == 0x00)
        printf("<empty> ");
    else
        printf("%s ",token);
}

float celsius_to_fahrenheit(float celsius)
{
    return celsius * (9.0f/5.0f) + 32;
}

void parse_csv_file(HashTable* cities, FILE* f)
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
        Date date;
        get_next_token(f, token, TK_SIZE, SEP);
        parse_date(token, &date.day, &date.month, &date.year);

        // Read city
        get_next_token(f, token, TK_SIZE, SEP);

        // Tries to get the node associated to that city
        HashNode* city_node = HashTable_GetNode(cities, token);

        // If not found, insert a new node with a new MeteoData for that city
        if (city_node == NULL)
        {
            MeteoData* city_data = MeteoData_Make(30);
            city_node = HashTable_InsertNode(cities, token, city_data);
        }

        MeteoRecord record;
        record.date = date;

        get_next_token(f, token, TK_SIZE, SEP);
        record.max_temp = parse_float(token);

        get_next_token(f, token, TK_SIZE, SEP);
        record.min_temp = parse_float(token);

        get_next_token(f, token, TK_SIZE, SEP);
        record.precipitation = parse_float(token);

        next_available = get_next_token(f, token, TK_SIZE, SEP);
        record.cloudiness = atoi(token);

        MeteoData_InsertRecord((MeteoData*)city_node->data, record);

        row++;
    }
}

int get_closest_record(MeteoData data, Date date, const char * city, TemperatureUnits units)
{
    for (int i = 0; i < data.rows - 1; i++)
    {
        MeteoRecord * row = &data.records[i];
//        if (strcmp(city, row->city) != 0)
//            continue;

    }
}


void test_hash_table()
{
    HashTable* cities = HashTable_Make(5);

    const char * filePath = "C:/Users/sergi/Desktop/Meteologica Prueba/Meteologica_vacante_ProgramadorC_20200901/Meteologica_vacante_ProgramadorC_20200901_datos.csv";
    FILE * f = fopen(filePath, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filePath);
        return;
    }

    parse_csv_file(cities, f);



}

int main(int argc, char *argv[])
{

    test_hash_table();
    return 0;

    const char * filePath = "C:/Users/sergi/Desktop/Meteologica Prueba/Meteologica_vacante_ProgramadorC_20200901/Meteologica_vacante_ProgramadorC_20200901_datos.csv";
    FILE * f = fopen(filePath, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", filePath);
        return -1;
    }

    int lines = count_rows(f);
    rewind(f);

    MeteoData data = parse_csv_file(f, lines);


    for (int i = 0; i < data.rows - 1; i++)
    {
        MeteoRecord * row = &data.records[i];
//        printf("%d/%d/%d %s -> [%.2f, %.2f], %.2f, %d\n",
//               row->date.day, row->date.month, row->date.year,
//               row->city,
//               row->min_temp, row->max_temp,
//               row->precipitation, row->cloudiness);

    }



    free(data.records);
    return 0;
}
