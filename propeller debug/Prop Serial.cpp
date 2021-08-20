
#include "stdafx.h"
#include "messages.h"
#include "globalunit.h"
#include "compatibility.h"
#include "debugdisplayunit.h"
#include "propserial.h"
#include "ComThread.h"


#ifdef _DEBUG
void com_object::com_error(CString msg, int err)
{
	CString str;
	m_iErrorCount++;
	str = msg;
	str.Append(_T("\r\n"));
	str.Append(m_strCommId);
	str.Append(_T("\r\n"));
	TRACE(msg);

	close_com();
	if (m_bAbortMode==true)
	{
		abort();
		return;
	}
	if (m_iErrorCount>8)
	{
		throw ((int)HARDWARE_UNRESPONSIVE);
	}
}
#endif

void com_object::tx_string(const char *s)
{
	UINT i;
	size_t sz;
	sz = strlen (s);
	for (i=0;i<sz;i++)
		tx_char(char(s[i]));
#if 0
	tx_comm();
#endif
}

// Add string to comm buffer
void com_object::tx_string(CString s)
{
	int i, sz;
	sz = s.GetLength();
	for (i=0;i<sz;i++)
	  tx_char (char(s[i]));
#if 0
	tx_comm();
#endif
}

prop_serial::prop_serial()
{
	m_strVersion = _T("NOT AVAILABLE");
	m_cVersion = '?';
	reset_flags();
}

void prop_serial::reset_flags ()
{
	m_bDebugActive = false;
	m_bVersionMode = false;
	m_bAbortMode = false;
	m_bFatalError = false;
}

// Parallax Propeller P2 Specific Routines
//
// bool prop_serial::detect_hardware()
// bool prop_serial::HardwareFound()
// bool prop_serial::version_check()
// bool prop_serial::set_version_string ()
// void prop_serial::LoadHardware()
// bool prop_serial::open_forth ()
//

bool prop_serial::detect_hardware()
{
	int i, FirstPort;
	// check com99..com1 for hardware
	FirstPort = m_iPortId;
	i = 100;
	while (i!=0)
	try
	{
		if (i!=100)
			m_iPortId = i;
		if ((i==100)||(i!=FirstPort))
		{
			if (HardwareFound())				
				return true;
		}
		i--;
	}
	catch (...)
	{
		m_bAbortMode = true;
		com_object::com_error (_T("\r\nNo hardware found on COM1 through "),0);
		m_iPortId = 99;
	}
	return false;
}

bool prop_serial::HardwareFound()
{
	_com_error error_code;
	bool result;
	m_bAbortMode = false;
	result = open_com ();
	if(result==false)
		return false;
	reset_flags ();
	reset_hardware();
	result = version_check ();
	if (result==false)
	{
		error_code = HARDWARE_MISMATCH;
	}
	m_bAbortMode = true;
	result = true;
	// error, result is false, allow abort
	if (error_code.get()==HARDWARE_MISMATCH)
	{
		CComThread::OutputDebugString
			("\r\nHardware Type MISMATCH:");
		m_bAbortMode = true;
		result = false;
	}
	if (error_code.get()==HARDWARE_UNRESPONSIVE) {
		CComThread::OutputDebugString
			("\r\nHardware Device UNRESPONSIVE:");
		m_bAbortMode = true;
		result = false;
	}
	if (m_bOverlapped)
		CancelIo(m_hCommHandle);

	if (result==false)
	{
		close_com ();
	}
	return result;
}

// Reset hardware
bool prop_serial::reset_hardware()
{
	BOOL res;
	CComThread::OutputDebugString
		("\r\nResetting Hardware on COM%d",m_iPortId);

	m_iErrorCount = 0;
	m_bFatalError = false;

	// generate reset pulse via dtr&wait 25ms
	Sleep(10);
	res =  EscapeCommFunction(m_hCommHandle, SETDTR);
	Sleep(10);
	res =  EscapeCommFunction(m_hCommHandle, CLRDTR);
	Sleep(10);
	if (m_hCommHandle==NULL)
		return false;

	flush_rx (true);
	CComThread::OutputDebugString
		("\r\nReset COM%d Completed. (SET/CLEAR DTR)",m_iPortId);
	return true;
}

bool prop_serial::version_check()
{
	CString s, ver;
	ver.AppendChar(char(13));
	ver.AppendChar(char(10));
	ver.Append(_T("Prop_Ver X"));
	ver.AppendChar(char(13));
	ver.AppendChar(char(10));

// assmuing that we have jusst successfully
// opened a com port and flashed the DTR/RTS
// lines, now probe for P2 hardware on the
// detected and hopefully open port.
	
	CComThread::OutputDebugString(
		"\r\nTransmitting \"> Prop_Chk 0 0 0 0 \" on COM%d:",m_iPortId);
	tx_string(PROPELLER_BAUD_RATE_SYNC);
	tx_comm();
	tx_string(PROPELLER_CHECK_STRING);
	tx_comm();
	CComThread::OutputDebugString(
		"\r\nChecking for response ... ",m_iPortId);

// receive version string
	s = _T("");
	char c;
	try {
	for (int i=0;i<14;i++)
	{
		c = (char)RxByte();
		CComThread::OutputDebugString(
			"\r\nReading version check RxByte returning: '%c'",c);
		s.AppendChar(c);
	}}
	catch (_com_error err)
	{
		AfxMessageBox (_T("unknown com error"),IDOK);
		return false;
	}
	m_strVersion = s;
	m_cVersion = char(s[11]);
	s.SetAt(11,'X');	
	CComThread::OutputDebugString(
		"\r\nPerforming CPU-ID validation test");	
// if we made it this far - then is there
// something sending us data?

	bool result;
	if (s.Compare(ver)!=0)
	{
		c = (char)(m_strVersion[1]);
		CComThread::OutputDebugString
			("\r\nI/O error or non Parallax device! (code %0x02)",c);
		result = false;
	}
	else
	{
		CComThread::OutputDebugString
		("\r\nPropeller Device Found on COM%d.",m_iPortId);
		result = true;
	}
	return result;
}

// Get hardware version
bool prop_serial::set_version_string ()
{
	CString s,str;
	s = _T("Unknown.");
	if (m_cVersion==char('A'))
		s = _T("FPGA-8 cogs, 512KB hub, 48 smart pins 63..56, 39..0, 80MHz.");
	else if (m_cVersion==char('B'))
		s = _T("FPGA-4 cogs, 256KB hub, 12 smart pins 63..60/7..0, 80MHz.");
	else if (m_cVersion==char('C'))
		s = _T("unsupported"); //1 cog, 32KB hub, 8 smart pins 63..62/5..0, 80MHz, No CORDIC';
	else if (m_cVersion==char('D'))
		s = _T("unsupported"); //1 cog, 128KB hub, 7 smart pins 63..62/4..0, 80MHz, No CORDIC';
	if (m_cVersion==char('E'))
		s = _T("FPGA-4 cogs, 512KB hub, 18 smart pins 63..62/15..0, 80MHz.");
	if (m_cVersion==char('F'))
		s = _T("unsupported"); //16 cogs, 1024KB hub, 7 smart pins 63..62/33..32/2..0, 80MHz';
	if (m_cVersion==char('G'))
		s = _T("P2X8C4M64P Rev B/C-8 cogs, 512KB hub, 64 smart pins.");
	m_strVersion = s;
	return true;
}

bool prop_serial::open_forth ()
{
	bool res;
	m_bAbortMode = false;
	res = open_com ();
	if(res==false)
		return false;
	reset_hardware();	
	CComThread::OutputDebugString(
		"\r\nTransmitting PROPELLER_BAUD_RATE_SYNC on COM%d:",m_iPortId);
	tx_string(PROPELLER_BAUD_RATE_SYNC);
	tx_comm();
	CComThread::OutputDebugString(
		"\r\nTransmitting LOAD_FORTH command on COM%d:",m_iPortId);
	tx_string(PROPELLER_FORTH);
	tx_comm();
	return true;
}

// Load Hardware
void prop_serial::load_hardware(LPVOID mem, int sz)
{
	CString msg;
	CString iTemp;
	int LoadLimit, i, n;
	int m = 0;
	unsigned char *data;
	data = (unsigned char*) mem;
// todo - parse the binary image to
// properly manage trailing zeroes
// if present, and adjust accordingly.
	size_t size = sz;

	// find hardware
	m_bVersionMode = false;
	if (m_port_speed>230400)
		set_overlapped(true);
	else
		set_overlapped(false);

	detect_hardware ();
	CComThread::OutputDebugString
		("\r\nChecking CPU version");

	// check version
	if ((m_cVersion<char('A'))
		||(m_cVersion>char('G')))
	{
		msg = _T("Invalid Hardware Version: ");
		msg.AppendChar(m_cVersion);
		msg.Append(_T(" Found on"));
		com_object::com_error (msg,0);
	}
	// update progress form
//	ProgressForm.StatusLabel = _T("\r\nLoading RAM");
	//  ProgressForm.StatusLabel.Repaint();
	// cap load sizes by Version
	if (m_cVersion==char('A'))
		LoadLimit = PGLOBAL::Smaller(size, 0x100000);
	else if (m_cVersion==char('B'))
		LoadLimit = PGLOBAL::Smaller(size, 0x040000);
	else if (m_cVersion==char('C'))
		LoadLimit = PGLOBAL::Smaller(size, 0x008000);
	else if (m_cVersion==char('D'))
		LoadLimit = PGLOBAL::Smaller(size, 0x020000);
	else if (m_cVersion==char('E'))
		LoadLimit = PGLOBAL::Smaller(size, 0x080000);
	else if (m_cVersion==char('F'))
		LoadLimit = PGLOBAL::Smaller(size, 0x100000);
	else if (m_cVersion==char('G'))
		LoadLimit = PGLOBAL::Smaller(size, 0x100000);
	// send bytes in Base64 format
	CComThread::OutputDebugString
		("\r\nTransmitting \"Prop_Txt 0 0 0 0 \"");

	tx_string(_T("Prop_Txt 0 0 0 0 "));
	n = 0;
	m_obj_file_size = LoadLimit;
	m_bytes_uploaded = 0;
	CComThread::OutputDebugString ("\r\nUploading");
	int bytes_per_frame = 1+m_port_speed/600;
	for (i=0;i<LoadLimit;i++)
	{			
		m = (m<<8)+data[i];
		n = n+8;
		if (n>=6) {
			tx_base64((m>>(n-6))&0x3F);
			n = n-6;
		}
		if (n>=6) {
			tx_base64((m>>(n-6))&0x3F);
			n = n-6;
		}
		m_bytes_uploaded++;
		if (i%bytes_per_frame==0) 
			CComThread::UpdateProgress (m_bytes_uploaded,m_obj_file_size);
	}
	tx_comm();
	if (n>0)
		tx_base64((m<<(6-n))&0x3F);
	CComThread::UpdateProgress (m_bytes_uploaded,m_obj_file_size);
	CComThread::OutputDebugString
		("\r\nUploaded %d bytes",i);
	CComThread::OutputDebugString
		("\r\nTransmitting ~\r\n");
	
	tx_string(_T("~"));
	tx_comm();
}
