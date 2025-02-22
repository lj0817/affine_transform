#include "detector.hpp"

#include <opencv2/opencv.hpp>
#include <algorithm>
#include <cmath>
#include <vector>

namespace rm_auto_aim
{
    Detector::Detector(
            const int & bin_thres, const int & color, const LightParams & l, const ArmorParams & a)
            : binary_thres(bin_thres), detect_color(color), l(l), a(a)
    {
    }
    void Detector::initDetector() {
       binary_thres = 100;
       //detect_color =  RED;
       detect_color =  0;
       l = {
               .min_ratio =  0.1,
               .max_ratio =  0.4,
               .max_angle = 40.0},
       a = {
               .min_light_ratio =  0.7,
               .min_small_center_distance =  0.8,
               .max_small_center_distance =  3.2,
               .min_large_center_distance = 3.2,
               .max_large_center_distance =  5.5,
               .max_angle =  35.0
       };
       //auto detector = std::make_unique<Detector>(binary_thres,detect_color,l,a);

   };

    std::vector<Armor> Detector::detect(const cv::Mat & input)
    {
        binary_img = preprocessImage(input);
        lights_ = findLights(input, binary_img);
        armors_ = matchLights(lights_);
        cv::Mat deepCopy;
        cv::Mat gray, dst;
        cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, dst);

        dst.copyTo(deepCopy);
        drawResults(deepCopy);
        cv::imshow("deepcopy",deepCopy);
        cv::waitKey(0);
        //cv::imshow("original",input);

        return armors_;
    }

    cv::Mat Detector::preprocessImage(const cv::Mat & rgb_img)
    {
        cv::Mat gray_img;
        cv::cvtColor(rgb_img, gray_img, cv::COLOR_RGB2GRAY);

        cv::Mat binary_img;
        cv::threshold(gray_img, binary_img, binary_thres, 255, cv::THRESH_BINARY);
       // cv::imshow("huidu1",binary_img);
       // cv::waitKey(0);
        return binary_img;
    }

    std::vector<Light> Detector::findLights(const cv::Mat & rbg_img, const cv::Mat & binary_img)
    {
        //std::cout<<"findlight ing"<<std::endl;
        using std::vector;
        vector<vector<cv::Point>> contours;
        vector<cv::Vec4i> hierarchy;
        cv::findContours(binary_img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        vector<Light> lights;

        for (const auto & contour : contours) {
            //if (contour.size() < 5) continue;

            auto r_rect = cv::minAreaRect(contour);

            // 绘制最小面积外接矩形
            cv::Point2f vertices[ 4 ];
            r_rect.points( vertices );

            /*for ( int i = 0; i < 4; ++i )
            {
                cv::line( rbg_img, vertices[ i ], vertices[ ( i + 1 ) % 4 ], cv::Scalar( 0, 255, 0 ), 2 );
            }
            cv::imshow( "rgb", rbg_img );
            cv::waitKey(0);*/


            auto light = Light(r_rect);

            if (isLight(light)) {
                //std::cout<<"islight ing"<<std::endl;
                auto rect = light.boundingRect();
                if (  // Avoid assertion failed
                        0 <= rect.x && 0 <= rect.width && rect.x + rect.width <= rbg_img.cols && 0 <= rect.y &&
                        0 <= rect.height && rect.y + rect.height <= rbg_img.rows) {
                    /*
                    //std::cout<<"pipei ing"<<std::endl;
                    int sum_r = 0, sum_b = 0;
                    auto roi = rbg_img(rect);
                    // Iterate through the ROI
                    for (int i = 0; i < roi.rows; i++) {
                        for (int j = 0; j < roi.cols; j++) {
                            if (cv::pointPolygonTest(contour, cv::Point2f(j + rect.x, i + rect.y), false) >= 0) {
                                // if point is inside contour
                                sum_r += roi.at<cv::Vec3b>(i, j)[0];
                                sum_b += roi.at<cv::Vec3b>(i, j)[2];
                            }
                        }
                    }
                    // Sum of red pixels > sum of blue pixels ?
                    //light.color = sum_r > sum_b ? RED : BLUE;
                    light.color = sum_r > sum_b ? BLUE : RED;

                    //printf("light.color is %d\n",light.color);*/

                    lights.emplace_back(light);
                    //printf("light.color is %d\n",lights.size);

                   /* if (lights.empty()) {
                        std::cout << "lights is empty" << std::endl;
                    } else {
                        printf("lights.size is %d\n",lights.size);
                    }*/
                }

            }
        }

        return lights;
    }

    bool Detector::isLight(const Light & light)
    {
        // The ratio of light (short side / long side)
        float ratio = light.width / light.length;
        bool ratio_ok = l.min_ratio < ratio && ratio < l.max_ratio;

        bool angle_ok = light.tilt_angle < l.max_angle;

        bool is_light = ratio_ok && angle_ok;

        return is_light;
    }

    std::vector<Armor> Detector::matchLights(const std::vector<Light> & lights)
    {
        std::vector<Armor> armors;
        //this->debug_armors.data.clear();

        // Loop all the pairing of lights
        for (auto light_1 = lights.begin(); light_1 != lights.end(); light_1++) {
            for (auto light_2 = light_1 + 1; light_2 != lights.end(); light_2++) {

                // if (light_1->color != detect_color || light_2->color != detect_color) continue;
                if(std::abs((light_1->top.x -light_2->top.x) - (light_1->bottom.x-light_2->bottom.x)) > 3)
                {
                    continue;
                }

                if (containLight(*light_1, *light_2, lights)) {
                    continue;
                }

                auto type = isArmor(*light_1, *light_2);
                if (type != ArmorType::INVALID) {
                    auto armor = Armor(*light_1, *light_2);
                    armor.type = type;
                    armors.emplace_back(armor);

                    //std::cout<<"armors numer: "<<armors.size()<<std::endl;

                    /* if (armors.empty()) {
                        std::cout << "armors is empty" << std::endl;
                    } else {
                        std::cout << "armors isnt empty" << std::endl;
                    }*/

                }
            }
        }

        return armors;
    }

// Check if there is another light in the boundingRect formed by the 2 lights
    bool Detector::containLight(
            const Light & light_1, const Light & light_2, const std::vector<Light> & lights)
    {

        auto points = std::vector<cv::Point2f>{light_1.top, light_1.bottom, light_2.top, light_2.bottom};
        auto bounding_rect = cv::boundingRect(points);

        for (const auto & test_light : lights) {

            //std::cout<<"containlight ing"<<std::endl;

            if (test_light.center == light_1.center || test_light.center == light_2.center) continue;


            if (
                    bounding_rect.contains(test_light.top) || bounding_rect.contains(test_light.bottom) ||
                    bounding_rect.contains(test_light.center)) {
                return true;
            }
        }

        return false;
    }

    ArmorType Detector::isArmor(const Light & light_1, const Light & light_2)
    {
        // Ratio of the length of 2 lights (short side / long side)
        float light_length_ratio = light_1.length < light_2.length ? light_1.length / light_2.length
                                                                   : light_2.length / light_1.length;
        bool light_ratio_ok = light_length_ratio > a.min_light_ratio;

        // Distance between the center of 2 lights (unit : light length)
        float avg_light_length = (light_1.length + light_2.length) / 2;
        float center_distance = cv::norm(light_1.center - light_2.center) / avg_light_length;
        bool center_distance_ok = (a.min_small_center_distance <= center_distance &&
                                   center_distance < a.max_small_center_distance) ||
                                  (a.min_large_center_distance <= center_distance &&
                                   center_distance < a.max_large_center_distance);

        // Angle of light center connection
        cv::Point2f diff = light_1.center - light_2.center;
        float angle = std::abs(std::atan(diff.y / diff.x)) / CV_PI * 180;
        bool angle_ok = angle < a.max_angle;

        bool is_armor = light_ratio_ok && center_distance_ok && angle_ok;

        // Judge armor type
        ArmorType type;
        if (is_armor) {
            type = center_distance > a.min_large_center_distance ? ArmorType::LARGE : ArmorType::SMALL;
        } else {
            type = ArmorType::INVALID;
        }

        return type;
    }

    void Detector::drawResults(cv::Mat & img)
    {
        // Draw Lights
        for (const auto & light : lights_) {
            cv::circle(img, light.top, 3, cv::Scalar(0, 255, 255), 2);
            cv::circle(img, light.bottom, 3, cv::Scalar(0, 255, 255), 21);
           //auto line_color = light.color == RED ? cv::Scalar(255, 255, 0) : cv::Scalar(255, 0, 255);
            //cv::line(img, light.top, light.bottom, line_color, 1);
            cv::line(img, light.top, light.bottom, (0,0,255), 1);
        }

        // Draw armors
        for (const auto & armor : armors_) {
            cv::line(img, armor.left_light.top, armor.right_light.bottom, cv::Scalar(0, 255, 0), 2);
            cv::line(img, armor.left_light.bottom, armor.right_light.top, cv::Scalar(0, 255, 0), 2);
        }

    }

}  // namespace rm_auto_aim
