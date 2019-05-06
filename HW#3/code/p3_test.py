# HW#3 Prob.1.(c): lens distortion correction
# Submission Date: March.03, 2019
# Name: Wenjun Li
# USC ID: 8372-7611-20

import numpy as np
import scipy
import matplotlib.pyplot as plt

classroom = np.fromfile('classroom.raw', dtype='uint8')
classroom = classroom.reshape(712, 1072)
output = np.ones(shape=(712, 1072))
output = output * 255

k1 = -0.3536; k2 = 0.1730; k3 = 0; fx = 600; fy = 600
uc = int((1+1072)/2); vc = int((1+712)/2)

x = np.zeros(shape=(712, 1072)); y = np.zeros(shape=(712, 1072))
xd = np.zeros(shape=(712, 1072)); yd = np.zeros(shape=(712, 1072))
xc = np.zeros(shape=(712, 1072)); yc = np.zeros(shape=(712, 1072))

for i in range(0, 712):
    for j in range(0, 1072):
        x[i][j] = (i-vc)/fx
        y[i][j] = (j-uc)/fy
        r = x[i][j]*x[i][j] + y[i][j]*y[i][j]
        xd[i][j] = x[i][j] * (1 + k1*(r**2) + k2*(r**4))
        yd[i][j] = y[i][j] * (1 + k1*(r**2) + k2*(r**4))
        xc = int(xd[i][j] * 600 + vc)
        yc = int(yd[i][j] * 600 + uc)
        if 0 < xc < 712 and 0 < yc < 1072:
            output[i][j] = classroom[xc][yc]
        else:
            pass
output.astype('uint8').tofile('classroom_correction.raw')
print('finish')

