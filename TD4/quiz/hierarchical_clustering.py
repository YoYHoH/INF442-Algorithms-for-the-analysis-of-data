import pandas as pd
import matplotlib.pyplot as plt
from scipy.cluster.hierarchy import dendrogram, linkage
from scipy.spatial.distance import squareform

fname = "../csv/bluered.csv"
data = pd.read_csv(fname, header=0)
## plt.figure(figsize=(1080, 960))
tree = linkage(data[['x', 'y']])
D = dendrogram(tree, labels=data['name'].to_numpy(), orientation='left')

plt.show()

def load_distance_matrix(fname):
    """
    Takes as input a name of a file containing the information about a graph:
      - first line: number of vertices
      - then labels, one per line
      - then the distance matrix, one row per line, the entries separated by commas
    Returns a tuple containing a distance matrix in the condenced 1-d format and a list of labels
    """
    with open(fname, "r") as f:
        n = int(f.readline())
        labels = [f.readline().strip() for _ in range(n)]
        dist_matrix = [[float(x) for x in f.readline().split(',')] for _ in range(n)]
        return (squareform(dist_matrix), labels)