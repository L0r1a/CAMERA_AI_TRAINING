from __future__ import division, print_function, unicode_literals
import pandas as pd
import numpy as np 
import math
from scipy import sparse 
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt

# Nhập dữ liệu Điểm số học sinh
file_name = 'Student_Marks.csv'
dataFrame = pd.read_csv(file_name)

# Phân loại giữa biến Label và biến Feature
Label=['Marks']
Feature=['number_courses','time_study']
y = dataFrame[Label].values    
X = dataFrame[Feature].values

# Chia tập dữ liệu thành 2 tập : Tập huấn luyện và tập thử nghiệm
one = np.ones((X.shape[0],1))
X = np.concatenate((one, X), axis = 1)

X_train,X_test,y_train,y_test = train_test_split(X,y,test_size=0.2,random_state=42)

N1 = X_train.shape[0]
N2 = X_test.shape[0]

# Hàm mất mát  
def cost(W1,N,Y,X):    
    return .5/N*(np.linalg.norm(Y - np.dot(X,W1), 2))**2;

# Gradient 
def gradient(W1,N):
    return 1/N*(X_train.T).dot(X_train.dot(W1) - y_train)  

d0 = X.shape[1]
d1 = C = 1
# Khởi tạo giá trị random cho các trọng số
W1 = np.random.randn(d0, d1)

eta = 0.01# Tốc độ học
for i in range(100):                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
    ## tính MSE
    loss = cost(W1,N1,y_train,X_train)
    # In ra Mean Square Error mỗi 1000 vòng lặp
    if i %10 == 0: 
        print("iter %d, loss: %f" %(i, loss))
    
    # backpropagation
    dW1 = gradient(W1,N1)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
    
    # Cặp nhật trọng số 
    W1 = W1 -eta*dW1

loss = cost(W1,N2,y_test,X_test)
print("Testing:  MSE: %f" %( loss))

x1 = x2 = np.linspace(0, 10, 2, endpoint=True)

line = W1[0] + W1[1]*x1 + W1[2]*x2;


# Hiển thị kết quả dưới dạng biểu đồ 
plt.plot(X[:,2],y, 'r.', markersize = 7);
plt.plot(x2,line);
plt.xlabel("Time Study (hours)")
plt.ylabel("Marks")
plt.show()


