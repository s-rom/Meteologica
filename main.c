#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "HashTable.h"
#include "MeteoData.h"
#include "Util.h"
#include "CSVParser.h"
#include "JSON.h"


/**
 * Retrieves a row from a MeteoData (associated with a city) and a date.
 * This functions assumes that the MeteoData records are sorted ascending by date.
 * Implements binary search (O(log n)).
 * @param city
 * @param date
 * @return index to the row
 */
int retrieve_row_index_by_date(struct MeteoData* city, struct Date date)
{
    struct Date* mid_date;
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

/**
 * Returns a json formatted string that contains days_forward + 1 records starting from date parameter.
 * @param cities
 * @param city
 * @param date
 * @param units
 * @param days_forward how many days it will look for
 * @return json buffer, must be freed by caller
 */
char * get_next_days_prevision(struct HashTable* cities, const char * city,
                               struct  Date date, enum Units units, int days_forward)
{
    struct HashNode* node = HashTable_GetNode(cities, city);
    if (node == NULL)
    {
        fprintf(stderr, "The city \"%s\" does not has any records\n", city);
        return NULL;
    }


    struct MeteoData* data = (struct MeteoData*) node->data;
    int idx = retrieve_row_index_by_date(data, date);
    if (idx == -1)
    {
        // TODO
        return NULL;
    }

    size_t BUFF_SIZE = 2048; // initial size
    char * json_buff = (char*) malloc(BUFF_SIZE);
    JSON_Start(json_buff, &BUFF_SIZE, city, units);


    const int MAX_ROWS = idx + days_forward;
    for (int i = idx; i <= MAX_ROWS && i < data->next_idx; i++)
    {
        struct MeteoRecord* row = &data->records[i];
        JSON_AddRecord(json_buff, &BUFF_SIZE, row, units,
                       (i+1 <= MAX_ROWS && i+1 < data->next_idx));
    }

    JSON_End(json_buff, &BUFF_SIZE);
    return json_buff;
}


/**
 * Given a html template containing &JSON&, generates a second file
 * replacing the token with the obtained json data.
 * @param templatePath html file containing the string &JSON&
 * @param outPath html file with JSON data
 * @param json_buff json raw data
 */
void generate_html(const char * templatePath, const char * outPath, char * json_buff)
{
    FILE * template_f = fopen(templatePath, "r");
    FILE * out_f = fopen(outPath, "w");


    char buffer[4096];
    fscanf(template_f, "%[^&]", buffer);
    fprintf(out_f, "%s",buffer);

    // &JSON&
    fgetc(template_f);
    fscanf(template_f, "%[^&]", buffer);
    fgetc(template_f);
    fscanf(template_f, "%[^&]", buffer);

    fprintf(out_f, "%s", json_buff);
    fprintf(out_f, "%s", buffer);
    fclose(template_f);
    fclose(out_f);
}



int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        fprintf(stderr, "Format: .exe City YYYY/MM/DD [F|C] CSV_FilePath\n");
        return 0;
    }

    /* Requested city */
    char * city = argv[1];

    /* Requested date */
    char * date_str = argv[2];
    struct Date date;
    parse_date(date_str, &date.day, &date.month, &date.year);

    /* Requested units */
    char * units_str = argv[3];
    enum Units units = (units_str[0] == 'F' ? FAHRENHEIT : CELSIUS);


    const char * csv_file_path = "data.csv";
    if (argc > 4)
    {
        csv_file_path = argv[4];
    }

    int days_forward = 6; // default: week including requested data

    struct HashTable* cities = HashTable_Make(5);

    FILE * f = fopen(csv_file_path, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Could not open file %s\n", csv_file_path);
        return 0;
    }

    parse_csv_file(cities, f);
    char * json = get_next_days_prevision(cities, city, date, units, days_forward);

    printf("Peticion: %s, %d/%d/%d, %s", city,
           date.day, date.month, date.year,
           units == FAHRENHEIT? "Fahrenheit":"Celsius");

    printf("\n\n=== Resultado === \n%s\n",json);

    const char * json_out_path = "result.json";
    const char * html_out_path = "index.html";
    const char * html_template_path = "template.html";
    FILE * fout = fopen("resultado.json", "w");
    fprintf(fout, "%s", json);

    printf("\n-->Generated %s", json_out_path);
    printf("\n-->Generated %s\n", html_out_path);

    generate_html(html_template_path, html_out_path, json);

    free(json);

    return 0;
}
