# Real time Rendering

## 3d Rendering Algorithm

Algorithms implemented:

* z-index
* shadow map
* `.obj` parser

## Image

`SDL` image buffer is replaced by ours so we can display the scene in real time.

## Usage

To build the program:

```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

How to run the program after build

```bash
$ ./rtr path_to_scene.obj
```

## Authors

* victor.guichard
* guillaume.valente
