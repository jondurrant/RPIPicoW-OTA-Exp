/*
 * RequestObserver.h
 *
 *  Created on: 14 Sept 2023
 *      Author: jondurrant
 */

#ifndef HTTPWS_GETTIMELWIPBM_SRC_REQUESTOBSERVER_H_
#define HTTPWS_GETTIMELWIPBM_SRC_REQUESTOBSERVER_H_

class Request;

class RequestObserver {
public:
	RequestObserver();
	virtual ~RequestObserver();

	virtual void requestComplete(Request *req);
};

#endif /* HTTPWS_GETTIMELWIPBM_SRC_REQUESTOBSERVER_H_ */
