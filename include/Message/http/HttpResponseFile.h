/*
 * HttpResponseFile.h
 *
 *  Created on: Jun 5, 2011
 *      Author: nazgee
 */

#ifndef HTTPRESPONSEFILE_H_
#define HTTPRESPONSEFILE_H_

#include <defines.h>
//#define LOGLEVEL LOGLEVEL_DBG
#include "../../Utilities/Logger.h"
#include "../../Message/http/HttpResponse.h"

#include <fstream>

namespace osock
{

class HttpResponseFile: public osock::HttpResponse
{
protected:
	std::string itsFileName;
private:
	std::ifstream itsIFile;
	void InitContent(std::string filename);

public:
	HttpResponseFile(std::string file, std::string code = "200",
			std::string status = "OK", std::string protocole = "HTTP/1.0");
	HttpResponseFile(std::string file, const http::Status& status, std::string protocole = "HTTP/1.0");
	HttpResponseFile(const HttpResponseFile& copy_from_me);
	virtual ~HttpResponseFile();
protected:
	virtual HttpResponseFile* doClone() const;
};

}

#endif /* HTTPRESPONSEFILE_H_ */
