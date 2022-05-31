#pragma once
#include"Command.h"

void CdCommand::Execute(VirtualDisk* virtualdisk)
{

	// �� / �ָ����,this->CommandOrder��������ֵ��ʱ�򣬵ڶ���Ϊ·��
	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);

	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))    //  C:/a/b/c  ����·��  ���� cd  /a/b/c  �� /��ͷ
		{
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
		}
		else if (parapath[i] == ".")
		{
			continue;
		}
		else
		{
			//Ŀ¼������,����
			if (containdirandfile.find(parapath[i]) == containdirandfile.end())
			{
				Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
				return;

			}
			//��map���ҵ������֣������ж����ļ�����Ŀ¼
			if (containdirandfile.find(parapath[i]) != containdirandfile.end())
			{
				//������ļ�,����,˵��û�����Ŀ¼
				if (containdirandfile[parapath[i]]->GetFType() == "File")
				{
					Error::CannotFindPathError(containdirandfile[parapath[i]]->GetPath() + "/" + parapath[i]);
					return;
				}
				//�����Ŀ¼
				else
				{
					//��ȡ��Ŀ¼������ת��ΪĿ¼����
					searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
					containdirandfile = searchdir->GetFileOrDir();
				}
			}
		}
	}
	virtualdisk->SetLocalDir(searchdir);
}