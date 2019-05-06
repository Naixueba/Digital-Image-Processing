# EE569 HW#6, Problem 3 -- Handwritten digits recognition using ensembles of FF-CNNS
# Name: Wenjun Li
# USC ID: 8372-7611-20
# Email: wenjunli@usc.edu
# Submission Date: April 27
# File Description: ensemble FF-CNN


import numpy as np
import keras
from keras.datasets import mnist
from individual_ff_cnn import lenet5_ff


def separate_data(xtr, ytr):
    pass
    return xtr_separated, ytr_separated


# load MNIST dataset
(xtr, ytr), (xte, yte) = mnist.load_data()
yte = keras.utils.to_categorical(yte, 10)


# split MNIST into 10 separated dataset
xtr_separated, ytr_separated = separate_data(xtr, ytr)

# array to store prediction result
prediction = np.empty(shape=(10000, 10), dtype='int8')

# pad training images
train_image = np.pad(xtr_separated, ((0,0),(2,2),(2,2),(0,0)), mode='constant')
train_label = keras.utils.to_categorical(ytr_separated, 10)

# train individual FF-CNN separately
for i in range(0, 10):
    pca_params_file = './pca_params_compact_'+str(i)+'.pkl'
    weight_file = './llsr_weights_compact_v2_'+str(i)+'.pkl'
    bias_file = './llsr_bias_compact_v2_'+str(i)+'.pkl'

    pred = lenet5_ff(pca_params_file, weight_file, bias_file, xtr_separated[6000*i:6000*(i+1)], train_label[6000*i:6000*(i+1)], xte, yte)
    prediction = prediction + np.array(pred)

# take the average of all individual FF-CNNs results
max = np.argmax(prediction, axis=1)
y_pred = np.eye(10)[max]
sum = np.sum(y_pred*yte)
accuracy = sum / 10

print('testing accuracy: %f' % accuracy)

