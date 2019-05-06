# HW#4 Prob.1.(c)
# Submission Date: Mar.19
# Name: Wenjun Li
# USC ID: 8372-7611-20


import numpy as np


image = np.fromfile('comb_output.raw', dtype='uint8')
# reshape image into np.array
image = image.reshape(510, 510)

window_size = 13; n = (int)((window_size-1)/2)
count0 = 0; count42 = 0; count84 = 0; count126 = 0
count168 = 0; count210 = 0; count255 = 0

print('finish')
# ==========================================================================
# search the window -> assign the center pixel with the most frequent pixel value
# ==========================================================================
for i in range(n, 510-n-1):
    for j in range(n, 510-n-1):
        for k in range(i-n, i+n+1):
            for l in range(j-n, j+n+1):
                if image[k, l] == 0:
                    count0 += 1
                if image[k, l] == 42:
                    count42 += 1
                if image[k, l] == 84:
                    count84 += 1
                if image[k, l] == 126:
                    count126 += 1
                if image[k, l] == 168:
                    count168 += 1
                if image[k, l] == 210:
                    count210 += 1
                if image[k, l] == 255:
                    count255 += 1
        number = np.array([count0, count42, count84, count126, count168, count210, count255])
        index = np.argmax(number)
        if index == 0:
            image[i, j] = 0
        if index == 1:
            image[i, j] = 42
        if index == 2:
            image[i, j] = 84
        if index == 3:
            image[i, j] = 126
        if index == 4:
            image[i, j] = 168
        if index == 5:
            image[i, j] = 210
        if index == 6:
            image[i, j] = 255


image.astype('uint8').tofile('image.raw')
print('finish')



