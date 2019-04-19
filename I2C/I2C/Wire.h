/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include "stddef.h"

#define BUFFER_LENGTH 32
#define false 0
#define true 1
// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

uint8_t rxBuffer[];
uint8_t rxBufferIndex;
uint8_t rxBufferLength;
uint8_t txAddress;
uint8_t txBuffer[];
uint8_t txBufferIndex;
uint8_t txBufferLength;
uint8_t transmitting;

void (*user_onRequest)(void);
void (*user_onReceive)(int);
void onRequestService(void);
void onReceiveService(uint8_t*, int);
void TwoWire();
void beginAsMaster();
void beginAsSlave(uint8_t);
void end();
void setClock(uint32_t);
void beginTransmission(uint8_t);
uint8_t endTransmission(uint8_t);
uint8_t endTransmissionThenStop(void);
uint8_t requestFrom(uint8_t, uint8_t);
uint8_t requestFromComplete(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
size_t write(uint8_t);
size_t writeN(const uint8_t *, size_t);
int available(void);
int read(void);
int peek(void);
void flush(void);
void onReceive( void (*)(int) );
void onRequest( void (*)(void) );

 size_t writeUL(unsigned long n) ;
 size_t writeL(long n) ;
 size_t writeU(unsigned int n) ;
#endif

