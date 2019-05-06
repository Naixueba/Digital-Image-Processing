# HW#4 Prob.1.(a)
# Submission Date: Mar.19
# Name: Wenjun Li
# USC ID: 8372-7611-20

"""
prob.(b)
this code build a LeNet-5 model
"""

import numpy as np
import struct
import matplotlib.pyplot as plt
import h5py

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D, Activation
from keras.datasets import mnist
from keras import backend as K


# ==========================================================================
# read in  dataset
# ==========================================================================
""" method to read in data """
"""
def read_idx(filename):
    with open(filename, 'rb') as f:
        zero, data_type, dims = struct.unpack('>HBB', f.read(4))
        shape = tuple(struct.unpack('>I', f.read(4))[0] for d in range(dims))
        return np.fromstring(f.read(), dtype=np.uint8).reshape(shape)


train_image = read_idx('train-images-idx3-ubyte')
train_label = read_idx('train-labels-idx1-ubyte')
test_image = read_idx('t10k-labels-idx1-ubyte')
test_label = read_idx('train-labels-idx1-ubyte')
print(train_image.shape); print(train_label.shape)
print(test_image.shape); print(test_label.shape)
"""
(xtr, ytr), (xte, yte) = mnist.load_data()
# xtr.shape = (60000, 28, 28); ytr.shape = (60000,)

if K.image_data_format() == 'channels_first':
    xtr = xtr.reshape(xtr.shape[0], 1, 28, 28)
    xte = xte.reshape(xte.shape[0], 1, 28, 28)
    input_shape = (1, 28, 28)
else:
    xtr = xtr.reshape(xtr.shape[0], 28, 28, 1)
    xte = xte.reshape(xte.shape[0], 28, 28, 1)
    input_shape = (28, 28, 1)

# xtr.shape = (60000, 28, 28, 1)


# normalization
xtr = xtr.astype('float32')
xte = xte.astype('float32')
xtr = xtr/255
xte = xte/255


# convert label to one-hot
ytr = keras.utils.to_categorical(ytr)          # ytr.shape = (60000, 10)
yte = keras.utils.to_categorical(yte)          # yte.shape = (60000, 10)


# ==========================================================================
# model structure
# ==========================================================================
model = Sequential()

# 1st conv layer
model.add(Conv2D(filters=6, kernel_size=(5, 5), padding='same', input_shape=(28, 28, 1)))
model.add(Activation(activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

# 2nd conv layer
model.add(Conv2D(filters=16, kernel_size=(5, 5), input_shape=(14, 14, 1)))
model.add(Activation(activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

# flatten
model.add(Flatten())

# fully connected layer
model.add(Dense(units=120, activation='relu'))
model.add(Dense(units=84, activation='relu'))
model.add(Dense(units=10, activation='softmax'))


# ==========================================================================
# compile model
# ==========================================================================
# adam = keras.optimizers.Adam(lr=0.0001)
# sgd = keras.optimizers.SGD(lr=0.00001)
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])


# ==========================================================================
# train model
# ==========================================================================
model.fit(xtr, ytr, batch_size=256, epochs=20, validation_data=(xte, yte))
score = model.evaluate(xte, yte, verbose=0)


# ==========================================================================
# save model
# ==========================================================================
model.save('cnn_mnist.h5')


print('finish')







