#include "transform.hpp"


namespace rm_auto_aim {

    cv::Mat Transform::getMeanAffine(const cv::Mat &input, std::vector<rm_auto_aim::Armor> &armors) {

        cv::Mat gray, dst;
        cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gray, dst);

        // Light length in image
        const int light_length = 12;
        // Image size after warp
        const int warp_height = 28;
        const int small_armor_width = 32;
        const int large_armor_width = 54;
        // Number ROI size
        const cv::Size roi_size(20, 28);

        for (auto &armor: armors) {
            // Warp perspective transform
            cv::Point2f lights_vertices[4] = {
                    armor.left_light.bottom, armor.left_light.top, armor.right_light.top,
                    armor.right_light.bottom};//原始图像四边形顶点

            const int top_light_y = (warp_height - light_length) / 2 - 1;
            const int bottom_light_y = top_light_y + light_length;
            const int warp_width = armor.type == ArmorType::SMALL ? small_armor_width : large_armor_width;
            cv::Point2f target_vertices[4] = {
                    cv::Point(0, bottom_light_y),
                    cv::Point(0, top_light_y),
                    cv::Point(warp_width - 1, top_light_y),
                    cv::Point(warp_width - 1, bottom_light_y),//变换后的四边形顶点
            };
            cv::Mat number_image;//变换后的图像
            auto rotation_matrix = cv::getPerspectiveTransform(lights_vertices, target_vertices);//透视变换矩阵
            cv::warpPerspective(dst, number_image, rotation_matrix, cv::Size(warp_width, warp_height));//透射变换

            // Get ROI
            number_image =
                    number_image(cv::Rect(cv::Point((warp_width - roi_size.width) / 2, 0), roi_size));

            // Binarize
            //cv::cvtColor(number_image, number_image, cv::COLOR_RGB2GRAY);
            //cv::threshold(number_image, number_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

            armor.number_img = number_image;
            cv::Mat mean_affine_img = armor.number_img;
            cv::Mat mean_affine_img2;
            cv::resize(mean_affine_img, mean_affine_img2, cv::Size(32, 32));
            //imshow("mean_affine_img", mean_affine_img2);
            return mean_affine_img2;
        }
    }

    cv::Mat Transform::getAffine(const cv::Mat &input, std::vector<rm_auto_aim::Armor> &armors) {

        // Light length in image
        const int light_length = 12;
        // Image size after warp
        const int warp_height = 28;
        const int small_armor_width = 32;
        const int large_armor_width = 54;
        // Number ROI size
        const cv::Size roi_size(20, 28);

        for (auto &armor: armors) {
            // Warp perspective transform
            cv::Point2f lights_vertices[4] = {
                    armor.left_light.bottom, armor.left_light.top, armor.right_light.top,
                    armor.right_light.bottom};//原始图像四边形顶点

            const int top_light_y = (warp_height - light_length) / 2 - 1;
            const int bottom_light_y = top_light_y + light_length;
            const int warp_width = armor.type == ArmorType::SMALL ? small_armor_width : large_armor_width;
            cv::Point2f target_vertices[4] = {
                    cv::Point(0, bottom_light_y),
                    cv::Point(0, top_light_y),
                    cv::Point(warp_width - 1, top_light_y),
                    cv::Point(warp_width - 1, bottom_light_y),//变换后的四边形顶点
            };
            cv::Mat number_image;//变换后的图像
            auto rotation_matrix = cv::getPerspectiveTransform(lights_vertices, target_vertices);//透视变换矩阵
            cv::warpPerspective(input, number_image, rotation_matrix, cv::Size(warp_width, warp_height));//透射变换

            // Get ROI
            number_image =
                    number_image(cv::Rect(cv::Point((warp_width - roi_size.width) / 2, 0), roi_size));

            armor.number_img = number_image;
            cv::Mat affine_img = armor.number_img;
            cv::Mat affine_img2;
            cv::resize(affine_img, affine_img2, cv::Size(32, 32));
            return affine_img2;
        }
    }

    cv::Mat Transform::getOriginal(const cv::Mat &input, std::vector<rm_auto_aim::Armor> &armors) {
        for (auto &armor: armors) {
            // Warp perspective transform
            cv::Point2f lights_vertices[4] = {
                    armor.left_light.bottom,
                    armor.left_light.top,
                    armor.right_light.top,
                    armor.right_light.bottom};//原始图像四边形顶点
            //原始矩形
            //数组转换为向量
            std::vector<cv::Point2f> points;

            for (int i = 0; i < 4; ++i) {
                points.push_back(lights_vertices[i]);
            }

            cv::RotatedRect minRect = cv::minAreaRect(points);
            cv::Point2f vertices[4];
            minRect.points(vertices);

            cv::Point2f center = minRect.center;
            float center_x = center.x;
            float center_y = center.y;
            float c_x,c_y;
            if(armor.type == ArmorType::SMALL ) {
                c_x = ((armor.left_light.bottom.y - armor.left_light.top.y) * 1.7) / 2;
                c_y = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.2) / 2;
            }
            else
            {
                c_x = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.4) / 2;
                c_y = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.1) / 2;
                //std::cout<<"da"<<std::endl;
            }
            float pt1_x = center_x - c_x;
            float pt1_y = center_y - c_y;
            float pt2_x = center_x + c_x;
            float pt2_y = center_y + c_y;

            if(pt2_x >= input.cols || pt2_y >= input.rows)
            {
                pt2_x = input.cols - 1;
                pt2_y = input.rows - 1;
            }
            cv::Point2f pt1 (pt1_x,pt1_y);
            cv::Point2f pt2 (pt2_x,pt2_y);

            int width = std::abs(pt2.x - pt1.x);
            int height = std::abs(pt2.y - pt1.y);
            cv::Rect roiRect(pt1, cv::Size(width, height));
            cv::Mat roi = input(roiRect);
            cv::Mat original_img;
            cv::resize(roi, original_img, cv::Size(32, 32));
            return original_img;
        }
    }

    cv::Mat Transform::getMean(const cv::Mat &input, std::vector<rm_auto_aim::Armor> &armors) {
        for (auto &armor: armors) {
            // Warp perspective transform
            cv::Point2f lights_vertices[4] = {
                    armor.left_light.bottom,
                    armor.left_light.top,
                    armor.right_light.top,
                    armor.right_light.bottom};//原始图像四边形顶点
            //原始矩形
            //数组转换为向量
            std::vector<cv::Point2f> points;

            for (int i = 0; i < 4; ++i) {
                points.push_back(lights_vertices[i]);
            }

            cv::RotatedRect minRect = cv::minAreaRect(points);
            cv::Point2f vertices[4];
            minRect.points(vertices);

            cv::Mat gray, dst;
            cvtColor(input, gray, cv::COLOR_BGR2GRAY);
            cv::equalizeHist(gray, dst);

            cv::Point2f center = minRect.center;
            float center_x = center.x;
            float center_y = center.y;
            float c_x,c_y;
            if(armor.type == ArmorType::SMALL ) {
                c_x = ((armor.left_light.bottom.y - armor.left_light.top.y) * 1.7) / 2;
                c_y = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.4) / 2;
                //std::cout<<"小"<<std::endl;
            }
            else
            {
                c_x = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.4) / 2;
                c_y = ((armor.left_light.bottom.y - armor.left_light.top.y) * 2.1) / 2;
               //std::cout<<"da"<<std::endl;
            }
            float pt1_x = center_x - c_x;
            float pt1_y = center_y - c_y;
            float pt2_x = center_x + c_x;
            float pt2_y = center_y + c_y;

            if(pt2_x >= dst.cols || pt2_y >= dst.rows)
            {
                pt2_x = dst.cols - 1;
                pt2_y = dst.rows - 1;
            }

            cv::Point2f pt1 (pt1_x,pt1_y);
            cv::Point2f pt2 (pt2_x,pt2_y);

           // cv::rectangle(dst, pt1, pt2, (0,0,255),2);

             int width = std::abs(pt2.x - pt1.x);
             int height = std::abs(pt2.y - pt1.y);
             cv::Rect roiRect(pt1, cv::Size(width, height));
             cv::Mat roi = dst(roiRect);
             cv::Mat mean_img;
             cv::resize(roi, mean_img, cv::Size(32, 32));
             return mean_img;
        }
    }

   }

