#ifndef _MYFIELD_H
#define _MYFIELD_H
/////////////////////////////////////////////////////////////////////////////
class CCriticalSection;
class CMyRecordset;

class CMyField
{
// Constructor
public:
	CMyField();
	virtual ~CMyField();

public:
	MYHEAP_DECLARATION(s_heap)

public:
#ifdef	MULTITHREAD_SAFE
	static CCriticalSection	m_xCtrl;
#endif

protected:
	unsigned long			m_ulType;
	union
	{
		unsigned char		m_ucVal;	//TINY			//★ 不必要，可直接使用LONG
		char				m_cVal;		//TINY			//★ 不必要，可直接使用LONG
		short int			m_nVal;		//SHORT			//★ 不必要，可直接使用LONG
		int					m_iVal;		//INT			//★ 不必要，可直接使用LONG
		long				m_lVal;		//LONG
		unsigned long		m_ulVal;	//UNSIGNED LONG

		float				m_fVal;		//FLOAT
		double				m_dVal;		//DOUBLE
		_int64				m_i64Val;	//int64
	
	};
	char*				m_szVal;	//CSTRING
	int					m_nIndex;
	MYSQL_FIELD			m_Field;
	CMyField *			m_pMyField;		//paled 指向根对象，用于更新。

	friend class	CMyRecordset;
	friend class	CMyRecord;
	friend class	CMyConstRecord;

// Operations
private:
	void Clear();

// Attributes
public:
	void					FieldName(char * szFileName);		
	unsigned int			FieldAttr() const;
	unsigned int			FieldType() const;
	unsigned int			FieldLen()	const;

	void					CopyTo(char * szField, int nLen);
	void					CopyFrom(const char * szField, int nLen);

// Operator
public:
	operator int	()
	{	if ( m_ulType == FIELD_TYPE_SHORT
				|| m_ulType == FIELD_TYPE_TINY
				|| m_ulType == FIELD_TYPE_LONG )
			return m_iVal;
		else return -1;
	};

	operator long()
	{
		if (m_ulType==FIELD_TYPE_SHORT 
				|| m_ulType == FIELD_TYPE_TINY
				|| m_ulType==FIELD_TYPE_LONG)
			return m_lVal;
		else return -1;
	};

	operator unsigned char ()
	{	if (m_ulType==FIELD_TYPE_TINY )
			return m_ucVal;
		else return 0;
	};

	operator char ()
	{	if (m_ulType==FIELD_TYPE_TINY )
			return m_cVal;
		else return 0;
	};

	operator float()
	{	
		if (m_ulType==FIELD_TYPE_DOUBLE ||
			m_ulType==FIELD_TYPE_FLOAT)
			return m_fVal;
		else return 0;
	};

	operator double()
	{
		if (m_ulType==FIELD_TYPE_DOUBLE ||
			m_ulType==FIELD_TYPE_FLOAT)
			return m_dVal;
		else return 0; 

	};

	
	operator unsigned int()
	{
		if (m_ulType == FIELD_TYPE_SHORT 
				|| m_ulType == FIELD_TYPE_TINY
				|| m_ulType == FIELD_TYPE_LONG)
			return m_iVal;
		else return 0; 

	};

	operator unsigned long()
	{
		if (m_ulType == FIELD_TYPE_SHORT 
				|| m_ulType == FIELD_TYPE_TINY
				|| m_ulType == FIELD_TYPE_LONG)
			return m_lVal;
		else return 0; 
	};

	operator unsigned short()
	{
		if (m_ulType == FIELD_TYPE_SHORT 
				|| m_ulType == FIELD_TYPE_TINY
				|| m_ulType == FIELD_TYPE_LONG)
			return m_nVal;
		else return 0; 
	};


#define EXTERN_C_DEFINE

#ifdef	EXTERN_C_DEFINE

	operator LPCTSTR ()			//★★★ BUG: 返回指针，外部可非法写串内部。可能会出现串空间溢出的BUG, 暂未解决
	{
		if (m_ulType==FIELD_TYPE_STRING ||
			m_ulType==FIELD_TYPE_VAR_STRING)
			return (m_szVal);
		else return "";				//paled return NULL
	}
	operator = (LPCTSTR szVal);
	

#else

	operator = (CString strOp);
	operator CString()
	{	CString str;
		switch (m_ulType)
		{
		case FIELD_TYPE_STRING:
		case FIELD_TYPE_VAR_STRING:
				return (m_szVal);
		case FIELD_TYPE_DOUBLE:
		case FIELD_TYPE_FLOAT:
				str.Format("%f",m_fVal);
				break;
		case FIELD_TYPE_TINY:
		case FIELD_TYPE_SHORT:
		case FIELD_TYPE_LONG:
				str.Format("%d",m_lVal);
				break;
		}
		return str;
	};
#endif

	operator = (int nOp);

	operator = (long lOp);
	operator = (unsigned long lOp );

	operator = (float fltOp);
	operator = (double dblOp);

	operator = (unsigned char chOp);
	operator = (char chOp);
};
	
/////////////////////////////////////////////////////////////////////////////
#endif
