/*! @file Receive.h
 *
 *  @version 3.3
 *  @date Jun 2017
 *
 *  @brief
 *  This function parses Rx buffer and execute commands sent from computer.
 *
 *  @Copyright (c) 2016-2017 DJI
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef RECEIVE_H
#define RECEIVE_H
#include "main.h"
#include "dji_mission_type.hpp"

#define MAX_RECEIVE 45
#define MAX_WP			210


	typedef struct {
  uint16_t trackingCol;
  uint16_t trackingRow;
  uint16_t sceneCol;
  uint16_t sceneRow;
  uint16_t offsetCol;
  uint16_t offsetRow;
  uint8_t trackingConfidence;
  uint8_t sceneConfidence;
  uint16_t rotation;
  uint8_t cameraIndex;
  uint8_t reserved;
  uint8_t sceneColFrac8;
  uint8_t sceneRowFrac8;
  uint16_t sceneAngle7;
  uint16_t sceneScale8;
  uint64_t timeStamp;
  uint32_t frameId;
} SLATrackingPosition_t;
	
class TerminalCommand
{
public:
  uint32_t cmdReadyFlag;       // Rx_Handle_Flag
  uint8_t  cmdIn[MAX_RECEIVE]; // Rx_buff
	uint8_t  WP_List[MAX_WP];
  int32_t  rxIndex;            // Rx_adr
  int32_t  rxLength;           // Rx_length
	bool     readyf;						 // Movement Command Ready Flag
	bool     waypoint;		 			 // Waypoint start 
	bool     waypoint_ready;		 // Waypoint Ready Flag
	uint8_t  wp_num;						 // Number of waypoints	
	SLATrackingPosition_t tracker;

	
  uint8_t* terminalCommandHandler();
	bool ready_status();
	uint8_t wp_num_r();
  TerminalCommand()
    : cmdReadyFlag(0)
    , rxIndex(0)
    , rxLength(0)
		, readyf(false)
		, tracker()
		, waypoint_ready(false)
		, wp_num(0)
  {
    ;
  }
};


#endif // RECEIVE_H
