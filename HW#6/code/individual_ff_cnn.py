# EE569 HW#6, Problem 3 -- Handwritten digits recognition using ensembles of FF-CNNS
# Name: Wenjun Li
# USC ID: 8372-7611-20
# Email: wenjunli@usc.edu
# Submission Date: April 27
# File Description: individual FF-CNN

import numpy as np
import pickle

import keras
from keras.layers import Conv2D, MaxPool2D, Dense, Flatten
from keras.models import Sequential
from keras.datasets import mnist

from tensorflow.python.platform import flags


flags.DEFINE_string("useclasses", "0-9", "Supported format: 0,1,5-9")
F = flags.FLAGS


class load_params():
    def __init__(self, pca_params_compact, llsr_weights_compact, llsr_bias_compact):
        self.pca_params_file = pca_params_compact
        self.weight_file = llsr_weights_compact
        self.bias_file = llsr_bias_compact
        self.pca_params, self.weight, self.bias = 0, 0, 0

    def load_file(self):
        # load pca params file
        f_pca_params = open(self.pca_params_file, 'rb')
        self.pca_params = pickle.load(f_pca_params)
        # load weight file
        f_weight = open(self.weight_file, 'rb')
        self.weights = pickle.load(f_weight)
        # load bias file
        f_bias = open(self.bias_file, 'rb')
        self.bias = pickle.load(f_bias)
        # close file
        f_pca_params.close()
        f_weight.close()
        f_bias.close()

    def weight_conv_1(self, shape):
        weight = self.pca_params['Layer_0/kernel'].astype(np.float32)
        weight = np.moveaxis(weight, 1, 0)
        return weight.reshape(shape)

    def weight_conv_2(self, shape):
        weight = self.pca_params['Layer_1/kernel'].astype(np.float32)
        weight = np.moveaxis(weight, 1, 0).reshape(6, 5, 5, 16)
        weight = np.moveaxis(weight, 0, 2)
        return weight.reshape(shape)

    def bias_conv_1(self, shape):
        temp_bias = (-1 * np.dot(self.pca_params['Layer_0/feature_expectation'], self.pca_params['Layer_0/kernel'].T))
        temp_bias = temp_bias.reshape(shape)
        return temp_bias

    def bias_conv_2(self, shape):
        weight = self.pca_params['Layer_1/kernel'].astype(np.float32).T
        temp_bias = (-1 * np.dot(self.pca_params['Layer_1/feature_expectation'].astype(np.float32), weight)).reshape(shape)

        bias = np.empty(shape=(150,))
        bias += 1 / np.sqrt(150) * self.pca_params['Layer_1/bias'].astype(np.float32)

        bias_1 = np.dot(bias, weight)

        bias_2 = np.empty(shape, dtype=np.float32)
        bias_2[0] = -1
        bias_2 = bias_2 * self.pca_params['Layer_1/bias'].astype(np.float32)

        bias_final = temp_bias + bias_1 + bias_2
        return bias_final.reshape(shape)

    # define functions to load parameters
    def weight_fc_1(self, shape):
        return self.weights['0 LLSR weight'].reshape(shape)

    def bias_fc_1(self, shape):
        return self.bias['0 LLSR bias'].reshape(shape)

    def weight_fc_2(self, shape):
        return self.weights['1 LLSR weight'].reshape(shape)

    def bias_fc_2(self, shape):
        return self.bias['1 LLSR bias'].reshape(shape)

    def weight_fc_3(self, shape):
        return self.weights['2 LLSR weight'].reshape(shape)

    def bias_fc_3(self, shape):
        return self.bias['2 LLSR bias'].reshape(shape)


def lenet5_ff(pca_params_compact, llsr_weights_compact, llsr_bias_compact, xtr, ytr, xte, yte):
    parameters = load_params(pca_params_compact, llsr_weights_compact, llsr_bias_compact)
    parameters.load_file()

    model = Sequential()

    # 1st conv layer
    model.add(Conv2D(filters=6, kernel_size=(5, 5), strides=1, padding='same', kernel_initializer=parameters.weight_conv_1, input_shape=(28, 28, 1), bias_initializer=parameters.bias_conv_1))
    model.add(MaxPool2D(pool_size=(2, 2), strides=(2, 2)))

    # 2nd conv layer
    model.add(Conv2D(filters=16, kernel_size=(5, 5), strides=1, padding='valid', kernel_initializer=parameters.weight_conv_2, bias_initializer=parameters.bias_conv_2))
    model.add(MaxPool2D(pool_size=(2, 2), strides=(2, 2)))

    # flatten
    model.add(Flatten())

    # fully connected layer
    model.add(Dense(120, activation='relu', kernel_initializer=parameters.weight_fc_1, bias_initializer=parameters.bias_fc_1))
    model.add(Dense(84, activation='relu', kernel_initializer=parameters.weight_fc_2, bias_initializer=parameters.bias_fc_2))
    model.add(Dense(10, activation='softmax', kernel_initializer=parameters.weight_fc_3,  bias_initializer=parameters.bias_fc_3))

    # compile model
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

    # print training acc and testing acc
    if ytr.all != None:
        scores = model.evaluate(xtr, ytr)
        print('training accuracy: %f' % scores[1])
        print('training loss: %f' % scores[0])

        # predict on testing sampel
        scores = model.evaluate(xte, yte)
        print('testing accuracy: %f' % scores[1])
        print('testing loss: %f' % scores[0])

    return model.predict(xtr), model.predict(xte)


# load MNIST dataset, split into xtr, ytr, xte, yte
(xtr, ytr), (xte, yte) = mnist.load_data()
xtr = xtr.reshape(xtr.shape[0], 28, 28, 1)
xtr = xtr/255
xte = xte.reshape(xte.shape[0], 28, 28, 1)
xte = xte/255
yte = keras.utils.to_categorical(yte, 10)
ytr = keras.utils.to_categorical(ytr, 10)

# run individual FF-CNN
lenet5_ff('./pca_params_compact.pkl', './llsr_weights_compact_v2.pkl', './llsr_bias_compact_v2.pkl', xtr, ytr, xte, yte)

