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

int get_closest_record(MeteoData data, Date date, const char * city, Units units)
{
    for (int i = 0; i < data.rows - 1; i++)
    {
        MeteoRecord * row = &data.records[i];
//        if (strcmp(city, row->city) != 0)
//            continue;

    }
}


int compare_number(int a, int b)
{
    if (a > b) return 1;
    else if (a < b) return -1;
    else return 0;
}

int Date_Compare(Date* a, Date* b)
{
    int year = compare_number(a->year, b->year);
    if (year != 0) return year;

    int month = compare_number(a->month, b->month);
    if (month != 0) return month;

    compare_number(a->day, b->day);
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


char * get_next_days_prevision(HashTable* cities, char * city, Date date, Units units, int days_forward)
{
    HashNode* node = HashTable_GetNode(cities, city);
    MeteoData* data = (MeteoData*) node->data;
    int idx = retrieve_row_index_by_date(data, date);
    if (idx == -1)
    {
        // TODO
        return NULL;
    }

    for (int i = idx; i < idx+days_forward; i++)
    {

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

    HashNode* madrid_node = HashTable_GetNode(cities, "Madrid");
    MeteoData* madrid_data = (MeteoData* ) madrid_node->data;
    printf("--- Madrid ---\n");
    for (int i = 0; i < madrid_data->next_idx; i++)
    {
        MeteoRecord * row = &madrid_data->records[i];
        printf("%d/%d/%d temperatura: [%.2f, %.2f], precipitacion: %.2f, nubosidad: %d\n",
               row->date.day, row->date.month, row->date.year,
               row->min_temp, row->max_temp, row->precipitation,
               row->cloudiness);
    }


    Date date;
    date.day = 10;
    date.month = 10;
    date.year = 2020;
    int idx = retrieve_row_index_by_date(madrid_data, date);
    printf("idx: %d\n",idx);

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

//    MeteoData data = parse_csv_file(cities, f);


//    for (int i = 0; i < data.rows - 1; i++)
//    {
//        MeteoRecord * row = &data.records[i];
//        printf("%d/%d/%d %s -> [%.2f, %.2f], %.2f, %d\n",
//               row->date.day, row->date.month, row->date.year,
//               row->city,
//               row->min_temp, row->max_temp,
//               row->precipitation, row->cloudiness);

//    }



//    free(data.records);
//    return 0;
}
