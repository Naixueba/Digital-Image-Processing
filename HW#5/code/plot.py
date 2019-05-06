# HW#5 Prob.(b) & (c)
# Submission Date: Mar.29
# Name: Wenjun Li
# USC ID: 8372-7611-20

"""
this code plot all the figures in HW#5
"""

import numpy as np
import matplotlib.pyplot as plt

set_1_train = np.array([77.07, 94.26, 96.36, 97.08, 97.55, 97.89, 98.06, 98.20, 98.37, 98.42])
set_1_test = np.array([92.66, 95.95, 97.14, 97.80, 97.76, 98.24, 98.30, 98.30, 98.46, 98.38])

set_2_train = np.array([83.76, 96.13, 97.20, 97.83, 98.15, 98.41, 98.63, 98.71, 98.88, 98.95,
                        99.04, 99.13, 99.26, 99.28, 99.35, 99.41, 99.40, 99.44, 99.50, 99.51])
set_2_test = np.array([95.62, 97.32, 97.93, 98.23, 98.18, 98.65, 98.23, 98.81, 98.69, 98.83,
                       99.03, 99.06, 98.87, 99.14, 98.79, 98.89, 99.01, 99.06, 99.18, 99.11])

set_3_train = np.array([83.58, 94.95, 96.77, 97.38, 97.95, 98.19, 98.46, 98.58, 98.75, 98.77,
                        98.95, 99.06, 99.20, 99.20, 99.30, 99.19, 99.33, 99.52, 99.48, 99.47])
set_3_test = np.array([93.36, 96.61, 97.49, 97.91, 98.39, 98.42, 98.50, 98.71, 98.56, 98.83,
                       98.85, 98.79, 98.90, 98.90, 98.97, 98.87, 98.91, 98.95, 98.79, 98.82])

set_4_train = np.array([89.38, 98.06, 98.59, 98.83, 98.82, 99.08, 99.19, 99.33, 99.33, 99.44,
                        99.41, 99.41, 99.50, 99.57, 99.61, 99.42, 99.44, 99.55, 99.47, 99.62])
set_4_test = np.array([97.90, 98.23, 97.95, 98.67, 98.84, 98.67, 98.74, 98.96, 98.63, 98.54,
                       98.62, 98.92, 98.97, 99.11, 98.63, 98.68, 98.66, 98.66, 98.76, 98.89])

set_5_train = np.array([15.41, 26.96, 34.94, 47.90, 57.44, 62.93, 68.12, 72.43, 76.01, 78.94,
                        80.63, 81.94, 83.09, 83.92, 84.77, 85.54, 86.20, 86.80, 87.36, 87.86])
set_5_test = np.array([22.98, 31.86, 41.13, 55.89, 61.83, 66.84, 71.92, 76.12, 79.60, 81.47,
                       83.01, 83.85, 84.91, 85.60, 86.40, 87.04, 87.74, 88.24, 88.73, 89.07])

set_best_train = np.array([86.20, 96.69, 97.73, 98.22, 98.46, 98.66, 98.78, 98.90, 99.00, 99.13,
                           99.30, 99.34, 99.39, 99.47, 99.44, 99.48, 99.51, 99.48, 99.53, 99.68])
set_best_test = np.array([96.10, 97.76, 98.10, 98.53, 98.60, 98.53, 98.62, 98.73, 98.79, 99.05,
                          98.91, 98.90, 99.11, 98.88, 98.91, 98.94, 98.87, 98.62, 99.11, 99.21])

# plot for prob.(c).(2)
set_new_nega_train = np.array([89.06, 97.19, 97.88, 98.25, 98.53, 98.71, 98.85, 98.97, 99.06, 99.09,
                               99.21, 99.23, 99.33, 99.35, 99.46, 99.51, 99.51, 99.53, 99.60, 99.64])
set_new_nega_test = np.array([96.26, 97.60, 97.99, 97.90, 98.24, 98.40, 98.48, 98.75, 98.50, 98.64,
                              98.75, 98.82, 98.63, 98.73, 98.73, 98.74, 98.88, 98.67, 98.81, 98.91])

set_new_posi_train = np.array([88.48, 97.07, 97.82, 98.23, 98.47, 98.67, 98.84, 98.98, 99.09, 99.14,
                                99.26, 99.28, 99.33, 99.47, 99.44, 99.53, 99.55, 99.56, 99.65, 99.66])
set_new_posi_test = np.array([96.72, 97.98, 98.15, 98.42, 98.32, 98.58, 98.44, 98.84, 99.02, 98.77,
                               98.97, 98.67, 98.92, 98.93, 98.79, 99.02, 98.89, 98.96, 98.95, 98.90])

# plot for prob.(b).(1) -- 5 para settings
plt.plot(set_1_train, label='train acc'); plt.plot(set_1_test, label='test acc'); plt.legend(); plt.title('batch_size=1024, optimizer=adam, epochs=10'); plt.xticks(range(1, 11)); plt.show()
plt.plot(set_2_train, label='train acc'); plt.plot(set_2_test, label='test acc'); plt.legend(); plt.title('batch_size=512, optimizer=adam, epochs=20'); plt.xticks(range(1, 21)); plt.show()
plt.plot(set_3_train, label='train acc'); plt.plot(set_3_test, label='test acc'); plt.legend(); plt.title('batch_size=512, optimizer=adam, learning_rate=0.001, epochs=20'); plt.xticks(range(1, 21)); plt.show()
plt.plot(set_4_train, label='train acc'); plt.plot(set_4_test, label='test acc'); plt.legend(); plt.title('batch_size=512, optimizer=adam, learning_rate=0.01, epochs=20'); plt.xticks(range(1, 21)); plt.show()
plt.plot(set_5_train, label='train acc'); plt.plot(set_5_test, label='test acc'); plt.legend(); plt.title('batch_size=512, optimizer=adam, learning_rate=0.0001, epochs=20'); plt.xticks(range(1, 21)); plt.show()

# plot for prob.(b).(2) -- best performance
plt.plot(set_best_train, label='train acc'); plt.plot(set_best_test, label='test acc'); plt.legend(); plt.title('batch_size=256, optimizer=adam, learning_rate=0.01, epochs=20'); plt.xticks(range(1, 21)); plt.show()

# plot for prob.(c).(2)
plt.plot(set_new_nega_train, label='train acc'); plt.plot(set_new_nega_test, label='test acc'); plt.legend(); plt.title('trained on combined dataset and test on negative dataset'); plt.xticks(range(1, 21)); plt.show()
plt.plot(set_new_posi_train, label='train acc'); plt.plot(set_new_posi_test, label='test acc'); plt.legend(); plt.title('trained on combined dataset and test on positive dataset'); plt.xticks(range(1, 21)); plt.show()


training = np.array([98.42, 99.51, 99.47, 99.62, 87.86])
testing = np.array([98.38, 99.11, 98.82, 98.89, 89.07])
print(np.mean(training)); print(np.var(training))
print(np.mean(testing)); print(np.var(testing))



