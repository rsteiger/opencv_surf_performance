#include <iostream>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <boost/filesystem.hpp>

// don't use cv namespace
// cv and std namespaces overlap (string, find, etc.)
using namespace std;

void readme(const string &program);
list<string> get_files_recursive(const string &dir, const string &extension);


int main(int argc, const char *argv[]) {
   if (argc != 2) { readme(argv[0]); return 0; }

   list<string> images = get_files_recursive(argv[1], ".jpg");
   
   // Detect features for each image
   for (list<string>::iterator image = images.begin(); image != images.end(); image++) {
      std::cout << *image << std::endl;
      cv::Mat grayscale_image = cv::imread( *image, CV_LOAD_IMAGE_GRAYSCALE );
   }
}

// Display run information
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


