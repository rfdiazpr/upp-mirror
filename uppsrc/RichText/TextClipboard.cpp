#include "RichText.h"

#include <CtrlLib/CtrlLib.h>

RichText::ClipboardType::ClipboardType()
{
}

RichText::ClipboardType::~ClipboardType()
{
}

int  RichText::ClipboardType::Level()
{
	return 5;
}

GLOBAL_VAR(Array<RichText::ClipboardType *>, sct);

void RichText::Register(RichText::ClipboardType& type)
{
	sct().Add(&type);
}

void RichText::WriteClipboard()
{
	for(int i = 0; i < sct().GetCount(); i++)
		sct()[i]->WriteClipboard(*this);
}

RichText RichText::ReadClipboard(const RichPara::Format& f) {
	for(int l = 0; l <= 10; l++)
		for(int i = 0; i < sct().GetCount(); i++)
			if(sct()[i]->Level() == l) {
				RichText clip = sct()[i]->ReadClipboard(f);
				if(!clip.IsEmpty())
					return clip;
			}
	return RichText();
}
