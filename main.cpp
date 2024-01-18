#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <omp.h>
#include <fstream>


int searchImage_serial_version(cv::Mat image, int patchSize, std::string outputFileName) {
    outputFileName = "serial_" + outputFileName;
    std::ofstream resultsFile(outputFileName);
    if (!resultsFile.is_open()){
        std::cerr << "Unable to open the file: " << outputFileName << " .\n" << std::endl;
        return 1;
    }
    double start_time = omp_get_wtime();
    std::cout << "Searching process with serial algorithm is started..." << std::endl;
    std::vector<cv::Point> points;
    int match_count = 0;

    for (int x = 0; x < image.rows - patchSize; x++) {
        for (int y = 0; y < image.cols - patchSize; y++) {
            cv::Mat patch = image(cv::Range(x, x + patchSize),
                                  cv::Range(y, y + patchSize));
            points = {};

            for (int x2 = 0; x2 < image.rows - patchSize; x2++) {
                for (int y2 = 0; y2 < image.cols - patchSize; y2++) {

                    cv::Mat patch2 = image(cv::Range(x2, x2 + patchSize),
                                           cv::Range(y2, y2 + patchSize));

                    bool match = true;
                    for (int x3 = 0; x3 < patchSize; x3++) {
                        for (int y3 = 0; y3 < patchSize; y3++) {
                            ushort pixelValue = patch.at<ushort>(x3,y3);
                            ushort pixelValue2 = patch2.at<ushort>(x3,y3);
                            if (pixelValue != pixelValue2){
                                match = false;
                                break;
                            }
                        }
                        if (!match){
                            break;
                        }
                    }



                    if (match){
                        match_count += 1;
                        points.emplace_back(x2,y2);
                    }
                }
            }
            int my_rank = omp_get_thread_num();
            resultsFile << "Worker {" << my_rank << "} - PATCH " << patchSize << "x" << patchSize << " AT (" << x
                        << ", " << y << ") MATCHES FOLLOWING PATCH START POINTS:\n";
            for (const cv::Point &point: points) {
                resultsFile << "(" << point.x << ", " << point.y << "), ";
            }
            resultsFile << "\n";
        }
    }

    double end_time = omp_get_wtime();
    std::cout << "Search process with serial algorithm is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile << "Search process with serial algorithm is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile.close();

    return 0;
}


int searchImage_parallel_version1(cv::Mat image, int patchSize, int thread_count, std::string outputFileName) {
    outputFileName = "v1_" + outputFileName;
    std::ofstream resultsFile(outputFileName);
    if (!resultsFile.is_open()){
        std::cerr << "Unable to open the file: " << outputFileName << " .\n" << std::endl;
        return 1;
    }
    double start_time = omp_get_wtime();
    std::cout << "Searching process with version 1 of OpenMP started..." << std::endl;
    std::vector<cv::Point> points;
    int match_count = 0;

#pragma omp parallel for num_threads(thread_count) private(points) reduction(+: match_count)
    for (int x = 0; x < image.rows - patchSize; x++) {
        for (int y = 0; y < image.cols - patchSize; y++) {
            cv::Mat patch = image(cv::Range(x, x + patchSize),
                                  cv::Range(y, y + patchSize));
            points = {};

            for (int x2 = 0; x2 < image.rows - patchSize; x2++) {
                for (int y2 = 0; y2 < image.cols - patchSize; y2++) {

                    cv::Mat patch2 = image(cv::Range(x2, x2 + patchSize),
                                           cv::Range(y2, y2 + patchSize));

                    bool match = true;
                    for (int x3 = 0; x3 < patchSize; x3++) {
                        for (int y3 = 0; y3 < patchSize; y3++) {
                            ushort pixelValue = patch.at<ushort>(x3,y3);
                            ushort pixelValue2 = patch2.at<ushort>(x3,y3);
                            if (pixelValue != pixelValue2){
                                match = false;
                                break;
                            }
                        }
                        if (!match){
                            break;
                        }
                    }



                    if (match){
                        match_count += 1;
                        points.emplace_back(x2,y2);
                    }
                }
            }
#pragma omp critical
            {
                int my_rank = omp_get_thread_num();
                resultsFile << "Worker {" << my_rank << "} - PATCH " << patchSize << "x" << patchSize << " AT (" << x
                            << ", " << y << ") MATCHES FOLLOWING PATCH START POINTS:\n";
                for (const cv::Point &point: points) {
                    resultsFile << "(" << point.x << ", " << point.y << "), ";
                }
                resultsFile << "\n";
            }
        }
    }

    double end_time = omp_get_wtime();
    std::cout << "Search process with version 1 is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile << "Search process with version 1 is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile.close();

    return 0;
}


int searchImage_parallel_version2(cv::Mat image, int patchSize, int thread_count, std::string outputFileName) {
    outputFileName = "v2_" + outputFileName;
    std::ofstream resultsFile(outputFileName);
    if (!resultsFile.is_open()){
        std::cerr << "Unable to open the file: " << outputFileName << " .\n" << std::endl;
        return 1;
    }
    double start_time = omp_get_wtime();
    std::cout << "Searching process with version 2 of OpenMP started..." << std::endl;
    std::vector<cv::Point> points;
    int match_count = 0;

    for (int x = 0; x < image.rows - patchSize; x++) {
#pragma omp parallel for num_threads(thread_count) private(points) reduction(+: match_count)
        for (int y = 0; y < image.cols - patchSize; y++) {
            cv::Mat patch = image(cv::Range(x, x + patchSize),
                                  cv::Range(y, y + patchSize));
            points = {};

            for (int x2 = 0; x2 < image.rows - patchSize; x2++) {
                for (int y2 = 0; y2 < image.cols - patchSize; y2++) {
                    cv::Mat patch2 = image(cv::Range(x2, x2 + patchSize),
                                           cv::Range(y2, y2 + patchSize));

                    bool match = true;
                    for (int x3 = 0; x3 < patchSize; x3++) {
                        for (int y3 = 0; y3 < patchSize; y3++) {
                            ushort pixelValue = patch.at<ushort>(x3,y3);
                            ushort pixelValue2 = patch2.at<ushort>(x3,y3);
                            if (pixelValue != pixelValue2){
                                match = false;
                                break;
                            }
                        }
                        if (!match){
                            break;
                        }
                    }

                    if (match){
                        match_count += 1;
                        points.emplace_back(x2,y2);
                    }
                }
            }
#pragma omp critical
            {
                int my_rank = omp_get_thread_num();
                resultsFile << "Worker {" << my_rank << "} - PATCH " << patchSize << "x" << patchSize << " AT (" << x
                            << ", " << y << ") MATCHES FOLLOWING PATCH START POINTS:\n";
                for (const cv::Point &point: points) {
                    resultsFile << "(" << point.x << ", " << point.y << "), ";
                }
                resultsFile << "\n";
            }
        }
    }

    double end_time = omp_get_wtime();
    std::cout << "Search process with version 2 is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile << "Search process with version 2 is done. Time elapsed: " << end_time-start_time << " seconds.\nResults saved at " << outputFileName << ".\nTotal matches found within image:" <<match_count << std::endl;
    resultsFile.close();

    return 0;
}

int main() {
    // Getting inputs from user, the image that will be used within search process and the output file name
    // that results will be written.

    std::string inputImgFilePath;
    std::cout << "Kindly enter the image file path (search will be done at this file, exact location or if its under build folder, then image file path only is enough):\n";
    std::getline(std::cin, inputImgFilePath);
    cv::Mat image = cv::imread(inputImgFilePath, cv::IMREAD_ANYDEPTH | cv::IMREAD_GRAYSCALE);
    if(image.empty()){
        std::cerr << "Error" << std::endl;
        return -1;
    }

    std::string outputFileName;
    std::cout << "Kindly enter the output file name (search results will be written to this file):\n";
    std::getline(std::cin, outputFileName);


    std::string patch_size_str;
    std::cout << "Kindly enter the patch_size to search (i.e. 5 for 5x5 patches):\n";
    std::getline(std::cin, patch_size_str);
    int patchSize = std::stoi(patch_size_str);

    std::string str_thread_counts;
    std::cout << "Kindly enter the thread count (an integer > 0 or 0 if you want to use maximum possible for your env.):\n";
    std::getline(std::cin, str_thread_counts);
    int thread_count = std::stoi(str_thread_counts);

    //searchImage_serial_version(image, patchSize, outputFileName);

    // version 1 of OpenMP (rows parallelized to threads)
    std::cout << "~~~~~~~~~~~~\n";
    searchImage_parallel_version1(image, patchSize, thread_count, outputFileName);
    std::cout << "~~~~~~~~~~~~\n";
    // version 2 of OpenMP (columns parallelized to threads)
    searchImage_parallel_version2(image, patchSize, thread_count, outputFileName);

    return 0;
}
