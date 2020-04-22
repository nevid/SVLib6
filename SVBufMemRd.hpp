#ifndef SVBufMemRd6_h__
#define SVBufMemRd6_h__

namespace SVLib6 {

//только для чтения (фактически указатель)
class SVBufMemRd
{
	void* buf;
	unsigned objsize;
	unsigned curreadpos;
public:
        SVBufMemRd();
	
    //устанавливает из существующей памяти 
        void SetFromPtr(void* _buf,unsigned bufsz);

        void* Ptr(){return buf;}
        unsigned Size(){return objsize;}


	//----
        void ResetReadPtr(){curreadpos=0;}
        void* ReadPtr(unsigned size);
	//size=запрашиваемое кол-во
	//ret: ret_size=кол-во байт, которые реально можно считать
        void* ReadPtr2(unsigned size,unsigned& ret_size);

        //запраш. указ. по смещен. offset и возможн. получ. qsize байт за ним
        //если возможн. нет: ret=0
        void* GetPtr(unsigned offset,unsigned qsize);

        //ret: указатель на текущ. позиц. чтения (0-конец) и ret_size-сколько еще можно прочитать
        //указатель чтения не изменяется!
        void* ReadPtr3(unsigned& ret_size);

};



} //namespace

#endif
