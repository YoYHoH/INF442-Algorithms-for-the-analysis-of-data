import sys
import numpy as np
import pandas as pd
import sklearn as sk
import sklearn.svm
import sklearn.datasets


if __name__=="__main__":
    if len(sys.argv) >= 3:
        readfrom_train = sys.argv[1]
        readfrom_test = sys.argv[2]
    else:
        print("Syntax: python %s <dataset_train> <dataset_test> [<id_label> <rest_of_cols> ['<char_sep>'] ]" % sys.argv[0])
        print(" <rest_of_cols> : columns to include in output file (ex: 1,2,3), starting in 0. Use - for including all.")
        exit(0)

    if len(sys.argv) >= 5:
        label = int(sys.argv[3])
        rest = sys.argv[4]
        if len(sys.argv) >= 6:
            char_sep = sys.argv[5]
        else:
            char_sep = ' '
    else:
        label = 0
        rest = "-"
        char_sep = ' '

    if rest != "-":
        some_cols = True
        id_cols = rest.rstrip().split(",")
    else:
        some_cols = False

    print("Character separation: [%c]"%char_sep)

    if ".svm" in readfrom_train:
        X_train, y_train = sk.datasets.load_svmlight_file(readfrom_train)
        X_test, y_test = sk.datasets.load_svmlight_file(readfrom_test)
        X_train = np.array(X_train.todense())  # by default it's a sparse CSR matrix which does not work with MinMaxScaler
        X_test = np.array(X_test.todense())
    else:
        dataset_train = pd.read_csv(readfrom_train,
                                    sep=char_sep,
                                    index_col=0,
                                    skiprows=5 * ("scooter-train" in readfrom_train),  # skip comments describing scooter dataset
                                    header=None)  # no name of columns
        dataset_test = pd.read_csv(readfrom_test, sep=char_sep, index_col=0,
                                   skiprows=5 * ("scooter-train" in readfrom_test),
                                   header=None)

        if rest != "-":
            X_train = dataset_train.iloc[:, rest]
            X_test = dataset_test.iloc[:, rest]
        else:
            X_train = dataset_train.drop(dataset_train.columns[label], axis=1)
            X_test = dataset_test.drop(dataset_test.columns[label], axis=1)

        y_train = dataset_train.iloc[:, label]
        y_test = dataset_test.iloc[:, label]

    # default c =1 gamma = 'auto'
    svm_model = sk.svm.SVC(C=1, gamma=1000)  # to match libSVM's svm-train default parameters, change parameters here for Q6 and Q8
    scaler = sk.preprocessing.MinMaxScaler(feature_range=(-1, 1))  # to match libSVM's svm-scale's default parameter
    svm_model.fit(X_train, y_train)

    predictions = svm_model.predict(X_test)

    print("Accuracy:", np.sum(predictions == y_test) / X_test.shape[0],
          f"({np.sum(predictions == y_test)}/{X_test.shape[0]})")
          
    svm_model.fit(scaler.fit_transform(X_train), y_train)
    predictions = svm_model.predict(scaler.transform(X_test))
    print("Accuracy after scaling:", np.sum(predictions == y_test) / X_test.shape[0],
          f"({np.sum(predictions == y_test)}/{X_test.shape[0]})")

