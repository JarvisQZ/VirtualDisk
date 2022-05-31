#pragma once
#include"Command.h"

bool DelCommand::IsCommandCorrect()
{

	//�жϲ����Ƿ�Ϸ�
	if (this->CommandOrder.size() == 1)
	{
		Help::HelpDelCommand();
		return false;
	}
	else if (this->CommandOrder.size() == 2)
	{
		this->PathSet.emplace_back(this->CommandOrder[1]);
		return true;
	}
	//�������ڵ�������
	else
	{
		//����������ʱ���ڶ�����/s ��ʾ�����������ļ�����Ŀ¼������
		if (this->CommandOrder[1] == "/s")
		{
			//�ݹ�ɾ���ļ�
			this->Recursion = true;
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 2, this->CommandOrder.end());
		}
		//���û��/s ֮����·��
		else
		{
			this->PathSet.insert(this->PathSet.begin(), this->CommandOrder.begin() + 1, this->CommandOrder.end());
		}

	}

	return true;
}

void DelCommand::SpecialPathExecute(vector<string> parapath, int index, Dir* targetdir)
{
	if (index == parapath.size() - 1)
	{
		//�����Ҫ�ݹ�ɾ��
		if (this->Recursion)
		{
			Dir::DeleteFileOfDirRecursion(targetdir);
		}
		//���ݹ�ֻɾ����ǰĿ¼���ļ�
		else
		{
			targetdir->DeleteFileFromDir();
		}
	}
}

void DelCommand::DeleteFileInPath(VirtualDisk* virtualdisk, string path)
{

	vector<string> parapath = FileHelper::GetSplitPathName(path);
	Dir* searchdir = virtualdisk->GetLocalDir();
	map<string, FileStruct*> containdirandfile = searchdir->GetFileOrDir();
	for (unsigned int i = 0; i < parapath.size(); i++)
	{
		//����·��
		if ((i == 0) && (parapath[i] == "C:" or parapath[i] == ""))   //  C:/a/b/c  ����·��  ���� del  /a/b/c  �� /��ͷ
		{
			//��ȡ��Ŀ¼����
			searchdir = VirtualDisk::GetRootDir();
			containdirandfile = searchdir->GetFileOrDir();
			//��������� C:
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else if (parapath[i] == "..")  //  ../a/b/c  �� .. ��ͷ������
		{
			//��ȡ��ǰĿ¼��֮���Ϊ��Ŀ¼
			searchdir = searchdir->GetFatherDir();
			containdirandfile = searchdir->GetFileOrDir();
			//��������� .
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else if (parapath[i] == ".")
		{
			if (i != parapath.size() - 1)
			{
				continue;
			}
			//��������� .
			this->SpecialPathExecute(parapath, i, searchdir);
		}
		else
		{
			//�ļ�����ǰ��Ŀ¼        a/b/c   ������a b
			if (i != parapath.size() - 1)
			{
				if (containdirandfile.find(parapath[i]) == containdirandfile.end())
				{
					Error::CannotFindPathError(searchdir->GetPath() + "/" + searchdir->GetName() + "/" + parapath[i]);
					return;
				}
				else
				{
					//�ҵ�������,�����ļ�������
					if (containdirandfile[parapath[i]]->GetFType() == "File")
					{
						Error::CannotFindFileError(searchdir->GetPath() + "/" + searchdir->GetName(), parapath[i]);
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
					//�ҵ���ͬ����Ŀ¼,�ݹ�ɾ����Ŀ¼�������ļ������ݲ���
					if (containdirandfile[parapath[i]]->GetFType() == "Dir")
					{
						//�ȱ任Ŀ¼��path�ϵ�Ŀ¼
						searchdir = dynamic_cast<Dir*>(containdirandfile[parapath[i]]);
						containdirandfile = searchdir->GetFileOrDir();
						//�����Ҫ�ݹ�ɾ��
						if (this->Recursion)
						{
							Dir::DeleteFileOfDirRecursion(searchdir);
						}
						//���ݹ�ֻɾ����ǰĿ¼���ļ�
						else
						{
							searchdir->DeleteFileFromDir();
						}
					}
					//������ļ�����ɾ��
					else
					{
						//ɾ������
						delete containdirandfile[parapath[i]];
						//ָ���
						containdirandfile[parapath[i]] = nullptr;
						//��map���Ƴ�,ֻ����ʱ�����ɾ���ˣ������Ļ�û��ɾ��,��Ϊ������ָ��
						containdirandfile.erase(parapath[i]);
						searchdir->RemoveFromMap(parapath[i]);
					}
				}
			}
		}
	}
}