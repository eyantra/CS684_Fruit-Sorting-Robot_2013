#define dc_motor_move_distance 25
#define dc_motor_move_distance_back 9
#define fruit_proximity_threshold1 30
#define fruit_proximity_threshold2 100

#define dispence_proximity_threshold 230
#define ip_proximity_threshold 200

//we have to remove jumper2 so IR proximity sensors 1,2,3,4 are disconnected & 
//we are using  pin 9 for IR proximity sensor of dispensing unit 
//we are using  pin 10 for IR proximity sensor of image processing unit 
#define ir_dispence_channel 5 //ADC channel 5 which is connected to pin 9 of processor
#define ir_ip_channel 4       //ADC channel 4 which is connected to pin 10 of processor


#define dc_motor_motion_delay 10

#define fun_delay 500

# define CLASS_1 0x31    // so 1 from PC
# define CLASS_2 0x32    // so 2 from PC
# define CLASS_3 0x33    // so 3 from PC

# define INIT_SIG 0x36	 // so 6 from PC
# define START_SIG 0x37  // so 7 from PC
# define PAUSE_SIG 0x38  // so 8 from PC
# define STOP_SIG 0x39   // so 9 from PC

# define CLASS1_DIR 1
# define CLASS2_DIR 20
# define CLASS3_DIR 45

