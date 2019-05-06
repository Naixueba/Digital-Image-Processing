# EE569 HW#6, Problem 2 -- Image reconstruction from Saab coefficients
# Name: Wenjun Li
# USC ID: 8372-7611-20
# Email: wenjunli@usc.edu
# Submission Date: April 27
#

import numpy as np
import cv2
from numpy import linalg
import pickle
from skimage.util.shape import view_as_windows


# define kernel numbers
N1 = 8
N2 = 128

# load the obtained kernels
f = open('./pca_params_compact_7_127.pkl', 'rb')
pca_params_compact = pickle.load(f)
# load weights and bias
k0 = np.array(pca_params_compact['Layer_0/kernel'])
k1 = np.array(pca_params_compact['Layer_1/kernel'])
bias = pca_params_compact['Layer_1/bias'].astype(np.float32)
# close file
f.close()

# load original images
image = cv2.imread('4.png', 0)
temp = view_as_windows(image, (4, 4), step=(4, 4)).reshape(8, 8, 1*4*4)
temp = np.matmul(temp, k0.T)
temp = view_as_windows(temp.copy(), (4, 4, 1), step=(4, 4, 1)).reshape(2, 2, N1*16)
temp += 1 / np.sqrt(N1*N2) * bias
temp = np.matmul(temp, k1.T)

# image reconstruction using weights and biases
inverse_temp = np.matmul(temp, linalg.pinv(k1.T))
inverse_temp += - 1 / np.sqrt(N1*N2) * bias
inverse_temp = inverse_temp.reshape(2, 2, N1, 16)
inverse_temp = np.moveaxis(inverse_temp, 2, 3)

# 2st stage reconstruction
stage_1 = np.zeros((8,8,N1))
for i in range(0,2):
    for j in range(0,2):
        for l in range(0,16):
            stage_1[i*4+l//4,j*4+l%4] = inverse_temp[i,j,l]

inverse_temp = stage_1
inverse_temp = np.dot(inverse_temp, linalg.pinv(np.transpose(k0)))

# 2nd stage reconstruction
stage_2 = np.zeros((32,32))
for i in range(0,8):
    for j in range(0,8):
        for l in range(0,16):
            stage_2[i*4+l//4,j*4+l%4]=inverse_temp[i,j,l]

inverse_temp = 255*(stage_2-np.min(stage_2)) / (np.max(stage_2)-np.min(stage_2))

# compute PSNR
square_error = np.square(stage_2 - image)
mean_square_error = np.mean(square_error)
PSNR = 10 * np.log10((255*255)/mean_square_error)
print(PSNR)

# output reconstrcuted image, the 1st number is 1st stage kernel number, the 2nd number is 2nd stage kernel number
cv2.imwrite('4_1st_%d_2nd_%d.jpg' % (N1, N2), inverse_temp)

# process complete
print('process completed')
