#include "pch.h"
#include "crypt.h"
#include "type.h"

#pragma warning(disable : 4996)
namespace Crypt
{
    void Close(const HCRYPTPROV& prov, const HCRYPTHASH& hash);

    void CalcHash(const std::string& src, byte srcSalt[],DWORD dwLenSalt,std::string& dst)
    {
        if (src.empty()) return;
        dst.clear();

        HCRYPTPROV prov; //crypt context
        HCRYPTHASH hash;
        
        byte buf[32]; // hash buffer
        DWORD len = sizeof(buf); // buff sz

        if (!CryptAcquireContextA(&prov, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        {
            DbgMsg("error CryptAcuqireContextA()");
            return;
        }
        if (!CryptCreateHash(prov, CALG_SHA_256, 0, 0, &hash))
        {
            DbgMsg("error CryptCreateHash()");
            return;
        }
        if (srcSalt && dwLenSalt >= 1)
        {
            if (!CryptHashData(hash, srcSalt, dwLenSalt, 0))
            {
                DbgMsg("error CryptHashData()");

                Close(prov, hash);
                return;
            }
        }
        if (!CryptHashData(hash, (byte*)src.c_str(), src.size(), 0))
        {
            DbgMsg("error CryptHashData()");

            Close(prov, hash);
            return;
        }
        if (!CryptGetHashParam(hash, HP_HASHVAL, buf, &len, 0))
        {
            DbgMsg("error CryptGetHashParam()");
            Close(prov, hash);
            return;
        }
        char strHash[65];
        for (DWORD i = 0; i < len; i++)
        {
            sprintf_s(strHash + i * 2, sizeof(strHash) - i * 2, "%02x", buf[i]); // print bytes to hex str representation
        }
        strHash[sizeof(strHash) - 1] = 0;

        dst = strHash;
        Close(prov, hash);
    }
    void GenRandom(byte dst[], DWORD dwLen)
    {
        if (!dst || dwLen == 0) return;

        HCRYPTPROV prov;
        if (!CryptAcquireContextA(&prov, 0, 0, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
        {
            DbgMsg("error CryptAcuqireContextA()");
            return;
        }
        if (!CryptGenRandom(prov, dwLen, dst))
        {
            DbgMsg("error CryptGenRandom()");
            return;
        }
        CryptReleaseContext(prov, 0);
    }
    void ByteToHex(const byte src[], DWORD dwLenSrc, std::string& hex)
    {
        if (!src || dwLenSrc == 0) return;
        hex.clear();

        char buf[3];
        for (DWORD i = 0; i < dwLenSrc;i++)
        {
            sprintf(buf, "%02x", src[i]);
            hex.append(buf);
        }
    }
    void HexToByte(const std::string& src, DWORD dwLenDst, byte dst[])
    {
        if (src.empty() || dwLenDst == 0 || !dst) return;

        for (DWORD i = 0; i < dwLenDst; i++)
        {
            uint v;
            sscanf(src.c_str() + i * 2, "%02x", &v);
            dst[i] = (byte)v;
        }
    }
    void Close(const HCRYPTPROV& prov, const HCRYPTHASH& hash)
    {
        if(prov)
        CryptReleaseContext(prov, 0);
        if(hash)
        CryptDestroyHash(hash);
    }
}