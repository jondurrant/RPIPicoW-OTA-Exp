/*
 * Transport.cpp
 *
 *  Created on: 19 Mar 2023
 *      Author: jondurrant
 */

#include "Transport.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>

#include <stdio.h>
#define DEBUG_LINE 25

Transport::Transport() {
}

Transport::~Transport() {
}



/***
 * Print the buffer in hex and plain text for debugging
 */
void Transport::debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes){
	size_t count =0;
	size_t lineEnd=0;
	const uint8_t *pBuf = (uint8_t *)pBuffer;

	printf("DEBUG: %s of size %d\n", title, bytes);

	while (count < bytes){
		lineEnd = count + DEBUG_LINE;
		if (lineEnd > bytes){
			lineEnd = bytes;
		}

		//Print HEX DUMP
		for (size_t i=count; i < lineEnd; i++){
			if (pBuf[i] <= 0x0F){
				printf("0%X ", pBuf[i]);
			} else {
				printf("%X ", pBuf[i]);
			}
		}

		//Pad for short lines
		size_t pad = (DEBUG_LINE - (lineEnd - count)) * 3;
		for (size_t i=0; i < pad; i++){
			printf(" ");
		}

		//Print Plain Text
		for (size_t i=count; i < lineEnd; i++){
			if ((pBuf[i] >= 0x20) && (pBuf[i] <= 0x7e)){
				printf("%c", pBuf[i]);
			} else {
				printf(".");
			}
		}

		printf("\n");

		count = lineEnd;

	}
}

/***
 * Callback C function to support the NetworkContext on coreHTTP
 * @param pNetworkContext
 * @param pBuffer
 * @param bytesToRecv
 * @return
 */
int32_t Transport:: staticTransRead(
			NetworkContext_t * pNetworkContext,
         void * pBuffer,
         size_t bytesToRecv ){
	Transport *t = (Transport *) pNetworkContext->pTransport;
	return t->transRead(pBuffer,  bytesToRecv);
}

/***
* Callback C function to support the NetworkContext on coreHTTP
* @param pNetworkContext
* @param pBuffer
* @param bytesToSend
* @return
*/
int32_t Transport::staticTransSend(
	NetworkContext_t * pNetworkContext,
	const void * pBuffer,
	size_t bytesToSend ){
	Transport *t = (Transport *) pNetworkContext->pTransport;
	return t->transSend(pBuffer,  bytesToSend);
}

