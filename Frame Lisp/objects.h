
class object;
typedef object *(*object_factory[])(void *);

#if 0
typedef enum
{
//	UNDEFINED1,
	STANDARD,
	DECLARED,
	SPECIAL,
} DECLKIND;
#endif

#if 1
class DECLKIND
{
public:
	typedef enum
	{
		STANDARD,
		DECLARED,
		SPECIAL,
		UNDEFINED,
	} decl_type;

protected:
	decl_type m_decl;

public:
	DECLKIND () {}
	DECLKIND (decl_type arg)
	{
		ASSERT(arg!=UNDEFINED);
		m_decl = arg;
	}
	DECLKIND &operator = (decl_type arg)
	{
		ASSERT(arg!=UNDEFINED);
		m_decl = arg;
		return *this;
	}
	operator decl_type () 
	{
		ASSERT(m_decl!=UNDEFINED);
		return m_decl;
	}
	bool operator == (decl_type arg)
	{
		bool result;
		ASSERT(arg!=UNDEFINED);
		ASSERT(m_decl!=UNDEFINED);
		if (m_decl==arg)
			result = true;
		else
			result = false;
		return result;
	}
	bool operator != (decl_type arg)
	{
		bool result;
		ASSERT(arg!=UNDEFINED);
		ASSERT(m_decl!=UNDEFINED);
		if (m_decl!=arg)
			result = true;
		else
			result = false;
		return result;
	}
};
#endif

template <typename T0>
class selector
{
protected:
	T0 S0;

public:
	operator T0 ()
	{
		return S0;
	}
	T0 &operator = (T0 arg)
	{
		S0 = arg;
		return S0;
	}
	bool operator == (T0 arg)
	{
		return (S0==arg);
	}
	bool operator != (T0 arg)
	{
		return (S0!=arg);
	}
};

template <typename PTR>
class debug_ptr
{
private:
	PTR m_ptr;

public:
	operator PTR ()
	{
		return m_ptr;
	}
	PTR &operator = (PTR arg)
	{
		ASSERT ((DWORD)arg!=0x01);
		m_ptr = arg;
		return m_ptr;
	}
	PTR &operator -> ()
	{
		return m_ptr;
	}
};

typedef debug_ptr<STP> type_ptr;

class structform
{
public:
	ADDRRANGE	m_size;
	selector<STRUCTFORM> m_form;

public:
	inline void resize (size_t sz)
	{
		m_size = (int) sz;
	}
	inline size_t size()
	{
		size_t sz = m_size;
		return m_size;
	}
	inline STRUCTFORM form()
	{
		return m_form;
	}
	inline bool is_scalar()
	{
		bool result;
		if (form()==SCALAR)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_long()
	{
		bool result;
		if (form()==LONGINT)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_pointer()
	{
		bool result;
		if (form()==POINTER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_power()
	{
		bool result;
		if (form()==POWER)
			result = true;
		else
			result = false;
		return result;
	}
	inline bool is_file()
	{
		bool result;
		if (form()==FILES)
			result = true;
		else
			result = false;
		return result;
	}
	bool is_packed();
	bool is_string();
	void set_packed(bool val);
	void set_string(bool val);
};

class structform1;
class object: public structform
{
public:
	class undefined;
	class scalar;
	class subrange;
	class pointer;
	class power;
	class arrays;
	class records;
	class files;
	class tagfld;
	class variant;

public:
	object (STRUCTFORM);
	void *operator new (size_t,void*);
	operator object* ();
	static object *allocate (STRUCTFORM S);
	static void debug1 (structure *stp);
	bool operator == (scalar &s);
	
protected:
	static size_t object_size (STRUCTFORM S);
	static object *(*ftab[10])(void *);
	template <STRUCTFORM X>
	class structform1: public structform
	{
	public:
		structform1<X>()
		{
			m_form = X;
		}
	};	
};

class object::undefined: public structform1<UNDEFINED>
{
public:
	static object *construct (void *ptr);
};

class object::scalar: public structform1<SCALAR>
{
public:
	static object *construct (void *ptr);

public:
	selector<DECLKIND> SCALKIND;
	union
	{
		CTP	DECLARED;
		CTP	FCONST;
	};
};

class object::subrange: public structform1<SUBRANGE>
{
public:
	static object *construct (void *ptr);

public:
	STP		RANGETYPE;
	VALU	MIN, MAX;	
};

class object::pointer: public structform1<POINTER>
{
public:
	static object *construct (void *ptr);

public:
	STP	ELTYPE;	
};

class object::power: public structform1<POWER>
{
public:
	static object *construct (void *ptr);

public:
	STP	ELSET;
};

class object::arrays: public structform1<ARRAYS>	
{
public:
	static object *construct (void *ptr);

public:
	explicit object::arrays (STP);

	STP	AELTYPE;
	STP INXTYPE;
	selector<bool> AISPACKD;
	struct
	{
		BITRANGE ELSPERWD;
		BITRANGE ELWIDTH;
		selector<bool> AISSTRNG;
		struct
		{
			RANGE<1,255> MAXLENG;
		};
	};
};

class object::records: public structform1<RECORDS>
{
public:
	static object *construct (void *ptr);
	
public:
	CTP	FSTFLD;
	STP	RECVAR;
};

class object::files: public structform1<FILES>
{
public:
	static object *construct (void *ptr);

public:
	STP	FILTYPE;	
};

class object::tagfld: public structform1<TAGFLD>
{
public:
	static object *construct (void *ptr);

public:
	CTP	TAGFIELDP;
	STP	FSTVAR;
};

class object::variant: public structform1<VARIANT2>
{
public:
	static object *construct (void *ptr);

public:
	STP	NXTVAR;
	STP	SUBVAR;
	VALU VARVAL;	
};