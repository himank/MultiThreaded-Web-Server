/*
 * senddata.h
 *
 *  Created on: 24-Sep-2012
 *      Author: himank
 */

#include <string>
#ifndef SENDDATA_H_
#define SENDDATA_H_

#include <fstream>
#include <sys/types.h>

using namespace std;


class SendData
{
	public:
		//ifstream::pos_type size;
		//SendData();
		void sendData(clientInfo );
		void fileRequestedData(clientInfo );
		void requestPrint(clientInfo );
		void generatingLog(clientInfo );
		void listingDir(clientInfo );
		void displaylog(clientInfo c);
		//bool sortDirectory(const string& str1, const string& str2);

		//void sendDataHTML(clientInfo);


		//static void request_helper();


};


#endif /* SCHEDULING_H_ */
