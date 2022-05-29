#!/usr/bin/env python3

import pandas as pd
import numpy as np
import math
from sklearn.linear_model import LinearRegression
from sklearn import metrics


train_fname = "../csv/salary_full_train.csv"
test_fname = "../csv/salary_full_test.csv"

train_dataset = pd.read_csv(train_fname, header=0)
print(f'Shape of the train data {train_dataset.shape}')
# print the first 5 rows of the dataset
print(train_dataset.head(5))
print('\n\n')
test_dataset = pd.read_csv(test_fname, header=0)
print(f'Shape of the test data {test_dataset.shape}')

# The features used to build matrix X
feature_cols = ["ry", "yd", "sex", "asoc", "full", "phd"]
#feature_cols = ["sex"]

# the target feature (vector y) of the regression
target = "salary"

X = train_dataset[feature_cols]
y = train_dataset[target]

# print(X.head(5))
# print(y.head(5))

regressor = LinearRegression()
regressor.fit(X, y)

print(f'\tintercept = {regressor.intercept_}')
print(f'\tcoefficient = {regressor.coef_}')

tX = test_dataset[feature_cols]
ty = test_dataset[target]

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
print('\n')
