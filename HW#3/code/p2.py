# HW#3 Prob.1.(b)
# Submission Date: Mar.03
# Wenjun Li; 8372-7611-20

import numpy as np

hat = np.fromfile('hat.raw', dtype='uint8')
hat = hat.reshape(512, 512)
output = np.ones(shape=(512, 512))
output = output * 255

# index of original triangle and new triangle
x0_A = 0; x1_A = 128; x2_A = 256; x3_A = 128; x4_A = 0;   x5_A = 0;         x0_B = 0; x1_B = 256; x2_B = 512; x3_B = 384; x4_B = 256; x5_B = 128
y0_A = 0; y1_A = 128; y2_A = 256; y3_A = 384; y4_A = 512; y5_A = 256;       y0_B = 0; y1_B = 0;   y2_B = 0;   y3_B = 128; y4_B = 256; y5_B = 128
u0_A = 0; u1_A = 128; u2_A = 256; u3_A = 128; u4_A = 0;   u5_A = 128;       u0_B = 0; u1_B = 256; u2_B = 512; u3_B = 384; u4_B = 256; u5_B = 128
v0_A = 0; v1_A = 128; v2_A = 256; v3_A = 384; v4_A = 512; v5_A = 256;       v0_B = 0; v1_B = 128; v2_B = 0;   v3_B = 128; v4_B = 256; v5_B = 128

x0_C = 512; x1_C = 512; x2_C = 512; x3_C = 384; x4_C = 256; x5_C = 384;     x0_D = 512; x1_D = 256; x2_D = 0;   x3_D = 128; x4_D = 256; x5_D = 384
y0_C = 0;   y1_C = 256; y2_C = 512; y3_C = 384; y4_C = 256; y5_C = 128;     y0_D = 512; y1_D = 512; y2_D = 512; y3_D = 384; y4_D = 256; y5_D = 384
u0_C = 512; u1_C = 384; u2_C = 512; u3_C = 384; u4_C = 256; u5_C = 384;     u0_D = 512; u1_D = 256; u2_D = 0;   u3_D = 128; u4_D = 256; u5_D = 384
v0_C = 0;   v1_C = 256; v2_C = 512; v3_C = 384; v4_C = 256; v5_C = 128;     v0_D = 512; v1_D = 384; v2_D = 512; v3_D = 384; v4_D = 256; v5_D = 384

"""
# compute transform matrix ab_a
"""
uv_A = np.matrix([[u0_A, v0_A],
                      [u1_A, v1_A],
                      [u2_A, v2_A],
                      [u3_A, v3_A],
                      [u4_A, v4_A],
                      [u5_A, v5_A]]).T
xy_A = np.matrix([[1, 1, 1, 1, 1, 1],
                [x0_A, x1_A, x2_A, x3_A, x4_A, x5_A],
                [y0_A, y1_A, y2_A, y3_A, y4_A, y5_A],
                [x0_A*x0_A, x1_A*x1_A, x2_A*x2_A, x3_A*x3_A, x4_A*x4_A, x5_A*x5_A],
                [x0_A*y0_A, x1_A*y1_A, x2_A*y2_A, x3_A*y3_A, x4_A*y4_A, x5_A*y5_A],
                [y0_A*y0_A, y1_A*y1_A, y2_A*y2_A, y3_A*y3_A, y4_A*y4_A, y5_A*y5_A]])
transform_A = np.matmul(uv_A, np.linalg.pinv(xy_A))
"""
# compute transform matrix ab_b
"""
uv_B = np.matrix([[u0_B, v0_B],
                      [u1_B, v1_B],
                      [u2_B, v2_B],
                      [u3_B, v3_B],
                      [u4_B, v4_B],
                      [u5_B, v5_B]]).T
xy_B = np.matrix([[1, 1, 1, 1, 1, 1],
                [x0_B, x1_B, x2_B, x3_B, x4_B, x5_B],
                [y0_B, y1_B, y2_B, y3_B, y4_B, y5_B],
                [x0_B*x0_B, x1_B*x1_B, x2_B*x2_B, x3_B*x3_B, x4_B*x4_B, x5_B*x5_B],
                [x0_B*y0_B, x1_B*y1_B, x2_B*y2_B, x3_B*y3_B, x4_B*y4_B, x5_B*y5_B],
                [y0_B*y0_B, y1_B*y1_B, y2_B*y2_B, y3_B*y3_B, y4_B*y4_B, y5_B*y5_B]])
transform_B = np.matmul(uv_B, np.linalg.pinv(xy_B))
"""
# compute transform matrix ab_c
"""
# compute transform matrix ab_C
uv_C = np.matrix([[u0_C, v0_C],
                      [u1_C, v1_C],
                      [u2_C, v2_C],
                      [u3_C, v3_C],
                      [u4_C, v4_C],
                      [u5_C, v5_C]]).T
xy_C = np.matrix([[1, 1, 1, 1, 1, 1],
                [x0_C, x1_C, x2_C, x3_C, x4_C, x5_C],
                [y0_C, y1_C, y2_C, y3_C, y4_C, y5_C],
                [x0_C*x0_C, x1_C*x1_C, x2_C*x2_C, x3_C*x3_C, x4_C*x4_C, x5_C*x5_C],
                [x0_C*y0_C, x1_C*y1_C, x2_C*y2_C, x3_C*y3_C, x4_C*y4_C, x5_C*y5_C],
                [y0_C*y0_C, y1_C*y1_C, y2_C*y2_C, y3_C*y3_C, y4_C*y4_C, y5_C*y5_C]])
transform_C = np.matmul(uv_C, np.linalg.pinv(xy_C))
"""
# compute transform matrix ab_D
"""
uv_D = np.matrix([[u0_D, v0_D],
                      [u1_D, v1_D],
                      [u2_D, v2_D],
                      [u3_D, v3_D],
                      [u4_D, v4_D],
                      [u5_D, v5_D]]).T
xy_D = np.matrix([[1, 1, 1, 1, 1, 1],
                [x0_D, x1_D, x2_D, x3_D, x4_D, x5_D],
                [y0_D, y1_D, y2_D, y3_D, y4_D, y5_D],
                [x0_D*x0_D, x1_D*x1_D, x2_D*x2_D, x3_D*x3_D, x4_D*x4_D, x5_D*x5_D],
                [x0_D*y0_D, x1_D*y1_D, x2_D*y2_D, x3_D*y3_D, x4_D*y4_D, x5_D*y5_D],
                [y0_D*y0_D, y1_D*y1_D, y2_D*y2_D, y3_D*y3_D, y4_D*y4_D, y5_D*y5_D]])
transform_D = np.matmul(uv_D, np.linalg.pinv(xy_D))


# forward mapping
for i in range(0, 512):
    for j in range(0, 512):
        if i <= j and i <= (512-j):
            index_old = np.matrix([[1, i, j, i*i, i*j, j*j]]).T
            index_new = np.matmul(transform_A, index_old)
            index_r = int(index_new[0])
            index_c = int(index_new[1])
            output[index_r][index_c] = hat[i][j]
        if i >= j and i <= (512-j):
            index_old = np.matrix([[1, i, j, i*i, i*j, j*j]]).T
            index_new = np.matmul(transform_B, index_old)
            index_r = int(index_new[0])
            index_c = int(index_new[1])
            output[index_r][index_c] = hat[i][j]
        if i <= j and i >= (512-j):
            index_old = np.matrix([[1, i, j, i*i, i*j, j*j]]).T
            index_new = np.matmul(transform_D, index_old)
            index_r = int(index_new[0])
            index_c = int(index_new[1])
            output[index_r][index_c] = hat[i][j]
        if i >= j and i >= (512-j):
            index_old = np.matrix([[1, i, j, i*i, i*j, j*j]]).T
            index_new = np.matmul(transform_C, index_old)
            index_r = int(index_new[0])
            index_c = int(index_new[1])
            output[index_r][index_c] = hat[i][j]

output.astype('uint8').tofile('hat_warping.raw')
print('finish')



