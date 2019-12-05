#pragma once
#include <shobjidl.h>

class COM
{
private:
	IFileOpenDialog*	pFileOpen;
	IShellItem*			pItem;

public:
	static COM com;

	HRESULT Init();
	HRESULT GetFile(LPWSTR *path);
	void	CleanUp() {}
};

