#pragma once
#include <bcrypt.h>
#include <string>


#ifndef NTSTATUS
typedef LONG NTSTATUS, *PNSTATUS;
#endif

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS                  ((NTSTATUS)0x00000000L)
#define STATUS_NOT_SUPPORTED            ((NTSTATUS)0xC00000BBL)
#define STATUS_UNSUCCESSFUL             ((NTSTATUS)0xC0000001L)
//#define STATUS_INVALID_PARAMETER        ((NTSTATUS)0xC000000DL)
#define STATUS_HMAC_NOT_SUPPORTED       ((NTSTATUS)0xC000A001L)
#define STATUS_BUFFER_TOO_SMALL         ((NTSTATUS)0xC0000023L)
#define STATUS_NOT_IMPLEMENTED          ((NTSTATUS)0xC0000002L)
//#define STATUS_ENTRYPOINT_NOT_FOUND     ((NTSTATUS)0xC0000139L)
#endif


const DWORD CHUNK_SIZE = 16;

#define SYMM_KEY_SIZE_SECRET 16

//Set IV
static const BYTE rgbIV[] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

//Set AES KEY
//static const BYTE rgbAES128Key[] =
//{
//	'D', 'r', '.', 'L', 'u', 'i', 'j', 'i',
//	'C', 'N', 'G', ' ', 'T', 'e', 's', 't'
//};

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

class CCryptHelper
{

public:
	CCryptHelper();
	virtual	~CCryptHelper();

	//File 
	void SetLenghtFileToOpen(ULONGLONG iLenghtFileToOpen);
	bool LastCryptBuffRead();
	void SetTotByteRead(UINT iByteRead);
	ULONGLONG m_iLenghtFileToOpen;
	ULONGLONG m_iTotalByteRead;


	//Handle 
	BCRYPT_ALG_HANDLE       m_hAesAlg;
	BCRYPT_KEY_HANDLE       m_hKey;
	PBYTE                   m_pbKeyObject;
	PBYTE                   m_pbIV;

	//Handle for Hash
	BCRYPT_HASH_HANDLE		m_hHash;
	PBYTE					m_pbHashObject;
	BCRYPT_ALG_HANDLE		m_hHashAlg;

	//hash
	CString m_sSHA512;

	CString m_sError;
	CString m_OutputFile;

	bool OpenMSPrimitiveProviderAES();
	//bool CreateSymmetricKey_AES_CBC(DWORD &cbKeyObject, DWORD &cbIV);
	bool CreateSymmetricKey_SHA1_Hash(PCWSTR pwszText, DWORD cbKeyObj);
	bool Crypt(bool bEncrypt, PUCHAR pbufFileToOpen, ULONG iBytesRead, ULONG cbIV, PBYTE pbufFileToSave, DWORD& iBufToSave);
	bool CryptLastByte(bool bEncrypt, PUCHAR pbufFileToOpen, ULONG iBytesRead, ULONG cbIV, PBYTE pbufFileToSave, DWORD& iBufToSave);
	
	bool Create_SHA512_Hash(CString pwszText);
	bool Create_SHA512_Hash_Bin(PBYTE data, ULONG nbytes);
	
	unsigned int Base64_Decode(const unsigned char * encoded_string, unsigned int in_len, unsigned char * decoded_buffer, unsigned int & out_len);
	unsigned int Base64_Encode(const unsigned char * bytes_to_encode, unsigned int in_len, unsigned char * encoded_buffer, unsigned int & out_len);
	static bool is_base64(unsigned char c);
	BOOL B64Encode(CString sfilename);
	BOOL B64Decode(CString sfilename);
	BOOL PredictEdStegEncode(CString srcimage, CString datafile, CString pass);
	BOOL PredictEdStegDecode(CString encimage, CString datafile, CString pass);

	CString PasswordGen(int len = 10);
	int GetPasswordStrength(CString pass);


	void Cleanup();

public:
	bool EnumProviders(CStringList *lstRegisteredProviders);
	bool CryptFile(bool bEncrypt, CString sFileToOpen, CString sFileToCrypt, CString sKey);
	CString GetLastError();

};

