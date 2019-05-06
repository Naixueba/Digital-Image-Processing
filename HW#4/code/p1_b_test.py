import numpy as np
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA


# ==========================================================================
# generate laws filter
# ==========================================================================
l5 = np.array([[1, 4, 6, 4, 1]])
e5 = np.array([[-1, -2, 0, 2, 1]])
s5 = np.array([[-1, 0, 2, 0, -1]])
w5 = np.array([[-1, 2, 0, -2, 1]])
r5 = np.array([[1, -4, 6, -4, 1]])

l5l5 = np.matmul(l5.T, l5); l5e5 = np.matmul(l5.T, e5); l5s5 = np.matmul(l5.T, s5); l5w5 = np.matmul(l5.T, w5); l5r5 = np.matmul(l5.T, r5)
e5l5 = np.matmul(e5.T, l5); e5e5 = np.matmul(e5.T, e5); e5s5 = np.matmul(e5.T, s5); e5w5 = np.matmul(e5.T, w5); e5r5 = np.matmul(e5.T, r5)
s5l5 = np.matmul(s5.T, l5); s5e5 = np.matmul(s5.T, e5); s5s5 = np.matmul(s5.T, s5); s5w5 = np.matmul(s5.T, w5); s5r5 = np.matmul(s5.T, r5)
w5l5 = np.matmul(w5.T, l5); w5e5 = np.matmul(w5.T, e5); w5s5 = np.matmul(w5.T, s5); w5w5 = np.matmul(w5.T, w5); w5r5 = np.matmul(w5.T, r5)
r5l5 = np.matmul(r5.T, l5); r5e5 = np.matmul(r5.T, e5); r5s5 = np.matmul(r5.T, s5); r5w5 = np.matmul(r5.T, w5); r5r5 = np.matmul(r5.T, r5)


# do boundary extension to original image
def boundary_extension(input):
    size = input.shape[0]
    output = np.zeros(shape=(size+4, size+4))
    for i in range(0, size):
        for j in range(0, size):
            # copy
            output[i+2][j+2] = input[i][j]
    # row extension
    for i in range(0, size):
        for j in range(0, size):
            if i == 0:
                output[1][j+2] = input[i][j]
            if i == 1:
                output[0][j+2] = input[i][j]
            if i == size-1:
                output[size+2][j+2] = input[i][j]
            if i == size-2:
                output[size+3][j+2] = input[i][j]

    # column extension
    for i in range(0, size+4):
        for j in range(0, size+4):
            if j == 0:
                output[i][0] = output[i][3]
            if j == 1:
                output[i][1] = output[i][2]
            if j == size+2:
                output[i][size+2] = output[i][size+1]
            if j == size+3:
                output[i][size+3] = output[i][size]

    return output


# weighted_sum convolution
def weighted_sum(pixel_stream, filter):
    size = filter.shape[0]
    matrix_product = np.multiply(pixel_stream, filter)
    sum = np.sum(matrix_product)
    return sum


# conv2D
def conv2D(input_image, filter):
    img_etd = boundary_extension(input_image)
    output = np.zeros(shape=(510, 510))
    for i in range(2, 512):
        for j in range(2, 512):
            ps = np.array([[img_etd[i - 2][j - 2], img_etd[i - 2][j - 1], img_etd[i - 2][j], img_etd[i - 2][j + 1], img_etd[i - 2][j + 2]],
                           [img_etd[i - 1][j - 2], img_etd[i - 1][j - 1], img_etd[i - 1][j], img_etd[i - 1][j + 1], img_etd[i - 1][j + 2]],
                           [img_etd[i][j - 2], img_etd[i][j - 1], img_etd[i][j], img_etd[i][j + 1], img_etd[i][j + 2]],
                           [img_etd[i + 1][j - 2], img_etd[i + 1][j - 1], img_etd[i + 1][j], img_etd[i + 1][j + 1], img_etd[i + 1][j + 2]],
                           [img_etd[i + 2][j - 2], img_etd[i + 2][j - 1], img_etd[i + 2][j], img_etd[i + 2][j + 1], img_etd[i + 2][j + 2]]])
            matrix_product = np.multiply(ps, filter); sum = np.sum(matrix_product)
            output[i-2, j-2] = sum
    return output


# averaging 25-D feature vector and form feature average vector
def energy_avg(filtered_img):
    eng_avg_vector = np.zeros(shape=(25, ))
    for channel in range(0, 25):
        square_value = np.square(filtered_img[channel])
        eng_avg_vector[channel] = np.sum(square_value) / (128*128)
    return eng_avg_vector


# PCA dimension reduction; input 12x25 vector & return 12x3 vector
def PCA_reduction(feature_vector):
    pca = PCA(n_components=3)
    data_PCA = pca.fit_transform(feature_vector)
    return data_PCA


# kmean clustering; return clustering label
def Kmean_cluster(data):
    kmeans = KMeans(n_clusters=4)
    kmeans.fit_transform(data)
    print(kmeans.labels_)


# ==========================================================================
# read in image
# ==========================================================================
comb = np.fromfile('comb.raw', dtype='uint8')
# reshape image into np.array
comb = comb.reshape(510, 510)
# subtract image mean
comb = comb - comb.mean()


# ==========================================================================
# convolution: compute 25-D filtered image
# ==========================================================================
img_filtered = np.array([conv2D(comb, l5l5), conv2D(comb, l5e5), conv2D(comb, l5s5), conv2D(comb, l5w5), conv2D(comb, l5r5),
                         conv2D(comb, e5l5), conv2D(comb, e5e5), conv2D(comb, e5s5), conv2D(comb, e5w5), conv2D(comb, e5r5),
                         conv2D(comb, s5l5), conv2D(comb, s5e5), conv2D(comb, s5s5), conv2D(comb, s5w5), conv2D(comb, s5r5),
                         conv2D(comb, w5l5), conv2D(comb, w5e5), conv2D(comb, w5s5), conv2D(comb, w5w5), conv2D(comb, w5r5),
                         conv2D(comb, r5l5), conv2D(comb, r5e5), conv2D(comb, r5s5), conv2D(comb, r5w5), conv2D(comb, r5r5)])

print(img_filtered.shape)










