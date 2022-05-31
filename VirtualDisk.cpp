#include"VirtualDisk.h"

VirtualDisk* VirtualDisk::virtualdisk = nullptr;
Dir* VirtualDisk::RootDir = nullptr;
string const VirtualDisk::RealSavePath = "F:/VirtualDisk/";
string const VirtualDisk::VirtualPath = "C:";