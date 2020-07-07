/*
 *    STM32CubeIDE_file/BMX055.c
 *    
 *    Copyright(C) 2020 RT Corporation <support@rt-net.jp>
 *      All rights reserved.
 *      
 *    License: Apache License, Version 2.0
 *     https://www.apache.org/licenses/LICENSE-2.0
 *     
 */

//I2C DMA

#include "main.h"
#include "BMX055.h"

extern I2C_HandleTypeDef hi2c1;

static uint8_t BMX055_write_buffer[10];
static uint8_t BMX055_read_buffer[10];

void I2C1_Read_NBytes(uint8_t IC_Addr,uint8_t number_Bytes){
	HAL_I2C_Master_Receive_DMA(&hi2c1,(IC_Addr<<1),BMX055_read_buffer,number_Bytes);

}

static void I2C1_Write_NBytes(uint8_t IC_Addr,uint8_t number_Bytes){
	HAL_I2C_Master_Transmit_DMA(&hi2c1,(IC_Addr<<1),BMX055_write_buffer,number_Bytes);
}

short I2C1_BMX055_GetGyro16(void){
	return (((unsigned short)BMX055_read_buffer[1]<<8)&0xFF00) | BMX055_read_buffer[0];
}

void I2C1_BMX055_zGyro(void){
	BMX055_write_buffer[0]=BMX055_GYRO_RATE_Z_LSB;
	I2C1_Write_NBytes(BMX055_Addr_Gyro,1);
}


void I2C1_BMX055_Init(void){
	BMX055_write_buffer[0]=BMX055_GYRO_RANGE;//レジスタアドレス
	BMX055_write_buffer[1]=0x00;//+/-2000 dps
	I2C1_Write_NBytes(BMX055_Addr_Gyro,2);
	HAL_Delay(2);

	BMX055_write_buffer[0]=BMX055_GYRO_BANDWIDTH;//レジスタアドレス
	BMX055_write_buffer[1]=0x01;//ODR 2000Hz
	I2C1_Write_NBytes(BMX055_Addr_Gyro,2);
	HAL_Delay(2);

	BMX055_write_buffer[0]=BMX055_GYRO_LPM1;//レジスタアドレス
	BMX055_write_buffer[1]=0x00;//Normal mode,Sleep duration = 2ms
	I2C1_Write_NBytes(BMX055_Addr_Gyro,2);
	HAL_Delay(2);
}
