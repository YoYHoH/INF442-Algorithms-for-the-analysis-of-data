import os
import sys
from loguru import logger
from sklearn.neighbors import KernelDensity, KNeighborsClassifier
from sklearn.datasets import load_digits
from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import time;


def main():
    # usage message
    if len(sys.argv) != 4:
        print(f'''
Usage: {sys.argv[0]} csv_file kernel bandwidth

where:
        - csv_file is the path to the csv file to analyze

        - kernel can be either gaussian, tophat (corresponding to flat), (epanechnikov, exponential, linear or cosine)

Examples
        {sys.argv[0]} ../csv/double_spiral.csv tophat 1

        {sys.argv[0]} ../csv/galaxies.csv gaussian 2
''')
        sys.exit(1)

    # manage arguments
    csv_file = sys.argv[1]
    kernel = sys.argv[2]
    bandwidth = float(sys.argv[3])

    # read in data
    if sys.argv[1] == "digits":
        # load the data
        digits = load_digits()
        data = digits.data

    else:
        data = pd.read_csv(sys.argv[1], delimiter=' ', header=None).to_numpy()
    logger.info('Read in the (' + str(data.shape[0]) + ',' + str(data.shape[1]) + ') data (see first few lines):')
    logger.info('\n' + '\t' + pd.DataFrame(data).head().to_string().replace('\n', '\n\t'))

    # Kernel density estimation
    kde = KernelDensity(kernel=kernel, bandwidth=bandwidth).fit(data)

    # Plot the kernel
    if sys.argv[1] == "digits":
        # sample 100 new points from the data
        new_data = kde.sample(100, random_state=0)
        # take their nearest neighbor's "class"
        knn_classif = KNeighborsClassifier(n_neighbors=1).fit(data, np.eye(10)[digits.target])
        predictions = np.argmax(knn_classif.predict(new_data), axis=1)

        f, axes = plt.subplots(10, 10, figsize=(11, 8))

        # Loop over digits
        for i in range(10):
            axes[0, i].axis('off')
            count = 1
            j = 0
            # Display true digits on first 4 lines (and beware not to go beyond the available data)
            while count < 5 and j < data.shape[0]:
                # Put an example of digit i in column i
                if digits.target[j] == i:
                    # imshow displays an image given grayscale or RGB numerical values
                    axes[count, i].imshow(digits.data[j, :].reshape((8, 8)), cmap=plt.cm.binary)
                    # we won't need x, y axes for images
                    axes[count, i].axis('off')
                    if count == 1:
                        # display the digit as a column title
                        axes[count, i].set_title(digits.target[j])
                    count += 1
                j += 1
            
            # Add empty line (we'll put a title there later on)
            count += 1
            j = 0
            axes[5, i].set_visible(False)

            # Display drawn digits on last 4 lines (and beware not to go beyond the new drawn data)
            while count < 10 and j < new_data.shape[0]:
                if predictions[j] == i:
                    axes[count, i].imshow(new_data[j, :].reshape((8, 8)), cmap=plt.cm.binary)
                    axes[count, i].axis('off')
                    count += 1
                j += 1
        axes[0, 5].set_title("Selection from the input data")
        axes[6, 5].set_title('"New" digits drawn from the kernel density model')

    else:
        # Draw scatter plot on top of density estimation
        fig, ax = plt.subplots(2, 1, sharex=True, sharey=True)
        ## Scatter plot of true points
        ax[0].scatter(data[:, 0], data[:, 1], s=0.3)

        # Create a mesh, i.e. many points on the grid to predict the value of the density
        xx, yy = np.mgrid[data[:, 0].min():data[:, 0].max():100j,
                          data[:, 1].min():data[:, 1].max():100j]
        positions = np.vstack([xx.ravel(), yy.ravel()])
        logger.info(f"Estimating kernel density on {positions.shape[1]} new points")
        ## Color based on exp(estimated density)
        f = np.reshape(np.exp(kde.score_samples(positions.T)).T, xx.shape)

        ## "Contour" plot
        logger.info(f"Plotting original data and estimated kernel on the same support")
        cfset = ax[1].contourf(xx, yy, f, cmap='Blues')
    # Save plot
    plt.savefig(f'{os.path.splitext(os.path.basename(sys.argv[1]))[0]}_{sys.argv[2]}_{sys.argv[3].replace(".", "_")}.png')


if __name__ == '__main__':
    #tic = time.perf_counter()
    main()
    #tac = time.perf_counter()
    #print(tac-tic)

