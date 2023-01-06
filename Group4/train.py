
import tensorflow as tf
try:
    import tensorflow.python.keras as keras
except:
    import tensorflow.keras as keras
from tensorflow.python.keras import layers
from tensorflow import keras

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Flatten, Conv2D, MaxPool2D,BatchNormalization
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.callbacks import ReduceLROnPlateau
from sklearn.model_selection import train_test_split
from tensorflow.keras.preprocessing.image import ImageDataGenerator

mnist = keras.datasets.mnist
(x_train,y_train),(x_test,y_test) = mnist.load_data()
#x_train, x_test = x_train/255.0, x_test/255.0  # 除以 255 是为了归一化。

X_train4D = x_train.reshape(x_train.shape[0], 28, 28, 1).astype('float32')
X_test4D = x_test.reshape(x_test.shape[0], 28, 28, 1).astype('float32')

X_train4D_Normalize = X_train4D / 255 # 归一化
X_test4D_Normalize = X_test4D / 255

y_trainOnehot = to_categorical(y_train)
y_testOnehot = to_categorical(y_test)
X_train, X_val, Y_train, Y_val = train_test_split(X_train4D_Normalize, y_trainOnehot, test_size = 0.1, random_state=2)

model = Sequential()
# 构建网络


# 设置CNN模型
model = Sequential()

model.add(Conv2D(filters = 32, kernel_size = (3,3), padding = 'Same', activation = 'relu', input_shape = (28,28,1)))
model.add(BatchNormalization())

model.add(Conv2D(filters = 32, kernel_size = (3,3), padding = 'Same', activation = 'relu'))
model.add(BatchNormalization())

model.add(Conv2D(filters = 32, kernel_size = (5,5), padding = 'Same', activation = 'relu'))
model.add(BatchNormalization())

model.add(MaxPool2D(pool_size=(2,2)))
model.add(Dropout(0.4))

model.add(Conv2D(filters = 64, kernel_size = (3,3), padding = 'Same', activation = 'relu'))
model.add(BatchNormalization())

model.add(Conv2D(filters = 64, kernel_size = (3,3), padding = 'Same', activation = 'relu'))
model.add(BatchNormalization())

model.add(Conv2D(filters = 64, kernel_size = (5,5), padding = 'Same', activation = 'relu'))
model.add(BatchNormalization())

model.add(MaxPool2D(pool_size=(2,2), strides=(2,2)))
model.add(Dropout(0.4))

model.add(Conv2D(filters = 128, kernel_size = 4, activation = 'relu'))
model.add(BatchNormalization())

model.add(Flatten())
model.add(Dense(256, activation = "relu"))
model.add(Dropout(0.5))
model.add(Dense(10, activation = "softmax"))




model.compile(loss='categorical_crossentropy', optimizer="adam", metrics=['accuracy'])

# 设置一个学习率衰减
learning_rate_reduction = ReduceLROnPlateau(monitor='val_accuracy', 
                                            patience=3, 
                                            verbose=1, 
                                            factor=0.5, 
                                            min_lr=0.00001)
# 增加数据以防止过拟合
datagen = ImageDataGenerator(
        featurewise_center=False,  				# 在数据集上将输入平均值设置为0
        samplewise_center=False,  				# 将每个样本的平均值设置为0
        featurewise_std_normalization=False,    # 将输入除以数据集的std
        samplewise_std_normalization=False,  	# 将每个输入除以它的std
        zca_whitening=False,  					# 使用ZCA白化
        rotation_range=10,  					# 在范围内随机旋转图像（0到180度）
        zoom_range = 0.1, 						# 随机缩放图像
        width_shift_range=0.1,  				# 水平随机移动图像（总宽度的一部分）
        height_shift_range=0.1,  				# 垂直随机移动图像（总高度的一部分）
        horizontal_flip=False,  				# 随机翻转图像
        vertical_flip=False)  					# 随机翻转图像


datagen.fit(X_train)

# 拟合模型
history = model.fit(datagen.flow(X_train,Y_train, batch_size=128),
                              epochs = 30, validation_data = (X_val,Y_val),
                              verbose = 2, steps_per_epoch=X_train.shape[0] // 128,
                              callbacks=[learning_rate_reduction])

model.save('my_model.h5')
model.evaluate(X_test4D_Normalize,y_testOnehot)
