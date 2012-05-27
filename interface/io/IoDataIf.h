#ifndef __IO_DATA_IF_H_
#define __IO_DATA_IF_H_

class TempMemBuffer_c;

class IoDataIf_i
{
    public:
        IoDataIf_i() {}
        virtual ~IoDataIf_i() {}

        virtual TempMemBuffer_c* retrieveRequestContent() = 0;
        //virtual void generateRequest(TempMemBuffer_c& tbBuffer) = 0;
        virtual void analyzeResponse(TempMemBuffer_c& tbBuffer) = 0;

    protected:
    private:
};

#endif
