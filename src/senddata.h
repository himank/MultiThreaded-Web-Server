// Copyright 2022 @HimankChaudhary
//
// Author      : Himank Chaudhary
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
