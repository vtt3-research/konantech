

/* Extracting timecode values from UINT: */
#define GET_TIMECODE_FIELDS(tc)         (((tc) >> 0) & 0x3f)
#define GET_TIMECODE_FRAMES(tc)         ((((tc) >> 0) & 0x3f)>>1)
#define GET_TIMECODE_SECONDS(tc)        (((tc) >> 8) & 0x3f)
#define GET_TIMECODE_MINUTES(tc)        (((tc) >> 16) & 0x3f)
#define GET_TIMECODE_HOURS(tc)          (((tc) >> 24) & 0x1f)
#define GET_TIMECODE_DROPFRAME(tc)      (((tc) >> 29) & 0x01)
#define GET_TIMECODE_COLORFRAME(tc)     (((tc) >> 30) & 0x01)
#define IS_TIMECODE_INVALID(tc)         (((tc) >> 31) & 0x01)

/* Formatting timecode values into UINT: */
#define SET_TIMECODE_FIELD(hr, min, sec, fld) \
        ((((fld) & 0x3f) << 0) | (((sec) & 0x3f) << 8) | \
        (((min) & 0x3f) << 16) | (((hr) & 0x1f) << 24))
#define SET_TIMECODE_FRAME(hr, min, sec, frame) \
        ((((frame<<1) & 0x3f) << 0) | (((sec) & 0x3f) << 8) | \
        (((min) & 0x3f) << 16) | (((hr) & 0x1f) << 24))

#define SET_TIMECODE_DROPFRAME(tc)      ((tc) | ((1) << 29))
#define SET_TIMECODE_NON_DF(tc)         ((tc) & ~((1) << 29))

#define SET_TIMECODE_INVALID(tc)        ((tc) | ((1) << 31))
#define SET_TIMECODE_VALID(tc)          ((tc) & ~((1) << 31))

#define INVALID_TIMECODE	0x80000000

#define	NTSC	0
#define	PAL		1
#define	DROP24	2
#define DROP60	3
#define	ETC		4

#define FR_HOUR_DROP    107892L  // frames per hour in drop frame mode
#define FR_TENMIN_DROP  17982L   // frames per ten minutes in drop frame
#define FR_MINUTE_DROP  1798L    // frame per minute in drop frame mode
#define FR_MINUTE_ND    1800L    // frames per minute in non-drop mode
#define FR_SECOND       30L      // frames per second, both modes NTSC
#define FR_HOUR_PAL     90000L   // frames per hour in PAL
#define FR_MINUTE_PAL   1500L    // frames per minute in PAL
#define FR_SECOND_PAL   25L      // frames per second in PAL

// 2008/12/10, yslee, Support 24.97 fps
#define FR_HOUR_24_DROP		86292L	// frames per hour in 24fps drop frame mode
#define FR_HOUR_24_ND		86400L	// frames per hour in 24fps non-drop frame mode
#define	FR_TENMIN_24_DROP	14382L	// frames per ten minutes in 24fps drop frame
#define FR_MINUTE_24_DROP	1438L	// frames per minute in 24fps drop frame mode
#define	FR_MINUTE_24_ND		1440L	// frames per minute in 24fps non-drop mode
#define FR_SECOND_24		24L		// frames per second, both modes 24fps

//
//UINT GetDFDuration(UINT tc1, UINT tc2);
//UINT GetNDFDuration(UINT tc1, UINT tc2);
class CTimecodeFrameCountOperation
{
public:
	CTimecodeFrameCountOperation();
	CTimecodeFrameCountOperation(double dbFrameRate);
	virtual ~CTimecodeFrameCountOperation();

	void SetVideoStandard(int nVideoStandard);
	void SetFrameRate(double dbFrameRate);

	void FrameToTimecode(LONG dwFrames, int &h, int &m, int &s, int &frames);
	LONG TimecodeToFrame(int h, int m, int s, int frames);

	void SecondsToTimecode(int nSeconds, int &h, int &m, int &s, int &frames);
	void SecondsToFrame(int nSeconds, int &frames);

protected:
	int m_nVideoStandard;
	double m_dbFrameRate;
};

void FrameToTimecode(LONG dwFrames, int &h, int &m, int &s, int &frames);
UINT FrameToTimecode(LONG dwFrames);

void FrameToTimecode24DropFPS (LONG dwFrames, int &h, int &m, int &s, int &frames);

LONG TimecodeToFrame(int h, int m, int s, int frames, UINT uTimerMode);
LONG TimecodeToFrame(int h, int m, int s, int frames);
LONG TimecodeToFrame(UINT tc);
LONG TimecodeToFrame(UINT tc, UINT uTimerMode);

LONG TimecodeToField(int h, int m, int s, int fields);
LONG TimecodeToField(UINT tc);

void SetVideoStandard(int nVideoStandard);
int GetVideoStandard();
