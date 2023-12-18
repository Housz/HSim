## Coordinate System:

```
Right-handed coordinate system:

    y (j)
    |
    |   
   0|_ _ _ _ x (i)
   /
  /
 /
z (k)
```

## Grids

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

