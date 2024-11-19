//new system by Nathan
class DriveMotor{
    public:
    int pin_number;
    DriveMotor(int pin_number){
	this->pin_number = pin_number;
    }

    //variables for get_velocity
    int prev_pos = 0;
    float prevVelocity = 0;
    float velocity = 0; 
    float prevTime = 0;
    float get_velocity() {
	unsigned long myTime = micros();
	readEncoders();
	int delta_pos = enc_pos - prev_pos;
	float deltaTime = (myTime - prevTime) * 0.000001; // Convert microseconds to seconds
	if (abs(delta_pos) > (enc_per_rev / 2)) {delta_pos = enc_pos + (enc_per_rev - prev_pos);}
	velocity = (1.0 / enc_per_rev) * (PI * PHYSICAL::WHEEL_DIAMETER) * (delta_pos / deltaTime);
	prev_pos = enc_pos;
	prevTime = myTime;
	prevVelocity = velocity;
	return velocity;

    }

    //variables for read_position
    int prev_pos = 0;
    float pos = 0;
    float read_position(){
	pos += ((1.0/enc_per_rev)*(enc_pos-prev_pos)*PI*PHYSICAL::WHEEL_DIAMETER);
	float s_pos2 = rotations + pos;
	prev_pos = enc_pos;
	return s_pos2;
    }
    
}


//old system
float fl_d_vel() {
    unsigned long myTime = micros();
    static int prev_pos_fl = 0;
    static float prevVelocity = 0;
    static float velocity = 0; 
    static float prevTime = 0;
    readEncoders();
    int delta_pos = fl_enc_pos - prev_pos_fl;
    float deltaTime = (myTime - prevTime) * 0.000001; // Convert microseconds to seconds
    if (abs(delta_pos) > (enc_per_rev / 2)) {delta_pos = fl_enc_pos + (enc_per_rev - prev_pos_fl);}
    velocity = (1.0 / enc_per_rev) * (PI * wheel_diameter) * (delta_pos / deltaTime);
    prev_pos_fl = fl_enc_pos;
    prevTime = myTime;
    prevVelocity = velocity;
    return velocity;
}


float fr_d_vel(){
    unsigned long myTime = micros();
    static int prev_pos_fr = 0;
    static float prevVelocity = 0;
    static float velocity = 0; 
    static float prevTime = 0;
    readEncoders();
    int delta_pos = fr_enc_pos - prev_pos_fr;
    float deltaTime = (myTime - prevTime) * 0.000001; // Convert microseconds to seconds
    if (abs(delta_pos) > (enc_per_rev / 2)) {delta_pos = fr_enc_pos + (enc_per_rev - prev_pos_fr);}
    velocity = (1.0 / enc_per_rev) * (PI * wheel_diameter) * (delta_pos / deltaTime);
    prev_pos_fr = fr_enc_pos;
    prevTime = myTime;
    prevVelocity = velocity;
    return velocity;
  }

float bl_d_vel(){
    unsigned long myTime = micros();
    static int prev_pos_bl = 0;
    static float prevVelocity = 0;
    static float velocity = 0; 
    static float prevTime = 0;
    readEncoders();
    int delta_pos = bl_enc_pos - prev_pos_bl;
    float deltaTime = (myTime - prevTime) * 0.000001; // Convert microseconds to seconds
    if (abs(delta_pos) > (enc_per_rev / 2)) {delta_pos = bl_enc_pos + (enc_per_rev - prev_pos_bl);}
    velocity = (1.0 / enc_per_rev) * (PI * wheel_diameter) * (delta_pos / deltaTime);
    prev_pos_bl = bl_enc_pos;
    prevTime = myTime;
    prevVelocity = velocity;
  return velocity;
  }


float br_d_vel(){
    unsigned long myTime = micros();
    static int prev_pos_br = 0;
    static float prevVelocity = 0;
    static float velocity = 0; 
    static float prevTime = 0;
    readEncoders();
    int delta_pos = br_enc_pos - prev_pos_br;
    float deltaTime = (myTime - prevTime) * 0.000001; // Convert microseconds to seconds
    if (abs(delta_pos) > (enc_per_rev / 2)) { delta_pos = br_enc_pos + (enc_per_rev - prev_pos_br);}
    velocity = (1.0 / enc_per_rev) * (PI * wheel_diameter) * (delta_pos / deltaTime);
    prev_pos_br = br_enc_pos;
    prevTime = myTime;
    prevVelocity = velocity;
  return velocity;
  }
  // m/s = (1 rev / 5281 counts) * (pi*.345 m)/1 rev*((number of current counts - number of previous counts)/seconds passed)  
 
float fl_pos(){
  static int prev_pos_fl = 0;
  static float pos = 0;
  pos += ((1.0/enc_per_rev)*(fl_enc_pos-prev_pos_fl)*PI*wheel_diameter);
  float fl_s_pos2 = fl_rotations + pos;
  prev_pos_fl = fl_enc_pos;
  return fl_s_pos2;
  }

float fr_pos(){
  static int prev_pos_fr = 0;
  static float pos = 0;
  pos += (1.0/enc_per_rev)*(fr_enc_pos-prev_pos_fr)*PI*wheel_diameter;
  float fr_s_pos2 = fr_rotations + pos;
  prev_pos_fr = fr_enc_pos;
  return fr_s_pos2;
  }

float bl_pos(){
  static int prev_pos_bl = 0;
  static float pos = 0;
  pos += (1.0/enc_per_rev)*(bl_enc_pos-prev_pos_bl)*PI*wheel_diameter;
  float bl_s_pos2 = bl_rotations + pos;
  prev_pos_bl = bl_enc_pos;
  return bl_s_pos2;
  }

float br_pos(){
  static int prev_pos_br = 0;
  static float pos = 0;
  pos += (1.0/enc_per_rev)*(br_enc_pos-prev_pos_br)*PI*wheel_diameter;
  float br_s_pos2 = br_rotations + pos;
  prev_pos_br = br_enc_pos;
  return br_s_pos2;
  }
