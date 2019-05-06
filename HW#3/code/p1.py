# HW#3 Prob.1.(a)
# Submission Date: Mar.03
# Wenjun Li; 8372-7611-20

import numpy as np

lighthouse = np.fromfile('lighthouse.raw', dtype='uint8')
lighthouse1 = np.fromfile('lighthouse1.raw', dtype='uint8')
lighthouse2 = np.fromfile('lighthouse2.raw', dtype='uint8')
lighthouse3 = np.fromfile('lighthouse3.raw', dtype='uint8')
lighthouse = lighthouse.reshape(512, 512)
lighthouse1 = lighthouse1.reshape(256, 256)
lighthouse2 = lighthouse2.reshape(256, 256)
lighthouse3 = lighthouse3.reshape(256, 256)

lighthouse_all = np.zeros(shape=(512, 1024))
for i in range(0, 512):
    for j in range(0, 512):
        lighthouse_all[i][j] = lighthouse[i][j]
for i in range(0, 256):
    for j in range(0, 256):
        lighthouse_all[i][j+512] = lighthouse1[i][j]
for i in range(0, 256):
    for j in range(0, 256):
        lighthouse_all[i][j+768] = lighthouse2[i][j]
for i in range(0, 256):
    for j in range(0, 256):
        lighthouse_all[i+256][j+512] = lighthouse3[i][j]

# i is row, j is colomn
"""
cornel pixels: 
lighthouse1: tr = (80, 115), tl = (182, 78), bl = (223, 182), br = (118, 221)
lighthouse2: tr = (100, 15), tl = (212, 108),bl = (116, 218), br = (8, 125)
lighthouse3: tr = (80, 115), tl = (182, 78), bl = (223, 182), br = (118, 221)
lighthouse:  tr = (, ), tl = (, ), bl = (, ), br = (, )

flag = True
for i in range(0, 180):
    for j in range(0, 180):
        if flag and lighthouse[i][j] == 255:
            print("Hole A is @ (%d, %d)" % (i, j))
            flag = False
flag = True
for i in range(0, 50):
    for j in range(0, 420):
        if flag and lighthouse[i][j] == 255:
            print("Hole B is @ (%d, %d)" % (i, j))
            flag = False
flag = True
for i in range(323, 350):
    for j in range(0, 350):
        if flag and lighthouse[i][j] == 255:
            print("Hole C is @ (%d, %d)" % (i, j))
            flag = False
"""

# rotation angle theta
theta_A = -np.arctan((223-118+1.5)/(221-182))
theta_B = -np.arctan((986-876-30)/(212-116)) - np.arctan(10000000/1)    # arctan(infinity) is 90°
theta_C = np.arctan((251-209-6.5)/(210-3))

# rotation matrix
rotation_A = np.matrix([[np.cos(theta_A), -np.sin(theta_A), 0], [np.sin(theta_A), np.cos(theta_A), 0], [0, 0, 1]])
rotation_B = np.matrix([[np.cos(theta_B), -np.sin(theta_B), 0], [np.sin(theta_B), np.cos(theta_B), 0], [0, 0, 1]])
rotation_C = np.matrix([[np.cos(theta_C), -np.sin(theta_C), 0], [np.sin(theta_C), np.cos(theta_C), 0], [0, 0, 1]])

# scaling matrix
scaling_A = np.matrix([[160/111, 0, 0], [0, 160/111, 0], [0, 0, 1]])
scaling_B = np.matrix([[160/139, 0, 0], [0, 160/144, 0], [0, 0, 1]])
scaling_C = np.matrix([[160/213, 0, 0], [0, 160/213, 0], [0, 0, 1]])

# translation matrix
translation_A = np.matrix([[1, 0, 86],          # ty changes vertical location
                         [0, 1, -75.5],          # ty changes horizontal location
                         [0, 0, 1]])
translation_B = np.matrix([[1, 0, 94],          # ty changes vertical location; -: move up; +: move down
                         [0, 1, -144],          # ty changes horizontal location: -: move left; +: move right
                         [0, 0, 1]])
translation_C = np.matrix([[1, 0, 2],          # ty changes vertical location; -: move up; +: move down
                         [0, 1, -31],          # ty changes horizontal location: -: move left; +: move right
                         [0, 0, 1]])

# transform matrix
transform_A = np.matmul(np.matmul(translation_A, scaling_A), rotation_A)
transform_A_inverse = np.linalg.pinv(transform_A)
transform_B = np.matmul(np.matmul(translation_B, scaling_B), rotation_B)
transform_B_inverse = np.linalg.pinv(transform_B)
transform_C = np.matmul(np.matmul(translation_C, scaling_C), rotation_C)
transform_C_inverse = np.linalg.pinv(transform_C)

# output array initialization
lh_trans = np.ones(shape=(512, 512))
lh_trans = lh_trans * 255


"""reverse mapping test for A"""
for i in range(157, 317):
    for j in range(62, 222):
        ori_index = np.matrix([i, j, 1])
        a = np.matmul(transform_A_inverse, ori_index.T)
        index_r = int(a[0])
        index_c = int(a[1])
        lighthouse[i][j] = lighthouse1[index_r][index_c]


"""reverse mapping test for B"""
for i in range(0, 162):
    for j in range(0, 162):
        ori_index = np.matrix([i, j, 1])
        a = np.matmul(transform_B_inverse, ori_index.T)
        index_r = int(a[0])
        index_c = int(a[1])
        lh_trans[i][j] = lighthouse2[index_r][index_c]
# move transformed part B to right location
translation_B_final = np.matrix([[1, 0, 31],          # ty changes vertical location
                                [0, 1, 276],          # ty changes horizontal location
                                [0, 0, 1]])
for i in range(0, 162):
    for j in range(0, 162):
        ori_index = np.matrix([i, j, 1])
        a = np.matmul(translation_B_final, ori_index.T)
        index_r = int(a[0])
        index_c = int(a[1])
        lighthouse[index_r][index_c] = lh_trans[i][j]
# lh_trans.astype('uint8').tofile('part_B_test.raw')


"""reverse mapping test for C"""
for i in range(0, 160):
    for j in range(0, 160):
        ori_index = np.matrix([i, j, 1])
        a = np.matmul(transform_C_inverse, ori_index.T)
        index_r = int(a[0])
        index_c = int(a[1])
        lh_trans[i][j] = lighthouse3[index_r][index_c]
# move transformed part C to right location
translation_C_final = np.matrix([[1, 0, 328],          # ty changes vertical location
                                [0, 1, 326],          # ty changes horizontal location
                                [0, 0, 1]])
for i in range(0, 160):
    for j in range(0, 160):
        ori_index = np.matrix([i, j, 1])
        a = np.matmul(translation_C_final, ori_index.T)
        index_r = int(a[0])
        index_c = int(a[1])
        lighthouse[index_r][index_c] = lh_trans[i][j]

lighthouse.astype('uint8').tofile('lighthouse_transformed.raw')












