#pragma once
#include"head.h"

//��������
class Error
{
public:
	//�ļ���Ŀ¼���ư����Ƿ��ַ�
	void static NameError()
	{
		cout << "   ����:" << endl;
		cout << "        �����в��ܰ��������ַ� \\ / : * ? \" < > | " << endl;
	}
	void static MemoryMallocError()
	{
		cout << "   ����:" << endl;
		cout << "        �ڴ�ռ䲻�㣡" << endl;
	}
	//�½���Ŀ¼�����ļ�������
	void static DirNameSaveAsFileError()
	{
		cout << "   ����:" << endl;
		cout << "        ָ�����ļ��к����е�ĳ���ļ���������ָ����������" << endl;
	}
	//�½����ļ���Ŀ¼��������
	void static FileNameSaveAsDirError()
	{
		cout << "   ����:" << endl;
		cout << "        ָ�����ļ������Ѵ��ڵ��ļ�����������ָ����������" << endl;
	}
	// ϵͳ�Ҳ���·������
	void static CannotFindPathError(string errorpath)
	{
		cout << "   ����:" << endl;
		cout << "        ϵͳ�Ҳ���·�� " << errorpath << " ,��Ϊ��·��������" << endl;
	}
	//·����ʽ����  ·���а����˿�  a//b/c
	void static PathFormatError()
	{
		cout << "   ����:" << endl;
		cout << "        ·��������Ϊ���ַ���" << endl;
	}
	//Ŀ¼�Ѿ����ڣ����½�Ŀ¼
	void static DirSameNameError(string path, string name)
	{
		cout << "   ����:" << endl;
		cout << "        ·�� " << path << " ���Ѿ���������" << name << " ���ļ���,����������" << endl;
	}
	//�����ļ�ʱ·��������
	void static TouchNoDirFileError(string path)
	{
		cout << "   ����:" << endl;
		cout << "        ·�������ڣ�����ͨ��mkdir����Ŀ¼:" << path << endl;
	}
	void static CannotFindFileError(string path, string filename)
	{
		cout << "   ����:" << endl;
		cout << "        ·�� " << path << " ��û����Ϊ" << filename << " ���ļ�" << endl;
	}
	void static CannotFindDirError(string path, string dirname)
	{
		cout << "   ����:" << endl;
		cout << "        ·�� " << path << " ��û����Ϊ" << dirname << " ��Ŀ¼" << endl;
	}
	void static RenameSaveAsDirError(string dirname)
	{
		cout << "   ����:" << endl;
		cout << "        ��Ŀ¼�´�����Ϊ" << dirname << "���ļ���,��ָ����������" << endl;
	}
	void static RenameSaveAsFileError(string filename)
	{
		cout << "   ����:" << endl;
		cout << "        ��Ŀ¼�´�����Ϊ" << filename << "���ļ�,��ָ����������" << endl;
	}
	void static CreateSelfError()
	{
		cout << "   ����:" << endl;
		cout << "        ��Ŀ¼���ļ� . �Ѿ�����" << endl;
	}
	void static CreateFatherSelfError()
	{
		cout << "   ����:" << endl;
		cout << "        ��Ŀ¼���ļ� .. �Ѿ�����" << endl;
	}
	void static VisitRootError()
	{
		cout << "   ����:" << endl;
		cout << "        �ܾ�����" << endl;
	}
	void static CannotVisitError()
	{
		cout << "   ����:" << endl;
		cout << "        ��һ����������ʹ�ô��ļ��������޷�����" << endl;
	}
	void static DeleteDirNotEmptyError()
	{
		cout << "   ����:" << endl;
		cout << "        Ŀ¼���ǿյ�" << endl;
	}
	void static InvalidDirNameError()
	{
		cout << "   ����:" << endl;
		cout << "        Ŀ¼������Ч" << endl;
	}
	//û���ҵ���ͨ�����ƥ����ļ�
	void static NoFileMatchedError()
	{
		cout << "   ����:" << endl;
		cout << "        û��ƥ����ļ�" << endl;
	}
	//���Ƶ�ʱ������
	void static FileNameAlreadyExistError(string filename)
	{
		cout << "   ����:" << endl;
		cout << "        ��Ϊ" << filename << "�Ѿ�����" << endl;
	}
	void static NoMatchedFile()
	{
		cout << "   ����:" << endl;
		cout << "        δƥ�䵽�κ��ļ�" << endl;
	}
};
