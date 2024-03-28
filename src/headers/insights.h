#ifndef INSIGHTS_H
#define INSIGHTS_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif //STDIO_H

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif //STRING_H

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif //MATH_H

typedef struct{
    float 
        sum,
        mean,
        median,
        var,
        dp,
        min,
        max;
} Statistics;

float calculate_dp(float var)
{
    return pow(var, 2);
}

float calculate_variance(float dataset[], int length, float median)
{
    float var = 0;
    for (int i = 0; i < length; i++)
    {
        var += pow(dataset[i] - median, 2);
    }

    return var = var/length;
}

Statistics calculate_statistics(float dataset[], int length)
{
    Statistics summary = {.sum = 0, .min = dataset[0], .max = 0};

    for (int i = 0; i < length; i++)
    {
        // --- VALOR MÍNIMO ---
        if(dataset[i] < summary.min) summary.min = dataset[i];

        // --- VALOR MÁXIMO ---
        if(dataset[i] > summary.max) summary.max = dataset[i];

        summary.sum += dataset[i];
    }

    summary.mean = summary.sum/length;
    summary.var = calculate_variance(dataset, length, summary.mean);
    summary.dp = calculate_dp(summary.var);

    if(length%2 == 0) summary.median = (dataset[length/2] + dataset[(length/2) - 1])/2;
    else summary.median = dataset[length/2];
    
    return summary; 
} 

void print_summary(Statistics summary)
{
    printf("min: %.2f\nmax: %.2f\nmean: %.2f\nmedian: %.2f\nvariance: %.2f\ndp: %.2f\n",  summary.min, summary.max, summary.mean, summary.median, summary.var, summary.dp);
}

void save_data_in_file(char *filename, float dataset[], int length)
{
    FILE *file;

    file = fopen(filename, "w");

    for (size_t i = 0; i < length; i++)
    {
        fprintf(file, "%zu %.2f\n", i, dataset[i]);
    }
    
    fclose(file);
}

// void export_data(char *filename, float *dataset, char *headers, int rows, int columns, char *separator)
// {
//     FILE *file;

//     file = fopen(filename, "w");

//     char *tok = strtok(headers, ";");

//     while (tok != NULL) {
//         fprintf(file, "%s%s", tok, separator);
//         tok = strtok(NULL, ";");
//     }

//     fprintf(file, "\n");

//     for (size_t i = 0; i < rows; i++)
//     {
//         for (size_t j = 0; j < columns; j++)
//         {
//             fprintf(file, "%.2f%s", dataset[i * columns + j], separator);
//         }
//         fprintf(file, "\n");
//     }

//     fclose(file);
// }

#endif // INSIGHTS_H