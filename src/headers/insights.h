#ifndef INSIGHTS_H
#define INSIGHTS_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif //STDIO_H

typedef struct{
    float 
        mean,
        median,
        min,
        max;
} Statistics;

Statistics calculate_statistics(float dataset[], int length)
{
    Statistics summary = {.min = dataset[0], .max = 0};

    float sum = 0;

    for (int i = 0; i < length; i++)
    {
        // --- VALOR MÍNIMO ---
        if(dataset[i] < summary.min) summary.min = dataset[i];

        // --- VALOR MÁXIMO ---
        if(dataset[i] > summary.max) summary.max = dataset[i];

        sum += dataset[i];
    }

    summary.mean = sum/length;

    if(length%2 == 0) summary.median = (dataset[length/2] + dataset[(length/2) - 1])/2;
    else summary.median = dataset[length/2];
    
    return summary; 
} 

void print_summary(Statistics summary)
{
    printf("min: %.2f\nmax: %.2f\nmean: %.2f\nmedian: %.2f\n",  summary.min, summary.max, summary.mean, summary.median);
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

#endif // INSIGHTS_H