#include "Debuggers.h"

#ifdef COMPILER_MSC

#define LLOG(x) // LOG(x)

int Pdb::Disassemble(dword ip)
{
	char out[256];
	byte code[32];
	memset(code, 0, 32);
	int i;
	for(i = 0; i < 32; i++) {
		int q = Byte(i + ip);
		if(q < 0)
			break;
		code[i] = q;
	}
	int sz = NDisassemble(out, code, ip);
	if(sz > i)
		return -1;
	disas.Add(ip, out, Null);
	return sz;
}

void Pdb::Reg(Label& reg, dword val)
{
	String h = Sprintf("%08X", val);
	if(h != reg.GetText()) {
		reg.SetLabel(h);
		reg.SetInk(SLtRed);
	}
	else
		reg.SetInk(SBlack);
}

bool Pdb::IsValidFrame(dword eip)
{
	for(int i = 0; i < module.GetCount(); i++) {
		const ModuleInfo& f = module[i];
		if(eip >= f.base && eip < f.base + f.size)
			return true;
	}
	return false;
}

void Pdb::Sync0()
{
	stop = false;
	dword eip = context.Eip;
	dword ebp = context.Ebp;
	dword spmax = threadsp.Get(event.dwThreadId, 0);
	framelist.Clear();
	frame.Clear();
	int ndx = 0;
	FnInfo fn = GetFnInfo(eip);
	for(;;) {
		Frame& f = frame.Add();
		f.eip = eip;
		f.ebp = ebp;
		f.fn = fn;
		String r;
		if(IsNull(fn.name)) {
			r = Sprintf("0x%08x", f.eip);
			for(int i = 0; i < module.GetCount(); i++) {
				const ModuleInfo& f = module[i];
				if(eip >= f.base && eip < f.base + f.size) {
					r << " (" << GetFileName(f.path) << ")";
					break;
				}
			}
		}
		else {
			GetLocals(eip, ebp, f.param, f.local);
			r = fn.name;
			r << '(';
			for(int i = 0; i < f.param.GetCount(); i++) {
				if(i)
					r << ", ";
				r << f.param.GetKey(i) << "=" << Visualise(f.param[i], 30).GetString();
			}
			r << ')';
		}
		framelist.Add(frame.GetCount() - 1, r);
		int q = 0;
		for(;;) {
			if(ebp > spmax || ++q > 1024 * 64)
				return;
			dword neip, nebp;
			if(!Copy(ebp, &nebp, 4))
				return;
			if(!Copy(ebp + 4, &neip, 4))
				return;
			if(nebp >= ebp && nebp < spmax && IsValidFrame(neip)) {
				fn = GetFnInfo(neip);
				if(!IsNull(fn.name)) {
					eip = neip;
					ebp = nebp;
					break;
				}
			}
			ebp += 4;
		}
	}
}

void Pdb::Sync()
{
	Sync0();
	framelist <<= 0;
	SetFrame();
	IdeActivateBottom();
}

void Pdb::SetFrame()
{
	int q = ~framelist;
	if(q >= 0 && q < frame.GetCount()) {
		Frame& f = frame[q];
		Image ptrimg = q == 0 ? DbgImg::IpLinePtr() : DbgImg::FrameLinePtr();
		bool df = disas.HasFocus();
		FilePos fp = GetFilePos(f.eip);
		IdeHidePtr();
		autotext.Clear();
		if(fp) {
			IdeSetDebugPos(fp.path, fp.line, ptrimg, 0);
			autotext = IdeGetLine(fp.line - 1) + ' ' + IdeGetLine(fp.line)
			           + ' ' + IdeGetLine(fp.line + 1);
		}
		if(!disas.InRange(f.eip) || f.fn.name != disas_name) {
			disas_name = f.fn.name;
			disas.Clear();
			dword ip = f.fn.address;
			dword h = f.fn.address + f.fn.size;
			if(f.eip < ip || f.eip >= h) {
				ip = f.eip;
				h = ip + 1024;
			}
			while(ip < h) {
				int q = Disassemble(ip);
				if(q < 0)
					break;
				ip += q;
			}
		}
		disas.SetCursor(f.eip);
		disas.SetIp(f.eip, ptrimg);
		Reg(regs.eax, context.Eax);
		Reg(regs.ebx, context.Ebx);
		Reg(regs.ecx, context.Ecx);
		Reg(regs.edx, context.Edx);
		Reg(regs.esi, context.Esi);
		Reg(regs.edi, context.Edi);
		Reg(regs.ebp, context.Ebp);
		Reg(regs.esp, context.Esp);
		if(df)
			disas.SetFocus();
		Data();
	}
}

bool Pdb::SetBreakpoint(const String& filename, int line, const String& bp)
{
	dword a = GetAddress(FilePos(filename, line));
	if(!a)
		return false;
	int q = breakpoint.Find(a);
	if(bp.IsEmpty()) {
		if(q >= 0) {
			if(!RemoveBp(a))
				return false;
			breakpoint.Remove(q);
		}
	}
	else {
		if(q < 0) {
			if(!AddBp(a))
				return false;
			breakpoint.Add(a);
		}
	}
	return true;
}

dword Pdb::CursorAdr()
{
	dword a = disas.HasFocus() ? disas.GetCursor() : GetAddress(FilePos(IdeGetFileName(), IdeGetFileLine()));
	if(!a)
		Exclamation("No code at choosen location !");
	return a;
}

bool Pdb::RunTo()
{
	dword a = CursorAdr();
	if(!a)
		return false;
	if(!SingleStep())
		return false;
	if(context.Eip != a) {
		SetBreakpoints();
		AddBp(a);
		if(!Continue())
			return false;
	}
	Sync();
	return true;
}

void Pdb::Run()
{
	SingleStep();
	SetBreakpoints();
	if(!Continue())
		return;
	Sync();
}

void Pdb::SetIp()
{
	dword a = CursorAdr();
	if(!a)
		return;
	context.Eip = a;
	WriteContext();
	frame[0].eip = a;
	framelist <<= 0;
	SetFrame();
}

bool Pdb::Step(bool over)
{
	TimeStop ts;
	byte b = Byte(context.Eip);
	if(b == 0xe8 || b == 0xff || b == 0x9a || b == 0xff) {
		if(over) {
			int l = 5;
			if(b != 0xe8) {
				char out[256];
				byte code[32];
				memset(code, 0, 32);
				for(int i = 0; i < 32; i++) {
					int q = Byte(context.Eip + i);
					if(q < 0)
						break;
					code[i] = q;
				}
				l = NDisassemble(out, code, context.Eip);
			}
			dword bp0 = context.Eip;
			dword bp = context.Eip + l;
			int lvl = 0;
			Lock();
			for(;;) {
				if(!SingleStep()) {
					Unlock();
					return false;
				}
				SetBreakpoints();
				if(breakpoint.Find(bp0) < 0)
					AddBp(bp0);
				else
					bp0 = 0;
				AddBp(bp);
				if(!Continue()) {
					Unlock();
					return false;
				}
				if(context.Eip == bp0)
					lvl++;
				else
				if(context.Eip == bp) {
					if(lvl <= 0) {
						Unlock();
						return true;
					}
					lvl--;
				}
				else {
					Unlock();
					return true;
				}
				if(ts.Elapsed() > 100)
					Ctrl::ProcessEvents();
			}
		}
		else {
			if(!SingleStep())
				return false;
			byte b = Byte(context.Eip);
			if(b == 0xeb || b == 0xe9)
				return SingleStep();
			return true;
		}
	}
	else
		return SingleStep();
}

void Pdb::Trace(bool over)
{
	dword ip0 = context.Eip;
	FilePos p0 = GetFilePos(ip0);
	if(IsNull(p0.path) || disas.HasFocus()) {
		if(!Step(over))
			return;
		Sync();
		return;
	}
	bool locked = false;
	int n = 0;
	TimeStop ts;
	for(;;) {
		if(ts.Elapsed() > 100) {
			if(!locked) {
				Lock();
				locked = true;
			}
			ProcessEvents();
		}
		if(!Step(over))
			break;
		FilePos p = GetFilePos(context.Eip);
		if(context.Eip < ip0 || p.path != p0.path || p.line != p0.line || stop) {
			Sync();
			break;
		}
	}
	if(locked)
		Unlock();
}

void Pdb::StepOut()
{
	Lock();
	TimeStop ts;
	for(;;) {
		if(Byte(context.Eip) == 0xc2 || Byte(context.Eip) == 0xc3) {
			if(!SingleStep())
				break;
			Sync();
			break;
		}
		if(stop) {
			Sync();
			break;
		}
		if(!Step(true))
			break;
		if(ts.Elapsed() > 100)
			Ctrl::ProcessEvents();
	}
	Unlock();
}

#endif
