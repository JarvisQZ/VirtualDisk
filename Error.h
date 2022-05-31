#pragma once
#include"head.h"

//错误类型
class Error
{
public:
	//文件或目录名称包含非法字符
	void static NameError()
	{
		cout << "   错误:" << endl;
		cout << "        名称中不能包含特殊字符 \\ / : * ? \" < > | " << endl;
	}
	void static MemoryMallocError()
	{
		cout << "   错误:" << endl;
		cout << "        内存空间不足！" << endl;
	}
	//新建的目录名与文件重名了
	void static DirNameSaveAsFileError()
	{
		cout << "   错误:" << endl;
		cout << "        指定的文件夹和已有的某个文件重名。请指定其他名称" << endl;
	}
	//新建的文件与目录名重名了
	void static FileNameSaveAsDirError()
	{
		cout << "   错误:" << endl;
		cout << "        指定的文件名和已存在的文件夹重名。请指定其他名称" << endl;
	}
	// 系统找不到路径错误
	void static CannotFindPathError(string errorpath)
	{
		cout << "   错误:" << endl;
		cout << "        系统找不到路径 " << errorpath << " ,因为该路径不存在" << endl;
	}
	//路径格式错误  路径中包含了空  a//b/c
	void static PathFormatError()
	{
		cout << "   错误:" << endl;
		cout << "        路径名不能为空字符串" << endl;
	}
	//目录已经存在，又新建目录
	void static DirSameNameError(string path, string name)
	{
		cout << "   错误:" << endl;
		cout << "        路径 " << path << " 下已经包含名叫" << name << " 的文件夹,请重新命名" << endl;
	}
	//创建文件时路径不存在
	void static TouchNoDirFileError(string path)
	{
		cout << "   错误:" << endl;
		cout << "        路径不存在，请先通过mkdir创建目录:" << path << endl;
	}
	void static CannotFindFileError(string path, string filename)
	{
		cout << "   错误:" << endl;
		cout << "        路径 " << path << " 下没有名为" << filename << " 的文件" << endl;
	}
	void static CannotFindDirError(string path, string dirname)
	{
		cout << "   错误:" << endl;
		cout << "        路径 " << path << " 下没有名为" << dirname << " 的目录" << endl;
	}
	void static RenameSaveAsDirError(string dirname)
	{
		cout << "   错误:" << endl;
		cout << "        该目录下存在名为" << dirname << "的文件夹,请指定其他名称" << endl;
	}
	void static RenameSaveAsFileError(string filename)
	{
		cout << "   错误:" << endl;
		cout << "        该目录下存在名为" << filename << "的文件,请指定其他名称" << endl;
	}
	void static CreateSelfError()
	{
		cout << "   错误:" << endl;
		cout << "        子目录或文件 . 已经存在" << endl;
	}
	void static CreateFatherSelfError()
	{
		cout << "   错误:" << endl;
		cout << "        子目录或文件 .. 已经存在" << endl;
	}
	void static VisitRootError()
	{
		cout << "   错误:" << endl;
		cout << "        拒绝访问" << endl;
	}
	void static CannotVisitError()
	{
		cout << "   错误:" << endl;
		cout << "        另一个程序正在使用此文件，进程无法访问" << endl;
	}
	void static DeleteDirNotEmptyError()
	{
		cout << "   错误:" << endl;
		cout << "        目录不是空的" << endl;
	}
	void static InvalidDirNameError()
	{
		cout << "   错误:" << endl;
		cout << "        目录名称无效" << endl;
	}
	//没有找到与通配符相匹配的文件
	void static NoFileMatchedError()
	{
		cout << "   错误:" << endl;
		cout << "        没有匹配的文件" << endl;
	}
	//复制的时候重名
	void static FileNameAlreadyExistError(string filename)
	{
		cout << "   错误:" << endl;
		cout << "        名为" << filename << "已经存在" << endl;
	}
	void static NoMatchedFile()
	{
		cout << "   错误:" << endl;
		cout << "        未匹配到任何文件" << endl;
	}
};
