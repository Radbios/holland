#include <time.h>
#include "headers/insights.h"
#include "headers/holland.h"

#define LOOPS 100

int main(){
    float data[LOOPS];

    clock_t start, end;

    double cpu_time_used;

    start = clock();

    Population population;
    Population children;
    Population parents;

    srand(time(NULL));

    int generation = 0;

    for (int i = 0; i < LOOPS; i++)
    {
        population.generation = 0;
        population = init_population(population);
        population = fitness(population);
        population = population_sort(population);
        while (population.individuals[0].fitness != GEN_SIZE/2)
        {
            children.size = 0;
            parents.size = 0;

            // --- SELEÇÃO ---
            parents = select_parents(population, parents);
            // --- CRUZAMENTO ---
            children = crossover(parents, children);
            // --- MUTATION ---
            children = mutation(parents, children);
            // --- INVERSION ---
            // ???
            // --- FITNEES ---
            children = fitness(children);
            // --- SORT ---
            children = population_sort(children);
            // --- SUBSTITUTION ---
            population = substitution(population, children);
        }
        generation += population.generation;
        data[i] = population.generation;
    }

    // --- PRINTAR RESULTADO ---
    printf("-----------------------------------------\n");

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time: %f seconds\nGeneration: %d\n", cpu_time_used, generation);
    
    printf("-----------------------------------------\n");

    Statistics summary = calculate_statistics(data, LOOPS);
    print_summary(summary);
    save_data_in_file("data.csv", data, LOOPS);

    printf("-----------------------------------------\n");
    // int i, j;
    // for (i = 0; i < POP_SIZE; i++)
    // {
    //     for (j = 0; j < GEN_SIZE; j++)
    //     {
    //         printf("%d ", population.individuals[i].genes[j]);
    //     } 
    //     printf("Value : %d\n", population.individuals[i].fitness);
    // }
    // ---
    return 0;
}