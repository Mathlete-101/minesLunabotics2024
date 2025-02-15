/**
 * @file Tracker.h
 * @author Blake Billharz and Benjamin Sprik
 * @brief Given wheel encoder instances, keeps track of position, translational velocity, and rotational velocity.
 * @example
 * Encoder leftEnc(pin1, pin2);
 * Encoder rightEnc(pin3, pin4);
 * Tracker tracker(&rightEnc, &leftEnc);
 * 
 * void setup() {
 *      tracker.zero();
 * }
 * 
 * void loop() {
 *      // other stuff
 * 
 *      tracker.update();
 * 
 *      // see the readme for a complete list of tracked metrics.
 *      double phiSpeed = tracker.getPhiSpeedMpS();
 *      double phiPos = tracker.getPhiPosRad();
 *      double forwardVelocity = tracker.getRhoSpeedMpS();
 * 
 *      // re-zero only rho and phi
 *      tracker.zeroRho();
 *      tracker.zeroPhi();
 * }
 * 
 * @version 0.1
 * @date 2024-02-28
 * 
 * 
 */


#ifndef LOCALIZER_H
#define LOCALIZER_H

#include <Arduino.h>
#include <Encoder.h>
#include <FIR.h>
#include "robotConstants.h"

#define VELOCITY_READ_INTERVAL_MS 6
#define FILTER_TAP_NUM 7

class Tracker {
    public:

    /**
     * @brief Encoders must be pass-by-pointer. To instantiate, do Tracker(&encLeft, &encRight);
     * 
     * @param right_F_Encoder address of the front right encoder
     * @param left_F_Encoder address of the front left encoder
     * @param right_B_Encoder address of the back right encoder
     * @param left_B_Encoder address of the back left encoder
     */
    Tracker(Encoder* right_F_Encoder, Encoder* left_F_Encoder, Encoder* right_B_Encoder, Encoder* left_B_Encoder) {
        enc_F_Right = right_F_Encoder;
        enc_F_Left = left_F_Encoder;
        enc_B_Right = right_B_Encoder;
        enc_B_Left = left_B_Encoder;
        zero();
        right_F_Filter.setFilterCoeffs(filter_taps);
        left_F_Filter.setFilterCoeffs(filter_taps);
        right_B_Filter.setFilterCoeffs(filter_taps);
        left_B_Filter.setFilterCoeffs(filter_taps);
    }

    /**
     * @brief Zero's out all data
     * 
     */
    void zero() {
        enc_F_Right->readAndReset();
        enc_F_Left->readAndReset();
        enc_B_Right->readAndReset();
        enc_B_Left->readAndReset();
        right_F_PosRad = 0;
        left_F_PosRad = 0;
        right_B_PosRad = 0;
        left_B_PosRad = 0;
        right_F_EncCntLast = 0;
        left_F_EncCntLast = 0;
        right_B_EncCntLast = 0;
        left_B_EncCntLast = 0;
        right_F_PosRadLastRead = 0;
        left_F_PosRadLastRead = 0;
        right_B_PosRadLastRead = 0;
        left_B_PosRadLastRead = 0;
        phiPosRad = 0;
        rhoPosM = 0;
        xPosM = 0;
        yPosM = 0;
        right_F_SpeedRpS = 0;
        left_F_SpeedRpS = 0;
        right_B_SpeedRpS = 0;
        left_B_SpeedRpS = 0;
        rhoSpeedMpS = 0;
        phiSpeedRpS = 0;
        lastVelocityReadTime = millis();
        right_F_PosRadLastVelRead = 0;
        left_F_PosRadLastVelRead = 0;
        right_B_PosRadLastVelRead = 0;
        left_B_PosRadLastVelRead = 0;
    }

    /**
     * @brief Zero out rho position
     * 
     */
    void zeroRho() {
        rhoPosM = 0;
        rhoSpeedMpS = 0;
    }

    /**
     * @brief Zero out phi position
     * 
     */
    void zeroPhi() {
        phiPosRad = 0;
        phiSpeedRpS = 0;
    }

    /**
     * @brief Turns on and off FIR filter for velocity inputs. Defaults to true
     * 
     * @param filterVelocity 
     */
    void filterInputs(bool filterVelocity) {
        this->filterVelocity = filterVelocity;
    }

    /**
     * @brief Update the data using the encoders. This method should be called once every loop, or at least once every 10ms.
     * 
     */
    void update() {
        //FIXME I think the first few update after initialization might be way off?

        // read encoders and calculate positions in radians
        right_F_EncCnt = enc_F_Right->read();
        left_F_EncCnt = enc_F_Left->read();
        right_B_EncCnt = enc_B_Right->read();
        left_B_EncCnt = enc_B_Left->read();

        // update speeds if enough time has passed
        long curTime = millis();
        if (curTime - lastVelocityReadTime >= VELOCITY_READ_INTERVAL_MS) {

            // setup variables
            double timeStepS = (curTime - lastVelocityReadTime)/1000.0;
            right_F_PosRad = cnt2Rad(right_F_EncCnt);
            left_F_PosRad = cnt2Rad(left_F_EncCnt);
            right_B_PosRad = cnt2Rad(right_B_EncCnt);
            left_B_PosRad = cnt2Rad(left_B_EncCnt);

            // theta
            right_F_SpeedRpS = (right_F_PosRad - right_F_PosRadLastVelRead) / timeStepS;
            left_F_SpeedRpS = (left_F_PosRad - left_F_PosRadLastVelRead) / timeStepS;
            right_B_SpeedRpS = (right_B_PosRad - right_B_PosRadLastVelRead) / timeStepS;
            left_B_SpeedRpS = (left_B_PosRad - left_B_PosRadLastVelRead) / timeStepS;

            if (filterVelocity) {
                right_F_SpeedRpS = right_F_Filter.processReading(right_F_SpeedRpS);
                left_F_SpeedRpS = left_F_Filter.processReading(left_F_SpeedRpS);
                right_B_SpeedRpS = right_B_Filter.processReading(right_B_SpeedRpS);
                left_B_SpeedRpS = left_B_Filter.processReading(left_B_SpeedRpS);
            }

            
            // get average speeds for both sides
            double rightSpeedAvgRpS = (right_F_SpeedRpS + right_B_SpeedRpS) / 2.0;
            double leftSpeedAvgRpS = (left_F_SpeedRpS + left_B_SpeedRpS) / 2.0;
            // rho and phi

            // Caluclate speed with 4 wheels
            rhoSpeedMpS = (WHEEL_RADIUS_M / 2) * (rightSpeedAvgRpS + leftSpeedAvgRpS);
            phiSpeedRpS = (WHEEL_RADIUS_M / WHEEL_BASE_M) * (rightSpeedAvgRpS - leftSpeedAvgRpS);

            // save variables for next time
            lastVelocityReadTime = curTime;
            right_F_PosRadLastVelRead = right_F_PosRad;
            left_F_PosRadLastVelRead = left_F_PosRad;
            right_B_PosRadLastVelRead = right_B_PosRad;
            left_B_PosRadLastVelRead = left_B_PosRad;
        }


        // only update position when encoder counts change
        if ((right_F_EncCnt != right_F_PosRadLastVelRead) || (left_F_EncCnt != left_F_PosRadLastVelRead) || 
        (right_B_EncCnt != right_B_PosRadLastVelRead) || (left_B_EncCnt != left_B_PosRadLastVelRead)) {

            // wheel positions
            right_F_PosRad = cnt2Rad(right_F_EncCnt);
            left_F_PosRad = cnt2Rad(left_F_EncCnt);
            right_B_PosRad = cnt2Rad(right_B_EncCnt);
            left_B_PosRad = cnt2Rad(left_B_EncCnt);

            double delta_F_Right = right_F_PosRad - right_F_PosRadLastRead;
            double delta_F_Left = left_F_PosRad - left_F_PosRadLastRead;
            double delta_B_Right = right_B_PosRad - right_B_PosRadLastRead;
            double delta_B_Left = left_B_PosRad - left_B_PosRadLastRead;

            double deltaRight = (delta_F_Right + delta_B_Right) / 2.0;
            double deltaLeft = (delta_F_Left + delta_B_Left) / 2.0;

            right_F_PosRadLastRead = right_F_PosRad;
            left_F_PosRadLastRead = left_F_PosRad;
            right_B_PosRadLastRead = right_B_PosRad;
            left_B_PosRadLastRead = left_B_PosRad;

        
            // x and y
            xPosM = xPosM + ((WHEEL_RADIUS_M/2) * cos(phiPosRad) * (deltaLeft + deltaRight));
            yPosM = yPosM + ((WHEEL_RADIUS_M/2) * sin(phiPosRad) * (deltaLeft + deltaRight));

            // phi and rho for 4 wheels
            phiPosRad = (WHEEL_RADIUS_M / WHEEL_BASE_M) * ((right_F_PosRad + right_B_PosRad) - (left_F_PosRad + left_B_PosRad)) / 2.0; // average PosRad for each side and apply
            rhoPosM = (WHEEL_RADIUS_M / 4) * (right_F_PosRad + right_B_PosRad + left_F_PosRad + left_B_PosRad); // average PosRad  for each side and apply

        }

    }


    /**
     * @brief Convert encoder counts to radians
     * 
     * @param count 
     * @return double 
     */
    double cnt2Rad(long count) {
        return 2 * pi * (double) count / ENC_CNT_PER_REV;
    }


    // getters

    long getRight_F_EncCnt() {
        return right_F_EncCnt;
    }

    long getLeft_F_EncCnt() {
        return left_F_EncCnt;
    }

    long getRight_B_EncCnt() {
        return right_B_EncCnt;
    }

    long getLeft_B_EncCnt() {
        return left_B_EncCnt;
    }

    double getRight_F_PosRad() {
        return right_F_PosRad;
    }

    double getLeft_F_PosRad() {
        return left_F_PosRad;
    }

    double getRight_B_PosRad() {
        return right_B_PosRad;
    }

    double getLeft_B_PosRad() {
        return left_B_PosRad;
    }

    double getXPosM() {
        return xPosM;
    }

    double getYPosM() {
        return yPosM;
    }

    double getPhiPosRad() {
        return phiPosRad;
    }

    double getRhoPosM() {
        return rhoPosM;
    }

    double getRight_F_SpeedRpS() {
        return right_F_SpeedRpS;
    }

    double getLeft_F_SpeedRpS() {
        return left_F_SpeedRpS;
    }

     double getRight_B_SpeedRpS() {
        return right_F_SpeedRpS;
    }

    double getLeft_B_SpeedRpS() {
        return left_F_SpeedRpS;
    }

    double getRhoSpeedMpS() {
        return rhoSpeedMpS;
    }

    double getPhiSpeedRpS() {
        return phiSpeedRpS;
    }


    private:

    Encoder* enc_F_Right;
    Encoder* enc_F_Left;
    Encoder* enc_B_Right;
    Encoder* enc_B_Left;
    
    // position variables
    long right_F_EncCnt;
    long left_F_EncCnt;
    long right_B_EncCnt;
    long left_B_EncCnt;
    long right_F_EncCntLast;
    long left_F_EncCntLast;
    long right_B_EncCntLast;
    long left_B_EncCntLast;
    double right_F_PosRad;
    double left_F_PosRad;
    double right_B_PosRad;
    double left_B_PosRad;
    double right_F_PosRadLastRead;
    double left_F_PosRadLastRead;
    double right_B_PosRadLastRead;
    double left_B_PosRadLastRead;
    double phiPosRad;
    double rhoPosM;
    double xPosM;
    double yPosM;

    // speed variables
    double right_F_SpeedRpS;
    double left_F_SpeedRpS;
    double right_B_SpeedRpS;
    double left_B_SpeedRpS;
    double rhoSpeedMpS;
    double phiSpeedRpS;
    long lastVelocityReadTime;
    double right_F_PosRadLastVelRead;
    double left_F_PosRadLastVelRead;
    double right_B_PosRadLastVelRead;
    double left_B_PosRadLastVelRead;

    // velocity sample filter
    static double filter_taps[FILTER_TAP_NUM];
    bool filterVelocity = true;
    FIR<double, FILTER_TAP_NUM> right_F_Filter;
    FIR<double, FILTER_TAP_NUM> left_F_Filter;
    FIR<double, FILTER_TAP_NUM> right_B_Filter;
    FIR<double, FILTER_TAP_NUM> left_B_Filter;

    


};

/*

FIR filter designed with
http://t-filter.appspot.com

sampling frequency: 168 Hz

* 0 Hz - 30 Hz
  gain = 1
  desired ripple = 5 dB
  actual ripple = 0.9876762398562766 dB

* 70 Hz - 84 Hz
  gain = 0
  desired attenuation = -40 dB
  actual attenuation = -52.367093077634955 dB

*/

#define FILTER_TAP_NUM 7

double Tracker::filter_taps[FILTER_TAP_NUM] = {
  -0.05231456040945602,
  -0.023658218343350714,
  0.3021875219735181,
  0.5494702980680782,
  0.3021875219735181,
  -0.023658218343350714,
  -0.05231456040945602
};


#endif