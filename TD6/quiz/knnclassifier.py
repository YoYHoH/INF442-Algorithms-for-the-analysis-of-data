import matplotlib.pyplot as plt
import pandas as pd
from sklearn.metrics import confusion_matrix, RocCurveDisplay, plot_roc_curve
from sklearn.neighbors import KNeighborsClassifier

def train_and_evaluate(train_data, test_data, class_col, nneighbors, roc_fname="roc.png"):
    """
    Trains a kNN classifier and displays metrics of its performance on the test data
    """
    ## Training the classifier
    cls = KNeighborsClassifier(n_neighbors=nneighbors, algorithm='kd_tree')
    cls.fit(train_data.drop(class_col, axis=1), train_data[class_col])
    
    ## Predicting
    predicitons = cls.predict(test_data.drop(class_col, axis=1))
    
    ## Computing the confusion matrix
    cm = confusion_matrix(test_data[class_col], predicitons)
    print("Confusion matrix is:")
    print(cm)
    tn, fp, fn, tp = cm.ravel()
    print(f"Error rate {(fp + fn) * 1. / (fp + fn + tp + tn)}")
    print(f"False alarm rate {fp * 1. / (fp + tn)}")
    detection = tp * 1. / (tp + fn)
    print(f"Detection rate {detection}")
    precision = tp * 1. / (tp + fp)
    print(f"Precision {precision}")
    print(f"F-score {2 * detection * precision / (detection + precision)}")

    ## Drawing a ROC curve
    plot_roc_curve(cls, test_data.drop(class_col, axis=1), test_data[class_col])
    plt.savefig(roc_fname)
    # We use plot_roc_curve to be compatible with the older versions of scikit-learn, in particular,
    # the ones at the salles informatiques. Here is a more modern way of doing the same:
    # RocCurveDisplay.from_estimator(cls, test_data.drop(class_col, axis=1), test_data[class_col])

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
    return pd.concat([normalized_train, train_data[col_class]], axis=1), pd.concat([normalized_test, test_data[col_class]], axis=1)

# -------------------------------------------------------------------------------

if __name__ == "__main__":
    ## Reading the data
    train_fname = "../csv/audit_train.csv"
    test_fname = "../csv/audit_test.csv"
    train_data = pd.read_csv(train_fname, header=0)
    print(train_data)
    train_data.drop(columns='Loss')
    print(train_data)
    test_data = pd.read_csv(test_fname, header=0)
    ## train_data.drop
    train, test = normalize(train_data, test_data, 'Risk', 'maxmin')
    
    ## test.drop(columns=['PARA_A'])
    ## Train & Test !
    N = 7

    train_and_evaluate(train, test, 'Risk', N)
