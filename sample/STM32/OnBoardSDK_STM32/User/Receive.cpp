/*! @file Receive.cpp
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

#include "stm32f4xx.h"
#include "Receive.h"
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
//#include <algorithm>

uint8_t ci = 0,li=0;
bool equal = false;

using namespace DJI::OSDK;

/*
 * @brief Helper function to assemble two bytes into a float number
 */
static float32_t
hex2Float(uint8_t HighByte, uint8_t LowByte)
{
  float32_t high = (float32_t)(HighByte & 0x7f);
  float32_t low  = (float32_t)LowByte;
  if (HighByte & 0x80) // MSB is 1 means a negative number
  {
    return -(high * 256.0f + low) / 100.0f;
  }
  else
  {
    return (high * 256.0f + low) / 100.0f;
  }
}

TerminalCommand myTerminal;

uint8_t* TerminalCommand::terminalCommandHandler()
{
  if (cmdReadyFlag == 1)
  {
    cmdReadyFlag = 0;
		readyf = true;
		return myTerminal.cmdIn;
  }
	if (waypoint_ready == true)
  {
    waypoint_ready = false;
		readyf = true;
		return myTerminal.WP_List;
  }
  else
  { // No full command has been received yet.
  }
}
bool TerminalCommand::ready_status()
{
	
	if ((readyf == true))
	{
		return false;
	}
	return true;
	
}

uint8_t wp_num_r()
{
	return myTerminal.wp_num;
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
void
USART2_IRQHandler(void)
{
  if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)
  {
    uint8_t oneByte = USART_ReceiveData(USART2);
    if (myTerminal.rxIndex == 0)
    {
      if (oneByte == 0xDE)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 1;
      }
			else if (oneByte == 0xAA)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 1;
      }
      else
      {
        ;
      }
    }
		else if (myTerminal.rxIndex == 1)
    {
      if (oneByte == 0xAD)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 2;
      }
			else if (oneByte == 0xBB)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 2;
      }
      else
      {
        ;
      }
    }
		else if (myTerminal.rxIndex == 2)
    {
      if (oneByte == 0xBE)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 3;
      }
			else if (oneByte == 0xCC)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 3;
      }
      else
      {
        ;
      }
    }
		else if (myTerminal.rxIndex == 3)
    {
      if (oneByte == 0xEF)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 4;
      }
			else if (oneByte == 0xDD)
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex                   = 4;
				myTerminal.waypoint						 = true;
      }
      else
      {
        ;
      }
    }
		else if ((myTerminal.rxIndex == 4)&&(myTerminal.waypoint == true))
    {
      myTerminal.wp_num = oneByte;
    }
    else
    {
      if ((myTerminal.rxIndex == MAX_RECEIVE-1)&&(myTerminal.waypoint == false)) 
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxLength                  = myTerminal.rxIndex + 1;
        myTerminal.rxIndex                   = 0;
        myTerminal.cmdReadyFlag              = 1;
				
      }
			else if ((myTerminal.rxIndex == MAX_WP-1)&&(myTerminal.waypoint == true)) 
      {
        myTerminal.WP_List[myTerminal.rxIndex] = oneByte;
        myTerminal.rxLength                  = myTerminal.rxIndex + 1;
        myTerminal.rxIndex                   = 0;
        myTerminal.waypoint_ready            = true;
				
      }
			else if (((myTerminal.rxIndex > MAX_WP-1)&&(myTerminal.waypoint == true))||((myTerminal.rxIndex > MAX_RECEIVE-1)&&(myTerminal.waypoint == false)))
			{
				;
			}				
      else if(myTerminal.waypoint == true)
      {
        myTerminal.WP_List[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex++;
      }
			else
      {
        myTerminal.cmdIn[myTerminal.rxIndex] = oneByte;
        myTerminal.rxIndex++;
      }
    }
  }
}
#ifdef __cplusplus
}
#endif //__cplusplus
