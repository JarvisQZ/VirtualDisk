#pragma once
#include"Command.h"

void CatCommand:: Execute(VirtualDisk* virtualdisk)
{

	vector<string> parapath = FileHelper::GetSplitPathName(this->CommandOrder[1]);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (size_t i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))   //  C:/a/b/c  ����·��  ���� del  /a/b/c  �� /��ͷ
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
			//�ļ�����ǰ��Ŀ¼        a/b/c   ������a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					//��Ŀ¼
					if (searchdir->GetName() == "")
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
					}
					else
					{
						Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

					}
					return;
				}
				else
				{
					//�ҵ�������,�����ļ�������
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						//��Ŀ¼
						if (searchdir->GetName() == "")
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + parapath[i]);
						}
						else
						{
							Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);

						}
						return;
					}
					else
					{
						//��ȡ��Ŀ¼������ת��ΪĿ¼����
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
					}
				}
			}
			else
			{
				//û���ļ�����Ŀ¼���������,����
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
					return;
				}
				else
				{
					//�ҵ���ͬ����Ŀ¼,�����ļ�������
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
						return;
					}
					//������ļ��������Ϊ���������
					else
					{
						File* file = dynamic_cast<File*>(containdirandfile[parapath[i]]);
						file->PrintContent();
					}
				}
			}
		}
	}
}