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
Tests run on Kodak Lossless True Color Image Suite http://r0k.us/graphics/kodak/

2010 11" MacBook Air: 1.4 GHz Intel Core 2 Duo, 2 GB 1067 MHz DDR3, NVIDIA
GeForce 320M 256 MB, OS X 10.8.3

      +=====================================================+
      | CPU                                                 |
      +=====================================================+
      | Total Feature Detection Time:              13.81 s  |
      | Average Feature Detection Time:           575.26 ms |
      | Total Descriptor Extraction Time:          23.05 s  |
      | Average Descriptor Extraction Time:       960.42 ms |
      | Total Calculation Time:                    36.86 s  |
      | Average Calculation Time:                1535.69 ms |
      +=====================================================+
      | GPU                                                 |
      +=====================================================+
      | Total Feature Detection Time:               2.12 s  |
      | Average Feature Detection Time:            88.38 ms |
      | Total Descriptor Extraction Time:           1.09 s  |
      | Average Descriptor Extraction Time:        45.23 ms |
      | Total Calculation Time (including bus):    19.03 s  |
      | Average Calculation Time:                 792.97 ms |
      +=====================================================+
      | Comparison                                          |
      +=====================================================+
      | GPU Feature Detection Speedup:              6.51 x  |
      | GPU Descriptor Extraction Speedup:         21.24 x  |
      | GPU Combined Speedup:                      11.49 x  |
      | GPU Overall Speedup:                        1.94 x  |
      +=====================================================+

