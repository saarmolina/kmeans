# K-means Clustering Implementation

This project implements the K-means clustering algorithm in both C and Python. The algorithm partitions N data points into K clusters, where each data point belongs to the cluster with the nearest mean (centroid).

## Features

- Implementations in both C and Python
- Handles multi-dimensional data points
- Customizable number of clusters (K) and maximum iterations
- Convergence detection using Euclidean distance

## C Implementation

### Compilation

To compile the C program, use:

```bash
gcc -o kmeans kmeans.c -lm
```

### Usage

```bash
./kmeans K [iter] < input_file.txt
```

- `K`: Number of clusters
- `iter`: (Optional) Maximum number of iterations (default: 200), should be between 1 and 1000.
- `input_file.txt`: Input data file (read from stdin)

### Data Structure

The C implementation uses linked lists to represent vectors and coordinates:

- `struct cord`: Represents a coordinate in a vector
- `struct vector`: Represents a vector (data point or centroid)

### Key Functions

- `create_datapoints`: Reads input data and creates the data structure
- `initialize_centroids`: Initializes centroids from the first K data points
- `assign_clusters`: Assigns each data point to the nearest centroid
- `update_centroids`: Updates centroids based on current cluster assignments

## Python Implementation

### Usage

```bash
python kmeans.py K [iter] input_file.txt
```

- `K`: Number of clusters
- `iter`: (Optional) Maximum number of iterations (default: 200), should be between 1 and 1000.
- `input_file.txt`: Input data file

### Key Functions

- `eucDistance`: Calculates Euclidean distance between two vectors
- `assignToClusters`: Assigns vectors to the nearest centroid
- `updateCentroids`: Updates centroids based on current clusters
- `kmeans`: Main function implementing the K-means algorithm

## Input Format

The input data should be provided in a text file where each line represents a vector, with coordinates separated by commas. For example:

```bash
1.0,2.0,3.0
4.0,5.0,6.0
7.0,8.0,9.0
```

## Output

The program prints the final centroids to the standard output, with each centroid on a new line and coordinates separated by commas. For example:

```bash
1.0000,2.0000,3.0000
4.0000,5.0000,6.0000
```

## Error Handling

The programs perform input validation and handle various error conditions, such as invalid number of clusters or iterations.

## Performance Considerations

- The C implementation uses dynamic memory allocation for flexibility
- The Python implementation uses list comprehensions for efficiency

## Dependencies

- C implementation: Standard C libraries and math library (-lm)
- Python implementation: Python standard library (math, sys)

## Note

This implementation assumes that the input data is well-formed and contains only numeric values. Additional input validation may be needed for robustness in production environments.
