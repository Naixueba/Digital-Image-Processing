# HW#4 Prob.1.(b)
# Submission Date: Mar.19
# Name: Wenjun Li
# USC ID: 8372-7611-20


import numpy as np
from sklearn.cluster import KMeans
from sklearn.decomposition import PCA
import cv2 as cv


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
    # weighted_sum = sum/(size*size)
    # return weighted_sum
    return sum


# form a feature vector of 25-D
def vector_stack(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25):
    v = np.zeros(shape=(25, 128, 128))
    v[0] = v1; v[5] = v6; v[10] = v11; v[15] = v16; v[20] = v21
    v[1] = v2; v[6] = v7; v[11] = v12; v[16] = v17; v[21] = v22
    v[2] = v3; v[7] = v8; v[12] = v13; v[17] = v18; v[22] = v23
    v[3] = v4; v[8] = v9; v[13] = v14; v[18] = v19; v[23] = v24
    v[4] = v5; v[9] = v10;v[14] = v15; v[19] = v20; v[24] = v25
    return v


# get an 25-D output feature vector from input image
def get_feature_matrix(etd_img):
    # generate filtered image bank
    ll = np.zeros(shape=(128, 128)); le = np.zeros(shape=(128, 128)); ls = np.zeros(shape=(128, 128)); lw = np.zeros(shape=(128, 128)); lr = np.zeros(shape=(128, 128))
    el = np.zeros(shape=(128, 128)); ee = np.zeros(shape=(128, 128)); es = np.zeros(shape=(128, 128)); ew = np.zeros(shape=(128, 128)); er = np.zeros(shape=(128, 128))
    sl = np.zeros(shape=(128, 128)); se = np.zeros(shape=(128, 128)); ss = np.zeros(shape=(128, 128)); sw = np.zeros(shape=(128, 128)); sr = np.zeros(shape=(128, 128))
    wl = np.zeros(shape=(128, 128)); we = np.zeros(shape=(128, 128)); ws = np.zeros(shape=(128, 128)); ww = np.zeros(shape=(128, 128)); wr = np.zeros(shape=(128, 128))
    rl = np.zeros(shape=(128, 128)); re = np.zeros(shape=(128, 128)); rs = np.zeros(shape=(128, 128)); rw = np.zeros(shape=(128, 128)); rr = np.zeros(shape=(128, 128))

    # convolution operation & get filtered image & square
    for i in range(2, 130):
        for j in range(2, 130):
            ps = np.array([[etd_img[i-2][j-2], etd_img[i-2][j-1], etd_img[i-2][j], etd_img[i-2][j+1], etd_img[i-2][j+2]],
                           [etd_img[i-1][j-2], etd_img[i-1][j-1], etd_img[i-1][j], etd_img[i-1][j+1], etd_img[i-1][j+2]],
                           [etd_img[i][j - 2], etd_img[i][j - 1], etd_img[i][j], etd_img[i][j + 1], etd_img[i][j + 2]],
                           [etd_img[i+1][j-2], etd_img[i+1][j-1], etd_img[i+1][j], etd_img[i+1][j+1], etd_img[i+1][j+2]],
                           [etd_img[i+2][j-2], etd_img[i+2][j-1], etd_img[i+2][j], etd_img[i+2][j+1], etd_img[i+2][j+2]]])

            ll[i-2][j-2] = np.square(weighted_sum(ps, l5l5)); le[i-2][j-2] = np.square(weighted_sum(ps, l5e5)); ls[i-2][j-2] = np.square(weighted_sum(ps, l5s5)); lw[i-2][j-2] = np.square(weighted_sum(ps, l5w5)); lr[i-2][j-2] = np.square(weighted_sum(ps, l5r5))
            el[i-2][j-2] = np.square(weighted_sum(ps, e5l5)); ee[i-2][j-2] = np.square(weighted_sum(ps, e5e5)); es[i-2][j-2] = np.square(weighted_sum(ps, e5s5)); ew[i-2][j-2] = np.square(weighted_sum(ps, e5w5)); er[i-2][j-2] = np.square(weighted_sum(ps, e5r5))
            sl[i-2][j-2] = np.square(weighted_sum(ps, s5l5)); se[i-2][j-2] = np.square(weighted_sum(ps, s5e5)); ss[i-2][j-2] = np.square(weighted_sum(ps, s5s5)); sw[i-2][j-2] = np.square(weighted_sum(ps, s5w5)); sr[i-2][j-2] = np.square(weighted_sum(ps, s5r5))
            wl[i-2][j-2] = np.square(weighted_sum(ps, w5l5)); we[i-2][j-2] = np.square(weighted_sum(ps, w5e5)); ws[i-2][j-2] = np.square(weighted_sum(ps, w5s5)); ww[i-2][j-2] = np.square(weighted_sum(ps, w5w5)); wr[i-2][j-2] = np.square(weighted_sum(ps, w5r5))
            rl[i-2][j-2] = np.square(weighted_sum(ps, r5l5)); re[i-2][j-2] = np.square(weighted_sum(ps, r5e5)); rs[i-2][j-2] = np.square(weighted_sum(ps, r5s5)); rw[i-2][j-2] = np.square(weighted_sum(ps, r5w5)); rr[i-2][j-2] = np.square(weighted_sum(ps, r5r5))

    # 25-D feature vector
    output_img = np.zeros(shape=(25, 128, 128))
    output_img = vector_stack(ll, le, ls, lw, lr,
                              el, ee, es, ew, er,
                              sl, se, ss, sw, sr,
                              wl, we, ws, ww, wr,
                              rl, re, rs, rw, rr)

    # output
    return output_img


# averaging 25-D feature vector and form feature average vector
def feature_avg(fea_vector):
    avg_vector = np.zeros(shape=(25, 1, 1))
    avg_vector[0] = np.sum(fea_vector[0]) / (128*128);  avg_vector[15] = np.sum(fea_vector[15]) / (128*128)
    avg_vector[1] = np.sum(fea_vector[1]) / (128*128);  avg_vector[16] = np.sum(fea_vector[16]) / (128*128)
    avg_vector[2] = np.sum(fea_vector[2]) / (128*128);  avg_vector[17] = np.sum(fea_vector[17]) / (128*128)
    avg_vector[3] = np.sum(fea_vector[3]) / (128*128);  avg_vector[18] = np.sum(fea_vector[18]) / (128*128)
    avg_vector[4] = np.sum(fea_vector[4]) / (128*128);  avg_vector[19] = np.sum(fea_vector[19]) / (128*128)
    avg_vector[5] = np.sum(fea_vector[5]) / (128*128);  avg_vector[20] = np.sum(fea_vector[20]) / (128*128)
    avg_vector[6] = np.sum(fea_vector[6]) / (128*128);  avg_vector[21] = np.sum(fea_vector[21]) / (128*128)
    avg_vector[7] = np.sum(fea_vector[7]) / (128*128);  avg_vector[22] = np.sum(fea_vector[22]) / (128*128)
    avg_vector[8] = np.sum(fea_vector[8]) / (128*128);  avg_vector[23] = np.sum(fea_vector[23]) / (128*128)
    avg_vector[9] = np.sum(fea_vector[9]) / (128*128);  avg_vector[24] = np.sum(fea_vector[24]) / (128*128)
    avg_vector[10] = np.sum(fea_vector[10]) / (128*128)
    avg_vector[11] = np.sum(fea_vector[11]) / (128*128)
    avg_vector[12] = np.sum(fea_vector[12]) / (128*128)
    avg_vector[13] = np.sum(fea_vector[13]) / (128*128)
    avg_vector[14] = np.sum(fea_vector[14]) / (128*128)

    return avg_vector


# feature normalization: (**) / max(**)
def energy_featuer_normalization(energy_feature):
    energy_feature = energy_feature / np.max(energy_feature)
    return energy_feature


# PCA dimension reduction; input 12x25 vector & return 12x3 vector
def PCA_reduction(feature_vector_bank):
    pca = PCA(n_components=3)
    data_PCA = pca.fit_transform(feature_vector_bank)
    return data_PCA


def feature_energy_computation(input_image, window_size):
    """
    N = window_size
    size = input_image.shape[0]
    output = np.zeros(shape=(size + N-1, size + N-1))
    n = (N-1)/2
    # copy
    for i in range(0, size):
        for j in range(0, size):
            output[i + n][j + n] = input_image[i][j]

    # row extension
    for i in range(0, n):
        for j in range(0, size):
            output[i + n - (2*i+1)][j] = input_image[i][j]
    for i in range(size-1-n, size-1):
        for j in range(0, size):
            output[size-1+n + 2*(size-i)-1][j] = input_image[i][j]

    # column extension
    for j in range(0, n):
        for i in range(0, size+N-1):
            output[i][j + n - (2*j+1)] = input_image[i][j]
    for j in range(size-1-n, size-1):
        for i in range(0, size+N-1):
            output[i][size-1+n + 2*(size-j)-1] = input_image[i][j]
    return output
    """
    # image boundary extension
    size = input_image.shape[0]
    N = window_size; n = (int)((N-1)/2)
    img_exd = np.zeros(shape=(size+N-1, size+N-1))
    img_exd = cv.copyMakeBorder(input_image, n, n, n, n, cv.BORDER_REFLECT)

    # energy feature computation
    sum = 0
    for i in range(n, size+n):
        for j in range(n, size+n):
            for k in range(i-n, i+n+1):
                for l in range(j-n, j+n+1):
                    sum += np.square(img_exd[k][l])
            input_image[i-n][j-n] = sum/(N*N)
            sum = 0
    # output result image
    return input_image


# reshape (25, 510, 510) vector to (510, 510, 25) vector
def vector_reshape(energy_feature_normalized):
    new_vector = np.zeros(shape=(510, 510, 25))
    for i in range(0, 510):
        for j in range(0, 510):
            a = energy_feature_normalized[:, i, j]
            new_vector[i][j] = a
    return new_vector


# kmean clustering; return clustering label
def Kmean_cluster(seg_vector_51051025):
    kmeans = KMeans(n_clusters=7)
    kmeans.fit_transform(seg_vector_51051025[:, :, :])
    print(kmeans.labels_)


# ==========================================================================
# read in image
# ==========================================================================
comb = np.fromfile('comb.raw', dtype='uint8')
# reshape image into np.array
comb = comb.reshape(510, 510)
# subtract image mean
comb = comb - comb.mean()
# do boundary extension
comb_etd = boundary_extension(comb)


# ==========================================================================
# 25-D feature filter
# ==========================================================================
# create empty 25-D feature vector
fea_vec = np.zeros(shape=(25, 510, 510))

# form 25-D feature vector
fea_vec = get_feature_matrix(comb_etd)


# ==========================================================================
# energy feature computation
# ==========================================================================
# window size = 9
energy_feature = np.zeros(shape=(25, 510, 510))

for i in range(0, 25):
    energy_feature[i] = feature_energy_computation(fea_vec[i], 9)


# ==========================================================================
# energy normalization
# ==========================================================================
energy_feature_normalized = np.zeros(shape=(25, 510, 510))
for i in range(0, 25):
    energy_feature_normalized[i] = energy_featuer_normalization(energy_feature[i])


# ==========================================================================
# reshape vector from (25, 510, 510) to (510, 510, 25)
# ==========================================================================
seg_vector_51051025 = vector_reshape(energy_feature_normalized)


# ==========================================================================
# k-mean classification
# ==========================================================================


print('finish')












