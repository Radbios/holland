#ifndef HOLLAND_H
#define HOLLAND_H

#define GEN_SIZE 8
#define POP_SIZE 10
#define MUT_RATE 80
#define CROSS_RATE 70

#ifndef STDLIB_H
#include <stdlib.h>
#define STDLIB_H
#endif

// --- STRUCTS ---
typedef struct{
    int genes[GEN_SIZE];
    int fitness;
} Individual;

typedef struct{
    Individual individuals[POP_SIZE];
    int size;
    int generation;
} Population;

// ---

// --- CHECAR ADAPTAÇÃO ---
Population fitness(Population population){
    int i, j;
    for(j = 0; j < population.size; j++){
        population.individuals[j].fitness = 0;
        for (i = 0; i < GEN_SIZE - 1; i++){
            if(population.individuals[j].genes[i] == 0 && population.individuals[j].genes[i+1] == 1){
                population.individuals[j].fitness++;
            }
        }
    }
    return population;
}
// ---

// --- INICIAR UMA POPULAÇÃO (Dar valores [0 & 1]) ---
Population init_population(Population population){
    int i, j;
    population.size = POP_SIZE;
    for (i = 0; i < POP_SIZE; i++){
        population.individuals[i].fitness = 0;
        for (j = 0; j < GEN_SIZE; j++){
            population.individuals[i].genes[j] = rand() % 2;
        }
    }
    return population;
}
// ---

// --- INICIAR SCHEMA ---

Individual load_schema(Population population, Individual schema, int length)
{
    schema.fitness = 0;
    int i, j;
    for (i = 0; i < GEN_SIZE; i++)
    {
        schema.genes[i] = population.individuals[0].genes[i];
        for (j = 0; j < length; j++)
        {
            if(schema.genes[i] != population.individuals[j].genes[i])
            {
                schema.genes[i] = -1;
                break;
            }
            else if(j == length - 1) schema.genes[i] = population.individuals[j].genes[i];
        } 
    }
    return schema;
}

// ---

// --- ORDENAR POPULAÇÃO PELO FITNESS ---
Population population_sort(Population population){
    int i, j, bigger;
    Individual aux_sort;
    for (i = 0; i < population.size; i++){
        bigger = i;
        for (j = i+1; j < population.size; j++){
            if(population.individuals[bigger].fitness < population.individuals[j].fitness){
                bigger = j;
            }
        }
        aux_sort = population.individuals[i];
        population.individuals[i] = population.individuals[bigger];
        population.individuals[bigger] = aux_sort;
    }
    return population;
}
// ---

// --- SELECIONAR PAI ---
Population select_parents(Population population, Population parents, int length){
    int i;
    parents.size = length;
    for (i = 0; i < parents.size; i++)
    {
        parents.individuals[i] = population.individuals[i];
    }
    return parents;
    
}
// ---

// --- OPERAÇÃO CRUZAMENTO ---
Population crossover(Population parents, Population children){
    int i, j, k, l, prob;
    for (i = 0; i < parents.size - 1; i++)
    {
        for (j = i + 1; j < parents.size; j++)
        {
            prob = rand() % 101;
            if(prob >= CROSS_RATE){
                int cut = (rand() % (GEN_SIZE - 2)) + 1;
                for(k = 0; k <= cut; k++){
                    children.individuals[children.size].genes[k] = parents.individuals[i].genes[k];
                    children.individuals[children.size + 1].genes[k] = parents.individuals[j].genes[k];
                }
                for(l = cut + 1; l < GEN_SIZE; l++){
                    children.individuals[children.size].genes[l] = parents.individuals[j].genes[l];
                    children.individuals[children.size + 1].genes[l] = parents.individuals[i].genes[l];
                }
                children.size += 2;
            }   
            if(children.size >= POP_SIZE){
                return children;
            }
        }
    }
    return children;
}
// ---

// --- OPERAÇÃO DE MUTAÇÃO ---
Population mutation(Population parents, Population children){
    int i, j, prob, gen;
    for (i = 0; i < parents.size; i++)
    {
        if(children.size < POP_SIZE){
            prob = rand() % 101;
            if(prob >= MUT_RATE){
                for (j = 0; j < GEN_SIZE; j++)
                {
                    gen = rand() % 2;
                    if(gen == 0){
                        if(parents.individuals[i].genes[j] == 0){
                            children.individuals[children.size].genes[j] = 1;
                        }
                        else children.individuals[children.size].genes[j] = 0;
                    }
                    else children.individuals[children.size].genes[j] = parents.individuals[i].genes[j]; 
                }
                children.size++;     
            } 
        }
    }
    return children;
}
// ---

int individual_is_equal(Individual ind1, Individual ind2)
{
    if(ind1.fitness != ind2.fitness) return 0;

    for (int i = 0; i < GEN_SIZE; i++)
    {
        if(ind1.genes[i] != ind2.genes[i]) return 0;
    }

    return 1;
}

int individual_exist(Individual individual, Population population)
{
    int exist = 0;
    for (int i = 0; i < population.size; i++)
    {
        exist = individual_is_equal(individual, population.individuals[i]);

        if(exist) break;
    }
    return exist;
}

// --- OPERAÇÃO SUBSTITUIÇÃO ---
Population substitution(Population old_population, Population children){
    Population new_population = { .size = 0 };
    int i, j, index_old_population = 0, index_children_population = 0;
    for (i = 0; i < POP_SIZE; i++)
    {
        if(index_children_population < children.size){
            if(old_population.individuals[index_old_population].fitness > children.individuals[index_children_population].fitness){
                new_population.individuals[i] = old_population.individuals[index_old_population];
                ++index_old_population;
            }
            else {
                new_population.individuals[i] = children.individuals[index_children_population];
                ++index_children_population;
            }
        }
        else new_population.individuals[i] = old_population.individuals[index_old_population];
        new_population.size++;
    }
    new_population.generation = old_population.generation + 1;
    return new_population;
}
// ---

// --- PRINTAR POPULAÇÃO --- 

void print_population(Population population, int length)
{
    int i, j;
    for (i = 0; i < length; i++)
    {
        for (j = 0; j < GEN_SIZE; j++)
        {
            printf("%d ", population.individuals[i].genes[j]);
        } 
        printf("Value : %d\n", population.individuals[i].fitness);
    }
}

// ---

// --- PRINTAR INDIVIDUO --- 

void print_individual(Individual individual)
{
    int i;
    for (i = 0; i < GEN_SIZE; i++)
    {
        printf("%d ", individual.genes[i]);
    } 
    printf("Value : %d\n", individual.fitness);
}

// ---

#endif