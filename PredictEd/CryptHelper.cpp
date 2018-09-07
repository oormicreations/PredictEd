/*

The code below is mostly from https://www.codeproject.com/Articles/18713/Simple-way-to-crypt-a-file-with-CNG
Which is based on Microsoft's CNG SDK sample code most probably.

*/

#include "stdafx.h"
#include "CryptHelper.h"

CCryptHelper::CCryptHelper()
{
	m_sError.Empty();
	m_iTotalByteRead = 0;
	m_iLenghtFileToOpen = 0;
	m_hAesAlg = NULL;
	m_hKey = NULL;
	m_pbKeyObject = NULL;
	m_pbIV = NULL;

	//Hash
	m_hHash = NULL;
	m_pbHashObject = NULL;
	m_hHashAlg = NULL;

}

CCryptHelper::~CCryptHelper()
{

	if (m_hAesAlg)
	{
		BCryptCloseAlgorithmProvider(m_hAesAlg, 0);
	}

	if (m_hKey)
	{
		BCryptDestroyKey(m_hKey);
	}

	if (m_pbKeyObject)
	{
		HeapFree(GetProcessHeap(), 0, m_pbKeyObject);
	}

	if (m_pbIV != NULL)
	{
		HeapFree(GetProcessHeap(), 0, m_pbIV);
	}

	//Hash
	if (m_hHash)
		BCryptDestroyHash(m_hHash);

	if (m_pbHashObject)
		free(m_pbHashObject);

	if (m_hHashAlg)
		BCryptCloseAlgorithmProvider(m_hHashAlg, 0);
}

bool CCryptHelper::CryptFile(bool bEncrypt, CString sFileToOpen, CString sFileToCrypt, CString sKey)
{
	CFile		oFileToOpen, oFileToCrypt;
	NTSTATUS	ntStatus = STATUS_UNSUCCESSFUL;
	DWORD		cbKeyObject = 0;
	DWORD		cbIV = 0;
	BOOL		bFileToOpen = FALSE;
	BOOL		bFileToCrypt = FALSE;

	UINT		iBytesRead = 0;
	BYTE		pbufFileToOpen[CHUNK_SIZE];
	BYTE		pbufFileToSave[CHUNK_SIZE * 2];

	if (!OpenMSPrimitiveProviderAES())
		return false;

	/*	if(!CreateSymmetricKey_AES_CBC(cbKeyObject,cbIV))
	return false;
	*/
	if (!CreateSymmetricKey_SHA1_Hash(sKey, cbKeyObject))
		return false;

	bFileToOpen = oFileToOpen.Open(sFileToOpen, CFile::modeRead | CFile::typeBinary);
	if (bFileToOpen = FALSE)
	{
		m_sError.Format(_T("Error opening the file '%s'"), sFileToOpen);
		return false;
	}

	bFileToCrypt = oFileToCrypt.Open(sFileToCrypt, CFile::modeWrite | CFile::shareExclusive | CFile::modeCreate);
	if (bFileToCrypt = FALSE)
	{
		m_sError.Format(_T("Error opening the file '%s'"), sFileToOpen);
		return false;
	}
	SetLenghtFileToOpen(oFileToOpen.GetLength());

	DWORD iBufToSave = 0;

	while (!LastCryptBuffRead())
	{
		iBufToSave = 0;
		iBytesRead = oFileToOpen.Read(pbufFileToOpen, CHUNK_SIZE);
		SetTotByteRead(iBytesRead);

		if (iBytesRead == 0)
		{
			CString sError;
			m_sError.Format(_T("Error reading the file '%s'"), sFileToOpen);
			return false;
		}

		if (!LastCryptBuffRead())
		{
			if (!Crypt(bEncrypt, pbufFileToOpen, iBytesRead, cbIV, pbufFileToSave, iBufToSave))
				return false;

			oFileToCrypt.Write(pbufFileToSave, iBytesRead);
		}
		else //(!LastCryptBuffRead())
		{
			if (!CryptLastByte(bEncrypt, pbufFileToOpen, iBytesRead, cbIV, pbufFileToSave, iBufToSave))
				return false;
			oFileToCrypt.Write(pbufFileToSave, iBufToSave);
		}
	}
	return true;
}





bool CCryptHelper::EnumProviders(CStringList *lstRegisteredProviders)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	DWORD cbBuffer = 0;
	PCRYPT_PROVIDERS pProviders = NULL;
	ntStatus = BCryptEnumRegisteredProviders(&cbBuffer, &pProviders);
	CString sProvider;
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("BCryptEnumRegisteredProviders failed with error code 0x%08x"), ntStatus);
		return false;
	}
	if (pProviders == NULL)
	{
		m_sError.Format(_T("BCryptEnumRegisteredProviders returned a NULL"));
		return false;
	}
	else
	{
		for (DWORD i = 0; i < pProviders->cProviders; i++)
		{
			sProvider.Format(_T("%s\n"), pProviders->rgpszProviders[i]);
			lstRegisteredProviders->AddHead(sProvider);
		}
	}

	if (pProviders != NULL)
	{
		BCryptFreeBuffer(pProviders);
	}
	return true;
}

CString CCryptHelper::GetLastError()
{
	return m_sError;
}

bool CCryptHelper::LastCryptBuffRead()
{
	return  (m_iLenghtFileToOpen <= m_iTotalByteRead);
}

void CCryptHelper::SetLenghtFileToOpen(ULONGLONG iLenghtFileToOpen)
{
	m_iLenghtFileToOpen = iLenghtFileToOpen;
}

void CCryptHelper::SetTotByteRead(UINT iByteRead)
{
	m_iTotalByteRead += iByteRead;
}


bool CCryptHelper::OpenMSPrimitiveProviderAES()
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	ntStatus = BCryptOpenAlgorithmProvider(&m_hAesAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
	switch (ntStatus)
	{
	case STATUS_SUCCESS:
		return true;
		/*	from msdn but not in NTSTATUS.H
		case STATUS_NOT_FOUND:
		m_sError.Format(_T("Error opening The algorithm handle. BCryptOpenAlgorithmProvider no provider was found for the specified algorithm ID. "));
		break;
		*/
	case STATUS_INVALID_PARAMETER:
		m_sError.Format(_T("Error opening The algorithm handle. BCryptOpenAlgorithmProvider one or more parameters are not valid. "));
		break;
	case STATUS_NO_MEMORY:
		m_sError.Format(_T("Error opening The algorithm handle. BCryptOpenAlgorithmProvider a memory allocation failure occurred. "));
		break;
	default:
		m_sError.Format(_T("Error opening The algorithm handle. BCryptOpenAlgorithmProvider return with error 0x%08x"), ntStatus);
		break;
	}
	return false;
}



bool CCryptHelper::Crypt(bool bEncrypt, PUCHAR pbufFileToOpen, ULONG iBytesRead, ULONG cbIV, PBYTE pbufFileToSave, DWORD& iBufToSave)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	DWORD		cbCipherText = 0;
	if (bEncrypt)
		ntStatus = BCryptEncrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, pbufFileToSave, iBytesRead, &iBufToSave, 0);
	else
		ntStatus = BCryptDecrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, pbufFileToSave, iBytesRead, &iBufToSave, 0);

	switch (ntStatus)
	{
	case STATUS_SUCCESS:

		return true;


	case STATUS_BUFFER_TOO_SMALL:
		m_sError.Format(_T("Error %s The size specified by the cbOutput parameter is not large enough to hold the ciphertext. Return with error 0x%08x"), bEncrypt ? _T("BCryptEncrypt") : _T("BCryptDecrypt"), ntStatus);

		break;
		//Line found on msdn but not in bcrypt.h
		//			case STATUS_INVALID_BUFFER_SIZE :
		//				m_sError.Format(_T("Error %s The cbInput parameter is not a multiple of the algorithm's block size and the BCRYPT_BLOCK_PADDING or the BCRYPT_PAD_NONE flag was not specified in the dwFlags parameter. Return with error 0x%08x"), bEncrypt?"BCryptEncrypt":"BCryptDecrypt",  ntStatus);
		//				break;
		//			case STATUS_INVALID_HANDLE :
		//				m_sError.Format(_T("Error %s The key handle in the hKey parameter is not valid. ", bEncrypt?"BCryptEncrypt":"BCryptDecrypt",  ntStatus);
		//					break;
	case STATUS_INVALID_PARAMETER:
		m_sError.Format(_T("Error %s One or more parameters are not valid. Return with error 0x%08x"), bEncrypt ? "BCryptEncrypt" : "BCryptDecrypt", ntStatus);
		break;
	case STATUS_NOT_SUPPORTED:
		m_sError.Format(_T("Error %s The algorithm does not support encryption. Return with error 0x%08x"), bEncrypt ? "BCryptEncrypt" : "BCryptDecrypt", ntStatus);
		break;
	default:
		m_sError.Format(_T("Error %s return with error 0x%08x"), bEncrypt ? "BCryptEncrypt" : "BCryptDecrypt", ntStatus);

	}

	return false;
}

bool CCryptHelper::CryptLastByte(bool bEncrypt, PUCHAR pbufFileToOpen, ULONG iBytesRead, ULONG cbIV, PBYTE pbufFileToSave, DWORD& iBufToSave)
{
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	DWORD		cbCipherText = 0;

	if (bEncrypt)
	{
		ntStatus = BCryptEncrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, NULL, 0, &cbCipherText, BCRYPT_BLOCK_PADDING);

		if (ntStatus != STATUS_SUCCESS)
		{
			m_sError.Format(_T("Error receiving the size required for the ciphertext. BCryptEncrypt return with error 0x%08x"), ntStatus);
			return false;
		}
		ntStatus = BCryptEncrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, pbufFileToSave, cbCipherText, &cbCipherText, BCRYPT_BLOCK_PADDING);
		switch (ntStatus)
		{
		case STATUS_SUCCESS:
			iBufToSave = cbCipherText;
			return true;
		default:
			m_sError.Format(_T("Error receiving ciphertext. BCryptEncrypt return with error 0x%08x"), ntStatus);
			return false;
		}
	}
	else
	{
		ntStatus = BCryptDecrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, NULL, 0, &cbCipherText, BCRYPT_BLOCK_PADDING);

		if (ntStatus != STATUS_SUCCESS)
		{
			m_sError.Format(_T("Error receiving the size required for the ciphertext. BCryptDecrypt return with error 0x%08x"), ntStatus);
			return false;
		}
		ntStatus = BCryptDecrypt(m_hKey, pbufFileToOpen, iBytesRead, NULL, m_pbIV, cbIV, pbufFileToSave, cbCipherText, &cbCipherText, BCRYPT_BLOCK_PADDING);

		switch (ntStatus)
		{
		case STATUS_SUCCESS:
			iBufToSave = cbCipherText;
			return true;
		default:
			m_sError.Format(_T("Error receiving ciphertext. BCryptDecrypt return with error 0x%08x"), ntStatus);
			return false;
		}
	}
	return false;
}


bool CCryptHelper::CreateSymmetricKey_SHA1_Hash(PCWSTR pwszText, DWORD cbKeyObject)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	BCRYPT_KEY_HANDLE	hKey = NULL;



	DWORD               cbHashObject, cbResult;
	BYTE                rgbHash[20];
	DWORD				cbData = 0;

	ntStatus = BCryptOpenAlgorithmProvider(&m_hHashAlg, BCRYPT_SHA1_ALGORITHM, NULL, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Open Algoritm for the key. BCryptOpenAlgorithmProvider failed with status: 0x%08x\n"), ntStatus);
		return false;
	}

	ntStatus = BCryptGetProperty(m_hAesAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbKeyObject, sizeof(DWORD), &cbData, 0);
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error calculating the size of KeyObject. BCryptGetProperty return with error 0x%08x"), ntStatus);
		return false;
	}

	m_pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (NULL == m_pbKeyObject)
	{
		m_sError.Format(_T("Error memory allocation key object on the heap failed"));
		return false;
	}


	//  Determine the size of the Hash object
	ntStatus = BCryptGetProperty(m_hHashAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbResult, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error determining the size of the Hash object. BCryptGetProperty failed with status: 0x%08x"), ntStatus);
		return false;
	}

	m_pbHashObject = (PBYTE)malloc(cbHashObject);

	if (NULL == m_pbHashObject)
	{
		m_sError.Format(_T("Error allocating Hash object memory"));
		return false;
	}

	//  Create the Hash object
	ntStatus = BCryptCreateHash(m_hHashAlg, &m_hHash, m_pbHashObject, cbHashObject, NULL, 0, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Creating the Hash object. BCryptCreateHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	// Hash the data
	ntStatus = BCryptHashData(m_hHash, (PBYTE)pwszText, (ULONG)sizeof(pwszText), 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error hashing the data. BCryptHashData failed with status: 0x%08x"), ntStatus);
		return false;
	}

	// Finish the hash
	ntStatus = BCryptFinishHash(m_hHash, rgbHash, sizeof(rgbHash), 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error finish the hash. BCryptFinishHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	ntStatus = BCryptGenerateSymmetricKey(m_hAesAlg, &hKey, m_pbKeyObject, cbKeyObject, rgbHash, SYMM_KEY_SIZE_SECRET, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error creating the key. BCryptGenerateSymmetricKey failed with status: 0x%08x"), ntStatus);
		hKey = NULL;
	}
	m_hKey = hKey;
	return true;
}


bool CCryptHelper::CreateSymmetricKey_AES_CBC(DWORD &cbKeyObject, DWORD &cbIV)
{
	NTSTATUS	ntStatus = STATUS_UNSUCCESSFUL;
	DWORD		cbData = 0;

	cbKeyObject = 0;
	cbIV = 0;

	ntStatus = BCryptGetProperty(m_hAesAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbKeyObject, sizeof(DWORD), &cbData, 0);
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error calculating the size of KeyObject. BCryptGetProperty return with error 0x%08x"), ntStatus);
		return false;
	}

	m_pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
	if (NULL == m_pbKeyObject)
	{
		m_sError.Format(_T("Error memory allocation key object on the heap failed"));
		return false;
	}

	ntStatus = BCryptGetProperty(m_hAesAlg, BCRYPT_BLOCK_LENGTH, (PBYTE)&cbIV, sizeof(DWORD), &cbData, 0);
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error calculating the block length for the IV. BCryptGetProperty return with error 0x%08x"), ntStatus);
		return false;
	}

	if (cbIV > sizeof(rgbIV))
	{
		m_sError.Format(_T("Block length is longer than the provided IV length\n"));
		return false;
	}

	m_pbIV = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbIV);
	if (NULL == m_pbIV)
	{
		m_sError.Format(_T("Error memory allocation buffer for the IV on the heap failed"));
		return false;
	}

	memcpy(m_pbIV, rgbIV, cbIV);

	ntStatus = BCryptSetProperty(m_hAesAlg, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error setting the ChainingMode CBC. BCryptSetProperty return with error 0x%08x"), ntStatus);
		return false;
	}

	// generate the key from supplied input key bytes
	ntStatus = BCryptGenerateSymmetricKey(m_hAesAlg, &m_hKey, m_pbKeyObject, cbKeyObject, (PBYTE)rgbAES128Key, sizeof(rgbAES128Key), 0);

	/*	PBYTE pKey;
	memcpy(pKey, sKey, sKey.GetLength());
	ntStatus = BCryptGenerateSymmetricKey(m_hAesAlg, &m_hKey, m_pbKeyObject, cbKeyObject, (PBYTE) pKey, sKey.GetLength(), 0);
	*/
	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error generate the key. BCryptGenerateSymmetricKey return with error 0x%08x"), ntStatus);
		return false;
	}

	return true;

}