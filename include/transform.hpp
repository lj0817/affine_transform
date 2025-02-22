//
// Created by 33489 on 2024/12/22.
//
#ifndef AFFINE_TRANSFORMATION_TRANSFORM_HPP
#define AFFINE_TRANSFORMATION_TRANSFORM_HPP

#include "armor.hpp"
#include "detector.hpp"

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <string>
#include <cmath>
#define PI 3.1415

namespace rm_auto_aim{
    class Transform {
    public:
        static cv::Mat getOriginal(const cv::Mat &input,std::vector<rm_auto_aim::Armor> & armors);

        static cv::Mat getMeanAffine(const cv::Mat &input,std::vector<rm_auto_aim::Armor> & armors);

        static cv::Mat getAffine(const cv::Mat &input,std::vector<rm_auto_aim::Armor> & armors);

        static cv::Mat  getMean(const cv::Mat &input,std::vector<rm_auto_aim::Armor> & armors);

    };
}

#endif //AFFINE_TRANSFORMATION_TRANSFORM_HPP
