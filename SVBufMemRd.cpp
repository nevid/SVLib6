

#include "SVBufMemRd.hpp"

namespace SVLib6 {


SVBufMemRd::SVBufMemRd(){
    ResetReadPtr();
}

void SVBufMemRd::SetFromPtr(void *_buf, unsigned bufsz)
{
    buf=_buf;
    objsize=bufsz;
    ResetReadPtr();
}

void *SVBufMemRd::ReadPtr(unsigned size){
    if(curreadpos+size>objsize)	return 0;
    void* p=((__int8*)buf)+curreadpos;
    curreadpos+=size;
    return p;
}

void *SVBufMemRd::ReadPtr2(unsigned size, unsigned &ret_size){
    ret_size=0;
    if(curreadpos>=objsize){return 0;}
    void* p=((__int8*)buf)+curreadpos;
    if(curreadpos+size>objsize)	{ret_size=objsize-curreadpos;}
    else ret_size=size;
    curreadpos+=size;
    return p;
}

void *SVBufMemRd::GetPtr(unsigned offset, unsigned qsize){
    if(offset+qsize>objsize) return 0;
    return (__int8*)buf+offset;
}

void *SVBufMemRd::ReadPtr3(unsigned &ret_size){
    if(curreadpos>=objsize){return 0;}
    void* p=((__int8*)buf)+curreadpos;
    ret_size=objsize-curreadpos;
    return p;
}

}
