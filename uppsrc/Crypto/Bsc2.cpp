
byte fixpwd[] = {
	0xba, 0x16, 0x3a, 0x8f, 0x38,
	0x43, 0x72, 0xff, 0x5d, 0x00,
	0x6e, 0x28, 0xe5, 0x68, 0xaa,
	0x10, 0xc7, 0x51, 0x7a, 0x4f
};

template <int N>
class Birc {
	byte  r[N];
	int   origin;
public:
	byte& operator[](int i)       { return r[(origin + i) % N]; }
	void  Restart()               { origin = 0; }
	void  Move()                  { origin = (origin + 1) % N; }

	Birc()                        { origin = 0; }
};

class Crypt {

		  enum {
			 Istep = 613,
			 Mx = 64
		  };

		  byte   T1[256];
		  byte   T2[256];
		  byte   T3[256];
		  byte   T4[256];
		  byte   TI[256];
		  byte   T5[16];

		  Birc<81> Q81;					 /* nove  */
		  Birc<81> R81;
		  Birc<28> R28;
		  Birc<21> R21;
		  Birc<23> R23;

		  byte   Riv[72];
		  byte   zv23, ne81, ne28, ne21, ne23;
		  byte   neq81;                  /* nove  */

public:
		  byte   KL[20];
		  byte   IV[9];
		  byte   vers;                   /* nove OLD=1 NEW=2  */

		  int    init();
		  void   setversion(int v)       { vers = v; }
		  void   Start();
		  void   Reg_step();
		  void   Init();

		  void   IV_step();
		  void   IVGen(const byte *s, long len);
		  void   password(const char *pwd);

		  byte   get();

		  ~Crypt() { memset(this, 0, sizeof(this)); }
};

int  crychar(int c) {
	c = toupper(c);
	return c >= 0x20 && c <= 0x7f ? c : 0;
}

int  Crypt::init() {
	FileIn in(GetExeDirFile(_CRYPT_BIN));
	if(!in) throw LoadingError();
	return in.GetAll(T1, 5 * 256 + 16) ? 0 : -1;
}

void Crypt::Init() {
	byte 	   i;

	for(i =  0; i <=  8; i++) Riv[i] = IV[i];
	for(i =  9; i <= 17; i++) Riv[i] = (IV[i % 9] >> 1) | (IV[i % 9] << 7);
	for(i = 18; i <= 26; i++) Riv[i] = (IV[i % 9] >> 2) | (IV[i % 9] << 6);
	for(i = 27; i <= 35; i++) Riv[i] = (IV[i % 9] >> 3) | (IV[i % 9] << 5);
	for(i = 36; i <= 44; i++) Riv[i] = (IV[i % 9] >> 4) | (IV[i % 9] << 4);
	for(i = 45; i <= 53; i++) Riv[i] = (IV[i % 9] >> 5) | (IV[i % 9] << 3);
	for(i = 54; i <= 62; i++) Riv[i] = (IV[i % 9] >> 6) | (IV[i % 9] << 2);
	for(i = 63; i <= 71; i++) Riv[i] = (IV[i % 9] >> 7) | (IV[i % 9] << 1);

	for(i = 0; i <= 22; i++) R23[i] = Riv[i];
	for(i = 0; i <= 20; i++) R21[i] = Riv[23 + i];
	for(i = 0; i <= 27; i++) R28[i] = Riv[44 + i];

	for(i =  0; i <= 19; i++) R81[i] = KL[i];
	for(i = 20; i <= 39; i++) R81[i] = (KL[i % 20] >> 2) | (KL[i % 20] << 6);
	for(i = 40; i <= 59; i++) R81[i] = (KL[i % 20] >> 4) | (KL[i % 20] << 4);
	for(i = 60; i <= 79; i++) R81[i] = (KL[i % 20] >> 6) | (KL[i % 20] << 2);

	R81[80] = 0xFF;
}

void Crypt::Reg_step() {
	byte  i, fa, zv81, zv28, zv21;

	ne81 = T1[R81[80]];
	ne21 = T3[R21[20]];
	ne23 = T4[R23[22]];

	fa = ((R28[20] & 1)) |
		((R28[21] & 1) << 1) |
		((R28[22] & 1) << 2) |
		((R28[23] & 1) << 3) |
		((R28[24] & 1) << 4) |
		((R28[25] & 1) << 5) |
		((R28[26] & 1) << 6) |
		((R28[27] & 1) << 7);
	fa = T2[fa] & 1;
	ne28 = fa;
	for(i = 1; i <= 7; i++) {
		fa = (((R28[20] >> i) & 1)) |
			(((R28[21] >> i) & 1) << 1) |
			(((R28[22] >> i) & 1) << 2) |
			(((R28[23] >> i) & 1) << 3) |
			(((R28[24] >> i) & 1) << 4) |
			(((R28[25] >> i) & 1) << 5) |
			(((R28[26] >> i) & 1) << 6) |
			(((R28[27] >> i) & 1) << 7);
		fa = (T2[fa] >> i) & 1;
		ne28 = fa + (ne28 << 1);
	}

	zv81 = R81[0];
	zv28 = R28[0];
	zv21 = R21[0];
	zv23 = R23[0];

	R81.Move();
	R81[80] = zv81;
	R81[57] ^= zv81;
	R81[32] ^= zv81;
	R81[18] ^= zv81;

	R28.Move();
	R28[27] = ne28 ^ zv81 ^ zv23 ^ zv28;
	R28[16] ^= zv28;
	R28[11] ^= zv28;
	R28[ 4] ^= zv28;

	R21.Move();
	R21[20] = ne21 ^ zv28 ^ zv21;
	R21[14] ^= zv21;
	R21[ 6] ^= zv21;
	R21[ 3] ^= zv21;

	R23.Move();
	R23[22] = ne23 ^ zv21 ^ zv23;
	R23[14] ^= zv23;
	R23[11] ^= zv23;
	R23[ 7] ^= zv23;
}

void  Crypt::Start()  {
	for(int k = Istep; k--;)
		Reg_step();
}

byte  Crypt::get() {
	byte i, fa, CryptByte;

	Reg_step();
	fa = (ne23 & 1) |
	     ((ne21 & 1) << 1) |
	     ((ne28 & 1) << 2) |
	     ((zv23 & 1) << 3);
	fa = T5[fa] & 1;
	CryptByte = fa;
	for(i = 1; i <= 7; i++) {
		fa = (((ne23 >> i) & 1)) |
	        (((ne21 >> i) & 1) << 1) |
	        (((ne28 >> i) & 1) << 2) |
	        (((zv23 >> i) & 1) << 3);
		fa = (T5[fa] >> i) & 1;
		CryptByte = fa + (CryptByte << 1);
	}
	CryptByte = CryptByte ^ ne81;

	return CryptByte;
}

/*  !!!!!!!!!! Zacatek useku se zmenami !!!!!!!!!!  */

void Crypt::IV_step() {
	byte 	   zvq81;						/* zmena znaceni */

	neq81 = TI[Q81[80]];					    /* zmena znaceni */
	zvq81 = Q81[0];						    /* zmena znaceni */

	Q81.Move();
	Q81[80] = zvq81 ^ neq81;					/* zmena znaceni */
	Q81[57] ^= zvq81;				   /* zmena znaceni */
	Q81[32] ^= zvq81;				   /* zmena znaceni */
	Q81[18] ^= zvq81;				   /* zmena znaceni */
}

void Crypt::IVGen(const byte *s, long len) {
	int i;

	if(vers > 1) {
		for(i =  0; i < 20; i++) R21[i] = (KL[i] >> 3) | (KL[i] << 5);
		for(i =  0; i < 20; i++) R23[i] = (KL[i] >> 4) | (KL[i] << 4);
		for(i =  0; i < 20; i++) R28[i] = (KL[i] >> 1) | (KL[i] << 7);
		R21[20] = 0xFF;
		for(i = 20; i < 23; i++) R23[i] = (KL[i - 20] >> 2) | (KL[i - 20] << 6);
		for(i = 20; i < 28; i++) R28[i] = (KL[i - 20] >> 5) | (KL[i - 20] << 3);
	}									   /* nove */

	for(i =  0; i <= 19; i++) R81[i] = KL[i];
	for(i = 20; i <= 39; i++) R81[i] = (KL[i % 20] >> 2) | (KL[i % 20] << 6);
	for(i = 40; i <= 59; i++) R81[i] = (KL[i % 20] >> 4) | (KL[i % 20] << 4);
	for(i = 60; i <= 79; i++) R81[i] = (KL[i % 20] >> 6) | (KL[i % 20] << 2);
	R81[80] = 0xFF;

	if(vers > 1)
		for(i = 0; i < 81; i++) Q81[i] = 0x00;
	else
		for(i = 0; i < 81; i++) Q81[i] = R81[i];	  /* nove */

	if(vers > 1)
		Start();					/* nove */

	for(i = Istep; i--;)  {
	   IV_step();
	   if(vers > 1) Q81[80] ^= get();		   /* nove */
	}

	while(len--) {
		IV_step();
		Q81[80] = Q81[80] ^ *s++;
	}

	for(i = Istep; i--;)  {
		IV_step();
		if(vers > 1) Q81[80] ^= get();		   /* nove */
	}

	for(i = 0; i < 9; i++)
		if(vers > 1)
			IV[i] = Q81[80 - i];
		else {
			IV_step();
			IV[i] = neq81;
    	}
}

void Crypt::password(const char *pwd) {
	int   a, l;
	static byte spw[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x5a, 0x69, 0xe1, 0xc3,
		0xf0, 0x4b, 0x87, 0x2d, 0x0f, 0x96, 0xd2, 0xb4, 0x3c, 0x78
	};
	byte   w[20];
	if(pwd)
		l = strlen(pwd);
	else {
		pwd = (const char *)fixpwd;
		l = 20;
	}
	memcpy(w, spw, 20);
	memcpy(w, pwd, l);
	for(a = 0; a < 20; a++)
		KL[a] = w[a % l] ^ (w[(43 * a) % 20] << 3) ^ (w[(17 * a) % 20] << 2);
}

bool Bsc2IV(const String& s, byte *IV, const char *password, int version) {
	Crypt c;
	c.setversion(version);
	c.password(password);
	if(c.init() < 0) return false;
	c.IVGen(s, s.GetLength());
	memcpy(IV, c.IV, 9);
	return true;
}

bool Bsc2(String& s, byte *IV, const char *password, int version) {
	Crypt c;
	c.setversion(version);
	c.password(password);
	memcpy(c.IV, IV, 9);
	if(c.init() < 0) return false;
	c.Init();
	c.Start();
	int ln = s.GetLength();
	StringBuffer q(ln);
	byte *w = (byte *)~q;
	for(int n = ln; n--;)
		*w++ ^= c.get();
	s = q;
	return true;
}

String GetPassword() {
	Dialog kd(IDD_KEY);
	EditField key1;
	EditField key2;
	kd << key1(IDC_KEY1) << key2(IDC_KEY2);
	key1.Password();
	key2.Password();
	for(;;) {
		if(kd.Run() == IDOK) {
			String s = key1;
			if(s != key2)
				MsgOK("Klíèe se neshodují !");
			else
			if(s.GetLength() < 5)
				MsgOK("Klíè musí mít minimálnì 5 znakù !");
			else
				return s;
		}
		else
			return EmptyString;
	}
}
