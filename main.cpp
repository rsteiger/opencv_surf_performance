/**
 * OpenCV SURF CPU/GPU Performance Test
 *
 * Richie Steigerwald
 *
 * Copyright 2013 Richie Steigerwald <richie.steigerwald@gmail.com>
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#include <iostream>

#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // imread
#include <opencv2/nonfree/features2d.hpp> // SURF
#include <opencv2/gpu/gpu.hpp> // GpuMat

#include "timer.hpp"
#include "files.hpp"

// don't use cv namespace
// cv and std namespaces overlap (string, find, etc.)
using namespace std;


/**
 * This struct initializes the OpenCV detectors and extractors and keeps track
 * of timing info.
 */
struct feature_calculator {
   // timing information
   double cpu_feat = 0, cpu_desc = 0, cpu_total = 0;
   double gpu_feat = 0, gpu_desc = 0, gpu_total = 0;

   // cpu detector and extractor
   cv::SurfFeatureDetector cpu_detector;
   cv::SurfDescriptorExtractor cpu_extractor;

   // gpu detector and extractor
   cv::gpu::SURF_GPU gpu_detector_extractor;

   // prototypes for feature calculation functions
   void cpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);
   void gpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);
};

// print usage information
void usage(const string &program);

/**
 * Gets all images in a directory. Computes all of the features and the
 * descriptors using the CPU, then computes all of the features and descriptors
 * using the GPU
 */
int main(int argc, const char *argv[]) {
   if (argc != 2) { usage(argv[0]); return 0; }

   list<string> images = get_files_recursive(argv[1], ".png");
   
   feature_calculator fc;
   vector<cv::KeyPoint> keypoints;
   cv::Mat descriptors;


   // Compute features for each image on the CPU
   for (list<string>::iterator image = images.begin(); image != images.end(); image++) {
      cv::Mat grayscale_image = cv::imread( *image, CV_LOAD_IMAGE_GRAYSCALE );
      time(&fc, fc.cpu_total)->cpu(grayscale_image, keypoints, descriptors);
   }

   // Output the results
   printf("+=====================================================+\n");
   printf("| CPU                                                 |\n");
   printf("+=====================================================+\n");
   printf("| Total Feature Detection Time:         %10.2f s  |\n", fc.cpu_feat);
   printf("| Average Feature Detection Time:       %10.2f ms |\n", 1000 * fc.cpu_feat / images.size());
   printf("| Total Descriptor Extraction Time:     %10.2f s  |\n", fc.cpu_desc);
   printf("| Average Descriptor Extraction Time:   %10.2f ms |\n", 1000 * fc.cpu_desc / images.size());
   printf("| Total Calculation Time:               %10.2f s  |\n", fc.cpu_total);
   printf("| Average Calculation Time:             %10.2f ms |\n", 1000 * fc.cpu_total / images.size());
   printf("+=====================================================+\n");

   // Compute features for each image on the GPU
   for (list<string>::iterator image = images.begin(); image != images.end(); image++) {
      cv::Mat grayscale_image = cv::imread( *image, CV_LOAD_IMAGE_GRAYSCALE );
      time(&fc, fc.gpu_total)->gpu(grayscale_image, keypoints, descriptors);
   }

   // Output the results
   printf("| GPU                                                 |\n");
   printf("+=====================================================+\n");
   printf("| Total Feature Detection Time:         %10.2f s  |\n", fc.gpu_feat);
   printf("| Average Feature Detection Time:       %10.2f ms |\n", 1000 * fc.gpu_feat / images.size());
   printf("| Total Descriptor Extraction Time:     %10.2f s  |\n", fc.gpu_desc);
   printf("| Average Descriptor Extraction Time:   %10.2f ms |\n", 1000 * fc.gpu_desc / images.size());
   printf("| Total Calculation Time (including bus): %8.2f s  |\n", fc.gpu_total);
   printf("| Average Calculation Time:             %10.2f ms |\n", 1000 * fc.gpu_total / images.size());
   printf("+=====================================================+\n");
   printf("| Comparison                                          |\n");
   printf("+=====================================================+\n");
   printf("| GPU Feature Detection Speedup:        %10.2f x  |\n", fc.cpu_feat / fc.gpu_feat);
   printf("| GPU Descriptor Extraction Speedup:    %10.2f x  |\n", fc.cpu_desc / fc.gpu_desc);
   printf("| GPU Combined Speedup:                 %10.2f x  |\n", (fc.cpu_feat + fc.cpu_desc) / 
                                                                   (fc.gpu_feat + fc.gpu_desc));
   printf("| GPU Overall Speedup (including bus):  %10.2f x  |\n", fc.cpu_total / fc.gpu_total);
   printf("+=====================================================+\n");
}

// Detect features on the CPU
void feature_calculator::cpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) {
   // Detect features
   time(&cpu_detector, cpu_feat)->detect(src_img, keypoints);
   // Extract descriptors
   time(&cpu_extractor, cpu_desc)->compute(src_img, keypoints, descriptors);
} 

// detect features on the GPU
void feature_calculator::gpu(cv::Mat &img, vector<cv::KeyPoint> &keypoints,
 cv::Mat &descriptors) { 
   cv::gpu::GpuMat img_gpu, keypoints_gpu, descriptors_gpu;
   
   // Send image to GPU
   img_gpu.upload(img);
 
   // Detect features
   time(&gpu_detector_extractor,gpu_feat)->operator()(img_gpu, 
    cv::gpu::GpuMat(), keypoints_gpu);

   // Compute descriptors
   time(&gpu_detector_extractor,gpu_desc)->operator()(img_gpu,
    cv::gpu::GpuMat(), keypoints_gpu, descriptors_gpu, true);
 
   // Donload features and keypoints from GPU
   gpu_detector_extractor.downloadKeypoints(keypoints_gpu, keypoints);
   vector<float> descriptors_vector;
   gpu_detector_extractor.downloadDescriptors(descriptors_gpu, descriptors_vector);
   descriptors = cv::Mat(descriptors_vector);
}


// Display usage information
void usage(const string &program) {
   cout << "Usage: " << program << " path/to/images" << std::endl;
}

