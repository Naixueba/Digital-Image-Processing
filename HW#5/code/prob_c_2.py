# HW#5 Prob.(c).(2)
# Submission Date: Mar.29
# Name: Wenjun Li
# USC ID: 8372-7611-20

"""
this code is for prob.(c).(2), a improved LeNet-5
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


# create a new training label and testing label
ytr_combined = np.empty(shape=(120000, 10))
yte_combined = np.empty(shape=(20000, 10))
for i in range(0, 60000):
    ytr_combined[2*i] = ytr[i]
    ytr_combined[2*i+1] = ytr[i]
for i in range(0, 10000):
    yte_combined[2*i] = yte[i]
    yte_combined[2*i+1] = yte[i]


# ==========================================================================
# convert dataset to negative dataset
# ==========================================================================
xtr_negative = np.empty(shape=(60000, 28, 28, 1))
for i in range(0, 60000):
    xtr_negative[i] = 255 - xtr[i]

xte_negative = np.empty(shape=(10000, 28, 28, 1))
for i in range(0, 10000):
    xte_negative[i] = 255 - xte[i]

# combine xtr_negative and xtr as one training dataset
xtr_combined = np.empty(shape=(120000, 28, 28, 1))
for i in range(0, 60000):
    xtr_combined[2*i] = xtr[i]
    xtr_combined[2*i+1] = xtr_negative[i]

# combine xte_negative and xte as one testing dataset
xte_combined = np.empty(shape=(20000, 28, 28, 1))
for i in range(0, 10000):
    xte_combined[2*i] = xte[i]
    xte_combined[2*i+1] = xte_negative[i]


# normalization
xtr_combined = xtr_combined.astype('float32'); xtr_combined = xtr_combined/255
xte_combined = xte_combined.astype('float32'); xte_combined = xte_combined/255
xte = xte.astype('float32'); xte = xte/255
xte_negative = xte_negative.astype('float32'); xte_negative = xte_negative/255

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
model.fit(xtr_combined, ytr_combined, batch_size=256, epochs=20, shuffle=True, validation_data=(xte, yte))
score = model.evaluate(xte_negative, yte, verbose=0)


# ==========================================================================
# save model
# ==========================================================================
model.save('cnn_mnist_new.h5')


print('finish')










