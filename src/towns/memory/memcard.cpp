#include "memcard.h"
#include "cpputil.h"



bool ICMemoryCard::LoadRawImage(std::string fName)
{
	auto data=cpputil::ReadBinaryFile(fName);
	if(0<data.size())
	{
		std::swap(this->data,data);
		this->fName=fName;
		this->modified=false;
	}
}
void ICMemoryCard::SetFileName(std::string fName)
{
	this->fName=fName;
}
bool ICMemoryCard::SaveRawImage(void) const
{
	if(0<data.size())
	{
		if(true==cpputil::WriteBinaryFile(this->fName,data.size(),data.data()))
		{
			modified=false;
			return true;
		}
	}
	return false;
}
bool ICMemoryCard::SaveRawImageIfModified(void) const
{
	if(true==modified)
	{
		return SaveRawImage();
	}
	return false;
}
