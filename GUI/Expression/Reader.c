
#include <string.h>

#include "Reader.h"

//< name> val </>
#define BEGIN_FLAG	'<'
#define	BEGIN_STR	"<"

#define END_FLAG	'>'
#define ATT_SPLIT_CHAR	' '
#define TAIL_FLAG	'/'

static char *Eliminate_char( char *str, char c);


//����ȥ��һ��< xxx>֮����׵�ַ
char *RemoveHead( char *context)
{
	char	*pp;
	pp = strchr( context, BEGIN_FLAG);
	if( pp == NULL)
	{
		pp = context;
		goto exit;
	}
	
	while(1)
	{
		if( *pp == TAIL_FLAG)
		{
			//�ⲻ��һ��ͷ�������ǽ�β
			pp = context;
			break;
			
		}
		if( *pp == END_FLAG)
		{
			pp ++;
			break;
		}
		if( *pp == '\0')
		{
			pp = context;
			break;
			
		}
		pp ++;
		
	}
	
//	pp = strchr( context, END_FLAG);
//	if( pp == NULL)
//		goto exit;
	
	
	
	exit:
		return pp;
	
}

//����ȥ��һ��</XXX>֮����׵�ַ
char *RemoveTail( char *context, char *tailName, int size)
{
	char	*pp = context;
	char 	*newpp = context;
	char 	i = 0;
	while(1)
	{
		pp = strchr( pp, BEGIN_FLAG);
		if( pp == NULL)
			goto exit;
		pp ++;
		if( *pp == TAIL_FLAG)
			break;
	}
	
	//ȥ���ո�
	pp = Eliminate_char( pp, ' ');
	
	i = 0;
	while(1)
	{
		if( ( pp[ i] == END_FLAG) || ( pp[ i] == '\0'))
			break;
		if( tailName && i < (size -1))
		{
			tailName[ i] = pp[i];
		}
		
		i++;
			
	}
	if( tailName)
		tailName[ size -1] = '\0';
	pp += i;

	if(  *pp == '\0')
		newpp = pp;
	else
		newpp = pp + 1;
	
	exit:
		return newpp;
	
}

//���ػ�ȡ�����ֵĳ���
int GetName( char *context, char *name, int nameLen)
{
	char	*pp;
	int 	idx = 0;
	
	
	
	
	pp = strchr( context, BEGIN_FLAG);
	if( pp == NULL)
		goto exit;
	pp++;
	//ȥ���ո�
	pp = Eliminate_char( pp, ' ');

//	if( *pp == END_TARGET)
//		goto exit;
	//��һ���ռ��������
	nameLen --;
	
	//ȡֵ
	while(1)
	{
		if( idx >= nameLen)
			break;
		
		if( pp[ idx] == ATT_SPLIT_CHAR)
			break;
		if( pp[ idx] == END_FLAG)
			break;
		
		name[idx] = pp[ idx];
		idx ++;
		
	}
	
	name[ idx] = '\0';
	
	exit:
	return idx;
}	

//�ú���������һ����ȫ��ȡ��
//att: c=red oth=someval

int GetAttribute( char *context, char *att, int attLen)
{
	char		*pp;
	short 		idx = 0;
	char		hasAtt = 0;
	uint8_t		count = 0;
	pp = strchr( context, BEGIN_FLAG);
	if( pp == NULL)
		goto exit;
	pp++;
	
	memset( att, 0, attLen);
	
	//ȥ���ո�
	pp = Eliminate_char( pp, ' ');
	
	//�ҵ����Էָ���
	idx = 0;
	hasAtt = 0;
	count = 0;
	
	//��һ���ռ��������
	attLen --;
	
	while(1)
	{
		if( count > attLen)
			break;
		
		if( pp[ idx] == ATT_SPLIT_CHAR)
		{
			hasAtt = 1;		//�ڽ�����֮ǰ�ҵ������Էָ���
		}
		
		
		
		if( pp[ idx] == END_FLAG)
		{
			
			break;
		}
		else
		{
			if( hasAtt)
				att[ count ++] = pp[ idx];
			
		}
		
		
		idx++;
		
		
	}
	
	
	
	exit:
	return count;
}

//< name ... > vale </>
void *GetNameVale( char *context, char *name, char **value, int *len)
{
	
	char	*pp;
	pp = strstr((const char*)context, name);
	if( pp == NULL)
	{
		*len = 0;
		return context;
	}
	while( *pp != END_FLAG)
		pp ++;
	pp ++;
	*value = pp;
//	while( *pp++ != BEGIN_FLAG)
//	{
//			(*len) ++;
//	}
	*len =  strcspn( pp, BEGIN_STR);	
	
//	while( *pp != END_FLAG)
	pp += *len;
	
	
	return pp;
}


static char *Eliminate_char( char *str, char c)
{
	while( *str == c && *str != '\0')
		str ++;
	
	return str;
	
}

