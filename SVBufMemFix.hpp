#ifndef SVBufMemFix6_h__
#define SVBufMemFix6_h__

namespace SVLib6 {

//буфер фиксированного размера (при выходе за границы вернет ошибку)
//SetMemSize(sz) - размер буфера
//ReserveObj(sz) и аналоги - размер объекта в буфере
class SVBufMemFix
{
	void* buf;
	unsigned bufsize;
	unsigned objsize;

	int nodelflag;        //не освобождать память в деструкторе

	unsigned curreadpos;
public:

        SVBufMemFix();
        ~SVBufMemFix();

	//inline void SetGranul(unsigned v){granul=v;}
	//ret: 1=память уже выделена
        int SetMemSize(unsigned sz);

	//ret: 1=переполнение памяти -1=память не выделена
	//созд. объекта
    int ReserveObj(unsigned size);

	//ret: !=0 -error
	//создание объекта копированием
    int CopyFrom(void* ptr,unsigned size);
	
	//копирование с добавлением к объекту
    //ret: !=0 = error
        int CopyFromAdd(void* ptr,unsigned size);

	//ret: 0 - при ошибке
	//увелич. размер объекта на size байт и возвращ. смещение на начало этой области
	//можно использ. для чтения из канала io
        void* ReserveObjAdd(unsigned size);

	//ret: size=сколько можно добавить к объекту и куда 
	//вызываем SizeForEnd, потом запрашиваем по каналу size и вызыв. ReserveObjAdd(сколько получено)
        void* SizeForEnd(unsigned& size);



	//
        //устанавливает объект из существующей памяти
        void SetFromPtr(void* _buf,unsigned bufsz);


        void* Ptr(){return buf;}
        unsigned Size(){return objsize;}
        unsigned BufSize(){return bufsize;}



	//----
        
        void ResetReadPtr(){curreadpos=0;}
        void* ReadPtr(unsigned size);
	//size=запрашиваемое кол-во
	//ret: ret_size=кол-во байт, которые реально можно считать
        void* ReadPtr2(unsigned size,unsigned& ret_size);

	//запраш. указ. по смещен. offset и возможн. получ. qsize байт за ним
	//если возможн. нет: ret=0
        void* GetPtr(unsigned offset,unsigned qsize);
        
};


//---

} //namespace

#endif // SVBufMemFix_h__
