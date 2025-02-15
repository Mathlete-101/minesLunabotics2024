/**
 * @file robotConstants.h
 * @author Blake Billharz
 * @brief Define physical constants used for the entire robot.
 * @version 0.1
 * @date 2024-02-27
 * 
 */

#ifndef ROBOTCONSTANTS_H
#define ROBOTCONSTANTS_H

// wheels
#define WHEEL_RADIUS_M  0.0765625
#define WHEEL_BASE_M    0.2711297
#define ENC_CNT_PER_REV  5281       // conflicting numbers from website, spec sheet, and experiment. Within 0.005% error

// laplace domain
#define K_VBAR       0.0585
#define SIGMA_VBAR   5
#define K_DELTAV     0.405
#define SIGMA_DELTAV 5

// math
#define pi 3.141592
#define FEET_PER_MEETER  3.28084
#define meters2feet(m)   (m * 3.28084)

// motors

/*
Left motor:
    encoderPwr -> 3v3
    encoderGnd -> GND
    encoderA -> 2
    encoderB -> 5
    direction -> 7
    pwm -> 9
Right motor:
    encoderPwr -> 3v3
    encoderGnd -> GND
    encoderA -> 3
    encoderB -> 6
    direction -> 8
    pwm -> 10
I2C:
    SCL -> A5
    SDA -> A4
    GND -> GND
*/
#define LEFT_F_PWM 9
#define LEFT_F_DIR 8
#define ENCR_A_FL 2
#define ENCR_B_FL 25

#define RIGHT_F_PWM 7
#define RIGHT_F_DIR 6
#define ENCR_A_FR 20
#define ENCR_B_FR 27

#define LEFT_B_PWM 12
#define LEFT_B_DIR 17
#define ENCR_A_BL 3
#define ENCR_B_BL 23

#define RIGHT_B_PWM 5
#define RIGHT_B_DIR 16
#define ENCR_A_BR 19
#define ENCR_B_BR 29










#define MAX_VOLTAGE 13.3    // guesses
#define UNUSABLE_VOLTAGE 0.5 // voltage where wheels don't turn at all, figure out for rover
#define volts2pwm(volts) ((volts/(MAX_VOLTAGE+UNUSABLE_VOLTAGE))*255)
#define volts2speed(volts) ((volts/(MAX_VOLTAGE+UNUSABLE_VOLTAGE))*400) // max speed for the motor driver library is +- 400


// control system
#define PHI_VEL_KP 3
#define PHI_VEL_KI 0
#define PHI_VEL_KD 0

#define PHI_POS_KP 20
#define PHI_POS_KI 12
#define PHI_POS_KD 0

#define RHO_VEL_KP 10
#define RHO_VEL_KI 0
#define RHO_VEL_KD 0

#define RHO_POS_KP 14.24
#define RHO_POS_KI 31.56
#define RHO_POS_KD 0



#endif