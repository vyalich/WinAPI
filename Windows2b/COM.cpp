#include "COM.h"

COM COM::com;

HRESULT COM::Init()
{
	return CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
}

HRESULT COM::GetFile(LPWSTR *path)
{
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		// Show the Open dialog box.
		hr = pFileOpen->Show(NULL);

		// Get the file name from the dialog box.
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, path);

				// Display the file name to the user.
				if (SUCCEEDED(hr))
				{
					MessageBox(NULL, *path, L"File Path", MB_OK);
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}

	return hr;
}