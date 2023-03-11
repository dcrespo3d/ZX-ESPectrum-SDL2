#ifndef wrap_ESP32Lib_h
#define wrap_ESP32Lib_h

class Mode
{
public:
	int hFront;
	int hSync;
	int hBack;
	int hRes;
	int vFront;
	int vSync;
	int vBack;
	int vRes;
	int vDiv;
	unsigned long pixelClock;
	int hSyncPolarity;
	int vSyncPolarity;
	float aspect;
	int activeLineCount;
	Mode(
		const int hFront = 0,
		const int hSync = 0,
		const int hBack = 0,
		const int hRes = 0,
		const int vFront = 0,
		const int vSync = 0,
		const int vBack = 0,
		const int vRes = 0,
		const int vDiv = 1,
		const unsigned long pixelClock = 0,
		const int hSyncPolarity = 1,
		const int vSyncPolarity = 1,
		const float aspect = 1.f)
		: hFront(hFront),
		hSync(hSync),
		hBack(hBack),
		hRes(hRes),
		vFront(vFront),
		vSync(vSync),
		vBack(vBack),
		vRes(vRes),
		vDiv(vDiv),
		pixelClock(pixelClock),
		hSyncPolarity(hSyncPolarity),
		vSyncPolarity(vSyncPolarity),
		aspect(aspect),
		activeLineCount(vRes / vDiv)
	{
	}

	int maxXRes() const
	{
		return (int(hRes * 19673499. / pixelClock) & 0xfffffffe);
	}

	int linesPerField() const
	{
		return vFront + vSync + vBack + vRes;
	}

	int pixelsPerLine() const
	{
		return hFront + hSync + hBack + hRes;
	}

	Mode custom(int xres, int yres, int fixedYDivider = 0) const
	{
		xres = (xres + 3) & 0xfffffffc;
		float f = float(xres) / hRes;
		int hs = int(hSync * f + 3) & 0xfffffffc;
		int hb = int((hSync + hBack - hs / f) * f + 3) & 0xfffffffc;
		int hr = xres;
		int hf = int((pixelsPerLine() - (hs + hb + hr) / f) * f + 3) & 0xfffffffc;

		int vd = fixedYDivider ? fixedYDivider : (vRes / yres);
		int vr = yres * vd;
		int vf = vFront + vRes / 2 - vr / 2;
		int vb = vBack + vRes / 2 - (vr - vr / 2);
		long pc = long(pixelClock * f);
		return Mode(hf, hs, hb, hr, vf, vSync, vb, vr, vd, pc, hSyncPolarity, vSyncPolarity);
	}

	template<class Output>
	void print(Output& output) const
	{
		output.print("hFront: ");
		output.println(hFront);
		output.print("hSync: ");
		output.println(hSync);
		output.print("hBack: ");
		output.println(hBack);
		output.print("hRes: ");
		output.println(hRes);
		output.print("vFront: ");
		output.println(vFront);
		output.print("vSync: ");
		output.println(vSync);
		output.print("vBack: ");
		output.println(vBack);
		output.print("vRes: ");
		output.println(vRes);
		output.print("vDiv: ");
		output.println(vDiv);
		output.print("pixelClock: ");
		output.println(pixelClock);
		output.print("hSyncPolarity: ");
		output.println(hSyncPolarity);
		output.print("vSyncPolarity: ");
		output.println(vSyncPolarity);
	}
};

class Font
{
  public:
	const int firstChar;
	const int charCount;
	const unsigned char *pixels;
	const int charWidth;
	const int charHeight;
	Font(int charWidth, int charHeight, const unsigned char *pixels, int firstChar = 32, int charCount = 96)
		:firstChar(firstChar),
		charCount(charCount),
		pixels(pixels),
		charWidth(charWidth),
		charHeight(charHeight)
	{
	}

	bool valid(char ch) const
	{
		return ch >= firstChar && ch < firstChar + charCount;
	}
};

class VGA6Bit
{
public:
	typedef unsigned char Color;
	static const Color RGBAXMask = 0x3f;
	static const Color SBits = 0xc0;

	static const Mode MODE320x240;
	static const Mode MODE360x200;

	Color** backBuffer;

	int xres;
	int yres;

	bool init(const Mode& mode, const int* redPins, const int* greenPins, const int* bluePins, const int hsyncPin, const int vsyncPin, const int clockPin = -1) { return true; }

    void setCursor(int x, int y) {}
    void setTextColor(long front, long back) {}
    void print(const char* str) {}

    void setFont(Font& font) {}
    void rect(int x, int y, int w, int h, unsigned char color) {}
    void fillRect(int x, int y, int w, int h, unsigned char color) {}
    void line(int x1, int y1, int x2, int y2, unsigned char color) {}
    void println(const char* str) {}
};

#endif