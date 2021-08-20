
// note that these buffers are independent
// of the buffering capacity withing the
// FTDI chip, or the UART FIFO's or whatever
// else might exist at the hardware level.

#define		MAX_BUFFERS	(512)
#define		TxBuffSize	(0x90)
#define		RxBuffSize	(0x90)

class com_object
{
protected:
	HANDLE		m_hCommHandle;
	int			m_iErrorCount;	
	bool		m_bAbortMode;
	bool		m_bFatalError;

private:
	DWORD		m_dwTxBuffPos;
	DWORD		m_dwLastError;
	DWORD		m_dwRxBuffStart;
	DWORD		m_dwRxBuffEnd;
	DWORD		m_dwBytesRead;
	DWORD		m_dwBytesWritten;	
	int			m_iCurrentOverlapRx;
	int			m_iCurrentOverlapTx;
	int			m_iNextOverlapRx;
	int			m_iNextOverlapTx;
	LPVOID		m_pRxBuff[MAX_BUFFERS];
	LPVOID		m_pTxBuff[MAX_BUFFERS];
	OVERLAPPED	m_overlapped_read[MAX_BUFFERS];
	OVERLAPPED	m_overlapped_write[MAX_BUFFERS];
	CString		m_strCommId;
	
public:
	int			m_iPortId;
	DCB			m_CommDCB;
	size_t		m_obj_file_size;
	size_t		m_bytes_uploaded;
	int			m_port_speed;
	bool		m_bOverlapped;
	HANDLE		m_hEventRx;
	HANDLE		m_hEventTx;

public:
	com_object();
	void config_buffers(LPVOID p_addr);
	void reset_dcb (LPDCB pDCB, bool init);
	void set_overlapped (bool arg);
	bool open_com();
	bool close_com();
	bool is_active() throw ();
	void read_buffer();
	bool RxCheck();
	void RxComm();
	void tx_comm();
	void tx_char(unsigned char);
	void tx_string(CString);
	void tx_string(const char*);
	void tx_base64(unsigned char);
	void tx_hex(unsigned char x);
	int GetComPortNumber() throw();

protected:
	void com_error (CString, int);
	void make_com_string(int id);
	void reset_com_timeouts ();
	void reset_buffers () throw();
	void flush_rx (bool z);
	static char *GetErrorString(int) throw();
	virtual void abort();
	unsigned char RxByte();

private:
#if 0
	LPOVERLAPPED_COMPLETION_ROUTINE
#endif
	static VOID CALLBACK write_callback
		(DWORD, DWORD, LPOVERLAPPED);
	static VOID CALLBACK read_callback
		(DWORD, DWORD, LPOVERLAPPED);
	static LPOVERLAPPED_COMPLETION_ROUTINE RxProcAddr;
	static LPOVERLAPPED_COMPLETION_ROUTINE TxProcAddr;
};

class prop_serial: public com_object
{
private:
	char		m_cVersion;
	bool		m_bVersionMode;
	bool		m_bDebugActive;

public:
	CString		m_strVersion;
	
public:
	prop_serial();
	bool reset_hardware();
	void reset_flags () throw();
	bool detect_hardware ();
	bool set_version_string();
	bool open_forth ();
	void load_hardware(LPVOID,int);
	void start_debug ();

protected:
	bool version_check();
	bool HardwareFound();
	void OperateDebug();
};
