#ifndef SL_TOOLS_H
#define SL_TOOLS_H

///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#include <ros/time.h>
#include <sensor_msgs/Image.h>
#include <sl/Camera.hpp>
#include <string>

//#if ZED_SDK_MAJOR_VERSION>=2
//#if ZED_SDK_MINOR_VERSION>=8 // TODO put to 8 when SDK v2.8 will be available
//#define TERRAIN_MAPPING 1
//#endif
//#endif


namespace sl_tools {

    /** \brief Check if a ZED camera is ready
    * \param serial_number : the serial number of the camera to be checked
    */
    int checkCameraReady(unsigned int serial_number);

    /** \brief Get ZED camera properties
    * \param serial_number : the serial number of the camera
    */
    sl::DeviceProperties getZEDFromSN(unsigned int serial_number);

    std::vector<float> convertRodrigues(sl::float3 r);

    /** \brief Test if a file exist
    * \param name : the path to the file
    */
    bool file_exist(const std::string& name);

    /** \brief Get Stereolabs SDK version
     * \param major : major value for version
     * \param minor : minor value for version
     * \param sub_minor _ sub_minor value for version
     */
    std::string getSDKVersion(int& major, int& minor, int& sub_minor);

    /** \brief Convert StereoLabs timestamp to ROS timestamp
     *  \param t : Stereolabs timestamp to be converted
     */
    ros::Time slTime2Ros(sl::timeStamp t);

    /** \brief sl::Mat to ros message conversion
     * \param img : the image to publish
     * \param frameId : the id of the reference frame of the image
     * \param t : the ros::Time to stamp the image
     */
    sensor_msgs::ImagePtr imageToROSmsg(sl::Mat img, std::string frameId, ros::Time t);

    // TODO Remove when it will be available in Terrain SDK
    inline float packColor(sl::uchar3 colorIn) {
        float color_uint;
        unsigned char* color_uchar = (unsigned char*) &color_uint;
        for (int c = 0; c < 3; c++) {
            color_uchar[c] = colorIn[c];
        }
        return color_uint;
    }

    // TODO Remove when it will be available in Terrain SDK
    inline float packColor(sl::uchar4 colorIn) {
        float color_uint; // = (colorIn.r << 16 | colorIn.g << 8 | colorIn.b);
        unsigned char* color_uchar = (unsigned char*) &color_uint;
        for (int c = 0; c < 4; c++) {
            color_uchar[c] = colorIn[c];
        }
        return color_uint;
    }

    // TODO Remove when it will be available in Terrain SDK
    inline sl::uchar3 depackColor3(float colorIn) {
        sl::uchar3 out;
        uint32_t color_uint = *(uint32_t*) & colorIn;
        unsigned char* color_uchar = (unsigned char*) &color_uint;
        for (int c = 0; c < 3; c++) {
            out[c] = static_cast<unsigned char>(color_uchar[c]);
        }
        return out;
    }

    // TODO Remove when it will be available in Terrain SDK
    inline sl::uchar4 depackColor4(float colorIn) {
        sl::uchar4 out;
        uint32_t color_uint = *(uint32_t*) & colorIn;
        unsigned char* color_uchar = (unsigned char*) &color_uint;
        for (int c = 0; c < 3; c++) {
            out[c] = static_cast<unsigned char>(color_uchar[c]);
        }
        out.w = 255;
        return out;
    }

    // TODO Remove when it will be available in Terrain SDK
    inline sl::float3 depackColor3f(float colorIn) {
        sl::float3 out;
        uint32_t color_uint = *(uint32_t*) & colorIn;
        unsigned char* color_uchar = (unsigned char*) &color_uint;
        for (int c = 0; c < 3; c++) {
            out[c] = static_cast<float>(color_uchar[2 - c] / 255.f);
        }
        return out;
    }
    
     /*!
     * \brief The CSmartMean class is used to
     * make a mobile window mean of a sequence of values
     * and reject outliers.
     * Tutorial:
     * https://www.myzhar.com/blog/tutorials/tutorial-exponential-weighted-average-good-moving-windows-average/
     */
    class CSmartMean {
      public:
        CSmartMean(int winSize);

        int getValCount() {
            return mValCount;   ///< Return the number of values in the sequence
        }

        double getMean() {
            return mMean;   ///< Return the updated mean
        }

        /*!
         * \brief addValue
         * Add a value to the sequence
         * \param val value to be added
         * \return mean value
         */
        double addValue(double val);

      private:
        int mWinSize; ///< The size of the window (number of values ti evaluate)
        int mValCount; ///< The number of values in sequence

        double mMeanCorr; ///< Used for bias correction
        double mMean;     ///< The mean of the last \ref mWinSize values

        double mGamma; ///< Weight value
    };
} // namespace

#endif // SL_TOOLS_H
