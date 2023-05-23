/*
 * DHT.h
 *
 *  Created on: May 20, 2023
 *      Author: Teerapat
 */

#ifndef INC_DHT_H_
#define INC_DHT_H_


typedef struct
{
	float Temperature;
	float Humidity;
}DHT_DataTypedef;


void DHT_GetData (DHT_DataTypedef *DHT_Data);


#endif /* INC_DHT_H_ */
