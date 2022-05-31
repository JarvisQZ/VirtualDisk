#pragma once
#include"head.h"
#include"FileHelper.h"
#include"Error.h"
//����ģʽ
//��������
//FileStruct
//Dir
//File
//�ļ����ļ��л���
class FileStruct
{
protected:
	//��С
	size_t Size = 0;
	string Name;
	string FType;
	string Path;
	//��Ŀ¼
	FileStruct* FatherDir = nullptr;
	//string Mklink;
	string CreateTime;
	string ModifyTime;
public:
	//Ĭ�Ϲ��캯�������๹�캯�����ã�Ϊ�����ֶθ�ֵ
	FileStruct()
	{
		this->CreateTime = FileHelper::GetNowTimeToString();
		this->ModifyTime = CreateTime;
	}
	//��������
	virtual ~FileStruct()
	{
		FatherDir = nullptr;
		cout << "FileStruct ����������" << endl;
	}

	size_t GetSize()
	{
		return this->Size;
	}
	void SetSize(size_t size)
	{
		this->Size = size;
	}
	//��ȡ�ļ����ļ�����
	string GetName()
	{
		return this->Name;
	}
	void SetCreateTime(string CreateTime)
	{
		this->CreateTime = CreateTime;
	}
	string GetCreateTime()
	{
		return this->CreateTime;
	}
	string GetModifyTime()
	{
		return this->ModifyTime;
	}
	//�����޸�ʱ��
	void SetModifyTime()
	{
		this->ModifyTime = FileHelper::GetNowTimeToString();
	}
	//�����޸�ʱ��
	void SetModifyTime(string ModifyTime)
	{
		this->ModifyTime = ModifyTime;
	}
	//��ȡ·��
	string GetPath()
	{
		return this->Path;
	}
	void SetPath(string Path)
	{
		this->Path = Path;
	}
	//��ȡ����
	string GetFType()
	{
		return this->FType;
	}
	//�޸ĸ�Ŀ¼ָ��
	void SetFatherDir(FileStruct* fatherdir)
	{
		this->FatherDir = fatherdir;
	}
	FileStruct* GetFatherDir()
	{
		return this->FatherDir;
	}
	//�����ļ����ļ�����
	void SetName(string Name)
	{
		this->Name = Name;
	}
	

};


//�ļ���
class Dir :public FileStruct
{
private:
	map<string, FileStruct*> FileOrDir;
public:
	Dir()
	{
		this->Name = "";
		this->FType = "Dir";
		this->Path = "";
		this->FatherDir = nullptr;
	}

	Dir(string Name, string Path, Dir* FatherDir)
	{
		this->Name = Name;
		this->FType = "Dir";
		this->Path = Path;
		if (FatherDir == nullptr)
		{
			this->FatherDir = this;
		}
		else
		{
			this->FatherDir = FatherDir;
		}
	}

	~Dir()
	{
		//DeleteFileAndDirFromDir();
		//FatherDir = nullptr;
		map<string, FileStruct*>().swap(this->FileOrDir);
		cout << "Dir ����������" << endl;
	}

	//��ȡ��Ŀ¼
	Dir* GetFatherDir()
	{
		return dynamic_cast<Dir*>(this->FatherDir);
	}



	//Ŀ¼�������ļ���Ŀ¼
	void FileOrDirAdd(string Name, FileStruct* New)
	{
		//�ļ��в����� ���д������޸ĵ�ǰ�ļ��� �޸�ʱ���ֶ�
		this->FileOrDir[Name] = New;
		this->ModifyTime = FileHelper::GetNowTimeToString();
		this->CountDirSizeUpward();
		//}
	}
	//�����ʱ����Ҫ�ĺ���
	void FileOrDirAdd(FileStruct* New)
	{
		this->FileOrDir[New->GetName()] = New;
	}
	//��ȡ���ǵķ�ʽ
	void FileOrDirAdd(string Name, FileStruct* New, bool Coverold)
	{

		map<string, FileStruct* >::iterator key = this->FileOrDir.find(Name);
		//�ļ�����Ŀ¼�Ѿ�����
		if (key != this->FileOrDir.end())
		{
			if (Coverold)
			{
				cout << "�������ļ�" << this->FileOrDir[Name]->GetPath() + "/" + this->FileOrDir[Name]->GetName() << endl;
				//ɾ���ɵĶ����ͷ�ָ��
				delete this->FileOrDir[Name];
				this->FileOrDir[Name] = nullptr;
				this->FileOrDir.erase(key++);
				//�ļ��в����� ���д������޸ĵ�ǰ�ļ��� �޸�ʱ���ֶ�
				this->FileOrDir[Name] = New;
				this->ModifyTime = FileHelper::GetNowTimeToString();
			}
			else
			{
				cout << "ȡ���˴β���" << endl;
			}

		}
		else
		{
			//�ļ��в����� ���д������޸ĵ�ǰ�ļ��� �޸�ʱ���ֶ�
			this->FileOrDir[Name] = New;
			this->ModifyTime = FileHelper::GetNowTimeToString();
		}
		this->CountDirSizeUpward();
	}

	//ɾ���ļ�
	void DeleteFileFromDir()
	{
		//ֻɾ���ļ�
		for (map<string, FileStruct*>::iterator it = this->FileOrDir.begin(); it != this->FileOrDir.end();)
		{
			if (it->second->GetFType() == "File")
			{
				//ָ��Ŀ¼��ָ���ÿ�
				it->second->SetFatherDir(nullptr);
				//ɾ������
				delete it->second;
				//ָ���
				it->second = nullptr;
				//��map���Ƴ�
				this->FileOrDir.erase(it++);

			}
			else { it++; }
		}
		this->CountDirSizeUpward();
	}

	//ɾ���ļ���Ŀ¼��ɾ������
	void DeleteFileAndDirFromDir()
	{
		//ɾ���ļ���Ŀ¼
		for (map<string, FileStruct*>::iterator it = this->FileOrDir.begin(); it != this->FileOrDir.end();)
		{
			//ָ��Ŀ¼��ָ���ÿ�
			it->second->SetFatherDir(nullptr);
			//ɾ������
			delete it->second;
			//ָ���
			it->second = nullptr;
			//��map���Ƴ�
			this->FileOrDir.erase(it++);
		}
		this->CountDirSizeUpward();
	}

	void RemoveFromMap(string Name)
	{
		//ɾ��Ŀ¼���ļ�,��û������ɾ������ֻ�Ǵ�map���Ƴ���
		//ɾ����Ŀ¼�µ�Ŀ���ļ��в��޸ĵ�ǰ�ļ��е� �޸�ʱ���ֶ�
		this->FileOrDir.erase(this->FileOrDir.find(Name));
		this->ModifyTime = FileHelper::GetNowTimeToString();
		this->CountDirSizeUpward();
	}
	//�ݹ�ɾ��ĳ��Ŀ¼�µ������ļ�,û��ɾ��Ŀ¼
	void static DeleteFileOfDirRecursion(Dir* targetdir)
	{
		for (map<string, FileStruct*>::iterator dir = targetdir->FileOrDir.begin(); dir != targetdir->FileOrDir.end();)
		{
			//�ݹ�
			if (dir->second->GetFType() == "Dir")
			{
				//ת��ΪDir����֮��ݹ��ӡ
				DeleteFileOfDirRecursion(dynamic_cast<Dir*>(dir->second));
				dir++;
			}
			//���ļ�
			else
			{
				//ָ��Ŀ¼��ָ���ÿ�
				dir->second->SetFatherDir(nullptr);
				//ɾ������
				delete dir->second;
				//ָ���
				dir->second = nullptr;
				//��map���Ƴ�
				targetdir->FileOrDir.erase(dir++);
			}
		}

		targetdir->CountDirSizeUpward();

	}

	//�ݹ�ɾ��Ŀ¼�ṹ
	void static DeleteEmptyDirRecursion(Dir* targetdir)
	{
		for (map<string, FileStruct*>::iterator dir = targetdir->FileOrDir.begin(); dir != targetdir->FileOrDir.end();)
		{
			//�ݹ�
			if (dir->second->GetFType() == "Dir")
			{
				if (dynamic_cast<Dir*>(dir->second)->FileOrDir.size() != 0)
				{
					DeleteEmptyDirRecursion(dynamic_cast<Dir*>(dir->second));
				}
				else
				{
					//ָ��Ŀ¼��ָ���ÿ�
					dir->second->SetFatherDir(nullptr);
					//ɾ������
					delete dir->second;
					//ָ���
					dir->second = nullptr;
					//��map���Ƴ�
					targetdir->FileOrDir.erase(dir++);
				}
			}
			
		}
	}

	//�����Ŀ¼�µ������ļ����ļ���
	void PrintAllFileAndDir()
	{
		size_t filenum = 0;
		size_t allfilesize = 0;
		cout << std::setw(20) << "����ʱ��" << std::setw(20) << "�޸�ʱ��" << std::setw(10) << "����" << std::setw(10) << "��С" << std::setw(10) << "����" << endl;
		cout << std::setw(20) << this->CreateTime << std::setw(20) << this->ModifyTime << std::setw(10) << this->FType << std::setw(10) << this->Size << std::setw(10) << "." << endl;

		if (this->FatherDir != this)
		{
			cout << std::setw(20) << this->FatherDir->GetCreateTime() << std::setw(20) << this->FatherDir->GetModifyTime() << std::setw(10) << this->FatherDir->GetFType() << std::setw(10) << "         " << std::setw(10) << ".." << endl;
		}
		for (auto f : this->FileOrDir)
		{
			if (f.second->GetFType() == "File")
			{
				filenum++;
				allfilesize += f.second->GetSize();
			}
			cout << std::setw(20) << f.second->GetCreateTime() << std::setw(20) << f.second->GetModifyTime() << std::setw(10) << f.second->GetFType() << std::setw(10) << f.second->GetSize() << std::setw(10) << f.second->GetName() << endl;
		}
		cout << "            " << filenum << "���ļ�" << "           " << allfilesize << "�ֽ�" << endl;
	}

	//ֻ��ӡĿ¼,���Ժ����溯���ϲ��ģ����Ҳ���
	void PrintAllDir()
	{
		cout << std::setw(20) << "����ʱ��" << std::setw(20) << "�޸�ʱ��" << std::setw(10) << "����" << std::setw(10) << "��С" << std::setw(10) << "����" << endl;
		cout << std::setw(20) << this->CreateTime << std::setw(20) << this->ModifyTime << std::setw(10) << this->FType << std::setw(10) << this->Size << std::setw(10) << "." << endl;

		if (this->FatherDir != this)
		{
			cout << std::setw(20) << this->FatherDir->GetCreateTime() << std::setw(20) << this->FatherDir->GetModifyTime() << std::setw(10) << this->FatherDir->GetFType() << std::setw(10) << "          " << std::setw(10) << ".." << endl;
		}
		for (auto f : this->FileOrDir)
		{
			if (f.second->GetFType() == "Dir")
			{
				cout << std::setw(20) << f.second->GetCreateTime() << std::setw(20) << f.second->GetModifyTime() << std::setw(10) << f.second->GetFType() << std::setw(10) << f.second->GetSize() << std::setw(10) << f.second->GetName() << endl;
			}
		}
		cout << "            0���ļ�" << "           " << 0 << "�ֽ�" << endl;
	}

	//�ݹ��ӡ ����Ŀ¼�µ������ļ���Ŀ¼
	void static PrintAllFileAndDirRecursion(Dir* targetdir)
	{
		cout << endl;
		cout << endl << "  " << targetdir->GetPath() + "/" + targetdir->GetName() << " ��Ŀ¼" << endl;
		targetdir->PrintAllFileAndDir();
		for (auto dir : targetdir->GetFileOrDir())
		{
			//�ݹ�
			if (dir.second->GetFType() == "Dir")
			{
				//ת��ΪDir����֮��ݹ��ӡ
				PrintAllFileAndDirRecursion(dynamic_cast<Dir*>(dir.second));
			}
		}
	}

	//�ݹ��ӡ ����Ŀ¼�µ�����Ŀ¼
	void static PrintAllDirRecursion(Dir* targetdir)
	{

		cout << endl;
		cout << endl << "  " << targetdir->GetPath() + "/" + targetdir->GetName() << " ��Ŀ¼" << endl;
		targetdir->PrintAllDir();
		for (auto dir : targetdir->GetFileOrDir())
		{
			//�ݹ�
			if (dir.second->GetFType() == "Dir")
			{
				//ת��ΪDir����֮��ݹ��ӡ
				PrintAllDirRecursion(dynamic_cast<Dir*>(dir.second));
			}
		}
	}

	//�ݹ��޸ĸ�Ŀ¼�µ�����Ŀ¼���ļ�·��
	void static ChangeAllFileAndDirPathRecursion(Dir* targetdir)
	{

		//Ŀ¼�޸�·��
		//targetdir->SetPath(targetdir->GetFatherDir()->GetPath()+"/"+ targetdir->GetFatherDir()->GetName());
		for (auto dir : targetdir->GetFileOrDir())
		{
			//�ݹ�
			dir.second->SetPath(targetdir->GetPath() + "/" + targetdir->GetName());
			if (dir.second->GetFType() == "Dir")
			{
				//ת��ΪDir����֮��ݹ��޸�·��
				ChangeAllFileAndDirPathRecursion(dynamic_cast<Dir*>(dir.second));
			}

		}
	}

	//���ش�Ŀ¼�µ���Ŀ¼���ļ�map
	map<string, FileStruct*> GetFileOrDir()
	{
		return this->FileOrDir;
	}

	//��ȡĿ¼�����е��ļ���
	void GetAllFileOfDir(vector<string>& files)
	{
		for (auto name : this->FileOrDir)
		{
			if (name.second->GetFType() == "File")
			{
				files.emplace_back(name.first);
			}
		}
	}

	//�����޸�Ŀ¼��С
	void CountDirSizeUpward()
	{
		Dir* dirptr = this;
		Dir* oldone = nullptr;
		while (dirptr != oldone)
		{
			size_t size = 0;
			for (auto f : dirptr->GetFileOrDir())
			{
				size += f.second->GetSize();
			}
			dirptr->SetSize(size);
			oldone = dirptr;
			dirptr = dynamic_cast<Dir*>(dirptr->GetFatherDir());
		}
	}

	
};


//�ļ�
class File :public FileStruct
{
private:
	char* Content = nullptr;
public:
	File(string Name, string Path, Dir* FatherDir)
	{
		this->Name = Name;
		this->FType = "File";
		this->Path = Path;
		this->FatherDir = FatherDir;
	}

	//�������캯��
	File(const File* obj)
	{
		cout << "��������" << endl;
		this->Name = obj->Name;
		this->FType = obj->FType;
		this->Path = obj->Path;
		this->FatherDir = obj->FatherDir;
		//this->Mklink = obj->Mklink;
		this->Size = obj->Size;
		this->CreateTime = obj->CreateTime;
		this->ModifyTime = obj->ModifyTime;

		try {
			this->Content = new char[this->Size];
		}
		catch (exception e)
		{
			Error::MemoryMallocError();
			return;
		}
		memcpy(this->Content, obj->Content, this->Size);
	}
	~File()
	{
		delete[] this->Content;
		cout << "File ����������" << endl;
	}
	//��ȡ�ļ�����
	char* GetContent()
	{
		return this->Content;
	}
	
	void PrintContent()
	{
	
		if (this->Content != nullptr)
		{
			cout << this->Content << endl;
		}
	}


	void SetContent(char* Content,size_t Size)
	{

		this->Size = Size;
		this->Content = Content;
	}
};