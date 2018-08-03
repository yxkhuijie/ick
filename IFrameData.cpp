#include "stdafx.h"
#include "IFrameData.h"
#ifdef __linux__
#include <string.h>
#endif

IFrameData::IFrameData()
{
	m_pRecvFrame = NULL;
	m_pSendFrame = NULL;
	m_recvFramelength = 0;
	m_sendFrameLength = 0;
	m_frameDescription = "";
}


IFrameData::~IFrameData()
{
	if (m_pRecvFrame != NULL)
	{
		delete[] m_pRecvFrame;
	}

	if (m_pSendFrame != NULL)
	{
		delete[] m_pSendFrame;
	}
}

void IFrameData::setRecvFrame(const unsigned char* frame, int length)
{
	if (frame == NULL) throw "frame null error!";

	if (m_pRecvFrame != NULL)
	{
		delete[] m_pRecvFrame;
	}

	m_pRecvFrame = new unsigned char[length];
	memcpy(m_pRecvFrame, frame, length);
	this->m_recvFramelength = length;
}

void IFrameData::setSendFrame(const unsigned char* frame, int length)
{
	if (frame == NULL || length == 0)
	{
		if (m_pSendFrame != NULL)
		{
			delete[] m_pSendFrame;
		}

		m_pSendFrame = NULL;
		return;
	}

	if (m_pSendFrame != NULL)
	{
		delete[] m_pSendFrame;
	}

	m_pSendFrame = new unsigned char[length];
	memcpy(m_pSendFrame, frame, length);
	this->m_sendFrameLength = length;
}

unsigned char* IFrameData::getRecvFrame()
{
	return this->m_pRecvFrame;
}

unsigned char* IFrameData::getSendFrame()
{
	return this->m_pSendFrame;
}

int IFrameData::getRecvFrameLength()
{
	return this->m_recvFramelength;
}

int IFrameData::getSendFrameLength()
{
	return this->m_sendFrameLength;
}

void IFrameData::onFrameReceived()
{

}

bool IFrameData::verifyFrameFormat(const unsigned char* frame, int length)
{
	return true;
}

bool IFrameData::analysisFrame(const unsigned char* frame, int length)
{
	return true;
}

bool IFrameData::verifyFrameNext(const unsigned char* frame, int length)
{
	return false;
}

void IFrameData::print(bool hex)
{

}
std::string IFrameData::getErrorMsg()
{
    return this->m_frameErrorMsg;
}

std::string IFrameData::getDescription()
{
	return this->m_frameDescription;
}


