#pragma once
#include"Command.h"

bool MkdirCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() != 2)
	{
		Help::HelpMkdirCommand();
		return false;
	}
	else
	{
		// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��
		vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

		//�ж�·�������Ƿ�Ϸ�  �Ƿ���������ַ�������   *a/b/c   a/*b/c   C:/*a/b/c  ../*a/b/c  ./a/b/c
		for (auto name : parapath)
		{
			if (name == "C:")
				continue;
			//·���а����� ��  a//b/c
			if (name == "" && name != parapath[0])
			{
				Error::PathFormatError();
				return false;
			}
			if (!FileHelper::IsContainIllegalChar(name))
			{
				Error::NameError();
				return false;
			}
		}
		return true;
	}
}

void MkdirCommand::Execute(VirtualDisk* virtualdisk)
{

	// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ���� mkdir  /a/b/c  �� /��ͷ
		{
			if (i == parapath.size() - 1)
			{
				Error::VisitRootError();
				return;
			}
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateFatherSelfError();
				return;
			}
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			if (i == parapath.size() - 1)
			{
				Error::CreateSelfError();
				return;
			}
			continue;
		}
		else
		{
			//Ŀ¼������,���½�Ŀ¼
			if (containdirandfile.find(parapath[i]) == containdirandfile.end())
			{
				//����Ǹ�Ŀ¼�µ��ļ����ļ��У�·����C:
				Dir* NewDir;
				if (searchdir == VirtualDisk::GetRootDir())
				{
					NewDir = new Dir(parapath[i], searchdir->GetPath(), searchdir);
				}
				else
				{
					//������Ǹ�·���µ�Ŀ¼��·���Ǹ�Ŀ¼��һ��
					NewDir = new Dir(parapath[i], searchdir->GetPath() + "/" + searchdir->GetName(), searchdir);
				}

				//�½�Ŀ¼������ӽ�map�У����������޸�ʱ��
				searchdir->FileOrDirAdd(parapath[i], NewDir);
				searchdir = NewDir;
				containdirandfile = searchdir->GetFileOrDir();
				//delete NewDir;

			}
			//��map���ҵ������֣������ж����ļ�����Ŀ¼
			else
			{
				//������ļ�,����,�����½����ļ�������Ŀ¼
				if (containdirandfile[parapath[i]]->GetFType() == "File")
				{
					Error::DirNameSaveAsFileError();
					return;
				}
				//�����Ŀ¼
				else
				{
					//�ҵ���Ŀ¼�����֣��½�Ŀ¼�����ֺ�����ͬ
					if (i != parapath.size() - 1)
					{
						//��ȡ��Ŀ¼������ת��ΪĿ¼����
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
					else
					{
						Error::DirSameNameError(searchdir->GetPath(), parapath[i]);
						return;
					}
				}
			}
		}
	}
}