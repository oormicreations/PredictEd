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
	srand((UINT)time(NULL));

}

CCryptHelper::~CCryptHelper()
{
	Cleanup();
}

void CCryptHelper::Cleanup()
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

	//convert unicode cstring to bytes
	CStringA utf8 = CW2A(pwszText, CP_UTF8);
	CByteArray Bytes;
	const size_t nBytes = sizeof(CStringA::XCHAR) * utf8.GetLength();
	Bytes.SetSize(nBytes);
	memcpy(Bytes.GetData(), (LPVOID)utf8.GetString(), nBytes);

	// Hash the data
	ntStatus = BCryptHashData(m_hHash, (PBYTE)Bytes.GetData(), nBytes, 0);
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

/*
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

	if (ntStatus != STATUS_SUCCESS)
	{
		m_sError.Format(_T("Error generate the key. BCryptGenerateSymmetricKey return with error 0x%08x"), ntStatus);
		return false;
	}

	return true;

}*/

bool CCryptHelper::Create_SHA512_Hash(CString pwszText)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	BCRYPT_KEY_HANDLE	hKey = NULL;
	DWORD               cbHashObject, cbResult;
	DWORD				cbData = 0;
	DWORD				cbHash = 0;
	PBYTE               pbHash = NULL;
	PBYTE				pbHashObject = NULL;

	m_sSHA512.Empty();

	ntStatus = BCryptOpenAlgorithmProvider(&m_hHashAlg,BCRYPT_SHA512_ALGORITHM, NULL, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Open SHA512 provider. BCryptOpenAlgorithmProvider failed with status: 0x%08x\n"), ntStatus);
		return false;
	}

	//  Determine the size of the Hash object
	ntStatus = BCryptGetProperty(m_hHashAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbResult, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error determining the size of the Hash object. BCryptGetProperty failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//allocate the hash object on the heap
	pbHashObject = (PBYTE)malloc(cbHashObject);

	if (NULL == pbHashObject)
	{
		m_sError = _T("Memory allocation failed for hash object");
		return false;
	}

	//calculate the length of the hash
	ntStatus = BCryptGetProperty(m_hHashAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD),	&cbData, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error calc hash length. BCryptGetProperty failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//allocate the hash buffer on the heap
	pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
	if (NULL == pbHash)
	{
		m_sError = _T("Memory allocation failed for hash buffer");
		return false;
	}

	//  Create the Hash object
	ntStatus = BCryptCreateHash(m_hHashAlg, &m_hHash, pbHashObject, cbHashObject, NULL, 0, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Creating the Hash object. BCryptCreateHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//convert unicode cstring to bytes
	CStringA utf8 = CW2A(pwszText, CP_UTF8);
	CByteArray Bytes;
	const size_t nBytes = sizeof(CStringA::XCHAR) * utf8.GetLength();
	Bytes.SetSize(nBytes);
	memcpy(Bytes.GetData(), (LPVOID)utf8.GetString(), nBytes);

	// Hash the data
	ntStatus = BCryptHashData(m_hHash, (PBYTE)Bytes.GetData(), nBytes, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error hashing the data. BCryptHashData failed with status: 0x%08x"), ntStatus);
		return false;
	}

	// Finish the hash
	ntStatus = BCryptFinishHash(m_hHash, pbHash, cbHash, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error finish the hash. BCryptFinishHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//convert to hex
	CString tmp;
	for (int i = 0; i < (int)cbHash; i++)
	{
		tmp.Format(_T("%02X"), pbHash[i]);
		m_sSHA512.Append(tmp);
	}

	if (m_sSHA512.IsEmpty()) return false;

	//cleanup
	if (pbHash)
	{
		HeapFree(GetProcessHeap(), 0, pbHash);
	}
	if (pbHashObject)
	{
		free(pbHashObject);
	}

	return true;
}


bool CCryptHelper::Create_SHA512_Hash_Bin(PBYTE data, ULONG nbytes)
{
	NTSTATUS ntStatus = STATUS_SUCCESS;
	BCRYPT_KEY_HANDLE	hKey = NULL;
	DWORD               cbHashObject, cbResult;
	DWORD				cbData = 0;
	DWORD				cbHash = 0;
	PBYTE               pbHash = NULL;
	PBYTE				pbHashObject = NULL;

	m_sSHA512.Empty();

	ntStatus = BCryptOpenAlgorithmProvider(&m_hHashAlg, BCRYPT_SHA512_ALGORITHM, NULL, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Open SHA512 provider. BCryptOpenAlgorithmProvider failed with status: 0x%08x\n"), ntStatus);
		return false;
	}

	//  Determine the size of the Hash object
	ntStatus = BCryptGetProperty(m_hHashAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbResult, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error determining the size of the Hash object. BCryptGetProperty failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//allocate the hash object on the heap
	pbHashObject = (PBYTE)malloc(cbHashObject);

	if (NULL == pbHashObject)
	{
		m_sError = _T("Memory allocation failed for hash object");
		return false;
	}

	//calculate the length of the hash
	ntStatus = BCryptGetProperty(m_hHashAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error calc hash length. BCryptGetProperty failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//allocate the hash buffer on the heap
	pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
	if (NULL == pbHash)
	{
		m_sError = _T("Memory allocation failed for hash buffer");
		return false;
	}

	//  Create the Hash object
	ntStatus = BCryptCreateHash(m_hHashAlg, &m_hHash, pbHashObject, cbHashObject, NULL, 0, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error Creating the Hash object. BCryptCreateHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	// Hash the data
	ntStatus = BCryptHashData(m_hHash, data, nbytes, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error hashing the data. BCryptHashData failed with status: 0x%08x"), ntStatus);
		return false;
	}

	// Finish the hash
	ntStatus = BCryptFinishHash(m_hHash, pbHash, cbHash, 0);
	if (STATUS_SUCCESS != ntStatus)
	{
		m_sError.Format(_T("Error finish the hash. BCryptFinishHash failed with status: 0x%08x"), ntStatus);
		return false;
	}

	//convert to hex
	CString tmp;
	for (int i = 0; i < (int)cbHash; i++)
	{
		tmp.Format(_T("%02X"), pbHash[i]);
		m_sSHA512.Append(tmp);
	}

	if (m_sSHA512.IsEmpty()) return false;

	//cleanup
	if (pbHash)
	{
		HeapFree(GetProcessHeap(), 0, pbHash);
	}
	if (pbHashObject)
	{
		free(pbHashObject);
	}

	return true;
}


//Base64 code is by Mihai MOGA, https://www.codeproject.com/Tips/813146/Fast-base-functions-for-encode-decode

inline bool CCryptHelper::is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

unsigned int CCryptHelper::Base64_Encode(const unsigned char* bytes_to_encode, unsigned int in_len, unsigned char* encoded_buffer, unsigned int& out_len)
{
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3] = { 0, 0, 0 };
	unsigned char char_array_4[4] = { 0, 0, 0, 0 };

	out_len = 0;
	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; i < 4; i++)
			{
				encoded_buffer[out_len++] = base64_chars[char_array_4[i]];
			}
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
		{
			char_array_3[j] = '\0';
		}

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; j < (i + 1); j++)
		{
			encoded_buffer[out_len++] = base64_chars[char_array_4[j]];
		}

		while (i++ < 3)
		{
			encoded_buffer[out_len++] = '=';
		}
	}

	return out_len;
}

unsigned int CCryptHelper::Base64_Decode(const unsigned char* encoded_string, unsigned int in_len, unsigned char* decoded_buffer, unsigned int& out_len)
{
	size_t i = 0;
	size_t j = 0;
	int in_ = 0;
	unsigned char char_array_3[3] = { 0, 0, 0 };
	unsigned char char_array_4[4] = { 0, 0, 0, 0 };

	out_len = 0;
	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
			{
				char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; i < 3; i++)
			{
				decoded_buffer[out_len++] = char_array_3[i];
			}
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 4; j++)
		{
			char_array_4[j] = 0;
		}

		for (j = 0; j < 4; j++)
		{
			char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
		{
			decoded_buffer[out_len++] = char_array_3[j];
		}
	}
	return out_len;
}


BOOL CCryptHelper::B64Encode(CString sfilename)
{
	m_OutputFile.Empty();

	CFile sfile;
	if (sfile.Open(sfilename, CFile::modeRead | CFile::typeBinary))
	{
		UINT slen = (UINT)sfile.GetLength();
		if (slen > 0)
		{
			CString tfilename = sfilename + _T(".b64.txt");

			CFile tfile;
			if (tfile.Open(tfilename, CFile::modeWrite | CFile::modeCreate))
			{
				unsigned char sbuf[300];//sz must be multiple of 3
				ZeroMemory(sbuf, 300);
				unsigned char tbuf[400];//sz must be multiple of 4
				ZeroMemory(tbuf, 400);

				UINT nread = 0;
				UINT nenc = 0;
				do {
					nread = sfile.Read(sbuf, 300);
					if (nread > 0)
					{
						Base64_Encode(sbuf, nread, tbuf, nenc);
						tfile.Write(tbuf, nenc);
					}
				} while (nread > 0);

				tfile.Close();
				sfile.Close();

				m_OutputFile = tfilename;
				return TRUE;
			}
		}

	}

	return FALSE;
}


BOOL CCryptHelper::B64Decode(CString sfilename)
{
	m_OutputFile.Empty();

	CFile sfile;
	if (sfile.Open(sfilename, CFile::modeRead | CFile::typeBinary))
	{
		UINT slen = (UINT)sfile.GetLength();
		if (slen > 0)
		{
			CString tfilename = sfilename + _T(".b64.decoded");

			CFile tfile;
			if (tfile.Open(tfilename, CFile::modeWrite | CFile::modeCreate))
			{
				unsigned char sbuf[400];//sz must be multiple of 4
				ZeroMemory(sbuf, 400);
				unsigned char tbuf[300];//sz must be multiple of 3
				ZeroMemory(tbuf, 300);

				UINT nread = 0;
				UINT nenc = 0;
				do {
					nread = sfile.Read(sbuf, 300);
					if (nread > 0)
					{
						Base64_Decode(sbuf, nread, tbuf, nenc);
						tfile.Write(tbuf, nenc);
					}
				} while (nread > 0);

				tfile.Close();
				sfile.Close();

				m_OutputFile = tfilename;
				return TRUE;
			}
		}

	}

	return FALSE;
}

BOOL CCryptHelper::PredictEdStegEncode(CString srcimage, CString datafile, CString pass)
{
	CByteArray passBytes;
	UINT npassBytes = 0;
	if (!pass.IsEmpty())
	{
		//convert unicode cstring to bytes
		CStringA stra = CW2A(pass, CP_UTF8);
		npassBytes = sizeof(CStringA::XCHAR) * stra.GetLength();
		passBytes.SetSize(npassBytes);
		memcpy(passBytes.GetData(), (LPVOID)stra.GetString(), npassBytes);
	}

	CString encimage = srcimage;
	encimage.Replace(_T("jpeg"), _T("jpg"));
	encimage.Replace(_T(".jpg"), _T("_StegEnc.png"));


	CImage image;
	HRESULT hr = image.Load(srcimage);
	if (hr == S_OK)
	{
		int h = image.GetHeight();
		int w = image.GetWidth();

		ULONGLONG npix = h*w;
		int npixdatasz = sizeof(ULONGLONG); // number of pix needed to store data size

		//open and check data size
		CFile sfile;
		if (sfile.Open(datafile, CFile::modeRead | CFile::typeBinary))
		{
			ULONGLONG slen = sfile.GetLength();
			if (slen > 0)
			{
				if (slen > npix + npixdatasz)
				{
					CString tmp;
					tmp.Format(_T("Error: Data size is too big to fit inside the chosen image.\r\n\r\nData size: %I64u\r\nNumber of pixels: %I64u\r\nNumber of pixels needed: %I64u"), slen, npix, (slen + npixdatasz));
					AfxMessageBox(tmp, MB_ICONERROR);
					sfile.Close();
					return FALSE;
				}

				//malloc
				BYTE *data = (BYTE*)malloc(slen);
				if (data)
				{
					//read data
					sfile.Read(data, slen);
					sfile.Close();

					COLORREF col;
					int n = 0;
					int p = 0;
					BYTE bt;
					ULONGLONG totalbytes = npixdatasz + slen;


					for (int y = 0; y < h; y++)
					{
						for (int x = 0; x < w; x++)
						{
							if (n < npixdatasz) bt = slen >> (n * 8); //embed data length
							else bt = data[n]; //embed data

							if (n >= totalbytes) bt = rand(); //fill the rest of pixels with random data to disable noise analysis of encoded image
							//int k = (int)bt;
							n++;

							//xor encrypt, repeat password bytes in sequence
							bt = bt ^ passBytes[p];
							p++;
							if (p >= npassBytes)p = 0;

							col = image.GetPixel(x, y);
							int r = GetRValue(col);
							int g = GetGValue(col);
							int b = GetBValue(col);

							//empty last 3 bits of each
							r = r & 0xF8;
							g = g & 0xF8;
							b = b & 0xF8;

							//extract bits and insert in rgb
							r = r | (bt & 7);
							g = g | ((bt >> 3) & 7);
							b = b | ((bt >> 6) & 7);

							//modified pixel
							image.SetPixel(x, y, RGB(r, g, b));

						}
					}

					image.Save(encimage);
				}
			}
			else
			{
				AfxMessageBox(_T("Error: No data!"), MB_ICONERROR);
				sfile.Close();
				return FALSE;
			}
		}


		return TRUE;
	}
	else AfxMessageBox(_T("Error: Failed to load the image"), MB_ICONERROR);

	return FALSE;
}


BOOL CCryptHelper::PredictEdStegDecode(CString encimage, CString datafile, CString pass)
{
	//CString srcimage = _T("C:\\Users\\Sanjeev\\Documents\\Oormi Creations\\PredictEd dev\\stegenc.png");
	//CString msg;

	CByteArray Bytes;
	int nBytes = 900;

	CImage image;
	HRESULT hr = image.Load(encimage);
	if (hr == S_OK)
	{
		int h = image.GetHeight();
		int w = image.GetWidth();

		COLORREF col;
		int n = 0;

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				col = image.GetPixel(x, y);
				int r = GetRValue(col);
				int g = GetGValue(col);
				int b = GetBValue(col);

				//extract msg bits
				BYTE bt = ((b & 7) << 6) | ((g & 7) << 3) | (r & 7);
				//msg.AppendChar((WCHAR)bt);

				n++;
				if (n >= nBytes) break;
			}
			if (n >= nBytes) break;
		}

		return TRUE;
	}

	return FALSE;
}

CString CCryptHelper::PasswordGen(int len)
{
	CString pass;
	int max = 126;
	int min = 33;
	int range = max - min + 1;

	for (int i = 0; i < len; i++)
	{
		int num = rand() % range + min;
		pass.AppendChar((WCHAR)num);
	}

	return pass;
}

int CCryptHelper::GetPasswordStrength(CString pass)
{
	if (pass.IsEmpty()) return 0;

	TCHAR nums[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		L'०', L'१', L'२', L'३', L'४', L'५', L'६', L'७', L'८', L'९' }; //L'x' is necessary

	TCHAR spl[] = { '~', '`', '!', '@', '#', '$', '%', '^', '&', '*',
		'(', ')', '-', '_', '+', '=', '|', '\\', '{', '[', '}', ']',
		':', ';', '\"', '\'', '<', ',', '>', '.', '?', '/' }; //32

	BOOL hasnum = FALSE;
	BOOL hasspl = FALSE;
	BOOL repeatpattern = FALSE;

	for (int i = 0; i < 20; i++)
	{
		if (pass.Find(nums[i]) >= 0)
		{
			hasnum = TRUE;
			break;
		}
	}

	for (int i = 0; i < 32; i++)
	{
		if (pass.Find(spl[i]) >= 0)
		{
			hasspl = TRUE;
			break;
		}
	}

	//repeating patterns
	int passlen = pass.GetLength();
	CString pattern;

	for (int i = 0; i < passlen; i++)
	{
		TCHAR tc = pass.GetAt(i);
		pattern.Empty();
		pattern.Format(_T("%c%c%c"), tc, tc, tc);
		if (pass.Find(pattern) >= 0) repeatpattern = TRUE;
	}

	int passstrength = 0;

	//these are arbitrary criteria
	passstrength = passlen * 5 + hasnum * 25 + hasspl * 25 - repeatpattern * 20;
	
	return passstrength;
}
