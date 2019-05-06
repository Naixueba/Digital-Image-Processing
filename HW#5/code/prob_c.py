# HW#5 Prob.(c).(1)
# Submission Date: Mar.29
# Name: Wenjun Li
# USC ID: 8372-7611-20

"""
this code test the model performance on negative image dataset
"""

import numpy as np
import h5py

import keras
from keras.models import Sequential, load_model
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D, Activation
from keras.datasets import mnist
from keras import backend as K


# ==========================================================================
# read in  dataset
# ==========================================================================
(xtr, ytr), (xte, yte) = mnist.load_data()
# xtr.shape = (60000, 28, 28); ytr.shape = (60000,)
# xtr.shape = (60000, 28, 28, 1)
if K.image_data_format() == 'channels_first':
    xtr = xtr.reshape(xtr.shape[0], 1, 28, 28)
    xte = xte.reshape(xte.shape[0], 1, 28, 28)
    input_shape = (1, 28, 28)
else:
    xtr = xtr.reshape(xtr.shape[0], 28, 28, 1)
    xte = xte.reshape(xte.shape[0], 28, 28, 1)
    input_shape = (28, 28, 1)


# convert label to one-hot
ytr = keras.utils.to_categorical(ytr)          # ytr.shape = (60000, 10)
yte = keras.utils.to_categorical(yte)          # yte.shape = (60000, 10)


# ==========================================================================
# convert dataset to negative dataset
# ==========================================================================
xte_negative = np.empty(shape=(10000, 28, 28, 1))
for i in range(0, 10000):
    xte_negative[i] = 255 - xte[i]


# normalization
xte_negative = xte_negative.astype('float32')
xte_negative = xte_negative/255


# ==========================================================================
# evaluate model on negative dataset
# ==========================================================================
model = load_model('cnn_mnist.h5')

loss, accuracy = model.evaluate(xte_negative, yte)
print('loss = %f' % loss)
print('accuracy = %f' % accuracy)




