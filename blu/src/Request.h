/*
 * Request.h
 *
 *  Created on: 14 Sept 2023
 *      Author: jondurrant
 */

#ifndef HTTPWS_GETTIMELWIPBM_SRC_REQUEST_H_
#define HTTPWS_GETTIMELWIPBM_SRC_REQUEST_H_

#include "pico/stdlib.h"
#include "RequestObserver.h"
#include <map>
#include <cstring>
#include <uri.hh>
#include "core_http_client.h"
#include "TCPTransport.h"

#ifndef REQUEST_BUFFER_SIZE
#define REQUEST_BUFFER_SIZE 256
#endif

#ifndef REQUEST_MAX_PATH
#define REQUEST_MAX_PATH 256
#endif

#define REQUEST_BUF_OVERFLOW -200

#ifdef REQUEST_DEBUG
#endif

class Request {
public:
	Request();
	Request(char * buffer, uint bufLen);
	virtual ~Request();
	void free();

	void setObserver(RequestObserver *obs);
	bool get(const char * url, std::map<std::string, std::string> *query = NULL);
	bool getOld(const char * url, std::map<std::string, std::string> *query = NULL);
	bool post(const char * url,  std::map<std::string, std::string> *query = NULL);

	void setCredentials(char * username, char * password);

	int getStatusCode();
	const char * getHeader();
	int getHeaderLen();
	const char * getPayload();
	int getPayloadLen();
	const char * getUriChar();

private:
	bool doRequest(const char * method, const char * url, const char * payload, uint payloadLen);
	bool b64Encode(const unsigned char *in, size_t inLen, unsigned char *out, size_t outLen);

	void heapStats(const char * prefix);

	void *pAllocBuffer = NULL;
	char * pBuffer = NULL;
	uint xBufferLen = 0;
	RequestObserver *pObserver = NULL;

	uri *pUri = NULL;

	TransportInterface_t xTransportInterface;
	NetworkContext_t xNetworkContext;
	HTTPRequestInfo_t xRequestInfo;
	HTTPResponse_t xResponse;
	HTTPRequestHeaders_t xRequestHeaders;
	HTTPStatus_t xHTTPStatus = HTTPSuccess;

	TCPTransport xSockTrans;
	Transport *pTrans;

	char * pUsername = NULL;
	char * pPassword = NULL;


};

#endif /* HTTPWS_GETTIMELWIPBM_SRC_REQUEST_H_ */
