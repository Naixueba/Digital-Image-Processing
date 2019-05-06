# HW#4 Prob.2.(b)
# Submission Date: Mar.19
# Name: Wenjun Li
# USC ID: 8372-7611-20


import numpy as np
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
import cv2 as cv
import matplotlib.pyplot as plt
import imageio


# ==========================================================================
# read in image
# ==========================================================================
river1 = np.fromfile('river1.raw', dtype='uint8'); river2 = np.fromfile('river2.raw', dtype='uint8')
# reshape image into np.array
river1 = river1.reshape(1024, 768, 3); river2 = river2.reshape(1024, 768, 3)


# ==========================================================================
# convert raw image to jpg image, read in as gray image
# ==========================================================================
river1_jpg = imageio.imwrite("river1.jpg", river1)
river1_gray = cv.imread("river1.jpg", cv.IMREAD_GRAYSCALE)
river2_jpg = imageio.imwrite("river2.jpg", river2)
river2_gray = cv.imread("river2.jpg", cv.IMREAD_GRAYSCALE)


# ==========================================================================
# key-point
# ==========================================================================
sift = cv.xfeatures2d.SIFT_create()

river1_kp, river1_des = sift.detectAndCompute(river1_gray, None)
river2_kp, river2_des = sift.detectAndCompute(river2_gray, None)


# ==========================================================================
# Flann matching
# ==========================================================================
FLANN_INDEX_KDTREE = 1
index_paras = dict(algorithm=FLANN_INDEX_KDTREE, tress=5)
search_paras = dict(checks=50)

flann = cv.FlannBasedMatcher(index_paras, search_paras)
matches = flann.knnMatch(river1_des, river2_des, k=2)

goodMatch = []
for m, n in matches:
    if m.distance < 0.50*n.distance:
        goodMatch.append(m)
goodMatch = np.expand_dims(goodMatch, 1)
print(goodMatch[:20])


img_out = cv.drawMatchesKnn(river1_gray, river1_kp, river2_gray, river2_kp, goodMatch[:15], None, flags=2)


plt.imshow(img_out)
plt.show()

cv.imshow('image', img_out)
cv.waitKey(0)
cv.destroyAllWindows()

# plt.imshow(river1); plt.imshow(river2)
# plt.show()





