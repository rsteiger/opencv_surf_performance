SURF CPU/GPU Performance Comparison
-----------------------------------

I evaluate the performance of OpenCV computation of SURF features and descriptors
for a set of images on both the CPU and the GPU. 

### Building

This project has been tested with OpenCV 2.4.3 and Boost 1.51.0

OpenCV must be built with CUDA support

    $> cmake && make

### Running

The program is called `surftest` and takes a single argument which is a path to
a directory with jpg images. `surftest` searches recursively and finds features
in each of the image using CPU and GPU implementations of SURF.

    $> surftest path/to/images

### Results
Tests run on 2010 11" MacBook Air

1.4 GHz Intel Core 2 Duo
2 GB 1067 MHz DDR3
NVIDIA GeForce 320M 256 MB
OS X 10.8.3

    +=====================================================+
    | CPU                                                 |
    +=====================================================+
    | Total Feature Detection Time:                0.0 s  |
    | Average Feature Detection Time:              0.0 ms |
    | Total Descriptor Extraction Time:            0.0 s  |
    | Average Descriptor Extraction Time:          0.0 ms |
    | Total Run Time:                              0.0 s  |
    +=====================================================+
    | GPU                                                 |
    +=====================================================+
    | Total Feature Detection Time:                0.0 s  |
    | Average Feature Detection Time:              0.0 ms |
    | Total Descriptor Extraction Time:            0.0 s  |
    | Average Descriptor Extraction Time:          0.0 ms |
    | Total Bus Time:                              0.0 s  |
    | Average Bus Time:                            0.0 ms |
    | Total Run Time:                              0.0 s  |
    +=====================================================+
    | Comparison                                          |
    +=====================================================+
    | GPU Feature Detection Speedup:               0.0 x  |
    | GPU Descriptor Extraction Speedup:           0.0 x  |
    | GPU Combined Speedup:                        0.0 x  |
    | GPU Overall Speedup:                         0.0 x  |
    +=====================================================+
