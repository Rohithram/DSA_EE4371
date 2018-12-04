import numpy as np

inputs = np.loadtxt('input_1000.in').astype(int)

outputs = np.loadtxt('output_1000.out').astype(int)

_, i = np.unique(outputs[::-1, 2], return_index=True)
outputs = outputs[::-1, :][i]

inputs = inputs[outputs[:, 2]-1, :]

violations = outputs[:, 1] > inputs[:, 2]

print(np.count_nonzero(violations))
