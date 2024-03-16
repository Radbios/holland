#include <time.h>
#include "headers/insights.h"
#include "headers/holland.h"

#define LOOPS 100

#define GRAPH 0

#define FILENAME "execution_x_generation2"
#define TITLE "Execution x Generation (without rules)"
#define X_LABEL "Execution"
#define Y_LABEL "Generation"

#define X_RANGE 1850
#define Y_RANGE 950

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

        // INICIAR PROGRESSO
        printf("\x1b[32mprocess in: %.2f%%\x1b[0m\n", ((float)i/LOOPS)*100);

        while (population.individuals[0].fitness != GEN_SIZE/2)
        {
            children.size = 0;
            parents.size = 0;

            // --- SELEÇÃO ---
            parents = select_parents(population, parents, POP_SIZE/2);

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

        printf("\x1b[A"); // MOVER PONTEIRO PARA CIMA

        // MOSTRAR PROGRESSO
        printf("\x1b[32mprocess in: %.2f%%\x1b[0m\n", ((float)i/LOOPS)*100);

        printf("\x1b[A");// MOVER PONTEIRO PARA CIMA
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

    if(GRAPH)
    {
        FILE *gnuplot = popen("gnuplot", "w");
        fprintf(gnuplot, "set title '%s'\n", TITLE);
        fprintf(gnuplot, "set xlabel '%s'\n", X_LABEL);
        fprintf(gnuplot, "set ylabel '%s'\n", Y_LABEL);
        fprintf(gnuplot, "set label 'GEN SIZE: %d' at graph 0.01, graph 0.96 left\n", GEN_SIZE);
        fprintf(gnuplot, "set label 'POP SIZE: %d' at graph 0.01, graph 0.93 left\n", POP_SIZE);
        fprintf(gnuplot, "set label 'CROSS RATE: %d' at graph 0.01, graph 0.90 left\n", CROSS_RATE);
        fprintf(gnuplot, "set label 'MUT RATE: %d' at graph 0.01, graph 0.87 left\n", MUT_RATE);
        fprintf(gnuplot, "set label 'LOOPS: %d' at graph 0.01, graph 0.84 left\n", LOOPS);

        fprintf(gnuplot, "set label 'SUM: %.2f' at graph 0.1, graph 0.96 left\n", summary.sum);
        fprintf(gnuplot, "set label 'MIN: %.2f' at graph 0.1, graph 0.93 left\n", summary.min);
        fprintf(gnuplot, "set label 'MAX: %.2f' at graph 0.1, graph 0.90 left\n", summary.max);
        fprintf(gnuplot, "set label 'MEAN: %.2f' at graph 0.1, graph 0.87 left\n", summary.mean);
        fprintf(gnuplot, "set label 'MEDIAN: %.2f' at graph 0.1, graph 0.84 left\n", summary.median);

        fprintf(gnuplot, "set label 'TIME: %.2f seconds' at graph 0.2, graph 0.96 left\n", cpu_time_used);

        fprintf(gnuplot, "unset key\n");
        fprintf(gnuplot, "set terminal png size %d,%d\n", X_RANGE, Y_RANGE);
        fprintf(gnuplot, "set output 'graphs/GEN%d_POP%d_CROSS%d_MUT%d_LOOPS%d.png\n", GEN_SIZE, POP_SIZE, CROSS_RATE, MUT_RATE, LOOPS);
        fprintf(gnuplot, "plot 'data.csv' w lp\n");
        fflush(gnuplot);
        fclose(gnuplot);
    }

    return 0;
}