
#include "stdafx.h"
#include "messages.h"
#include "globalunit.h"
#include "compatibility.h"
#include "debugdisplayunit.h"
#include "propserial.h"
#include "ComThread.h"

static char HexChr[16] = 
{
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,
	0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46
};

static char Base64Chr[64] = 
{
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,
	0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,
	0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,
	0x6F,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x2B,0x2F
};

////////////////////////////////////////////
//
// generic routines follow -
// these routines could be moved into the
// the comthread class - but I dont want to 
// break or otherwise prejudice portability
// since the current ComThread class is based
// on the Microsoft MFC UI Thread model.
//

com_object::com_object()
{
	m_strCommId = _T("NOT CONFIGURED");
	m_iPortId = 0;
	m_port_speed = port_speeds[13];
	m_hCommHandle = NULL;
	m_bOverlapped = false;
	m_hEventRx = CreateEvent
		(NULL,TRUE,FALSE,_T("RxData"));
	m_hEventTx = CreateEvent
		(NULL,TRUE,FALSE,_T("TxData"));
	for (int i=0;i<MAX_BUFFERS;i++)
	{
		memset((void*)&m_overlapped_read[i],0,sizeof(OVERLAPPED));
		memset((void*)&m_overlapped_write[i],0,sizeof(OVERLAPPED));
		m_overlapped_read[i].hEvent = m_hEventRx;
		m_overlapped_write[i].hEvent = m_hEventTx;
	}
	m_iCurrentOverlapRx = 0;
	m_iCurrentOverlapTx = 0;
	m_iNextOverlapRx = 0;
	m_iNextOverlapTx = 0;
	m_bytes_uploaded = 0;
	m_obj_file_size = 0;
	reset_buffers();
}

void com_object::config_buffers(LPVOID p_addr)
{
// sneaky way to increment or add/subract a void* by the correct
// number of bytes for the anticipated data type.
#define SERIAL_DATA_TYPE	char
	int i;
	LPVOID b_addr = p_addr;
	for (i=0;i<MAX_BUFFERS;i++)
	{
		m_pRxBuff[i]=b_addr;
		b_addr=(LPVOID)((SERIAL_DATA_TYPE*)b_addr
			+(size_t)RxBuffSize);
		m_pTxBuff[i]=b_addr;
		b_addr=(LPVOID)((SERIAL_DATA_TYPE*)b_addr
			+(size_t)RxBuffSize);
	}
}

// Return comm port string
void com_object::make_com_string(int id)
{
	CString result;
	result.Format(_T("COM%d"),id);
	m_strCommId = result;;
}

void com_object::set_overlapped (bool arg)
{
	m_bOverlapped = arg;
}

// Close comm port
bool com_object::is_active ()
{
	if (m_hCommHandle==NULL)
	{
		return false;
	}
	if (m_hCommHandle==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	return true;
}

// Open comm port
bool com_object::open_com()
{
	if (m_hCommHandle!=NULL)
		close_com();
	make_com_string(m_iPortId);
	CString str;	
	DWORD attributes  = FILE_ATTRIBUTE_NORMAL;
	if (m_bOverlapped==true)
		attributes |= FILE_FLAG_OVERLAPPED;
	str = _T("\\\\.\\");
	str.Append(m_strCommId);
	m_hCommHandle = CreateFile(str,
		GENERIC_READ|GENERIC_WRITE,
		0, NULL, OPEN_EXISTING,
		attributes,
		0);

	m_dwLastError = GetLastError();
	if ((m_dwLastError==ERROR_ACCESS_DENIED)
		||(m_dwLastError==ERROR_FILE_NOT_FOUND)
		||(m_hCommHandle==INVALID_HANDLE_VALUE))
	{
#if 0
		m_hCommHandle = NULL;
		CComThread::OutputDebugString 
			("\r\nUanable to Open COM%d:",m_iPortId);
#endif
		m_hCommHandle = NULL;
		return false;
	}
	CComThread::OutputDebugString 
		("\r\nHardware Detected COM%d:",m_iPortId);
	LPDCB pDCB = &m_CommDCB;
	reset_dcb (pDCB,true);
	reset_com_timeouts ();
	reset_buffers ();
	return true;
}

void com_object::reset_buffers()
{
	m_dwTxBuffPos = 0;
	m_dwBytesWritten = 0;
	m_dwBytesRead = 0;
	m_dwRxBuffStart = 0;
	m_dwRxBuffEnd = 0;
	m_iErrorCount = 0;
	m_dwLastError = 0;
	m_iNextOverlapRx = 0;
	m_iNextOverlapTx = 0;
}

void com_object::reset_dcb (LPDCB pDCB, bool init)
{
// get the current command state and the
// write it back after making any needed changes
	if (m_hCommHandle==NULL)
	{
// this happens if reset_dcb is called from
// the preferences menu/property sheet without
// having an active connection.
		return;
	}
	BOOL status;
	init = false;
	pDCB->DCBlength = sizeof(DCB);
	if (init==true)
	{
		BuildCommDCB(_T(_DEFAULT_PORT_SPEED_),pDCB);
		ASSERT (m_hCommHandle);
		status = SetCommState(m_hCommHandle,pDCB);
		if (status==false)
		{
			m_dwLastError = GetLastError();
			CComThread::OutputDebugString
			("\r\nSetCommState returned error %d",m_dwLastError);
		}
	}
	status = GetCommState(m_hCommHandle,pDCB);
	ASSERT (status);
	pDCB->BaudRate=m_port_speed;
	pDCB->fBinary=1;
	pDCB->ByteSize=8;
	pDCB->StopBits=ONESTOPBIT;
	pDCB->fParity=NOPARITY;
	pDCB->fDtrControl=DTR_CONTROL_DISABLE;
	pDCB->fRtsControl=RTS_CONTROL_DISABLE;
	pDCB->ByteSize=8;	
	pDCB->fTXContinueOnXoff=1;
	pDCB->XoffLim=512;
	pDCB->XonLim=2048;
	pDCB->XonChar=17;
	pDCB->XoffChar=19;
	pDCB->ErrorChar=(char)0xfe;
	pDCB->EofChar=(char)0xff;
#if 0
	pDCB->fTXContinueOnXoff=0;
	pDCB->XoffLim=0;
	pDCB->XonLim=0;
	pDCB->XonChar=0;
	pDCB->XoffChar=0;
	pDCB->ErrorChar=(char)0;
	pDCB->EofChar=(char)0;
#endif
	CComThread::OutputDebugString
		("\r\nSetting Com State: %d bps.",m_port_speed);
	status = SetCommState(m_hCommHandle,pDCB);
	if (status==false)
	{
		m_dwLastError = GetLastError();
		CComThread::OutputDebugString
			("\r\nSetCommState returned error %d",m_dwLastError);
	}
}

void com_object::reset_com_timeouts ()
{
// likewise, fiddle with the CommTimouts
	COMMTIMEOUTS CommTimeouts;
	int ReadIntervalTimeout = MAXDWORD;
	int ReadTotalTimeoutMultiplier = 0;
	int ReadTotalTimeoutConstant = 0;
	int WriteTotalTimeoutMultiplier = 0;
	int WriteTotalTimeoutConstant = 0;

	CComThread::OutputDebugString
		("\r\nResetting COM Timeoouts.");

	GetCommTimeouts(m_hCommHandle,&CommTimeouts);
	CommTimeouts.ReadIntervalTimeout = ReadIntervalTimeout;
	CommTimeouts.ReadTotalTimeoutConstant = ReadTotalTimeoutMultiplier;
	CommTimeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
	CommTimeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	CommTimeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;
	SetCommTimeouts(m_hCommHandle, &CommTimeouts);
}

void com_object::flush_rx (bool z)
{
// purge any previously-received bytes&clear any break
	PurgeComm(m_hCommHandle, PURGE_RXCLEAR);
	PurgeComm(m_hCommHandle, PURGE_TXCLEAR);
	for (int rcount=0;rcount<2;rcount++)
	{
		int count=0;
		do
		{
			RxComm();
			count++;
		}
		while (m_dwBytesRead!=0);
		CComThread::OutputDebugString
			("\r\nCalled RxComm %d times.",count);
		Sleep(10);
	}
// if parameter is true - also write zeroes to the
// recieve buffer
	if (z==true) {
		memset((void*)(m_pRxBuff[0]),0,RxBuffSize);
		reset_buffers();
	}
}

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

int com_object::GetComPortNumber()
{
	int portId;
	portId = m_iPortId;
	return portId;
}

bool com_object::close_com ()
{
	BOOL res;

// Your milage may vary depending on OS version.  Not
// responsible for lost dog, missed bus transfer, ballots
// cast or counted for the wrong candidate.	

	if (m_hCommHandle==NULL)
		return true;

	if (m_hCommHandle==INVALID_HANDLE_VALUE)
	{
		m_hCommHandle=NULL;
		return true;
	}
	CComThread::OutputDebugString
		("\r\nprop_serial::CloseComm: %08x",(DWORD)m_hCommHandle);

	res = CloseHandle(m_hCommHandle);
	m_hCommHandle = INVALID_HANDLE_VALUE;
	if (res==0)
	{
		CComThread::OutputDebugString
			("\r\nCLOSE HANDLE UNKNOWN ERROR");
	}
	return true;
}

/////////////////////////
//  Hardware Routines  //
/////////////////////////

void com_object::abort ()
{
	CComThread::OutputDebugString
		("\r\nprop_serial::Abort() ... COM port ERROR");
}
 
VOID CALLBACK com_object::read_callback (DWORD dwErr,
  DWORD cbBytesRead,LPOVERLAPPED lpOverlapped)
{
	CComThread::OutputDebugString ("\r\nprop_serial::read_callback %d bytes read",
		cbBytesRead);

	HANDLE hEvent = lpOverlapped->hEvent;
	LPVOID ptr = lpOverlapped->Pointer;
	if ((dwErr == 0) && (cbBytesRead != 0))
	{

	} 
 } 
 
VOID CALLBACK com_object::write_callback (DWORD dwErrorCode,
  DWORD cbWritten,LPOVERLAPPED lpOverlapped)
{
	CComThread::OutputDebugString ("\r\nprop_serial::read_callback %d bytes read",
		cbWritten);

} 
 
// Add hex byte to comm buffer
void com_object::tx_hex(unsigned char x)
{
	tx_char(HexChr[(x>>4)&0xF]);
	tx_char(HexChr[x&0xF]);
	tx_char(0x20);
}

// Add base64 byte to comm buffer
void com_object::tx_base64(unsigned char x)
{
	tx_char(Base64Chr[x]);
}

// Add byte to comm buffer
void com_object::tx_char(unsigned char x)
{
#if DEBUG_SERIAL_TX
	if (x<32)
		CComThread::ConsoleDebugOutput ("\r\nTxByte: %02x",x);
	else
		CComThread::ConsoleDebugOutput ("\r\nTxByte: '%c'",x);
#endif
	((char*)(m_pTxBuff[m_iNextOverlapTx]))[m_dwTxBuffPos] = x;
	m_dwTxBuffPos++;

	if (m_dwTxBuffPos==TxBuffSize)
		tx_comm();
}

// Transmit comm buffer
void com_object::tx_comm()
{
// spurious call - update total bytes written abd
// return
	if (m_dwTxBuffPos==0)
	{
#if DEBUG_SERIAL_TX
		CString msg;
		msg.Format(_T("\r\nTxComm: HANDLE = %08x,\
					  TotalBytesWritten = %d"),
					  m_hCommHandle,m_dwBytesWritten);
		TRACE(msg);
#endif
		return;
	}
	int N = 0;
	BOOL status;	
	DWORD BytesWritten = 0;
	OVERLAPPED *P = NULL;	
	if (m_bOverlapped)
	{	
		N = m_iCurrentOverlapTx;
		P = &(m_overlapped_write[N]);		
		(++m_iNextOverlapTx)%=MAX_BUFFERS;
		ResetEvent(m_hEventTx);
		m_iCurrentOverlapTx=m_iNextOverlapTx;
	}
	status = WriteFile(m_hCommHandle, m_pTxBuff[N], m_dwTxBuffPos,
		&BytesWritten,P);

	m_dwBytesWritten+=BytesWritten;
	m_dwLastError = GetLastError();
	if ((m_dwLastError==ERROR_INVALID_HANDLE)||
		((m_dwLastError!=ERROR_IO_PENDING)&&(!status)))
	{
		com_object::com_error(_T("\r\nUnable to write to"),0);
		ASSERT(false);
	}
#if DEBUG_SERIAL_TX
	CString msg;
	msg.Format(_T("\r\nTxComm: HANDLE = %08x,\
		TxBuffLength = %d,\
		BytesWritten = %d"),
		m_hCommHandle,m_dwTxBuffPos,BytesWritten);
	TRACE(msg);
#endif
	
	if ((!m_bOverlapped)&&(BytesWritten!=m_dwTxBuffPos))
		com_object::com_error(_T("\r\nTransmit stall on"),0);
	
	m_dwTxBuffPos = 0;
}

// Receive comm byte
unsigned char com_object::RxByte()
{
#if 0
	TRACE ("\r\nprop_serial::RxByte()");
#endif
	int N = 0;
	char *rxbuff = (char*)(m_pRxBuff[N]);
	unsigned char result;
	int start_tick, elapsed_ms;
	start_tick = GetTickCount();
	do
	{
		if (m_dwRxBuffStart==m_dwRxBuffEnd)
			RxComm();
		if (m_dwRxBuffStart!=m_dwRxBuffEnd)
		{
			result = rxbuff[m_dwRxBuffStart];
			m_dwRxBuffStart++;
			goto exit;
		}
		elapsed_ms = GetTickCount()-start_tick;
	}
	while (elapsed_ms<PROPELLER_RX_TIMEOUT)
		;
	CComThread::OutputDebugString
		("\r\nCOM TIMEOUT in RxByte: %dms",elapsed_ms);
	com_object::com_error (_T("\r\nHardware lost on"),0);
	result = (char)'?';
exit:
	return result;
};

// Check for received byte(s)
bool com_object::RxCheck()
{
	LPOVERLAPPED ptrOL;
	DWORD index = m_iCurrentOverlapRx;
	DWORD err = 0;
	DWORD dwBytesRx1,dwBytesRx2;
	BOOL result = FALSE;
	if (m_bOverlapped)
	{
		ptrOL = &(m_overlapped_read[index]);
		result = GetOverlappedResult
			(m_hCommHandle,ptrOL,
			&dwBytesRx2,FALSE);
		if (result==FALSE)
			err = GetLastError();

		if (err==ERROR_IO_INCOMPLETE)
		{
#ifdef DEBUG_OVERLAPPED_RX
			CComThread::OutputDebugString
				("\r\nprop_serial::RxCheck: FALSE (ERROR_IO_INCOMPLETE)\r\n");
#endif
			return false;
		}
		if (dwBytesRx2>0)
			result=TRUE;
#ifdef DEBUG_OVERLAPPED_RX
		CComThread::OutputDebugString
			("\r\nprop_serial::RxCheck: TRUE (%d bytes read)\r\n",dwBytesRx2);
#endif
		RxComm();
	}
	else if (m_dwRxBuffStart==m_dwRxBuffEnd)
		RxComm();
	dwBytesRx1 = m_dwRxBuffEnd-m_dwRxBuffStart;
	if (dwBytesRx1>0)
		return true;

	return result;
}

// Receive any data into comm buffer
void com_object::RxComm()
{
	DWORD dwEvent;
	BOOL ioresult1, ioresult2;
	m_dwRxBuffStart = 0;
	m_dwRxBuffEnd = 0;
	m_dwBytesRead = 0;
// there is the possiblity that a read request might
// somehow be in the message queue at a time when this
// thread or another thread closes the port.  If this
// happens then the read request should fail silently.
	if (m_hCommHandle==INVALID_HANDLE_VALUE)
		return;
	ioresult1 = SetCommMask(m_hCommHandle,EV_RXCHAR);
	ASSERT(ioresult1);
#if 0
	ioresult = ReadFileEx (m_hCommHandle, m_pRxBuff, RxBuffSize,
		&m_overlapped_read, prop_serial::read_callback);
#endif
	int N = 0;
	OVERLAPPED *P=NULL;
	LPVOID current_buffer = m_pRxBuff[N];
	if (m_bOverlapped)
	{
		m_iNextOverlapRx = (m_iNextOverlapRx++)%MAX_BUFFERS;
		P = &(m_overlapped_read[N]);
	}
	ioresult1 = ReadFile (m_hCommHandle, current_buffer, RxBuffSize,
		&m_dwBytesRead,P);

	m_dwRxBuffEnd += m_dwBytesRead;
	m_dwLastError = GetLastError();
	if (m_dwBytesRead>0)
	{
		CComThread::ConsoleDebugOutput ("\r\nprop_serial::RxCom read %d bytes",m_dwBytesRead);
	}
	if (m_bOverlapped)
		ioresult2 = WaitCommEvent(m_hCommHandle,&dwEvent,&m_overlapped_read[N]);
	
	if (m_dwLastError!=0)
	{
		char *errstr = GetErrorString(m_dwLastError);
		TRACE("\r\nRxComm: %s", errstr);
	}
	if ((!ioresult1)&&(m_dwLastError!=ERROR_IO_PENDING))
	{
		com_object::com_error(_T("Unable to read from"),0);
	}
}

void com_object::read_buffer ()
{
	int N = 0;
	int i,j,k;
	char *msg_buf;
	char *rxbuff = (char*) (m_pRxBuff[N]);
	bool read_status;
	read_status = RxCheck();
	while (read_status==true)
	{
		for (i=0,j=0;i<RxBuffSize;i++)
		if (rxbuff[i]!=0)
			j++;
		msg_buf = new char[j+1];
		for (i=0,k=0;i<RxBuffSize;i++)
		if (rxbuff[i]!=0)
		{
			msg_buf[k++]=rxbuff[i];
		}
		msg_buf[k]= '\0';
#ifdef DEBUG_OVERLAPPED_RX
		CComThread::ConsoleDebugOutput 
			("\r\nCComThread::ReadBuffer sendding %d bytes to GUI",k);
#endif
		memset((void*) (m_pRxBuff[N]),0,RxBuffSize);
		AfxGetApp()->m_pMainWnd->
		PostMessage(WM_RECIEVE_SERIAL_DATA,
			(WPARAM)msg_buf,0);
		if (j==0)
			break;
		RxComm ();
	}
}

char *com_object::GetErrorString(int id)
{
	char *str;
	bool m_bFatalError = false;
	switch (id)
	{
	case ERROR_IO_PENDING:
		str = "COM ERROR: IO_PENDING";
		m_bFatalError = false;
		break;

	case ERROR_INVALID_HANDLE:
		str = "COM ERROR: INVALID_HANDLE";
		m_bFatalError = true;
		break;

	case ERROR_ACCESS_DENIED:
		str = "COM ERROR: ACCESS_DENIED";
		m_bFatalError = true;
		break;

	default:
		str = "Unknown Error!!!";
		m_bFatalError = false;
		break;
	}
	return str;
}

void prop_serial::start_debug ()
{
	m_bAbortMode = false;
	m_bDebugActive = true;
}

// Start DEBUG
#if 0
void prop_serial::StartDebug()
{
	CString s;
	try {
		// disallow abort
		m_bAbortMode = false;
		// check hardware
		open_com();
		// allow abort
		m_bAbortMode = true;
		// Operate DEBUG
		OperateDebug();
	}
	catch (...)
	{
		// error, result is false, allow abort
		m_bAbortMode = true;
	}
}
#endif

// Operate DEBUG
#if 0
void prop_serial::OperateDebug()
{
	TDebugDisplayDoc DebugForm;
	m_bDebugActive = true;
	DebugForm.Show();
	DebugForm.ResetDisplays();
#if 0
	while (m_bDebugActive)
	{
		while (RxCheck())
			DebugForm.ChrIn(RxGet());
		Application.ProcessMessages();
		Sleep(10);
	}
#endif
	close_com ();
}
#endif