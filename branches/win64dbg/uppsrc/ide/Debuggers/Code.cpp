#include "Debuggers.h"

#ifdef COMPILER_MSC

#define LLOG(x)  LOG(x)

int Pdb::Disassemble(adr_t ip)
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
	int sz = NDisassemble(out, code, ip, win64);
	if(sz > i)
		return -1;
	disas.Add(ip, out, Null);
	return sz;
}

void Pdb::Reg(Label& reg, adr_t val)
{
	String h = Sprintf("%08X", val);
	if(h != reg.GetText()) {
		reg.SetLabel(h);
		reg.SetInk(LtRed);
	}
	else
		reg.SetInk(SColorText);
}

bool Pdb::IsValidFrame(adr_t eip)
{
	for(int i = 0; i < module.GetCount(); i++) {
		const ModuleInfo& f = module[i];
		if(eip >= f.base && eip < f.base + f.size)
			return true;
	}
	return false;
}

adr_t Pdb::GetIP()
{
#ifdef CPU_64
	if(win64)
		return threads.Get((int)~threadlist).context64.Rip;
#endif
	return threads.Get((int)~threadlist).context32.Eip;
}

adr_t Pdb::GetBP()
{
#ifdef CPU_64
	if(win64)
		return threads.Get((int)~threadlist).context64.Rbp;
#endif
	return threads.Get((int)~threadlist).context32.Ebp;
}

void Pdb::Sync0()
{
	stop = false;
	adr_t ip = GetIP();
	adr_t bp = GetBP();
	adr_t spmax = threads.Get(event.dwThreadId).sp;
	framelist.Clear();
	frame.Clear();
	int ndx = 0;
	FnInfo fn = GetFnInfo(ip);
	int c = -1;
	for(;;) { // Scan through stack frames
		Frame& f = frame.Add();
		f.reip = ip;
		f.rebp = bp;
		f.fn = fn;
		String r;
		if(IsNull(fn.name)) {
			r = Sprintf("0x%08x", f.reip);
			for(int i = 0; i < module.GetCount(); i++) {
				const ModuleInfo& f = module[i];
				if(ip >= f.base && ip < f.base + f.size) {
					r << " (" << GetFileName(f.path) << ")";
					break;
				}
			}
		}
		else {
			GetLocals(ip, bp, f.param, f.local);
			r = fn.name;
			r << '(';
			for(int i = 0; i < f.param.GetCount(); i++) {
				if(i)
					r << ", ";
				r << f.param.GetKey(i) << "=" << Visualise(f.param[i]).GetString();
			}
			r << ')';
			if(c < 0)
				c = frame.GetCount() - 1;
		}
		framelist.Add(frame.GetCount() - 1, r);
		int q = 0;
		for(;;) {
			if(bp > spmax || ++q > 1024 * 64)
				goto end;
			adr_t nip, nbp;
			if(!Copy(bp, &nbp, 4))
				goto end;
			if(!Copy(bp + 4, &nip, 4))
				goto end;
			if(nbp >= bp && nbp < spmax && IsValidFrame(nip)) {
				fn = GetFnInfo(nip);
				if(!IsNull(fn.name)) {
					ip = nip;
					bp = nbp;
					break;
				}
			}
			bp += 4;
		}
	}
end:
	framelist <<= max(c, 0);
}

void Pdb::Sync()
{
	threadlist.Clear();
	for(int i = 0; i < threads.GetCount(); i++) {
		int thid = threads.GetKey(i);
		AttrText x(Format("0x%x", thid));
		if(thid == event.dwThreadId)
			x.font = StdFont().Bold();
		threadlist.Add(thid, x);
	}
	threadlist <<= (int)event.dwThreadId;
	Sync0();
	SetFrame();
	IdeActivateBottom();
}

void Pdb::SetThread()
{
	Sync0();
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
		FilePos fp = GetFilePos(f.reip);
		IdeHidePtr();
		autotext.Clear();
		if(fp) {
			IdeSetDebugPos(fp.path, fp.line, ptrimg, 0);
			autotext = IdeGetLine(fp.line - 1) + ' ' + IdeGetLine(fp.line)
			           + ' ' + IdeGetLine(fp.line + 1);
		}
		if(!disas.InRange(f.reip) || f.fn.name != disas_name) {
			disas_name = f.fn.name;
			disas.Clear();
			adr_t ip = f.fn.address;
			adr_t h = f.fn.address + f.fn.size;
			if(f.reip < ip || f.reip >= h) {
				ip = f.reip;
				h = ip + 1024;
			}
			while(ip < h) {
				int q = Disassemble(ip);
				if(q < 0)
					break;
				ip += q;
			}
		}
		disas.SetCursor(f.reip);
		disas.SetIp(f.reip, ptrimg);
	
//TODO: Fix, refactor this
#ifdef CPU_32
		const CONTEXT& context32 = threads.Get((int)~threadlist).context32;
		Reg(regs.eax, context32.Eax);
		Reg(regs.ebx, context32.Ebx);
		Reg(regs.ecx, context32.Ecx);
		Reg(regs.edx, context32.Edx);
		Reg(regs.esi, context32.Esi);
		Reg(regs.edi, context32.Edi);
		Reg(regs.ebp, context32.Ebp);
		Reg(regs.esp, context32.Esp);
#endif
		if(df)
			disas.SetFocus();
		Data();
	}
}

bool Pdb::SetBreakpoint(const String& filename, int line, const String& bp)
{
	adr_t a = GetAddress(FilePos(filename, line));
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

adr_t Pdb::CursorAdr()
{
	adr_t a = disas.HasFocus() ? disas.GetCursor() : GetAddress(FilePos(IdeGetFileName(), IdeGetFileLine()));
	if(!a)
		Exclamation("No code at choosen location !");
	return a;
}

bool Pdb::RunTo()
{
	LLOG("== RunTo");
	adr_t a = CursorAdr();
	if(!a)
		return false;
	if(!SingleStep())
		return false;
	if(GetIP() != a) {
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
	LLOG("== Run");
	SingleStep();
	SetBreakpoints();
	if(!Continue()) {
		DLOG("Run: !Continue");
		return;
	}
	DLOG("Run: Sync");
	Sync();
}

void Pdb::SetIp()
{
	adr_t a = CursorAdr();
	if(!a)
		return;
#ifdef CPU_64
	if(win64)
		context64.Rip = a;
	else
#endif
		context32.Eip = (DWORD)a;
	WriteContext();
	frame[0].reip = a;
	framelist <<= 0;
	SetFrame();
}

bool Pdb::Step(bool over)
{
	LLOG("== Step over: " << over);
	TimeStop ts;
	adr_t ip = GetIP();
	adr_t bp = GetBP();
	byte b = Byte(ip);
	byte b1 = (Byte(ip + 1) >> 3) & 7;
	if(b == 0xe8 || b == 0x9a || b == 0xff && (b1 == 2 || b1 == 3)) { // Various CALL forms
		if(over) {
			int l = 5;
			if(b != 0xe8) {
				char out[256];
				byte code[32];
				memset(code, 0, 32);
				adr_t ip = GetIP();
				for(int i = 0; i < 32; i++) {
					int q = Byte(ip + i);
					if(q < 0)
						break;
					code[i] = q;
				}
#ifdef CPU_64
				l = NDisassemble(out, code, GetIP(), win64); // TODO Win64 offset?!
#else
				l = NDisassemble(out, code, GetIP());
#endif
			}
			adr_t bp0 = GetIP();
			adr_t bp = bp0 + l;
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
				if(GetIP() == bp0)
					lvl++;
				else
				if(GetIP() == bp) {
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
			byte b = Byte(GetIP());
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
	LLOG("== Trace over: " << over);
	adr_t ip0 = GetIP();
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

		adr_t ip = GetIP();
		FilePos p = GetFilePos(ip);
		if(ip < ip0 || p.path != p0.path || p.line != p0.line || stop) {
			Sync();
			break;
		}
	}
	if(locked)
		Unlock();
}

void Pdb::StepOut()
{
	LLOG("== StepOut");
	Lock();
	TimeStop ts;
	for(;;) {
		adr_t ip = GetIP();
		if(Byte(ip) == 0xc2 || Byte(ip) == 0xc3) {
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
