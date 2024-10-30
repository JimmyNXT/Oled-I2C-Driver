from PIL import Image
import numpy as np

path = './fleischer_420.jpg'
image = Image.open(path)
image = np.asarray(image)
(y,x,z) = image.shape

for i in range(y):
    for j in range(x):
        avg = 0
        for k in range(z):
            avg = avg + image[i, j, k]
        avg = avg / z

        if avg < (256/3)*2:
            print(1)
        else:
            print(0)

