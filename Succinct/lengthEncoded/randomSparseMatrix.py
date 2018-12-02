import numpy as np
from scipy.sparse import rand
from sys import argv

if len(argv) <= 1:
  print('Not enough arguments, quitting')
  quit()

np.set_printoptions(threshold=np.nan)

n = int(argv[1])

matrix = rand(n, n, density=0.1, format='csr')
matrix.data[:] = 1

print(matrix.todense())
