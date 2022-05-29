#!/usr/bin/env python3

import pandas as pd
import numpy as np
from sklearn.neighbors import KNeighborsRegressor
from sklearn import metrics


# -------------------------------------------------------------------------------

def normalize(train_data, test_data, col_class, method='mean_std'):
    """
    Normalizes all the features by linear transformation *except* for the target class specified as `col_class`.
    Two normalization methods are implemented:
      -- `mean_std` shifts by the mean and divides by the standard deviation
      -- `maxmin` shifts by the min and divides by the difference between max and min
      *Note*: mean/std/max/min are computed on the training data
    The function returns a pair normalized_train, normalized_test. For example,
    if you had `train` and `test` pandas DataFrames with the class stored in column `Col`, you can do

        train_norm, test_norm = normalize(train, test, 'Col')

    to get the normalized `train_norm` and `test_norm`.
    """
    # removing the class column so that it is not scaled
    no_class_train = train_data.drop(col_class, axis=1)
    no_class_test = test_data.drop(col_class, axis=1)

    # scaling
    normalized_train, normalized_test = None, None
    if method == 'mean_std':
        normalized_train = (no_class_train - no_class_train.mean()) / no_class_train.std()
        normalized_test = (no_class_test - no_class_train.mean()) / no_class_train.std()
    elif method == 'maxmin':
        normalized_train = (no_class_train - no_class_train.min()) / (no_class_train.max() - no_class_train.min())
        normalized_test = (no_class_test - no_class_train.min()) / (no_class_train.max() - no_class_train.min())
    else:
        raise f"Unknown method {method}"

    # gluing back the class column and returning
    return pd.concat([train_data[col_class], normalized_train], axis=1), pd.concat([test_data[col_class], normalized_test], axis=1)

# -------------------------------------------------------------------------------

if __name__ == "__main__":

    tr_dat = pd.read_csv("../csv/maisons_original_train.csv", header=0)
    print(f'Shape of the train data {tr_dat.shape}')
    # print the first 5 rows from the dataset
    print(tr_dat.head(5))


    print('\n\n')
    te_dat = pd.read_csv("../csv/maisons_original_test.csv", header=0)
    print(f'Shape of the test data {te_dat.shape}')

    train_dataset, test_dataset = tr_dat, te_dat

    train_dataset, test_dataset = normalize(train_dataset, test_dataset, 'price', 'mean_std')

    X = train_dataset.drop("price", axis=1)
    y = train_dataset["price"]
    regressor = KNeighborsRegressor(n_neighbors=5, algorithm='kd_tree', weights='distance')
    regressor.fit(X, y)

    tX = test_dataset.drop("price", axis=1)
    ty = test_dataset["price"]

    y_pred = regressor.predict(tX)

    print("\n")
    y_test = np.array(ty)
    X_test = np.array(tX)

    for a, b in zip(y_test, y_pred):
        print(f'  true value: {a} \t predicted value: {b}')

    print('\n\n')
    print('Mean Absolute Error:', metrics.mean_absolute_error(y_test, y_pred))
    print('Mean Squared Error:', metrics.mean_squared_error(y_test, y_pred))
    print('Root Mean Squared Error:', np.sqrt(metrics.mean_squared_error(y_test, y_pred)))
