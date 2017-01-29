#!/usr/bin/python
import matplotlib.pyplot as plt
#
#	CODE TO PLOT MANDLEBROT SET
#
N=100
max_iter=1000
x_max=2.0
x_min=-2.0
y_max=2.0
y_min=-2.0
area=0
c_real=[]
c_img=[]
plot=[]
for i in xrange(0,N):
	plot.append([0]*N)
	c_real.append([0]*N)
	c_img.append([0]*N)
for i in range(0,N):
	for j in range(0,N):
		iter=0
		x=y=0
		c_real[i][j]=(x_max*j+x_min*(N-j))/N
		c_img[i][j]=(y_max*i+y_min*(N-i))/N

		while (x*x +y*y < 4 and iter < max_iter):
			tmp= x*x-y*y + c_real[i][j]
			y=2*x*y + c_img[i][j]
			x=tmp
			iter+=1
		plot[i][j]=iter
		
plt.contourf(plot)
plt.show()