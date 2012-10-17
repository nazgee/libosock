/*
	Copyright 2011, Michal Stawinski <michal.stawinski@gmail.com>

	This file is part of libsockets- C++ wrapper over OpenSSL and raw sockets.

	libsockets is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	libsockets is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with libsockets.  If not, see "../http://www.gnu.org/licenses/>.
*/


#ifndef OSOCKSERVER_H_
#define OSOCKSERVER_H_

#include "../Address/Address.h"
#include "../Security/Auth.h"
#include "../BIO/BIOSocket.h"
#include <boost/utility.hpp>
namespace osock
{
//class SocketServer;
class Server : boost::noncopyable
{
public:
	typedef enum { serviceCallback, serviceProcess, serviceThread } serviceType;
private:
	Auth_p itsAuth;
	BIOSocket_p itsBIO;
	serviceType itsServiceType;
	bool isChild;
	bool isCancelRequested;
	std::string itsPortName;

	void doRunCallback(BIO_p client);
	void doRunProcess(BIO_p client);
	void doRunThread(BIO_p client);
	void serverThread(BIO_p client);

public:
//	Server(SocketServer* socketServer);
	Server(Auth_p auth, std::string portname, serviceType servicetype = serviceCallback);
	virtual ~Server();

	/**
	 * @brief Starts serving clients using Manage() method
	 */
	void Start();
	/**
	 * Called automagically after each client was served using Serve()
	 * @param servedClient Address of served client- can be used for logging
	 * @return determines if server should wait for other clients- this usually
	 * should be set to @true
	 */
	virtual bool onServed(Address& servedClient);
	/**
	 * @brief Implement this function to handle client connections
	 * @param Client Socket of client to be served
	 */
	virtual void Manage(osock::BIO_p bio) = 0;
	/**
	 * @brief Collects return codes of child-processess, to prevent zombies
	 * @param n
	 */
	static void ChildReaper(int n);
	/**
	 * @brief Installs children reaper, that should prevent zombies creation
	 * @param reaper
	 */
	static void InstallChildReaper(void (*reaper)(int) = ChildReaper);

	Auth_p getAuth() const {
		return itsAuth;
	}

	BIOSocket_p getBio() const {
		return itsBIO;
	}

	std::string getPortName() const {
		return itsPortName;
	}

	serviceType getServiceType() const {
		return itsServiceType;
	}
private:
//	SocketServer* itsSocketServer;
};

}

#endif /* SERVER_H_ */
