#ifndef	__TASKCT361INTERNALCOMMAND__
	#ifdef 	GLOBALS_CT361INTERNALCOMMAND
	#define CT361_EXTERN
	#else
	#define CT361_EXTERN extern
	#endif

	#define __TASKCT361INTERNALCOMMAND__
	#define CT361 9
	#define CT361_StartSearch 				0xF1
	#define	CT361_StopSearch					0xF2
	#define CT361_SendIRCmdInteral		0xF3
	#define CT361_KeepOnSearching			0xF4
	#define	CT361_SearchResutls				0xE1
	#define CT361_SearchReachTheEnd		0xE2
	#define ATV 0x03
	
	extern const u16 g_remote_ATV_info[166][65];
	void SendCmdToCt361(unsigned char data[6]);
	void SendCT361InternalCMD(unsigned char *temp);
	void Hex2Str(unsigned short int data, unsigned char str[5]);
	typedef struct CT361Status{
		unsigned char SearchDisable;
		unsigned char IsSearching;
		unsigned char NowSearchingProgress;
		unsigned char NowDeviceGroup;
		unsigned short int NowDeviceIndex;
	}CT361Status;

	CT361_EXTERN CT361Status CT361StatusGlobal;
#endif
