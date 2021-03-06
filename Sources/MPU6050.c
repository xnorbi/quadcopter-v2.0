/*
 * MPU6050.c
 *
 *  Created on: Oct 26, 2014
 *      Author: Norbi
 */

#include "init.h"
#include "math.h"

int16_t GYRO_XOUT_OFFSET = 0;
int16_t GYRO_YOUT_OFFSET = 0;
int16_t GYRO_ZOUT_OFFSET = 0;

int16_t ACCEL_XOUT_OFFSET = 0;
int16_t ACCEL_YOUT_OFFSET = 0;
int16_t ACCEL_ZOUT_OFFSET = 0;

float GYRO_XANGLE = 0;
float GYRO_YANGLE = 0;
float GYRO_ZANGLE = 0;

int16_t GYRO_XOUT = 0;
int16_t GYRO_YOUT = 0;
int16_t GYRO_ZOUT = 0;

int16_t ACCEL_XOUT = 0;
int16_t ACCEL_YOUT = 0;
int16_t ACCEL_ZOUT = 0;
float ACCEL_Gx = 0;
float ACCEL_Gy = 0;
float ACCEL_Gz = 0;
double ACCEL=0;

float GYRO_XRATE = 0;
float GYRO_YRATE = 0;
float GYRO_ZRATE = 0;

float ACCEL_XANGLE;
float ACCEL_YANGLE;

float gyro_xsensitivity=66.5;
float gyro_ysensitivity=66.5;
float gyro_zsensitivity=66.5;
float a=0.995;
float dt=0.0025;

float COMPLEMENTARY_XANGLE = 0;
float COMPLEMENTARY_YANGLE = 0;

float filter_xterm[3] = {0,0,0};
float filter_yterm[3] = {0,0,0};

float TEMP;
float timeConstant=0.1;//0.35;//0.35;

float Gx_offset=0,Gy_offset=0,Gz_offset=0;
float XANGLE_OFFSET=0, YANGLE_OFFSET=0;
float X_Angle_user=0.2, Y_Angle_user=-1.3;

char MPU6050_Test_I2C()
{
    char Data = 0x00;
    Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I);

    if(Data == 0x68)
    {
        return 0;
    }
    else
    {
    	_SLCDModule_TurnOffAllSegments();
    	_SLCDModule_PrintString("ERROR",0);
    	while(1) {}
    	return 19;  
    }

}

void MPU6050_Setup()
{
	Delay_mS(200);
	//Sets sample rate to 8000/1+7 = 500Hz
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x01);
	Delay_mS(200);
	//Disable FSync, 256Hz DLPF
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x03);
	Delay_mS(200);
	//Disable gyro self tests, scale of 500 degrees/s
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0b00001000);
	Delay_mS(200);
	//Disable accel self tests, scale of +-4g, no DHPF
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0b00001000);
	Delay_mS(200);
	//Freefall threshold of <|0mg|
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_FF_THR, 0x00);
	//Freefall duration limit of 0
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 0x00);
	//Motion threshold of >0mg
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 0x00);
	//Motion duration of >0s
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 0x00);
	//Zero motion threshold
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 0x00);
	//Zero motion duration threshold
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 0x00);
	//Disable sensor output to FIFO buffer
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, 0x00);
	
	//AUX I2C setup
	//Sets AUX I2C to single master control, plus other config
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, 0x00);
	//Setup AUX I2C slaves	
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, 0x00);
		 
	//MPU6050_RA_I2C_MST_STATUS //Read-only
	//Setup INT pin and AUX I2C pass through
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 0x00);
	//Enable data ready interrupt
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);
	
	//MPU6050_RA_DMP_INT_STATUS		//Read-only
	//MPU6050_RA_INT_STATUS 3A		//Read-only
	//MPU6050_RA_ACCEL_XOUT_H 		//Read-only
	//MPU6050_RA_ACCEL_XOUT_L 		//Read-only
	//MPU6050_RA_ACCEL_YOUT_H 		//Read-only
	//MPU6050_RA_ACCEL_YOUT_L 		//Read-only
	//MPU6050_RA_ACCEL_ZOUT_H 		//Read-only
	//MPU6050_RA_ACCEL_ZOUT_L 		//Read-only
	//MPU6050_RA_TEMP_OUT_H 		//Read-only
	//MPU6050_RA_TEMP_OUT_L 		//Read-only
	//MPU6050_RA_GYRO_XOUT_H 		//Read-only
	//MPU6050_RA_GYRO_XOUT_L 		//Read-only
	//MPU6050_RA_GYRO_YOUT_H 		//Read-only
	//MPU6050_RA_GYRO_YOUT_L 		//Read-only
	//MPU6050_RA_GYRO_ZOUT_H 		//Read-only
	//MPU6050_RA_GYRO_ZOUT_L 		//Read-only
	//MPU6050_RA_EXT_SENS_DATA_00 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_01 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_02 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_03 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_04 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_05 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_06 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_07 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_08 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_09 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_10 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_11 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_12 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_13 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_14 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_15 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_16 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_17 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_18 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_19 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_20 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_21 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_22 	//Read-only
	//MPU6050_RA_EXT_SENS_DATA_23 	//Read-only
	//MPU6050_RA_MOT_DETECT_STATUS 	//Read-only
		
	//Slave out, dont care
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, 0x00);
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, 0x00); 	
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, 0x00); 	
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, 0x00); 	
	//More slave config
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
	//Reset sensor signal paths
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00); 	
	//Motion detection control
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, 0x00); 		
	//Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, 0x00);
	//Sets clock source to gyro reference w/ PLL
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00000010);
	//Controls frequency of wakeups in accel low power mode plus the sensor standby modes
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0x00);
	//MPU6050_RA_BANK_SEL			//Not in datasheet
	//MPU6050_RA_MEM_START_ADDR		//Not in datasheet
	//MPU6050_RA_MEM_R_W			//Not in datasheet
	//MPU6050_RA_DMP_CFG_1			//Not in datasheet
	//MPU6050_RA_DMP_CFG_2			//Not in datasheet
	//MPU6050_RA_FIFO_COUNTH		//Read-only
	//MPU6050_RA_FIFO_COUNTL		//Read-only
	//Data transfer to and from the FIFO buffer
	I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, 0x00);
	//MPU6050_RA_WHO_AM_I 			//Read-only, I2C address
	
	//printf("\nMPU6050 Setup Complete");
	
	//For some reason somehere above this point the modul resetting, maybe power filtering problems
	//Sets sample rate to 8000/1+7 = 500Hz
		I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x01);
		Delay_mS(200);
		//Disable FSync, 48Hz DLPF
		I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x03);
		Delay_mS(200);
		//Disable gyro self tests, scale of 500 degrees/s
		I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0b00001000);
		Delay_mS(200);
		//Disable accel self tests, scale of +-4g, no DHPF
		I2CWriteRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0b00001000);
		Delay_mS(200);
}
int MPU6050_Check_Registers()
{
	unsigned char Data = 0x00;
	unsigned char Failed = 0;

	Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV);
	if(Data != 0x01) {  Failed = 1; }
	Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_CONFIG);
	if(Data != 0x03) {   Failed = 1; }
	Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG);
	if(Data != 0b00001000) {  Failed = 1; }
	Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG);
	if(Data != 0b00001000) {   Failed = 1; }
	Data=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_FF_THR);
	if(Data != 0x00) {   Failed = 1; }
	return Failed;
}
void Calibrate_Accel()
{
	double XANGLE_SUM=0,YANGLE_SUM=0,Gx_sum=0,Gy_sum=0,Gz_sum=0;
	int x;
	
	for(x=0;x<1000;x++)
	{
		read_MPU6050();
		Get_Angles();
		XANGLE_SUM=XANGLE_SUM+ACCEL_XANGLE;
		YANGLE_SUM=YANGLE_SUM+ACCEL_YANGLE;
	}
	XANGLE_OFFSET=XANGLE_SUM/1000;
	YANGLE_OFFSET=YANGLE_SUM/1000;

}

//Gets raw accelerometer data, performs no processing
void Get_Accel_Values()
{
	char temp[2];
	temp[0]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H);
	temp[1]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L);
	ACCEL_XOUT = (((temp[0]<<8) & 0xFF00)| (temp[1] & 0x00FF))-(float)Gx_offset*8192;//-ACCEL_XOUT_OFFSET;

	temp[0]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H);
	temp[1]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L);
	ACCEL_YOUT = (((temp[0]<<8) & 0xFF00)| (temp[1] & 0x00FF))-(float)Gy_offset*8192;//+ACCEL_YOUT_OFFSET;

	temp[0]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H);
	temp[1]=I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L);
	ACCEL_ZOUT = (((temp[0]<<8) & 0xFF00)| (temp[1] & 0x00FF))-(float)Gz_offset*8192;//-ACCEL_ZOUT_OFFSET;	
}
//Convert Acceleration values to G, 1 G = 9.81 m*s^-2
void Convert_Accel()
{
	ACCEL_Gx = (float)ACCEL_XOUT/8192;
	ACCEL_Gy = (float)ACCEL_YOUT/8192;
	ACCEL_Gz = (float)ACCEL_ZOUT/8192;
	ACCEL = pow(pow(ACCEL_Gx,2)+pow(ACCEL_Gy,2)+pow(ACCEL_Gz,2),0.5);
}

void Calibrate_Gyros()
{
	char temp[6], string_lcd[10];
	int x = 0;
	int32_t GYRO_XOUT_OFFSET_SUM = 0;
	int32_t GYRO_YOUT_OFFSET_SUM = 0;
	int32_t GYRO_ZOUT_OFFSET_SUM = 0;
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L);
	I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H);
	Delay_mS(1);

	for(x = 0; x<1000; x++)
	{
		temp[0]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H);
		temp[1]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L);
		temp[2]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H);
		temp[3]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L);
		temp[4]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H);
		temp[5]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L);
		
//		I2CReadMultiRegisters(MPU6050_ADDRESS,MPU6050_RA_GYRO_XOUT_H,&temp[0],6);

		
		GYRO_XOUT_OFFSET_SUM += ((temp[0]<<8)|temp[1]);
		GYRO_YOUT_OFFSET_SUM += ((temp[2]<<8)|temp[3]);
		GYRO_ZOUT_OFFSET_SUM += ((temp[4]<<8)|temp[5]);
 
		Delay_mS(1);
	}	
	GYRO_XOUT_OFFSET = GYRO_XOUT_OFFSET_SUM/1000;
	GYRO_YOUT_OFFSET = GYRO_YOUT_OFFSET_SUM/1000;
	GYRO_ZOUT_OFFSET = GYRO_ZOUT_OFFSET_SUM/1000;
	_SLCDModule_TurnOffAllSegments();
	sprintf(string_lcd,"%d",(int)(GYRO_XOUT_OFFSET));
	_SLCDModule_PrintString(string_lcd,5);	
	GYRO_XOUT_OFFSET = 119;
	GYRO_YOUT_OFFSET = -56;
	GYRO_ZOUT_OFFSET = 45;
//	printf("\nGyro X offset sum: %ld Gyro X offset: %d", GYRO_XOUT_OFFSET_1000SUM, GYRO_XOUT_OFFSET);
//	printf("\nGyro Y offset sum: %ld Gyro Y offset: %d", GYRO_YOUT_OFFSET_1000SUM, GYRO_YOUT_OFFSET);
//	printf("\nGyro Z offset sum: %ld Gyro Z offset: %d", GYRO_ZOUT_OFFSET_1000SUM, GYRO_ZOUT_OFFSET);
}
//Function to read the gyroscope rate data and convert it into degrees/s
void Get_Gyro_Rates()
{
	char temp[6];
	
	temp[0]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H);
	temp[1]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L);
	temp[2]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H);
	temp[3]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L);
	temp[4]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H);
	temp[5]=~I2CReadRegister(MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L);
	
	GYRO_XOUT = ((temp[0]<<8)|temp[1]) - GYRO_XOUT_OFFSET;
	GYRO_YOUT = ((temp[2]<<8)|temp[3]) - GYRO_YOUT_OFFSET;
	GYRO_ZOUT = ((temp[4]<<8)|temp[5]) - GYRO_ZOUT_OFFSET;
 
 
	GYRO_XRATE = (float)GYRO_XOUT/gyro_xsensitivity;
	GYRO_YRATE = (float)GYRO_YOUT/gyro_ysensitivity;
	GYRO_ZRATE = (float)GYRO_ZOUT/gyro_zsensitivity;
	
	GYRO_XANGLE += (float)GYRO_XRATE*dt;
	GYRO_YANGLE += (float)GYRO_YRATE*dt;
	GYRO_ZANGLE += (float)GYRO_ZRATE*dt;
}

void RAW_convert(char *MPU_6050_buffer){
	ACCEL_XOUT = (((MPU_6050_buffer[0]<<8) & 0xFF00)| (MPU_6050_buffer[1] & 0x00FF))-(float)Gx_offset*8192;
	ACCEL_YOUT = (((MPU_6050_buffer[2]<<8) & 0xFF00)| (MPU_6050_buffer[3] & 0x00FF))-(float)Gy_offset*8192;
	ACCEL_ZOUT = (((MPU_6050_buffer[4]<<8) & 0xFF00)| (MPU_6050_buffer[5] & 0x00FF))-(float)Gz_offset*8192;
	
	TEMP = (((((signed short)MPU_6050_buffer[6]<<8) & 0xFF00) | ((signed short)MPU_6050_buffer[7] & 0x00FF)));
	TEMP = (TEMP/340)+36.53;	//see datasheet
	
	GYRO_XOUT = ((MPU_6050_buffer[8]<<8)|MPU_6050_buffer[9]) - GYRO_XOUT_OFFSET;
	GYRO_YOUT = ((MPU_6050_buffer[10]<<8)|MPU_6050_buffer[11]) - GYRO_YOUT_OFFSET;
	GYRO_ZOUT = ((MPU_6050_buffer[12]<<8)|MPU_6050_buffer[13]) - GYRO_ZOUT_OFFSET;
	
	GYRO_XRATE = (float)GYRO_XOUT/gyro_xsensitivity;
	GYRO_YRATE = (float)GYRO_YOUT/gyro_ysensitivity;
	GYRO_ZRATE = (float)GYRO_ZOUT/gyro_zsensitivity;
}

void DMAread_MPU6050(){
	char MPU_6050_buffer[14];
	
	DMA_I2CReadMultiRegisters(MPU6050_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,&MPU_6050_buffer[0],14);
	RAW_convert(&MPU_6050_buffer[0]);	
}
void read_MPU6050(){
	char MPU_6050_buffer[14];
	
	I2CReadMultiRegisters(MPU6050_ADDRESS,MPU6050_RA_ACCEL_XOUT_H,&MPU_6050_buffer[0],14);
	RAW_convert(&MPU_6050_buffer[0]);	
}
//Converts the already acquired accelerometer data into 3D euler angles
void Get_Angles()
{
	ACCEL_XANGLE = (float)(57.295*atan((float)-ACCEL_YOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_XOUT,2))))+X_Angle_user/*-XANGLE_OFFSET*/;
	ACCEL_YANGLE = (float)(57.295*atan((float)ACCEL_XOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_YOUT,2))))+Y_Angle_user/*-YANGLE_OFFSET*/;
	
	GYRO_XANGLE += (float)GYRO_XRATE*dt;
	GYRO_YANGLE += (float)GYRO_YRATE*dt;
	GYRO_ZANGLE += (float)GYRO_ZRATE*dt;
}

//Runs a complementary filter configured via float a
void complementary_filter()
{
	COMPLEMENTARY_XANGLE = (COMPLEMENTARY_XANGLE + GYRO_XRATE*dt)*a + ACCEL_XANGLE*(1-a);
	COMPLEMENTARY_YANGLE = (COMPLEMENTARY_YANGLE + GYRO_YRATE*dt)*a + ACCEL_YANGLE*(1-a);		
}	

//Runs 2nd order complementary filter

void second_order_complementary_filter() //http://code.google.com/p/aeroquad/source/browse/trunk/AeroQuad/Filter.pde?r=143
{
	filter_xterm[0] = (ACCEL_XANGLE - COMPLEMENTARY_XANGLE) * timeConstant * timeConstant;
	filter_yterm[0] = (ACCEL_YANGLE - COMPLEMENTARY_YANGLE) * timeConstant * timeConstant;
  	filter_xterm[2] = (dt * filter_xterm[0]) + filter_xterm[2];
  	filter_yterm[2] = (dt * filter_yterm[0]) + filter_yterm[2];
  	filter_xterm[1] = filter_xterm[2] + (ACCEL_XANGLE - COMPLEMENTARY_XANGLE) * 2 * timeConstant + GYRO_XRATE;
  	filter_yterm[1] = filter_yterm[2] + (ACCEL_YANGLE - COMPLEMENTARY_YANGLE) * 2 * timeConstant + GYRO_YRATE;
  	COMPLEMENTARY_XANGLE = (dt * filter_xterm[1]) + COMPLEMENTARY_XANGLE;
  	COMPLEMENTARY_YANGLE = (dt * filter_yterm[1]) + COMPLEMENTARY_YANGLE;
}
void Get_Temp()
{
	char temp[2];
	I2CReadMultiRegisters(MPU6050_ADDRESS, MPU6050_RA_TEMP_OUT_H, &temp[0], 1);
	I2CReadMultiRegisters(MPU6050_ADDRESS, MPU6050_RA_TEMP_OUT_L, &temp[1], 1);
	
	TEMP = (((((signed short)temp[0]<<8) & 0xFF00) | ((signed short)temp[1] & 0x00FF)));
	TEMP = (TEMP/340)+36.53;	//see datasheet
}
void init_MPU6050()
{
	MPU6050_Test_I2C();
	MPU6050_Setup();
	Delay_mS(5000);
	_SLCDModule_TurnOffAllSegments();
	_SLCDModule_PrintString("CAL.",0);
	Calibrate_Gyros();
	Calibrate_Accel();
	if(MPU6050_Check_Registers()){
		_SLCDModule_TurnOffAllSegments();
		_SLCDModule_PrintString("ERR. MPU6050",0);
		while(1)	{}
	}	
}
