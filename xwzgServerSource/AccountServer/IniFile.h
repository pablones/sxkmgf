// INI�ļ�
// �ɽ��ޣ�2001.11.19


#ifndef INIFILE_H
#define INIFILE_H


#define INIWORDSIZE		512			//? INI ��ʹ�õĴ��Ŀռ䳤��


class	CIniFile
{
public:
	CIniFile(const char * pFilename, const char * pSection);
	~CIniFile();
public:
	void	SetSection(const char * pSect);
	bool	GetString(const char * pKey, char * bufString);
	int		GetInt(const char * pKey);
protected:
	char m_bufFilename[INIWORDSIZE];
	char m_bufSection[INIWORDSIZE];
};


#endif // INIFILE_H


/*	INI�ļ���ʽ�淶

[SECTION]

KEY1=STRING

KEY2=INT

; COMMENT


	˵����

	SECTION�У�	����Ϊ����ʶ�����������ִ�Сд��([)���붥�У�([])�м䲻���пհס�(])������Դ�ע�͡�

	KEY�У�		KEY���붥�У�����Ϊ����ʶ�����������ִ�Сд��
				(=)ǰ�����пհף���������
				STRING������(")��ס������ᱻ�����ַ�����һ���֡�β�ջᱻɾ����
				INTΪʮ��������
				STRING��INT������ע�͡�

	COMMENT��	(;)���붥�У��������������塣
	���У�		�����塣



//*/