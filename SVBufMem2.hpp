
/*
* SV Library 6
* Sergey Voskresensky (C)
* http://svsoft.svhome.org
* nevidprogr@gmail.com
*/


#ifndef SVBufMem6_h__
#define SVBufMem6_h__

#include "SVLib6Types.h"

namespace SVLib6 {


class SVBufMem2
{
	void* buf;
	unsigned bufsize;
	unsigned granul;

	unsigned objsize;

        unsigned maxsize=100000000;

public:

    SVBufMem2();
    ~SVBufMem2();

     void SetGranul(unsigned v);
     void SetMaxSize(unsigned _sz);

     //теперь функци может вернуть ошибку - 1,2
     int ReserveObj(unsigned size);
    void CopyFrom(void* ptr,unsigned size);
	//копирование с добавлением
    void CopyFromAdd(void* ptr,unsigned size);

        //резервирует дополнительно size байт и возвращ. указ. на начало этой области
        //можно использ. для чтения из канала io
        //при следующ. добавлении в данный объект этот указ. может измениться !
        //может вернуть 0 !!!
        void* ReserveObjAdd(unsigned size);


    void* Ptr();
    unsigned Size();

    //копирует байты с _pos до конца объекта в начало и изменяет размер объекта на это знач.
    //если _pos больше объекта, то ret=1, else ret=0
    int CopyToBegin(unsigned _pos);



	//----
    /*  для этого есть SVBufMemRd
    void ResetReadPtr();
    void* ReadPtr(unsigned size);
	//аналог ReadPtr, но смещает позицию чтения за конец объекта, возвращая 0
    void* ReadPtrA(unsigned size);
	//size=запрашиваемое кол-во
	//ret: ret_size=кол-во байт, которые реально можно считать
    void* ReadPtr2(unsigned size,unsigned& ret_size);

        //запраш. указ. по смещен. offset и возможн. получ. qsize байт за ним
        //если возможн. нет: ret=0
        void* GetPtr(unsigned offset,unsigned qsize);

        //ret: указатель на текущ. позиц. чтения (0-конец) и ret_size-сколько еще можно прочитать
        //указатель чтения не изменяется!
        void* ReadPtr3(unsigned& ret_size);
        */

};

//позволяет создать SVBufMem2 с заданной гранулярностью
//gr - гранулярность
//например: SVBufMem2E<50> ebuf;
template <int gr> class SVBufMem2E:public SVBufMem2
{
public:
    SVBufMem2E(){
        SetGranul(gr);
    }
};

} //namespace

#endif
