import math
import sys


# Global variables for k-means parameters
EPSILON = 0.001
DEFAULT_ITERATIONS = 200  # Default value for iter
MAX_ITER = 1000


def eucDistance(vector1,vector2):

    """
    Calculate the Euclidean distance between two vectors.
    """

    dis = 0 
    d = len(vector1) # Dimension of the vectors

    for i in range(d):

        dis += (vector1[i]-vector2[i])**2 # Compute squared differences

    return math.sqrt(dis) # Return the square root of the sum of squared differences


def assignToClusters(centroids,vectors):

    """
    Assign each vector to the closest centroid.
    """

    k = len(centroids)  # Number of clusters

    clusters = [[] for i in range(k)] # Initialize empty clusters

    for vector in vectors:

        minDistance = float('inf')
        closestCentroid = -1 

        for i in range(k):

            centroid = centroids[i]
            currDistance = eucDistance(vector,centroid) # Calculate distance to each centroid

            if currDistance < minDistance:

                minDistance = currDistance 
                closestCentroid = i # Update closest centroid index

        clusters[closestCentroid].append(vector) # Assign vector to the closest centroid's cluster

    return clusters


def newCentroid(cluster):

    """
    Calculate the new centroid of a cluster.
    """

    d = len(cluster[0]) # Dimension of the vectors
    n = len(cluster) # Number of vectors in the cluster
    newCentroid = [0 for i in range(d)] # Initialize the new centroid

    for vector in cluster:

        for i in range(d):

            newCentroid[i] += vector[i] # Sum all vectors
        
    for i in range(d):

        newCentroid[i] = newCentroid[i]/n # Divide by the number of vectors to get the average

    return newCentroid


def updateCentroids(clusters):

    """
    Update centroids based on current clusters.
    """

    k = len(clusters) # Number of clusters
    centroids = [-1 for i in range(k)] # Initialize centroids list

    for i in range(k):
        
        cluster = clusters[i]
        if len(cluster) == 0:  # Handle empty clusters
            continue
        newCentroidVal = newCentroid(cluster)
        centroids[i] = newCentroidVal # Calculate new centroid for each cluster
    
    return centroids


def finishIter(oldCentroids, centroids, EPSILON):

    """
    Check if the centroids have converged.
    """

    k = len(centroids)

    for i in range(k):

        distance = eucDistance(oldCentroids[i], centroids[i]) # Calculate distance between old and new centroids

        if distance >= EPSILON: # Return False if any centroid has not converged
            return False

    return True # Return True if all centroids have converged


def kmeans(k, iter, vectors):

    """
    k-means clustering algorithm.
    """

    centroids = vectors[:k] # Initialize centroids with the first k vectors

    for i in range(iter):

        oldCentroids = centroids
        clusters = assignToClusters(centroids,vectors) # Assign vectors to clusters
        centroids = updateCentroids(clusters) # Update centroids based on clusters

        if finishIter(oldCentroids, centroids, EPSILON):
            break # Stop if centroids have converged

    return centroids


def read_data(filename):

    vectors = [] # Initialize an empty list to store vectors

    with open(filename, 'r') as f: # Open filename in read mode

        for line in f: # Iterating through Lines

            vector = [float(x) for x in line.split(',')]  # Split line by comma and convert to float
            vectors.append(vector)  # Append the parsed vector to the list of vectors

    return vectors

    
if __name__ == "__main__":

    # Check command-line arguments
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        print("Usage: python kmeans.py K [iter] input_data.txt")
        sys.exit(1)
    
    # Number of clusters
    k = int(sys.argv[1]) 

    # Number of iterations is given 
    if len(sys.argv) == 4: 

        iter = int(sys.argv[2])

        # Maximum number of iterations
        if not (1 < iter < MAX_ITER):
            print("Invalid maximum iteration!")
            sys.exit(1)

        # Input data file
        input_file = sys.argv[3]

    # Number of iterations is not given 
    else:
        iter = DEFAULT_ITERATIONS
        input_file = sys.argv[2]

    # Read vectors from input file
    vectors = read_data(input_file)

    N = len(vectors)
    if not (1 < k < N):
        print("Invalid number of clusters!")
        sys.exit(1)

    # Perform k-means clustering
    centroids = kmeans(k, iter, vectors)

    # Print final centroids
    for centroid in centroids:

        formatted_centroid = [f'%.4f' % coord for coord in centroid]
        print(','.join(formatted_centroid))  # Convert centroid to formatted string and join by commas for output

  
  
        

        
       



        





    


    


