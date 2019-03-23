from sklearn.cluster import KMeans
import numpy as np
import pandas as pd
import pickle
kmeans = KMeans(n_clusters=100,verbose=1)
dF=pd.read_csv('../files/PotentiometerSNo870.csv').dropna(axis=1)
x = dF.to_numpy(dtype=float)[:,1:]
print(x[:,1:])
kmeans.fit(x)

pickle.dump(kmeans,open('./cluster.sav','wb'))

