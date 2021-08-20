  #pragma once
//#include <cstdlib>
//#include <vector>

//#define DEBUG_SERIAL_TX 1
#define HARDWARE_MISMATCH		(1)
#define HARDWARE_UNRESPONSIVE	(2)

#define DEFAULT_BAUD_RATE		(3000000)
#define DEFAULT_PORT_SPEED0	"115200,n,8,1"
#define DEFAULT_PORT_SPEED1	"230400,n,8,1"
#define DEFAULT_PORT_SPEED2	"460800,n,8,1"
#define DEFAULT_PORT_SPEED3	"921600,n,8,1"
#define DEFAULT_PORT_SPEED4	"1000000,n,8,1"
#define DEFAULT_PORT_SPEED5	"2000000,n,8,1"
#define DEFAULT_PORT_SPEED6	"3000000,n,8,1"
#define DEFAULT_PORT_SPEED7	"12000000,n,8,1"

#define _DEFAULT_PORT_SPEED_ DEFAULT_PORT_SPEED7

#if 0
#define HAS_CONSOLE YES
#endif
#define MEGABYTE  (1048576)
#define COM_THREAD_POLL_INTERVAL (10)
#define PROPELLER_RX_TIMEOUT (2000)
#define PROPELLER_BAUD_RATE_SYNC _T("\n>\n>\n>\n")
#define PROPELLER_CHECK_STRING	_T("> Prop_Chk 0 0 0 0 ")
#define PROPELLER_FORTH  "> \x1b"

extern int port_speeds[];
// CComThread

class TelnetServer;

class _com_error
{
protected:
	int m_err;

public:
	_com_error() { m_err=-1; }
	_com_error(int val) { m_err=val; };
	_com_error &operator = (int val){ m_err=val; return (*this); }
	int get() { return m_err; }
};

class CTerminalThread: public CWinThread
{
private:
	TelnetServer *m_server;
	bool	m_status;
	bool	m_bConnected;
	int		m_iBytesTransmitted;
	int		m_iBytesRecieved;

	DECLARE_DYNCREATE(CTerminalThread)

protected:
	CTerminalThread();           // protected constructor used by dynamic creation
	virtual ~CTerminalThread();
	DECLARE_MESSAGE_MAP()

public:
	virtual int Run();
	virtual BOOL InitInstance();
	virtual BOOL OnIdle (LONG lCount);
	virtual int ExitInstance();
	afx_msg	void CreateSession(WPARAM,LPARAM);
	afx_msg	void CloseSession(WPARAM,LPARAM);
	afx_msg	void ReadBuffer(WPARAM,LPARAM);
	afx_msg	void TransmitString(WPARAM,LPARAM);
	afx_msg	void OnTransmitData(WPARAM,LPARAM);
};


class CComThread: public CWinThread
{
public:
	int	GetComPort(void);
	static CString get_com_string (int);
	static void UpdateProgress (size_t, size_t);
	static void OutputDebugString (const char *);
	static void OutputDebugString (const char *, char *);
	static void OutputDebugString (const char *, int);
	static void ConsoleDebugOutput (const char *str, int val);
	static void EnumerateComPorts(CComThread*);
	static void	FindHardware(CComThread*);
	static bool	GetHardwareVersion(CComThread*);
	static void	LoadHardwareAndRun(CComThread*, CString);
	int				m_baud_rate;

private:
// windwo to recieve progress notifications.
	CPropertyPage	*m_pNotify;
	prop_serial		m_comunit;
	int				m_portId;
	int				m_iBytesTransmitted;
	int				m_iBytesRecieved;
// actual Tx and Rx buffer pages are
// part of the prop_serial object
	size_t			m_iPageSize;
	size_t			m_pages_allocated;
// FileBuffer Pages are here ...
// up to 1MB physical by default
	CString			m_strFileName;
	LPVOID			m_fileBuffer;
	DECLARE_DYNCREATE(CComThread)

protected:
	CComThread();
	virtual		~CComThread();
	virtual		BOOL OnIdle (LONG lCount);
	LPVOID		allocate_buffers(size_t);
	size_t		get_page_size();
	void		import_file ();
	CString		GetPortStatus(void);	
	void		poll_rx(void);
	bool		TransmitString(void);

protected:
	DECLARE_MESSAGE_MAP()
	virtual int Run();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	
	afx_msg void OnFindHardware (WPARAM,LPARAM);
	afx_msg void OnGetHeardwareVersion (WPARAM,LPARAM);
	afx_msg void OnGetVersionString (WPARAM,LPARAM);
	afx_msg void OnSetPortNum (WPARAM,LPARAM);
	afx_msg void OnSetBaudRate (WPARAM,LPARAM);
	afx_msg void OnCreateSession (WPARAM,LPARAM);
	afx_msg void OnCloseSession (WPARAM,LPARAM);
	afx_msg void OnSendResetCmd (WPARAM,LPARAM);
	afx_msg void OnTransmitString (WPARAM, LPARAM);
	afx_msg void OnTransmitData (WPARAM,LPARAM);
	afx_msg void OnLoadHardware (WPARAM,LPARAM);
	afx_msg void OnStartDebug (WPARAM,LPARAM);
};


