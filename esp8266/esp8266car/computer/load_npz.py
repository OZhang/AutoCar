import numpy as np

path = 'training_data/1546786435.npz' 
f = np.load(path)
x_train, y_train = f['train'], f['train_labels']
print(x_train.shape)
print(y_train.shape)
print(x_train)
print(y_train)
    #x_test, y_test = f['x_test'], f['y_test']
f.close()
