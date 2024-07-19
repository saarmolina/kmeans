#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 0.001
#define DEFAULT_ITERATIONS 200
#define MAX_ITER 1000

struct cord {
    double value;
    struct cord *next;
};

struct vector {
    struct vector *next;
    struct cord *cords;
};

double euclidean_distance(struct vector *a, struct vector *b, int d);
struct vector *deep_clone_vector(struct vector *src);
struct cord *deep_clone_cords(struct cord *src);
int isInteger(const char *str);
struct vector* create_datapoints(int *N, int *d);
void initialize_centroids(struct vector **centroids, struct vector *datapoints, int K);
void assign_clusters(struct vector *datapoints, struct vector **centroids, int *cluster_assignments, int N, int K, int d);
int update_centroids(struct vector **centroids, struct vector *datapoints, int *cluster_assignments, int N, int K, int d);
void print_centroids(struct vector **centroids, int K, int d);
void free_memory(struct vector *datapoints, struct vector **centroids, int K);

double euclidean_distance(struct vector *a, struct vector *b, int d) {

    /* 
    Calculates the Euclidean distance between two vectors 
    */

    double sum = 0;
    int i;
    struct cord *cord_a = a->cords;
    struct cord *cord_b = b->cords;

    for (i = 0; i < d; i++) {
        double diff = cord_a->value - cord_b->value;
        sum += diff * diff;
        cord_a = cord_a->next;
        cord_b = cord_b->next;
    }

    return sqrt(sum);
}

struct vector *deep_clone_vector(struct vector *src) {

    /* 
    Deep clone a vector 
    */

    struct vector *new_vector = NULL;

    if (src == NULL) {
        return NULL;
    }

    new_vector = malloc(sizeof(struct vector));
    if (new_vector == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    new_vector->cords = deep_clone_cords(src->cords);
    new_vector->next = NULL;

    return new_vector;
}

struct cord *deep_clone_cords(struct cord *src) {

    /* 
    Deep clone a list of cords 
    */

    struct cord *new_cord = NULL;

    if (src == NULL) {
        return NULL;
    }

    new_cord = malloc(sizeof(struct cord));
    if (new_cord == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }

    new_cord->value = src->value;
    new_cord->next = deep_clone_cords(src->next);

    return new_cord;
}

int isInteger(const char *str) {

    /* 
    Function to check if a string is an integer 
    */

    char *endptr;
    double num = strtod(str, &endptr);
    
    /* Check if conversion was successful and the entire string was used */
    if (endptr == str || *endptr != '\0') {
        return 0;  /* Not a valid number */ 
    }
    
    /* Check if the number is a positive integer or a positive round double */
    if (num <= 0 || num != floor(num)) {
        return 0;  /* Not a positive integer or round double */ 
    }
    
    return 1;  /* Valid number */
}

struct vector* create_datapoints(int *N, int *d) {

    /* 
    Function to create data points from standard input
    *d will contain the length of each vector and *N the number of vectors 
    */

    struct vector *head_vec = NULL, *curr_vec = NULL;
    struct cord *head_cord = NULL, *curr_cord = NULL;
    double n;
    char c;

    /* Loop to read data points from input - read two items from the input: a floating-point number and a character */
    while (scanf("%lf%c", &n, &c) == 2) {

        /* If it's the first coordinate in the current vector */
        if (!head_cord) {
            head_cord = malloc(sizeof(struct cord)); /* Allocate memory for the first coordinate */
            curr_cord = head_cord;                   /* Set current coordinate to head coordinate */
        } else {
            curr_cord->next = malloc(sizeof(struct cord)); /* Allocate memory for the next coordinate */
            curr_cord = curr_cord->next;                   /* Move to the next coordinate */
        }

        curr_cord->value = n;   /* Set the value of the current coordinate */
        curr_cord->next = NULL; /* Set the next pointer of the current coordinate to NULL */

        /* If the end of the line or end of input is reached */
        if (c == '\n' || c == EOF) {

            /* If it's the first vector */
            if (!head_vec) {
                head_vec = malloc(sizeof(struct vector)); /* Allocate memory for the first vector */
                curr_vec = head_vec;                      /* Set current vector to head vector */
            } 
            
            else {
                curr_vec->next = malloc(sizeof(struct vector)); /* Allocate memory for the next vector */
                curr_vec = curr_vec->next;                      /* Move to the next vector */
            }

            curr_vec->cords = head_cord; /* Link the current vector to the head coordinate */
            curr_vec->next = NULL;       /* Set the next pointer of the current vector to NULL */
            head_cord = NULL;            /* Reset the head coordinate for the next vector */
            (*N)++;                       /* Increment the number of vectors */

            /* Calculate the number of coordinates in the first vector */
            if (*N == 1) {

                struct cord *temp = curr_vec->cords; /* Temporary pointer to iterate through coordinates */
                while (temp) {
                    (*d)++;         /* Increment the number of coordinates */
                    temp = temp->next; /* Move to the next coordinate */
                }
            }
        }
    }

    return head_vec; /* Return the head of the linked list of vectors */
}

void initialize_centroids(struct vector **centroids, struct vector *datapoints, int K) {

    /* 
    Initializes the centroids by deep cloning the first K data points. 
    */

    struct vector *curr = datapoints;
    int i;

    for (i = 0; i < K; i++) {
        centroids[i] = deep_clone_vector(curr);
        curr = curr->next;
    }
}

void assign_clusters(struct vector *datapoints, struct vector **centroids, int *cluster_assignments, int N, int K, int d) {
    
    /* 
    Assigns each data point to the nearest centroid 
    */

    struct vector *curr = datapoints;
    int i;

    /* For every datapoint */
    for (i = 0; i < N; i++) {

        /* Calculates the distance from the first centroid */
        double min_dist = euclidean_distance(curr, centroids[0], d);
        int min_cluster = 0; /* Saves the index of the closest centroid */
        int j;

        /* For every centroid */
        for (j = 1; j < K; j++) {

            /* Calculates the distance from the j centroid */
            double dist = euclidean_distance(curr, centroids[j], d);
            if (dist < min_dist) {
                min_dist = dist;
                min_cluster = j;
            }
        }

        cluster_assignments[i] = min_cluster; /* Updates the centroid of the datapoint */
        curr = curr->next; /* Moves to the next datapoint */
    }
}

int update_centroids(struct vector **centroids, struct vector *datapoints, int *cluster_assignments, int N, int K, int d) {
    
    /* 
    Updates the centroids based on the current cluster assignments.
    Checks if the centroids have converged by comparing the old and new centroids. 
    */
  
    /* Allocate memory for sums and counts of coordinates */
    double **sums = malloc(K * sizeof(double*));
    int *counts = calloc(K, sizeof(int));

    struct vector *curr = datapoints;
    struct cord *new_cord = NULL, *curr_cord = NULL, *next_cord = NULL;
    struct cord *old_cord = NULL;

    int converged = 1; /* Flag to check if centroids have converged */
    double distance;

    int i, j;

    /* Allocate memory for new centroids */
    struct vector **new_centroids = malloc(K * sizeof(struct vector*));
    for (i = 0; i < K; i++) {
        sums[i] = calloc(d, sizeof(double)); /* Initialize sum for each centroid */
        new_centroids[i] = malloc(sizeof(struct vector));
        new_centroids[i]->cords = malloc(d * sizeof(struct cord)); /* Allocate cords for new centroid */
        new_cord = new_centroids[i]->cords;

        /* Initialize coordinates of the new centroid */
        for (j = 0; j < d; j++) {
            new_cord->value = 0; /* Initialize to 0 for averaging */
            new_cord->next = (j < d - 1) ? malloc(sizeof(struct cord)) : NULL; /* Allocate next only if not last */
            new_cord = new_cord->next; /* Move to the next coordinate */
        }
    }

    /* Calculate sums and counts of points assigned to each centroid */
    for (i = 0; i < N; i++) {
        int cluster = cluster_assignments[i]; /* Get the cluster for current data point */
        struct cord *cord = curr->cords;

        /* Accumulate sum of coordinates for the assigned cluster */
        for (j = 0; j < d; j++) {
            sums[cluster][j] += cord->value; /* Add value to the corresponding sum */
            cord = cord->next; /* Move to the next coordinate */
        }

        counts[cluster]++; /* Increment count of points in the cluster */
        curr = curr->next; /* Move to the next data point */
    }

    /* Update centroids based on accumulated sums and counts */
    for (i = 0; i < K; i++) {
        struct cord *new_cord = new_centroids[i]->cords;

        for (j = 0; j < d; j++) {
            new_cord->value = sums[i][j] / counts[i]; /* Calculate average for centroid */
            new_cord = new_cord->next; /* Move to the next coordinate */
        }

        /* Check for convergence by calculating distance between old and new centroids */
        distance = euclidean_distance(centroids[i], new_centroids[i], d);
        if (distance > EPSILON) {
            converged = 0; /* Centroid has not converged if distance exceeds EPSILON */
        }

        /* Update the old centroid with the new centroid values */
        old_cord = centroids[i]->cords; 
        new_cord = new_centroids[i]->cords; /* Reset new_cord to the start */
        for (j = 0; j < d; j++) {
            old_cord->value = new_cord->value; /* Update old centroid with new values */
            old_cord = old_cord->next; /* Move to the next coordinate of old centroid */
            new_cord = new_cord->next; /* Move to the next coordinate of new centroid */
        }
    }

    /* Free allocated memory of new centroids */
    for (i = 0; i < K; i++) {
        free(sums[i]);
        curr_cord = new_centroids[i]->cords;

        while (curr_cord) {
            next_cord = curr_cord->next;
            free(curr_cord);
            curr_cord = next_cord; /* Move to next cord */
        }
        free(new_centroids[i]); /* Free the new centroid structure */
    }

    free(sums);
    free(counts);
    free(new_centroids);
    
    return converged; /* Return convergence status */
}

void free_memory(struct vector *datapoints, struct vector **centroids, int K) {

    struct vector *curr_vec, *next_vec;
    struct cord *curr_cord, *next_cord;
    int i;

    /* Free datapoints */
    curr_vec = datapoints;
    while (curr_vec) {
        curr_cord = curr_vec->cords;
        while (curr_cord) {
            next_cord = curr_cord->next;
            free(curr_cord);
            curr_cord = next_cord;
        }
        next_vec = curr_vec->next;
        free(curr_vec);
        curr_vec = next_vec;
    }

    /* Free centroids */
    if (centroids) {
        for (i = 0; i < K; i++) {
            curr_cord = centroids[i]->cords;
            while (curr_cord) {
                next_cord = curr_cord->next;
                free(curr_cord);
                curr_cord = next_cord;
            }
            free(centroids[i]);
        }
        free(centroids);
    }
}

int main(int argc, char **argv) {

    int K = 0, N = 0, d = 0;
    int iter;
    int i;
    struct vector *datapoints, **centroids;
    int *cluster_assignments;
    int converged = 0;

    /* If we got wrong number of arguments */
    if (argc != 2 && argc != 3) {
        printf("An Error Has Occurred\n");
        return 1;
    }

    /* Check if argv[1] (which contains K) is an integer */
    if (!isInteger(argv[1])) {
        printf("Invalid number of clusters!\n");
        return 1;
    }

    /* Converts a command-line argument to a double, then to an integer */
    K = (int)strtod(argv[1], NULL);

    /* K and iter are provided */
    if (argc == 3) {
        /* Check if argv[2] (which contains iter) is not an integer */
        if (!isInteger(argv[2])) {
            printf("Invalid maximum iteration!\n");
            return 1;
        }
    }

    /* If we got 3 arguments, iter is provided, if not - use DEFAULT_ITER */
    iter = (argc == 3) ? (int)strtod(argv[2], NULL) : DEFAULT_ITERATIONS;

    /* 
    Create datapoints - the original vectors we got from the input
    *d will contain the length of each vectors and *N the number of vectors 
    */

    datapoints = create_datapoints(&N, &d);

    /* Validate input */
    if (K <= 1 || K >= N) {
        printf("Invalid number of clusters!\n");
        free_memory(datapoints, NULL, K);
        return 1;
    }
    if (iter <= 1 || iter >= MAX_ITER) {
        printf("Invalid maximum iteration!\n");
        free_memory(datapoints, NULL, K);
        return 1;
    }

    /* Allocate memory for an array of K pointers to vectors */
    centroids = malloc(K * sizeof(struct vector*));

    /* 
    Allocate memory for an array of N integers
    cluster_assignments will contain the index of the centroid for every datapoint 
    */
    cluster_assignments = malloc(N * sizeof(int));

    if (!centroids || !cluster_assignments) {
        printf("An Error Has Occurred\n");
        free_memory(datapoints, centroids, K);
        free(cluster_assignments);
        return 1;
    }

    /* Initialize centroids from the first k datapoints */
    initialize_centroids(centroids, datapoints, K);

    /* Main K-means loop */
    for (i = 0; i < iter && !converged; i++) {

        /* Update the centroid assignments */
        assign_clusters(datapoints, centroids, cluster_assignments, N, K, d);

        /* Updates the centroids based on the current cluster assignments
           Checks if the centroids have converged by comparing the old and new centroids */
        converged = update_centroids(centroids, datapoints, cluster_assignments, N, K, d);
    }

    /* Print results */
    print_centroids(centroids, K, d);

    /* Free memory */
    free_memory(datapoints, centroids, K);
    free(cluster_assignments);

    return 0;
}

void print_centroids(struct vector **centroids, int K, int d) {

    /* Prints the final centroids to standard output. */
    int i, j;
    for (i = 0; i < K; i++) {
        struct cord *cord = centroids[i]->cords;
        for (j = 0; j < d; j++) {
            printf("%.4f", cord->value);
            if (j < d - 1) printf(",");
            cord = cord->next;
        }
        printf("\n");
    }
}