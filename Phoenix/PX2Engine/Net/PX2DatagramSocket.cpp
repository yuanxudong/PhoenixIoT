// PX2DatagramSocket.cpp

#include "PX2DatagramSocket.hpp"
#include "PX2DatagramSocketImpl.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DatagramSocket::DatagramSocket() : 
Socket(new0 DatagramSocketImpl)
{
}
//----------------------------------------------------------------------------
DatagramSocket::DatagramSocket(IPAddress::Family family) : 
Socket(new0 DatagramSocketImpl(family))
{
}
//----------------------------------------------------------------------------
DatagramSocket::DatagramSocket(const SocketAddress& address, 
	bool reuseAddress) :
	Socket(new0 DatagramSocketImpl(address.GetFamily()))
{
	Bind(address, reuseAddress);
}
//----------------------------------------------------------------------------
DatagramSocket::DatagramSocket(const Socket& socket) :
Socket(socket)
{
	if (!dynamic_cast<DatagramSocketImpl*>(GetImpl()))
	{
		assertion(false, "");
	}
}
//----------------------------------------------------------------------------
DatagramSocket::DatagramSocket(SocketImpl* pImpl) :
Socket(pImpl)
{
	if (!dynamic_cast<DatagramSocketImpl*>(GetImpl()))
	{
		assertion(false, "");
	}
}
//----------------------------------------------------------------------------
DatagramSocket::~DatagramSocket()
{
}
//----------------------------------------------------------------------------
DatagramSocket& DatagramSocket::operator = (const Socket& socket)
{
	if (dynamic_cast<DatagramSocketImpl*>(socket.GetImpl()))
		Socket::operator = (socket);
	else
	{
		assertion(false, "");
	}

	return *this;
}
//----------------------------------------------------------------------------
void DatagramSocket::ConnectB(const SocketAddress& address)
{
	GetImpl()->ConnectB(address);
}
//----------------------------------------------------------------------------
void DatagramSocket::Bind(const SocketAddress& address, bool reuseAddress)
{
	GetImpl()->Bind(address, reuseAddress);
}
//----------------------------------------------------------------------------
int DatagramSocket::SendBytes(const void* buffer, int length, int flags)
{
	return GetImpl()->SendBytes(buffer, length, flags);
}
//----------------------------------------------------------------------------
int DatagramSocket::ReceiveBytes(void* buffer, int length, int flags)
{
	return GetImpl()->ReceiveBytes(buffer, length, flags);
}
//----------------------------------------------------------------------------
int DatagramSocket::SendTo(const void* buffer, int length, 
	const SocketAddress& address, int flags)
{
	return GetImpl()->SendTo(buffer, length, address, flags);
}
//----------------------------------------------------------------------------
int DatagramSocket::ReceiveFrom(void* buffer, int length,
	SocketAddress& address, int flags)
{
	return GetImpl()->ReceiveFrom(buffer, length, address, flags);
}
//----------------------------------------------------------------------------
void DatagramSocket::SetBroadcast(bool flag)
{
	GetImpl()->SetBroadcast(flag);
}
//----------------------------------------------------------------------------
bool DatagramSocket::IsBroadcast() const
{
	return GetImpl()->GetBroadcast();
}
//----------------------------------------------------------------------------