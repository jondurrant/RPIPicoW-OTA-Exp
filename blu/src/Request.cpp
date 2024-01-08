/*
 * Request.cpp
 *
 *  Created on: 14 Sept 2023
 *      Author: jondurrant
 */

#include "Request.h"
#include "json-maker/json-maker.h"
#include "FreeRTOS.h"
#include "task.h"


Request::Request() {
	pAllocBuffer = pvPortMalloc(REQUEST_BUFFER_SIZE);
	pBuffer = (char *) pAllocBuffer;
	xBufferLen = REQUEST_BUFFER_SIZE;
	pTrans = &xSockTrans;
}

Request::Request(char * buffer, uint bufLen) {
	pAllocBuffer = NULL;
	pBuffer = (char *) buffer;
	xBufferLen = bufLen;
	pTrans = &xSockTrans;
}

Request::~Request() {
	if (pAllocBuffer != NULL){
		vPortFree(pAllocBuffer);
	}
	if (pUri != NULL){
		delete(pUri);
	}
}

void Request::free(){
	if (pUri != NULL){
		delete(pUri);
		pUri = NULL;
	}
}

void Request::setObserver(RequestObserver *obs){
	pObserver = obs;
}

bool Request::getOld(const char * url, std::map<std::string, std::string> *query){
	char payload[] = "";
	char path[REQUEST_MAX_PATH];
	pUri = new uri (url);

	if (pUri->get_scheme().compare("http") != 0 ){
		printf("Schema not implemented %s\n",
				pUri->get_scheme().c_str()
				);
		return false;
	}

	int serverPort = pUri->get_port();
	if (serverPort == 0){
		serverPort = 80;
	}


	if (!pTrans->transConnect(pUri->get_host().c_str(), serverPort)){
		printf("Socket Connect Failed\r\n");
		return false;
	}

	xNetworkContext.pTransport = pTrans;
	xTransportInterface.pNetworkContext = &xNetworkContext;
	xTransportInterface.send = Transport::staticTransSend;
	xTransportInterface.recv = Transport::staticTransRead;

	  /* Initialize all HTTP Client library API structs to 0. */
	( void ) memset( &xRequestInfo, 0, sizeof( xRequestInfo ) );
	( void ) memset( &xResponse, 0, sizeof( xResponse ) );
	( void ) memset( &xRequestHeaders, 0, sizeof( xRequestHeaders ) );

	/* Initialize the request object. */
	xRequestInfo.pHost = pUri->get_host().c_str();
	xRequestInfo.hostLen = pUri->get_host().length();
	xRequestInfo.pMethod = "GET";
	xRequestInfo.methodLen = 3;
	xRequestInfo.pPath = pUri->get_path().c_str();
	xRequestInfo.pathLen = pUri->get_path().length();
	xRequestInfo.reqFlags = false;

	/* Set the buffer used for storing request headers. */
	xRequestHeaders.pBuffer = (unsigned char *)pBuffer;
	xRequestHeaders.bufferLen = xBufferLen;

	//Handle GET Query construction
	if (query != NULL){
		strcpy(path, xRequestInfo.pPath);
		int length=strlen(path);
		path[length++] = '?';

		std::map<std::string, std::string>::iterator it = query->begin();
		while (it != query->end()){
			int newLen = length + it->first.length() + it->second.length() + 2;
			if (newLen > REQUEST_MAX_PATH){
#ifdef REQUEST_DEBUG
				printf("HTTP Request path exceeded building query\n");
#endif
				return false;
			}
			strcpy(&path[length],  it->first.c_str());
			length +=  it->first.length();
			path[length++] = '=';
			strcpy(&path[length],  it->second.c_str());
			length +=  it->second.length();

			it++;
			if ( it != query->end()){
				path[length++] = '&';
			}
		}
		xRequestInfo.pPath = path;
		xRequestInfo.pathLen = length;

	}

	xHTTPStatus = HTTPClient_InitializeRequestHeaders( &xRequestHeaders,
													   &xRequestInfo );

	if( xHTTPStatus == HTTPSuccess ) {
			/* Initialize the response object. The same buffer used for storing
			 * request headers is reused here. */
			xResponse.pBuffer = (unsigned char *)pBuffer;
			xResponse.bufferLen = xBufferLen;

			/* Send the request and receive the response. */
			xHTTPStatus = HTTPClient_Send( &xTransportInterface,
										   &xRequestHeaders,
										   ( uint8_t * ) payload,
										   0,
										   &xResponse,
										   0 );
		} else {
			printf ( "Failed to initialize HTTP request headers: Error=%s.",
						HTTPClient_strerror( xHTTPStatus ) );
		}


	if (pObserver != NULL){
		pObserver->requestComplete(this);
	}


	pTrans->transClose();


	if (xHTTPStatus == HTTPSuccess){
		return true;
	}
	return false;
}

bool Request::get(const char * url, std::map<std::string, std::string> *query){
	char path[REQUEST_MAX_PATH];
	char payload[]="";
	bool res;

	if (query != NULL){
			strcpy(path,  url);
			int length=strlen(path);
			path[length++] = '?';

			std::map<std::string, std::string>::iterator it = query->begin();
			while (it != query->end()){
				int newLen = length + it->first.length() + it->second.length() + 2;
				if (newLen > REQUEST_MAX_PATH){
	#ifdef REQUEST_DEBUG
					printf("HTTP Request path exceeded building query\n");
	#endif
					return false;
				}
				strcpy(&path[length],  it->first.c_str());
				length +=  it->first.length();
				path[length++] = '=';
				strcpy(&path[length],  it->second.c_str());
				length +=  it->second.length();

				it++;
				if ( it != query->end()){
					path[length++] = '&';
					path[length++] = 0;
				}
			}

			res = doRequest("GET", path, payload, 0);
			return res;
		}

	res = doRequest("GET", url, payload, 0);
	return res;

}


bool Request::doRequest(const char * method, const char * url, const char * payload, uint payloadLen){
	char path[REQUEST_MAX_PATH];
	char authPlain[REQUEST_MAX_PATH];
	char auth64[REQUEST_MAX_PATH];

	free();
	pUri = new uri (url);

	if (pUri->get_scheme().compare("http") != 0 ){
		printf("Schema not implemented %s\n",
				pUri->get_scheme().c_str()
				);
		return false;
	}

	int serverPort = pUri->get_port();
	if (serverPort == 0){
		serverPort = 80;
	}


	if (!pTrans->transConnect(pUri->get_host().c_str(), serverPort)){
		printf("Socket Connect Failed\r\n");
		return false;
	}

	xNetworkContext.pTransport = pTrans;
	xTransportInterface.pNetworkContext = &xNetworkContext;
	xTransportInterface.send = Transport::staticTransSend;
	xTransportInterface.recv = Transport::staticTransRead;

	  /* Initialize all HTTP Client library API structs to 0. */
	( void ) memset( &xRequestInfo, 0, sizeof( xRequestInfo ) );
	( void ) memset( &xResponse, 0, sizeof( xResponse ) );
	( void ) memset( &xRequestHeaders, 0, sizeof( xRequestHeaders ) );


	if (pUri->get_query().length() == 0){
		sprintf(path, "/%s", pUri->get_path().c_str() );
	} else {
		sprintf(path, "/%s?%s", pUri->get_path().c_str(), pUri->get_query().c_str() );
	}

	/* Initialize the request object. */
	xRequestInfo.pHost = pUri->get_host().c_str();
	xRequestInfo.hostLen = pUri->get_host().length();
	xRequestInfo.pMethod =  method;
	xRequestInfo.methodLen = strlen(method);
	xRequestInfo.pPath = path;
	xRequestInfo.pathLen = strlen(path);
	xRequestInfo.reqFlags = false;

	/* Set the buffer used for storing request headers. */
	xRequestHeaders.pBuffer = (unsigned char *)pBuffer;
	xRequestHeaders.bufferLen = xBufferLen;


	xHTTPStatus = HTTPClient_InitializeRequestHeaders( &xRequestHeaders,
													   &xRequestInfo );

	if (xHTTPStatus == HTTPSuccess){
		if (payloadLen > 0){
			xHTTPStatus = HTTPClient_AddHeader(
					&xRequestHeaders,
					"Content-Type",
					strlen("Content-Type") ,
					"application/json",
					strlen("application/json")
				   );
		}
	}

	if (xHTTPStatus == HTTPSuccess){

		if ((pUsername != NULL)  && (pPassword != NULL)){
			sprintf(authPlain, "%s:%s", pUsername, pPassword);
			b64Encode((unsigned char *)authPlain, strlen(authPlain), (unsigned char *)auth64, REQUEST_MAX_PATH);
			sprintf(path, "Basic %s", auth64 );

			xHTTPStatus = HTTPClient_AddHeader(
						&xRequestHeaders,
						"Authorization",
						strlen("Authorization") ,
						path,
						strlen(path)
					   );

		} else if (pUri->get_username().length() > 0){
			sprintf(authPlain, "%s:%s", pUri->get_username().c_str(), pUri->get_password().c_str());
			b64Encode((unsigned char *)authPlain, strlen(authPlain), (unsigned char *)auth64, REQUEST_MAX_PATH);
			sprintf(path, "Basic %s", auth64 );

			xHTTPStatus = HTTPClient_AddHeader(
						&xRequestHeaders,
						"Authorization",
						strlen("Authorization") ,
						path,
						strlen(path)
					   );
		}
	}

	if( xHTTPStatus == HTTPSuccess ) {
			/* Initialize the response object. The same buffer used for storing
			 * request headers is reused here. */
			xResponse.pBuffer = (unsigned char *)pBuffer;
			xResponse.bufferLen = xBufferLen;

#ifdef REQUEST_DEBUG
			if (payloadLen != 0){
				printf("Method %.*s Host %.*s Path %.*s Payload %.*s\n",
						xRequestInfo.methodLen,
						xRequestInfo.pMethod,
						xRequestInfo.hostLen,
						xRequestInfo.pHost,
						xRequestInfo.pathLen ,
						xRequestInfo.pPath,
						payloadLen,
						payload
						);
				 printf( "Request Headers:\n%.*s\n",
				                   ( int32_t ) xRequestHeaders.headersLen,
				                   ( char * ) xRequestHeaders.pBuffer  );
			} else {
				printf("Method %.*s Host %.*s Path %.*s \n",
						xRequestInfo.methodLen,
						xRequestInfo.pMethod,
						xRequestInfo.hostLen,
						xRequestInfo.pHost,
						xRequestInfo.pathLen ,
						xRequestInfo.pPath
						);
			}
#endif

			/* Send the request and receive the response. */
			xHTTPStatus = HTTPClient_Send( &xTransportInterface,
										   &xRequestHeaders,
										   ( uint8_t * ) payload,
										  payloadLen,
										   &xResponse,
										   0 );
		} else {
			printf ( "Failed to initialize HTTP request headers: Error=%s.",
						HTTPClient_strerror( xHTTPStatus ) );
		}


	if (pObserver != NULL){
		pObserver->requestComplete(this);
	}


	pTrans->transClose();

	if (xHTTPStatus == HTTPSuccess){
		return true;
	}
	return false;

}



bool Request::post(const char * url,  std::map<std::string, std::string> *query){
	char payload[REQUEST_MAX_PATH] = "";
	int length=0;
	bool res;


	if (query != NULL){

			size_t remLen = REQUEST_MAX_PATH;
		   char* s = json_objOpen( payload,  NULL,  &remLen );

			std::map<std::string, std::string>::iterator it = query->begin();
			while (it != query->end()){

				s = json_str( s,  it->first.c_str(), it->second.c_str(), &remLen );

				it++;
			}
			s = json_objClose( s, &remLen );
			s = json_end( s, &remLen );

#ifdef REQUEST_DEBUG
			printf("Payload %s\n", payload);
#endif
			length = strlen(payload);
	}

	res = doRequest("POST", url, payload, length);
	return res;
}

int Request::getStatusCode(){
	return xResponse.statusCode;
}

const char * Request::getHeader(){
	return (const char *)xResponse.pHeaders;
}

int Request::getHeaderLen(){
	return xResponse.headersLen;
}


const char * Request::getPayload(){
	return (const char *)xResponse.pBody;
}

int  Request::getPayloadLen(){
	return xResponse.bodyLen;
}


const char * Request::getUriChar(){
	return pUri->to_string().c_str();
}


bool Request::b64Encode(const unsigned char *in, size_t inLen, unsigned char *out, size_t outLen){
	char b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	size_t  i;
	size_t  j;
	size_t  v;

	if (in == NULL || inLen == 0)
		return false;

	memset(out, 0, outLen);

	for (i=0, j=0; i<inLen; i+=3, j+=4) {
		if ( (j+3) >= outLen){
			return false;
		}

		v = in[i];
		v = i+1 < inLen ? v << 8 | in[i+1] : v << 8;
		v = i+2 < inLen ? v << 8 | in[i+2] : v << 8;

		out[j]   = b64chars[(v >> 18) & 0x3F];
		out[j+1] = b64chars[(v >> 12) & 0x3F];
		if (i+1 < inLen) {
			out[j+2] = b64chars[(v >> 6) & 0x3F];
		} else {
			out[j+2] = '=';
		}
		if (i+2 < inLen) {
			out[j+3] = b64chars[v & 0x3F];
		} else {
			out[j+3] = '=';
		}
	}

	return true;
}


void Request::setCredentials(char * username, char * password){
	pUsername =  username;
	pPassword = password;
}

void Request::heapStats(const char * prefix){
	HeapStats_t heapStats;

	vPortGetHeapStats(&heapStats);
	printf("%s HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
			prefix,
		   heapStats.xAvailableHeapSpaceInBytes,
		   heapStats.xNumberOfFreeBlocks,
		   heapStats.xNumberOfSuccessfulAllocations,
		   heapStats.xNumberOfSuccessfulFrees
	);
}
