import numpy as np
from Teracada import TeracadaArray as ta
from Teracada import LinearRegression as TeraLReg
from sklearn.linear_model import LinearRegression as SklearnLReg


npaX = np.array([5, 15, 25, 35, 45, 55]).reshape((-1, 1))
npaY = np.array([5, 20, 14, 32, 22, 38])

modelSKlearnLReg = SklearnLReg()
modelSKlearnLReg.fit(npaX, npaY)

print("Intercept: ", modelSKlearnLReg.intercept_)
print("Slope: ", modelSKlearnLReg.coef_[0])


tcaX = ta.array([5, 15, 25, 35, 45, 55])
tcaY = ta.array([5, 20, 14, 32, 22, 38])

print(tcaX)
print(tcaY)

modelTeraLReg = TeraLReg()
modelTeraLReg.fit()

