# this file produces two matrices, writes them to disk in matrix1.txt and matrix2.txt,
# then it computes their product, and writes the result to product.txt.
import os
import numpy as np
from scipy.sparse import rand
from sys import argv

if len(argv) <= 1:
  print('Not enough arguments, quitting')
  quit()

np.set_printoptions(threshold=np.nan)

n = int(argv[1])

matrix1 = rand(n, n, density=0.05, format='csr')
matrix2 = rand(n, n, density=0.05, format='csr')
matrix1.data[:] = 1
matrix2.data[:] = 1

# write these matrices out
file_mat1 = open("../Matrix/matrix1.txt", "w")
file_mat1.write(str(matrix1.todense()))  
file_mat2 = open("../Matrix/matrix2.txt", "w")
file_mat2.write(str(matrix2.todense()))  

# calculate the product
A = matrix1.toarray()
B = matrix2.toarray()
R = np.matmul(A, B)
# modify so it's still a boolean matrix
R[R > 1] = 1


# write what the matrix product should be
f = open(os.path.expanduser("~/Utils/product.txt"), "w")
f.write(np.array_str(R))

