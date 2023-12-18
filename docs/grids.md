# Grids

## class design

```
Grid3
	├─ ScalarGrid3
	│			 ├─ CellCenterScalarGrid3
	│			 └─ VertexCenterScalarGrid3
	│
	└─ VectorGrid3
				 ├─ FaceCenterVectorGrid3 (MAC/Staggered Grid)
				 └─ CollocatedVectorGrid3
									   ├─ CellCenterVectorGrid3
									   └─ VertexCenterVectorGrid3


```


## spacial data structures

```
Grid
grid origin: x
+---+---+---+
|   |   |   |
+---+---+---+
|   |   |   |
+---+---+---+
|   |   |   |
x---+---+---+
```
### CellCenterScalarGrid
```
CellCenterScalarGrid

dataOrigin: x = orgin + 0.5 * gridSpacing
+---+---+---+
|   |   |   |
+---+---+---+
|   |   |   |
+---+---+---+
| x |   |   |
+---+---+---+

data : x
dataSize: = resolution
+---+---+---+
| x | x | x |
+---+---+---+
| x | x | x |
+---+---+---+
| x | x | x |
+---+---+---+
```

### VertexCenterScalarGrid

```
VertexCenterScalarGrid

dataOrigin: x = orgin
+---+---+---+
|   |   |   |
+---+---+---+
|   |   |   |
+---+---+---+
|   |   |   |
x---+---+---+

data: x
dataSize: x = resolution + {1, 1, 1}
x---x---x---x
|   |   |   |
x---x---x---x
|   |   |   |
x---x---x---x
|   |   |   |
x---x---x---x

```

