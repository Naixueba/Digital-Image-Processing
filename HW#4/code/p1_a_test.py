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
    output = np.zeros(shape=(128, 128))
    for i in range(2, 130):
        for j in range(2, 130):
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


# feature normalization: (** - mean) / std
def featuer_normalization(feature_vector):
    # feature_vector.shape = (12, 25); feature_vector.shape.T = (25, 12)
    feature_vector = feature_vector.T
    for channel in range(0, 25):
        feature_vector[channel] = (feature_vector[channel] - np.mean(feature_vector[channel])) / np.std(feature_vector[channel])
    feature_vector = feature_vector.T
    return feature_vector


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
texture1 = np.fromfile('texture1.raw', dtype='uint8'); texture2 = np.fromfile('texture2.raw', dtype='uint8'); texture3 = np.fromfile('texture3.raw', dtype='uint8')
texture4 = np.fromfile('texture4.raw', dtype='uint8'); texture5 = np.fromfile('texture5.raw', dtype='uint8'); texture6 = np.fromfile('texture6.raw', dtype='uint8')
texture7 = np.fromfile('texture7.raw', dtype='uint8'); texture8 = np.fromfile('texture8.raw', dtype='uint8'); texture9 = np.fromfile('texture9.raw', dtype='uint8')
texture10= np.fromfile('texture10.raw', dtype='uint8');texture11 =np.fromfile('texture11.raw', dtype='uint8');texture12 =np.fromfile('texture12.raw', dtype='uint8')
# reshape image into np.array
texture1 = texture1.reshape(128, 128); texture2 = texture2.reshape(128, 128); texture3 = texture3.reshape(128, 128); texture4 = texture4.reshape(128, 128)
texture5 = texture5.reshape(128, 128); texture6 = texture6.reshape(128, 128); texture7 = texture7.reshape(128, 128); texture8 = texture8.reshape(128, 128)
texture9 = texture9.reshape(128, 128); texture10 =texture10.reshape(128, 128);texture11 =texture11.reshape(128, 128);texture12 =texture12.reshape(128, 128)
# subtract image mean
texture1 = texture1 - texture1.mean(); texture2 = texture2 - texture2.mean(); texture3 = texture3 - texture3.mean(); texture4 = texture4 - texture4.mean()
texture5 = texture5 - texture5.mean(); texture6 = texture6 - texture6.mean(); texture7 = texture7 - texture7.mean(); texture8 = texture8 - texture8.mean()
texture9 = texture9 - texture9.mean(); texture10 =texture10 -texture10.mean();texture11 =texture11 -texture11.mean();texture12 =texture12 -texture12.mean()


# ==========================================================================
# convolution: compute 25-D filtered image
# ==========================================================================
img1_filtered = np.array([conv2D(texture1, l5l5), conv2D(texture1, l5e5), conv2D(texture1, l5s5), conv2D(texture1, l5w5), conv2D(texture1, l5r5),
                          conv2D(texture1, e5l5), conv2D(texture1, e5e5), conv2D(texture1, e5s5), conv2D(texture1, e5w5), conv2D(texture1, e5r5),
                          conv2D(texture1, s5l5), conv2D(texture1, s5e5), conv2D(texture1, s5s5), conv2D(texture1, s5w5), conv2D(texture1, s5r5),
                          conv2D(texture1, w5l5), conv2D(texture1, w5e5), conv2D(texture1, w5s5), conv2D(texture1, w5w5), conv2D(texture1, w5r5),
                          conv2D(texture1, r5l5), conv2D(texture1, r5e5), conv2D(texture1, r5s5), conv2D(texture1, r5w5), conv2D(texture1, r5r5)])

img2_filtered = np.array([conv2D(texture2, l5l5), conv2D(texture2, l5e5), conv2D(texture2, l5s5), conv2D(texture2, l5w5), conv2D(texture2, l5r5),
                          conv2D(texture2, e5l5), conv2D(texture2, e5e5), conv2D(texture2, e5s5), conv2D(texture2, e5w5), conv2D(texture2, e5r5),
                          conv2D(texture2, s5l5), conv2D(texture2, s5e5), conv2D(texture2, s5s5), conv2D(texture2, s5w5), conv2D(texture2, s5r5),
                          conv2D(texture2, w5l5), conv2D(texture2, w5e5), conv2D(texture2, w5s5), conv2D(texture2, w5w5), conv2D(texture2, w5r5),
                          conv2D(texture2, r5l5), conv2D(texture2, r5e5), conv2D(texture2, r5s5), conv2D(texture2, r5w5), conv2D(texture2, r5r5)])

img3_filtered = np.array([conv2D(texture3, l5l5), conv2D(texture3, l5e5), conv2D(texture3, l5s5), conv2D(texture3, l5w5), conv2D(texture3, l5r5),
                          conv2D(texture3, e5l5), conv2D(texture3, e5e5), conv2D(texture3, e5s5), conv2D(texture3, e5w5), conv2D(texture3, e5r5),
                          conv2D(texture3, s5l5), conv2D(texture3, s5e5), conv2D(texture3, s5s5), conv2D(texture3, s5w5), conv2D(texture3, s5r5),
                          conv2D(texture3, w5l5), conv2D(texture3, w5e5), conv2D(texture3, w5s5), conv2D(texture3, w5w5), conv2D(texture3, w5r5),
                          conv2D(texture3, r5l5), conv2D(texture3, r5e5), conv2D(texture3, r5s5), conv2D(texture3, r5w5), conv2D(texture3, r5r5)])

img4_filtered = np.array([conv2D(texture4, l5l5), conv2D(texture4, l5e5), conv2D(texture4, l5s5), conv2D(texture4, l5w5), conv2D(texture4, l5r5),
                          conv2D(texture4, e5l5), conv2D(texture4, e5e5), conv2D(texture4, e5s5), conv2D(texture4, e5w5), conv2D(texture4, e5r5),
                          conv2D(texture4, s5l5), conv2D(texture4, s5e5), conv2D(texture4, s5s5), conv2D(texture4, s5w5), conv2D(texture4, s5r5),
                          conv2D(texture4, w5l5), conv2D(texture4, w5e5), conv2D(texture4, w5s5), conv2D(texture4, w5w5), conv2D(texture4, w5r5),
                          conv2D(texture4, r5l5), conv2D(texture4, r5e5), conv2D(texture4, r5s5), conv2D(texture4, r5w5), conv2D(texture4, r5r5)])

img5_filtered = np.array([conv2D(texture5, l5l5), conv2D(texture5, l5e5), conv2D(texture5, l5s5), conv2D(texture5, l5w5), conv2D(texture5, l5r5),
                          conv2D(texture5, e5l5), conv2D(texture5, e5e5), conv2D(texture5, e5s5), conv2D(texture5, e5w5), conv2D(texture5, e5r5),
                          conv2D(texture5, s5l5), conv2D(texture5, s5e5), conv2D(texture5, s5s5), conv2D(texture5, s5w5), conv2D(texture5, s5r5),
                          conv2D(texture5, w5l5), conv2D(texture5, w5e5), conv2D(texture5, w5s5), conv2D(texture5, w5w5), conv2D(texture5, w5r5),
                          conv2D(texture5, r5l5), conv2D(texture5, r5e5), conv2D(texture5, r5s5), conv2D(texture5, r5w5), conv2D(texture5, r5r5)])

img6_filtered = np.array([conv2D(texture6, l5l5), conv2D(texture6, l5e5), conv2D(texture6, l5s5), conv2D(texture6, l5w5), conv2D(texture6, l5r5),
                          conv2D(texture6, e5l5), conv2D(texture6, e5e5), conv2D(texture6, e5s5), conv2D(texture6, e5w5), conv2D(texture6, e5r5),
                          conv2D(texture6, s5l5), conv2D(texture6, s5e5), conv2D(texture6, s5s5), conv2D(texture6, s5w5), conv2D(texture6, s5r5),
                          conv2D(texture6, w5l5), conv2D(texture6, w5e5), conv2D(texture6, w5s5), conv2D(texture6, w5w5), conv2D(texture6, w5r5),
                          conv2D(texture6, r5l5), conv2D(texture6, r5e5), conv2D(texture6, r5s5), conv2D(texture6, r5w5), conv2D(texture6, r5r5)])

img7_filtered = np.array([conv2D(texture7, l5l5), conv2D(texture7, l5e5), conv2D(texture7, l5s5), conv2D(texture7, l5w5), conv2D(texture7, l5r5),
                          conv2D(texture7, e5l5), conv2D(texture7, e5e5), conv2D(texture7, e5s5), conv2D(texture7, e5w5), conv2D(texture7, e5r5),
                          conv2D(texture7, s5l5), conv2D(texture7, s5e5), conv2D(texture7, s5s5), conv2D(texture7, s5w5), conv2D(texture7, s5r5),
                          conv2D(texture7, w5l5), conv2D(texture7, w5e5), conv2D(texture7, w5s5), conv2D(texture7, w5w5), conv2D(texture7, w5r5),
                          conv2D(texture7, r5l5), conv2D(texture7, r5e5), conv2D(texture7, r5s5), conv2D(texture7, r5w5), conv2D(texture7, r5r5)])

img8_filtered = np.array([conv2D(texture8, l5l5), conv2D(texture8, l5e5), conv2D(texture8, l5s5), conv2D(texture8, l5w5), conv2D(texture8, l5r5),
                          conv2D(texture8, e5l5), conv2D(texture8, e5e5), conv2D(texture8, e5s5), conv2D(texture8, e5w5), conv2D(texture8, e5r5),
                          conv2D(texture8, s5l5), conv2D(texture8, s5e5), conv2D(texture8, s5s5), conv2D(texture8, s5w5), conv2D(texture8, s5r5),
                          conv2D(texture8, w5l5), conv2D(texture8, w5e5), conv2D(texture8, w5s5), conv2D(texture8, w5w5), conv2D(texture8, w5r5),
                          conv2D(texture8, r5l5), conv2D(texture8, r5e5), conv2D(texture8, r5s5), conv2D(texture8, r5w5), conv2D(texture8, r5r5)])

img9_filtered = np.array([conv2D(texture9, l5l5), conv2D(texture9, l5e5), conv2D(texture9, l5s5), conv2D(texture9, l5w5), conv2D(texture9, l5r5),
                          conv2D(texture9, e5l5), conv2D(texture9, e5e5), conv2D(texture9, e5s5), conv2D(texture9, e5w5), conv2D(texture9, e5r5),
                          conv2D(texture9, s5l5), conv2D(texture9, s5e5), conv2D(texture9, s5s5), conv2D(texture9, s5w5), conv2D(texture9, s5r5),
                          conv2D(texture9, w5l5), conv2D(texture9, w5e5), conv2D(texture9, w5s5), conv2D(texture9, w5w5), conv2D(texture9, w5r5),
                          conv2D(texture9, r5l5), conv2D(texture9, r5e5), conv2D(texture9, r5s5), conv2D(texture9, r5w5), conv2D(texture9, r5r5)])

img10_filtered = np.array([conv2D(texture10, l5l5), conv2D(texture10, l5e5), conv2D(texture10, l5s5), conv2D(texture10, l5w5), conv2D(texture10, l5r5),
                          conv2D(texture10, e5l5), conv2D(texture10, e5e5), conv2D(texture10, e5s5), conv2D(texture10, e5w5), conv2D(texture10, e5r5),
                          conv2D(texture10, s5l5), conv2D(texture10, s5e5), conv2D(texture10, s5s5), conv2D(texture10, s5w5), conv2D(texture10, s5r5),
                          conv2D(texture10, w5l5), conv2D(texture10, w5e5), conv2D(texture10, w5s5), conv2D(texture10, w5w5), conv2D(texture10, w5r5),
                          conv2D(texture10, r5l5), conv2D(texture10, r5e5), conv2D(texture10, r5s5), conv2D(texture10, r5w5), conv2D(texture10, r5r5)])

img11_filtered = np.array([conv2D(texture11, l5l5), conv2D(texture11, l5e5), conv2D(texture11, l5s5), conv2D(texture11, l5w5), conv2D(texture11, l5r5),
                          conv2D(texture11, e5l5), conv2D(texture11, e5e5), conv2D(texture11, e5s5), conv2D(texture11, e5w5), conv2D(texture11, e5r5),
                          conv2D(texture11, s5l5), conv2D(texture11, s5e5), conv2D(texture11, s5s5), conv2D(texture11, s5w5), conv2D(texture11, s5r5),
                          conv2D(texture11, w5l5), conv2D(texture11, w5e5), conv2D(texture11, w5s5), conv2D(texture11, w5w5), conv2D(texture11, w5r5),
                          conv2D(texture11, r5l5), conv2D(texture11, r5e5), conv2D(texture11, r5s5), conv2D(texture11, r5w5), conv2D(texture11, r5r5)])

img12_filtered = np.array([conv2D(texture12, l5l5), conv2D(texture12, l5e5), conv2D(texture12, l5s5), conv2D(texture12, l5w5), conv2D(texture12, l5r5),
                          conv2D(texture12, e5l5), conv2D(texture12, e5e5), conv2D(texture12, e5s5), conv2D(texture12, e5w5), conv2D(texture12, e5r5),
                          conv2D(texture12, s5l5), conv2D(texture12, s5e5), conv2D(texture12, s5s5), conv2D(texture12, s5w5), conv2D(texture12, s5r5),
                          conv2D(texture12, w5l5), conv2D(texture12, w5e5), conv2D(texture12, w5s5), conv2D(texture12, w5w5), conv2D(texture12, w5r5),
                          conv2D(texture12, r5l5), conv2D(texture12, r5e5), conv2D(texture12, r5s5), conv2D(texture12, r5w5), conv2D(texture12, r5r5)])


# ==========================================================================
# energy averaging
# ==========================================================================
avg_eng_1 = energy_avg(img1_filtered); avg_eng_7 = energy_avg(img7_filtered)
avg_eng_2 = energy_avg(img2_filtered); avg_eng_8 = energy_avg(img8_filtered)
avg_eng_3 = energy_avg(img3_filtered); avg_eng_9 = energy_avg(img9_filtered)
avg_eng_4 = energy_avg(img4_filtered); avg_eng_10 = energy_avg(img10_filtered)
avg_eng_5 = energy_avg(img5_filtered); avg_eng_11 = energy_avg(img11_filtered)
avg_eng_6 = energy_avg(img6_filtered); avg_eng_12 = energy_avg(img12_filtered)


# ==========================================================================
# 12x25 feature vector
# ==========================================================================
feature_vector = np.zeros(shape=(12, 25))
feature_vector[0] = avg_eng_1; feature_vector[6] = avg_eng_7
feature_vector[1] = avg_eng_2; feature_vector[7] = avg_eng_8
feature_vector[2] = avg_eng_3; feature_vector[8] = avg_eng_9
feature_vector[3] = avg_eng_4; feature_vector[9] = avg_eng_10
feature_vector[4] = avg_eng_5; feature_vector[10] = avg_eng_11
feature_vector[5] = avg_eng_6; feature_vector[11] = avg_eng_12


# ==========================================================================
# Normalization
# ==========================================================================
feature_vector_normalized = featuer_normalization(feature_vector)


# ==========================================================================
# k-means cluster to 12x25
# ==========================================================================
Kmean_cluster(feature_vector_normalized)


# ==========================================================================
# PCA: 12x25 to 12x3
# ==========================================================================
feature_vector_PCA = np.zeros(shape=(12, 3))
feature_vector_PCA = PCA_reduction(feature_vector_normalized)

Kmean_cluster(feature_vector_PCA)




