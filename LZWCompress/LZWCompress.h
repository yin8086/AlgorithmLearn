
// code is 12 bits, code table size is 4096
// Divisor is 4099

class LZWComp;

class compElement
{
    // when compress use key to find code, so key is the the main search area
    friend class LZWComp;
    uint16_t m_code;
    uint32_t m_key;
public:
    operator uint32_t() const 
    {
        return m_key;
    }
    compElement& operator=(uint32_t rhs)
    {
        m_key = rhs;
        return *this;
    }
};

/// when decompress, since code is 0-4095, and the dict size is <= 4096, we
/// can use ht[4096] and f(k) = k ideal hash func for simplicity
class decompElement
{
    friend class LZWComp;

    // use index for key
    uint16_t m_prefix;
    uint8_t m_suffix;
};

class LZWComp
{
    static const int m_D = 4099; // new in C++11
    static const int m_codes = 4096; 

    // compress
    LinkHash<compElement, uint32_t> *m_compDict;
    int m_cStatus;
    uint8_t m_left;
    uint32_t m_used;
    void code2byte(uint32_t code,uint8_t *&pos);
    // decompress
    void byte2code(uint32_t &code, const uint8_t* pos, uint32_t& posSize);
    uint8_t code2str(uint32_t code,uint8_t *&pos); // we must return the first character to make the new longest prefix
    decompElement *m_decompDict;
    uint8_t *m_reconStr;
    

public:
    LZWComp():m_compDict(0),m_cStatus(0), m_used(0), m_decompDict(0),m_reconStr(0)
    {

    }
    void resetCompressEngine();
    void resetDecompEngine();

    ~LZWComp()
    {
        delete m_compDict;
        delete [] m_decompDict;
        delete [] m_reconStr;
    }

    uint8_t* compress(uint8_t* dst, const uint8_t* src, uint32_t srcSize);
    uint8_t* decompress(uint8_t* dst, const uint8_t* src, uint32_t srcSize);
    void unitTest(const char* fName = 0);
};



void LZWComp::resetCompressEngine()
{  
    if(!m_compDict)
        m_compDict = new LinkHash<compElement, uint32_t>(m_D);
    else
        m_compDict->clear();

    compElement ce;
    for(int i=0; i< 256; ++i)
    {
        ce.m_code = i;
        ce.m_key = i;
        m_compDict->Insert(ce);
    }
    m_used = 256;
    m_cStatus = 0;
    m_left = 0;
}

void LZWComp::resetDecompEngine()
{
    if(!m_decompDict)
        m_decompDict = new decompElement [m_codes];
    if(!m_reconStr)
        m_reconStr = new uint8_t[m_codes+1];

    for (int i=0; i<m_codes; ++i)
    {
        if (i < 256)
        {
            m_decompDict[i].m_suffix = static_cast<int>(i);
        }
        else
        {
            m_decompDict[i].m_suffix = 0;
        }
        m_decompDict[i].m_prefix = 0;
        m_reconStr[i] = 0;
    }

    m_reconStr[m_codes] = 0;
    m_used = 256;
    m_cStatus = 0;
    m_left = 0;

}

void LZWComp::code2byte(uint32_t code,uint8_t *&pos)
{
    // 2 * 12 bits to 3 * 8 bits
    if(m_cStatus == 0)
    {
        m_left = code & 0xf;
        *pos++ = code >> 4;
        m_cStatus = 1;
    }
    else
    {
        *pos++ = (m_left << 4) | (code >> 8);
        *pos++ = code & 0xff;
        m_cStatus = 0;
    }
}

void LZWComp::byte2code(uint32_t &code, const uint8_t* pos, uint32_t& posSize)
{
    // 3 * 8 bits to 2 * 12 bits
    uint8_t c,d ;
    
    c = pos[posSize++];

    if(m_cStatus == 0)
    {
        d = pos[posSize++];
        code = (c<<4) | (d>>4);
        m_left = d & 0xf;
    }
    else
    {
        code = (m_left<<8) | c;
    }
    m_cStatus = 1-m_cStatus;
}

uint8_t LZWComp::code2str(uint32_t code,uint8_t *&pos)
{
    int size=0;
    while(code >= 256)
    {
        m_reconStr[size++] = m_decompDict[code].m_suffix;
        code = m_decompDict[code].m_prefix;
    }
    m_reconStr[size++] = code;

    // reverse
    for (int i=0; i<size; i++)
    {
        *pos++ = m_reconStr[size - 1 - i];
    }
    return m_reconStr[size-1];
}

uint8_t* LZWComp::compress(uint8_t* dst, const uint8_t* src, uint32_t srcSize)
{
    resetCompressEngine();

    uint32_t pos = 0;
    uint32_t pcode;
    uint8_t c;
    compElement ce;

    if( pos < srcSize)
    {
        pcode = c = src[pos++];
        do 
        {
            if(pos >= srcSize) break;
            c = src[pos++];
            uint32_t longKey = (pcode << 8) | c;
            if(m_compDict->Search(longKey, ce))
            {
                pcode = ce.m_code;
            }
            else
            {
                // c is not output
                code2byte(pcode, dst);
                if (m_used < m_codes)
                {
                    ce.m_code = m_used++;
                    ce.m_key = longKey;
                    m_compDict->Insert(ce);
                }
                // reset pcode to c, important
                pcode = c;
            }
        } while (1);

        // break from end of buffer
        code2byte(pcode, dst);
        
        // in case for 4 bits
        if (m_cStatus == 1)
        {
            *dst++ = m_left << 4; // pad at right
            m_cStatus = 0;
        }
    }

    return dst;
}

uint8_t* LZWComp::decompress(uint8_t* dst, const uint8_t* src, uint32_t srcSize)
{
    resetDecompEngine();

    uint32_t pcode, code;
    uint8_t fc;
    
    uint32_t pos = 0;

    if(pos < srcSize )
    {
        byte2code(pcode, src, pos);
        fc = *dst++ = pcode;
        
        while(pos < srcSize )
        {
            byte2code(code, src, pos);
            if(code < m_used)
            {
                fc = code2str(code, dst);
                if(m_used < m_codes)
                {
                    m_decompDict[m_used].m_prefix = pcode;
                    m_decompDict[m_used++].m_suffix = fc;
                }
                
            }
            else
            {
                m_decompDict[m_used].m_prefix = pcode;
                m_decompDict[m_used++].m_suffix = fc;
                fc = code2str(code, dst);
                
            }
            pcode = code;
        }
    }
    
    return dst;

}


void LZWComp::unitTest(const char* fName /* = 0 */)
{
    
    const long MAX_IDX = 20 * 1024;
    uint8_t *uncompData= new uint8_t[MAX_IDX];
    uint8_t *compData= new uint8_t[MAX_IDX];
    uint8_t *resData= new uint8_t[MAX_IDX];
    
    std::default_random_engine engine(static_cast<int>(std::time(0)));
    std::uniform_int_distribution<int> uid(32, 126);
    uint32_t fSize, cSize;
    if (!fName)
    {
        return;
    }
    else
    {
        std::ifstream inf;
        inf.open(fName, std::ios::binary);
        if (inf.is_open())
        {
            // get file size
            inf.seekg(0, std::ios::end);
            fSize = inf.tellg();

            // point to start
            inf.seekg(0, std::ios::beg);
            uint64_t readPos;

            std::ofstream oufC;
            oufC.open("testCompress.lzw", std::ios::binary);
            std::ofstream oufD;
            oufD.open("testDecompress.txt", std::ios::binary);

            bool cont = true;
            uint32_t ucSize;
            while(cont)
            {
                readPos = inf.tellg();
                if (readPos + MAX_IDX < fSize)
                {
                    inf.read((char *)(uncompData), MAX_IDX);
                    ucSize = MAX_IDX;
                }
                else
                {
                    inf.read((char *)(uncompData), fSize-readPos);
                    ucSize = fSize - readPos;
                    cont = false;
                }

                {
                    uint8_t *pComp = compress(compData, uncompData, ucSize);
                    
                    cSize = pComp-compData;
                    oufC.write((const char*)compData, cSize);
                }


                {        
                    uint8_t *pUncomp = decompress(resData, compData, cSize);
                    
                    oufD.write((const char*)resData, pUncomp-resData);
                    
                }

            }

            oufD.close();
            oufC.close();
        }
        else
        {
            std::cerr << "read failed" << std::endl;
        }
        
    }
    
    delete [] uncompData;
    delete [] compData;
    delete [] resData;

}