//
// Created by 33489 on 2024/12/27.
//
#include "classify.hpp"
#include <opencv2/opencv.hpp>

#include <cmath>
#include <string>
#include <vector>
#include <memory>

namespace rm_auto_aim {

    bool Classify::ReadAndSaveImages(const cv::String &inputPattern) {
        std::vector<cv::String> filenames;
        cv::glob(inputPattern, filenames, false);

        bool oneSavedSuccessfully = false;
        for (const auto &filename: filenames) {
            cv::Mat input = cv::imread(filename);
            if (input.empty()) {
                std::cerr << "Error: Could not read image " << filename << std::endl;
                return false;
            }

            // 提取文件名和扩展名
            std::string baseName = filename.substr(filename.find_last_of("\\/") + 1);
            size_t extPos = baseName.find_last_of(".");
            std::string baseWithoutExt = (extPos != std::string::npos) ? baseName.substr(0, extPos) : baseName;


            std::unique_ptr<rm_auto_aim::Detector> detector_ = std::make_unique<rm_auto_aim::Detector>(0, 0,
                                                                                                       rm_auto_aim::Detector::LightParams{},
                                                                                                       rm_auto_aim::Detector::ArmorParams{});
            detector_->initDetector();
            std::vector<rm_auto_aim::Armor> armors = detector_->detect(input);

            if (armors.empty()) {
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/unrecognizable_armor";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(),
                                                        baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, input);
                continue;
            }
             cv::Mat classify_img;
             classify_img= rm_auto_aim::Transform::getMean(input, armors);
             cv::resize(classify_img, classify_img, cv::Size(300, 300));
             cv::imshow("classify_img",classify_img);
             cv::waitKey(0);

            int key = cv::waitKey(0);

            if (key == '1') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/1/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/1/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/1/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/1/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }


            if (key == '2') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/2/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/2/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/2/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/2/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '3') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/3/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/3/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/3/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/3/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '4') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/4/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/4/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/4/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/4/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '5') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/negative_template/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/negative_template/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/negative_template/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/negative_template/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '6') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/outpost/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/outpost/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/outpost/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/outpost/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '8') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/base/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/base/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/base/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/base/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }

            if (key == '7') {

                cv::Mat original_img, mean_affine_img, affine_img, mean_img;

                original_img = rm_auto_aim::Transform::getOriginal(input, armors);
                cv::String outputDir1 = "E:/clionProjects/affine_transformation/last_pictures/sentry/original_img";
                cv::String outputFilename1 = cv::format("%s\\%s.bmp", outputDir1.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename1, original_img);


                mean_affine_img = rm_auto_aim::Transform::getMeanAffine(input, armors);
                cv::String outputDir2 = "E:/clionProjects/affine_transformation/last_pictures/sentry/mean_affine_img";
                cv::String outputFilename2 = cv::format("%s\\%s.bmp", outputDir2.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename2, mean_affine_img);


                affine_img = rm_auto_aim::Transform::getAffine(input, armors);
                cv::String outputDir3 = "E:/clionProjects/affine_transformation/last_pictures/sentry/affine_img";
                cv::String outputFilename3 = cv::format("%s\\%s.bmp", outputDir3.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename3, affine_img);


                mean_img = rm_auto_aim::Transform::getMean(input, armors);
                cv::String outputDir4 = "E:/clionProjects/affine_transformation/last_pictures/sentry/mean_img";
                cv::String outputFilename4 = cv::format("%s\\%s.bmp", outputDir4.c_str(), baseWithoutExt.c_str());
                cv::imwrite(outputFilename4, mean_img);


                bool thisSavedSuccessfully = cv::imwrite(outputFilename1, original_img) &&
                                             cv::imwrite(outputFilename2, mean_affine_img) &&
                                             cv::imwrite(outputFilename3, affine_img) &&
                                             cv::imwrite(outputFilename4, mean_img);
                if (thisSavedSuccessfully) {
                    oneSavedSuccessfully = true;
                    cv::destroyAllWindows();
                }
                continue;
            }


        }
        return oneSavedSuccessfully;
    }
}