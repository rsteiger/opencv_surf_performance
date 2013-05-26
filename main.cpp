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
#include <list>

#include <opencv2/core/core.hpp> // Mat
#include <opencv2/highgui/highgui.hpp> // imread
#include <opencv2/nonfree/features2d.hpp> // SURF
#include <opencv2/gpu/gpu.hpp> // GpuMat

#include <boost/filesystem.hpp>

#include "timer.h"

// don't use cv namespace
// cv and std namespaces overlap (string, find, etc.)
using namespace std;

void readme(const string &program);
list<string> get_files_recursive(const string &dir, const string &extension);

void detect_features_cpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);
void detect_features_gpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

// aspect variables get globals for this toy
double cpu_feat = 0, cpu_desc = 0, gpu_feat = 0, gpu_desc = 0;

int main(int argc, const char *argv[]) {
   if (argc != 2) { readme(argv[0]); return 0; }

   list<string> images = get_files_recursive(argv[1], ".jpg");
   
   vector<cv::KeyPoint> keypoints;
   cv::Mat descriptors;

   // Detect features for each image on the CPU
   for (list<string>::iterator image = images.begin(); image != images.end(); image++) {
      cv::Mat grayscale_image = cv::imread( *image, CV_LOAD_IMAGE_GRAYSCALE );
      detect_features_cpu(grayscale_image, keypoints, descriptors);
   }

   // Output the results
   printf("+=====================================================+\n");
   printf("| CPU                                                 |\n");
   printf("+=====================================================+\n");
   printf("| Total Feature Detection Time:         %10.2f s  |\n", cpu_feat);
   printf("| Average Feature Detection Time:       %10.2f ms |\n", 1000 * cpu_feat / images.size());
   printf("| Total Descriptor Extraction Time:     %10.2f s  |\n", cpu_desc);
   printf("| Average Descriptor Extraction Time:   %10.2f ms |\n", 1000 * cpu_desc / images.size());
   printf("+=====================================================+\n");

   // Detect features for each image on the GPU
   for (list<string>::iterator image = images.begin(); image != images.end(); image++) {
      cv::Mat grayscale_image = cv::imread( *image, CV_LOAD_IMAGE_GRAYSCALE );
      detect_features_gpu(grayscale_image, keypoints, descriptors);
   }

   // Output the results
   printf("| GPU                                                 |\n");
   printf("+=====================================================+\n");
   printf("| Total Feature Detection Time:         %10.2f s  |\n", gpu_feat);
   printf("| Average Feature Detection Time:       %10.2f ms |\n", 1000 * gpu_feat / images.size());
   printf("| Total Descriptor Extraction Time:     %10.2f s  |\n", gpu_desc);
   printf("| Average Descriptor Extraction Time:   %10.2f ms |\n", 1000 * gpu_desc / images.size());
   printf("+=====================================================+\n");
   printf("| Comparison                                          |\n");
   printf("+=====================================================+\n");
   printf("| GPU Feature Detection Speedup:        %10.2f x  |\n", cpu_feat / gpu_feat);
   printf("| GPU Descriptor Extraction Speedup:    %10.2f x  |\n", cpu_desc / gpu_desc);
   printf("| GPU Combined Speedup:                 %10.2f x  |\n", (cpu_feat + cpu_desc) / (gpu_feat + gpu_desc));
   printf("+=====================================================+\n");
}

// detect features on the CPU
void detect_features_cpu(cv::Mat &src_img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) {
   static cv::SurfFeatureDetector detector;
   static cv::SurfDescriptorExtractor extractor;
   
   time(&detector, cpu_feat)->detect(src_img, keypoints);
   time(&extractor, cpu_desc)->compute(src_img, keypoints, descriptors);
} 

// detect features on the GPU
void detect_features_gpu(cv::Mat &img, vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) {
   static cv::gpu::SURF_GPU surf;
   
   cv::gpu::GpuMat img_gpu, keypoints_gpu, descriptors_gpu;
   
   // Send image to GPU
   img_gpu.upload(img);
 
   // Detect features
   time(&surf,gpu_feat)->operator()(img_gpu, cv::gpu::GpuMat(), keypoints_gpu);

   // Compute descriptors
   time(&surf,gpu_desc)->operator()(img_gpu, cv::gpu::GpuMat(), keypoints_gpu, descriptors_gpu, true);
 
   // Donload features and keypoints from GPU
   surf.downloadKeypoints(keypoints_gpu, keypoints);
   vector<float> descriptors_vector;
   surf.downloadDescriptors(descriptors_gpu, descriptors_vector);
   descriptors = cv::Mat(descriptors_vector);
}


// Display usage information
void readme(const string &program) {
   std::cout << "Usage: " << program << " path/to/images" << std::endl;
}


// Recursively get all of the files in the directory
list<string> get_files_recursive(const string &dir, const string &extension) {
   list<string> file_names;
   if (!boost::filesystem::exists(dir)) {
      throw std::runtime_error(string("Path not found: ")+dir);
   }
   boost::filesystem::directory_iterator end_itr;
   for (boost::filesystem::directory_iterator itr(dir); itr != end_itr; itr++) {
      if (boost::filesystem::is_directory(itr->status())) {
         // Search subdirectories for images
         list<string> nested_files = get_files_recursive(itr->path().string(), extension);
         file_names.insert(file_names.end(), nested_files.begin(), nested_files.end());
      } else {
         // Add files if they match the extensions in the list
         if (boost::filesystem::extension(itr->path()).compare(extension) == 0) {
            file_names.push_back(itr->path().string());
         }
      }
   }
   return file_names;
}

