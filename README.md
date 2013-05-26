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
Tests run on set of 80 images on 2010 11" MacBook Air

1.4 GHz Intel Core 2 Duo

2 GB 1067 MHz DDR3

NVIDIA GeForce 320M 256 MB

OS X 10.8.3

      +=====================================================+
      | CPU                                                 |
      +=====================================================+
      | Total Feature Detection Time:              18.95 s  |
      | Average Feature Detection Time:           236.92 ms |
      | Total Descriptor Extraction Time:          30.41 s  |
      | Average Descriptor Extraction Time:       380.18 ms |
      +=====================================================+
      | GPU                                                 |
      +=====================================================+
      | Total Feature Detection Time:               2.78 s  |
      | Average Feature Detection Time:            34.70 ms |
      | Total Descriptor Extraction Time:           1.34 s  |
      | Average Descriptor Extraction Time:        16.74 ms |
      +=====================================================+
      | Comparison                                          |
      +=====================================================+
      | GPU Feature Detection Speedup:              6.83 x  |
      | GPU Descriptor Extraction Speedup:         22.71 x  |
      | GPU Combined Speedup:                      12.00 x  |
      +=====================================================+
