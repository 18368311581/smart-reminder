#include "jpeg_on_lcd.h"
__attribute__((aligned(4))) static uint8_t jpg_buffer[10240];
typedef enum{
    Jpeg_OK   = 0,
    Jpeg_INTR = 1,
    Jpeg_INP  = 2,
    Jpeg_MEM1 = 3,
    Jpeg_MEM2 = 4,
    Jpeg_PAR  = 5,
    Jpeg_FMT1 = 6,
    Jpeg_FMT2 = 7,
    Jpeg_FMT3 = 8
}Jpeg_result;
struct Jpeg_size_rect{
    uint16_t left;
    uint16_t right;
    uint16_t top;
    uint16_t bottom;
};
struct Jpeg_decoding{
    size_t    dctr;
    uint8_t*  dptr;
    uint8_t*  inbuf;
    uint8_t   dbit;
    uint8_t   scale;
    uint8_t   msx,msy;
    uint8_t   qtid[3];
    uint8_t   ncomp;
    int16_t   dcv[3];
    uint16_t  nrst;
    uint16_t  width,height;
    uint8_t*  huffbits[2][2];
    uint16_t* huffcode[2][2];
    uint8_t*  huffdata[2][2];
    int32_t*  qttbl[4];
    uint32_t  wreg;
    uint8_t   marker;
    uint8_t   longofs[2][2];
    uint16_t* hufflut_ac[2];
    uint8_t*  hufflut_dc[2];
    void*     workbuf;
    int16_t*  mcubuf;
    void*     pool;
    size_t    sz_pool;
    void*     device;
};
static const uint8_t Zig[64]={
     0, 1, 8,16, 9, 2, 3,10,17,24,32,25,18,11, 4, 5,
    12,19,26,33,40,48,41,34,27,20,13, 6, 7,14,21,28,
    35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
    58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
};
static const uint16_t Ipsf[64]={
    (uint16_t)(1.00000*8192),(uint16_t)(1.38704*8192),(uint16_t)(1.30656*8192),(uint16_t)(1.17588*8192),(uint16_t)(1.00000*8192),(uint16_t)(0.78570*8192),(uint16_t)(0.54120*8192),(uint16_t)(0.27590*8192),
    (uint16_t)(1.38704*8192),(uint16_t)(1.92388*8192),(uint16_t)(1.81226*8192),(uint16_t)(1.63099*8192),(uint16_t)(1.38704*8192),(uint16_t)(1.08979*8192),(uint16_t)(0.75066*8192),(uint16_t)(0.38268*8192),
    (uint16_t)(1.30656*8192),(uint16_t)(1.81226*8192),(uint16_t)(1.70711*8192),(uint16_t)(1.53636*8192),(uint16_t)(1.30656*8192),(uint16_t)(1.02656*8192),(uint16_t)(0.70711*8192),(uint16_t)(0.36048*8192),
    (uint16_t)(1.17588*8192),(uint16_t)(1.63099*8192),(uint16_t)(1.53636*8192),(uint16_t)(1.38268*8192),(uint16_t)(1.17588*8192),(uint16_t)(0.92388*8192),(uint16_t)(0.63638*8192),(uint16_t)(0.32442*8192),
    (uint16_t)(1.00000*8192),(uint16_t)(1.38704*8192),(uint16_t)(1.30656*8192),(uint16_t)(1.17588*8192),(uint16_t)(1.00000*8192),(uint16_t)(0.78570*8192),(uint16_t)(0.54120*8192),(uint16_t)(0.27590*8192),
    (uint16_t)(0.78570*8192),(uint16_t)(1.08979*8192),(uint16_t)(1.02656*8192),(uint16_t)(0.92388*8192),(uint16_t)(0.78570*8192),(uint16_t)(0.61732*8192),(uint16_t)(0.42522*8192),(uint16_t)(0.21677*8192),
    (uint16_t)(0.54120*8192),(uint16_t)(0.75066*8192),(uint16_t)(0.70711*8192),(uint16_t)(0.63638*8192),(uint16_t)(0.54120*8192),(uint16_t)(0.42522*8192),(uint16_t)(0.29290*8192),(uint16_t)(0.14932*8192),
    (uint16_t)(0.27590*8192),(uint16_t)(0.38268*8192),(uint16_t)(0.36048*8192),(uint16_t)(0.32442*8192),(uint16_t)(0.27590*8192),(uint16_t)(0.21678*8192),(uint16_t)(0.14932*8192),(uint16_t)(0.07612*8192)
};
static const uint8_t Clip8[1024]={
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
     32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
     64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95,
     96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,
    128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,
    160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
    192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,
    224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};
inline static uint8_t BYTECLIP(int v){
    return Clip8[(unsigned int)(v)&0x3FF];
}
inline static uint16_t LDB_WORD(uint8_t *ptr){
    return (uint16_t)(((uint16_t)*((uint8_t*)(ptr))<<8)|(uint16_t)*(uint8_t*)((ptr)+1));
}
static void* alloc_pool(struct Jpeg_decoding* jd,size_t ndata){
    char* rp=0;
    ndata=(ndata+3)&~3;
    if(jd->sz_pool>=ndata){
        jd->sz_pool-=ndata;
        rp=(char*)jd->pool;
        jd->pool=(void*)(rp+ndata);
    }
    return (void*)rp;
}
static size_t jpeg_in_func(struct Jpeg_decoding* jd,uint8_t *buf,size_t num){
    uint16_t rb;
    FIL* dev=(FIL*)jd->device;
    if(buf){
        f_read(dev,buf,num,(UINT*)&rb);
        return rb;
    }
    else{
        return (f_lseek(dev,f_tell(dev)+num)==FR_OK)?num:0;
    }
}
static Jpeg_result create_qt_tbl(struct Jpeg_decoding* jd,const uint8_t *data,size_t ndata){
    uint32_t i,zi;uint8_t d;int32_t *pb;
    while(ndata){
        if(ndata<65)return Jpeg_FMT1;
        ndata-=65;
        d=*data++;
        if(d&0xF0)return Jpeg_FMT1;
        i=d&3;
        pb=alloc_pool(jd,64*sizeof(int32_t));
        if(!pb)return Jpeg_MEM1;
        jd->qttbl[i]=pb;
        for(i=0;i<64;i++){
            zi=Zig[i];
            pb[zi]=(int32_t)((uint32_t)*data++*Ipsf[zi]);
        }
    }
    return Jpeg_OK;
}
static Jpeg_result create_huffman_tbl(struct Jpeg_decoding* jd,const uint8_t *data,size_t ndata){
    uint32_t i,j,b,cls,num;
    uint16_t hc=0,*ph;
    uint8_t d,*pb,*pd;
    size_t np;
    while(ndata){
        if(ndata<17)return Jpeg_FMT1;
        ndata-=17;
        d=*data++;
        if(d&0xEE)return Jpeg_FMT1;
        cls=d>>4;
        num=d&0x0F;
        pb=alloc_pool(jd,16);
        if(!pb)return Jpeg_MEM1;
        jd->huffbits[num][cls]=pb;
        for(np=i=0;i<16;i++){
            np+=(pb[i]=*data++);
        }
        ph=alloc_pool(jd,np*sizeof(uint16_t));
        if(!ph)return Jpeg_MEM1;
        jd->huffcode[num][cls]=ph;
        for(j=i=0;i<16;i++){
            b=pb[i];
            while(b--)ph[j++]=hc++;
            hc<<=1;
        }
        if(ndata<np)return Jpeg_FMT1;
        ndata-=np;
        pd=alloc_pool(jd,np);
        if (!pd) return Jpeg_MEM1;
        jd->huffdata[num][cls]=pd;
        for(i=0;i<np;i++){
            d=*data++;
            if(!cls&&d>11)return Jpeg_FMT1;
            pd[i]=d;
        }
        uint32_t span,td,ti;
        uint16_t *tbl_ac=0;
        uint8_t *tbl_dc=0;
        if(cls){
            tbl_ac=alloc_pool(jd,1024*sizeof(uint16_t));
            if(!tbl_ac)return Jpeg_MEM1;
            jd->hufflut_ac[num]=tbl_ac;
            memset(tbl_ac,0xFF,1024*sizeof(uint16_t));
        }
        else{
            tbl_dc=alloc_pool(jd,1024*sizeof(uint8_t));
            if(!tbl_dc)return Jpeg_MEM1;
            jd->hufflut_dc[num]=tbl_dc;
            memset(tbl_dc,0xFF,1024*sizeof(uint8_t));
        }
        for(i=b=0;b<10;b++){
            for(j=pb[b];j;j--){
                ti=ph[i]<<(9-b)&1023;
                if(cls){
                    td=pd[i++]|((b+1)<<8);
                    for(span=1<<(9-b);span;span--,tbl_ac[ti++]=(uint16_t)td);
                }
                else{
                    td=pd[i++]|((b+1)<<4);
                    for(span=1<<(9-b);span;span--,tbl_dc[ti++]=(uint8_t)td);
                }
            }
        }
        jd->longofs[num][cls]=i;
    }
    return Jpeg_OK;
}
static int huffext(struct Jpeg_decoding* jd,uint32_t id,uint32_t cls){
    size_t dc=jd->dctr;
    uint8_t *dp=jd->dptr;
    uint32_t d,flg=0;
    const uint8_t *hb,*hd;
    const uint16_t *hc;
    uint32_t nc,bl,wbit=jd->dbit%32;
    uint32_t w=jd->wreg&((1UL<<wbit)-1);
    while(wbit<16){
        if(jd->marker){
            d=0xFF;
        }
        else{
            if(!dc){
                dp=jd->inbuf;
                dc=jpeg_in_func(jd,dp,1024);
                if(!dc)return 0-(int)Jpeg_INP;
            }
            d=*dp++;dc--;
            if(flg){
                flg=0;
                if(d!=0){jd->marker=d;}
                d=0xFF;
            }
            else{
                if(d==0xFF){
                    flg=1;continue;
                }
            }
        }
        w=w<<8|d;wbit+=8;
    }
    jd->dctr=dc;jd->dptr=dp;jd->wreg=w;
    d=(uint32_t)(w>>(wbit-10));
    if(cls){
        d=jd->hufflut_ac[id][d];
        if(d!=0xFFFF){
            jd->dbit=wbit-(d>>8);
            return d&0xFF;
        }
    }
    else{
        d=jd->hufflut_dc[id][d];
        if(d!=0xFF){
            jd->dbit=wbit-(d>>4);
            return d&0xF;
        }
    }
    hb=jd->huffbits[id][cls]+10;
    hc=jd->huffcode[id][cls]+jd->longofs[id][cls];
    hd=jd->huffdata[id][cls]+jd->longofs[id][cls];
    for(bl=11;bl<=16;bl++){
        nc=*hb++;
        if(nc){
            d=w>>(wbit-bl);
            do{
                if(d==*hc++){
                    jd->dbit=wbit-bl;
                    return *hd;
                }
                hd++;
            }while(--nc);
        }
    }
    return 0-(int)Jpeg_FMT1;
}
static int bitext(struct Jpeg_decoding* jd,uint32_t nbit){
    size_t dc=jd->dctr;
    uint8_t *dp=jd->dptr;
    uint32_t d,flg=0;
    uint32_t wbit=jd->dbit%32;
    uint32_t w=jd->wreg&((1UL<<wbit)-1);
    while(wbit<nbit){
        if(jd->marker){
            d=0xFF;
        }
        else{
            if(!dc){
                dp=jd->inbuf;
                dc=jpeg_in_func(jd,dp,1024);
                if(!dc)return 0-(int)Jpeg_INP;
            }
            d=*dp++;dc--;
            if(flg){
                flg=0;
                if(d!=0){jd->marker=d;}
                d=0xFF;
            }
            else{
                if(d==0xFF){
                    flg=1;continue;
                }
            }
        }
        w=w<<8|d;wbit+=8;
    }
    jd->wreg=w;jd->dbit=wbit-nbit;
    jd->dctr=dc;jd->dptr=dp;
    return (int)(w>>((wbit-nbit)%32));
}
static Jpeg_result restart(struct Jpeg_decoding* jd,uint16_t rstn){
    uint8_t i;
    uint8_t* dp=jd->dptr;
    size_t dc=jd->dctr;
    uint16_t marker;
    if(jd->marker){
        marker=0xFF00|jd->marker;
        jd->marker=0;
    }
    else{
        marker=0;
        for(i=0;i<2;i++){
            if(!dc){
                dp=jd->inbuf;
                dc=jpeg_in_func(jd,dp,1024);
                if(!dc)return Jpeg_INP;
            }
            marker=(marker<<8)|*dp++;
            dc--;
        }
        jd->dptr=dp;
        jd->dctr=dc;
    }
    if((marker&0xFFD8)!=0xFFD0||(marker&7)!=(rstn&7)){return Jpeg_FMT1;}
    jd->dbit=0;
    jd->dcv[2]=jd->dcv[1]=jd->dcv[0]=0;
    return Jpeg_OK;
}
static void block_idct(int32_t *src,int16_t *dst){
    const int32_t M13=(int32_t)(1.41421*4096),M2=(int32_t)(1.08239*4096),M4=(int32_t)(2.61313*4096),M5=(int32_t)(1.84776*4096);
    int32_t v0,v1,v2,v3,v4,v5,v6,v7;
    int32_t t10,t11,t12,t13;
    for(uint8_t i=0;i<8;i++){
        v0=src[ 0];v1=src[16];v2=src[32];v3=src[48];
        t10=v0+v2;t12=v0-v2;t11=(v1-v3)*M13>>12;v3+=v1;t11-=v3;
        v0=t10+v3;v3=t10-v3;v1=t11+t12;v2=t12-t11;
        v4=src[56];v5=src[ 8];v6=src[40];v7=src[24];
        t10=v5-v4;t11=v5+v4;t12=v6-v7;v7+=v6;v5=(t11-v7)*M13>>12;
        v7+=t11;t13=(t10+t12)*M5>>12;v4=t13-(t10*M2>>12);
        v6=t13-(t12*M4>>12)-v7;v5-=v6;v4-=v5;
        src[ 0]=v0+v7;src[56]=v0-v7;src[ 8]=v1+v6;src[48]=v1-v6;
        src[16]=v2+v5;src[40]=v2-v5;src[24]=v3+v4;src[32]=v3-v4;
        src++;
    }
    src-=8;
    for(uint8_t i=0;i<8;i++){
        v0=src[0]+(128L<<8);v1=src[2];v2=src[4];v3=src[6];
        t10=v0+v2;t12=v0-v2;t11=(v1-v3)*M13>>12;v3+=v1;t11-=v3;
        v0=t10+v3;v3=t10-v3;v1=t11+t12;v2=t12-t11;
        v4=src[7];v5=src[1];v6=src[5];v7=src[3];
        t10=v5-v4;t11=v5+v4;t12=v6-v7;v7+=v6;v5=(t11-v7)*M13>>12;
        v7+=t11;t13=(t10 + t12)*M5>>12;v4=t13-(t10*M2>>12);
        v6=t13-(t12*M4>>12)-v7;v5-=v6;v4-=v5;
        dst[0]=(int16_t)((v0+v7)>>8);
        dst[7]=(int16_t)((v0-v7)>>8);
        dst[1]=(int16_t)((v1+v6)>>8);
        dst[6]=(int16_t)((v1-v6)>>8);
        dst[2]=(int16_t)((v2+v5)>>8);
        dst[5]=(int16_t)((v2-v5)>>8);
        dst[3]=(int16_t)((v3+v4)>>8);
        dst[4]=(int16_t)((v3-v4)>>8);
        dst+=8;src+=8;
    }
}
static Jpeg_result mcu_load(struct Jpeg_decoding* jd){
    int32_t *tmp=(int32_t*)jd->workbuf;
    int d,e;uint32_t blk,nby,i,bc,z,id,cmp;
    int16_t *bp;const int32_t *dqf;
    nby=jd->msx*jd->msy;
    bp=jd->mcubuf;
    for(blk=0;blk<nby+2;blk++){
        cmp=(blk<nby)?0:blk-nby+1;
        if(cmp&&jd->ncomp!=3){
            for(i=0;i<64;bp[i++]=128);
        }
        else{
            id=cmp?1:0;
            d=huffext(jd,id,0);
            if(d<0)return (Jpeg_result)(0-d);
            bc=(uint32_t)d;d=jd->dcv[cmp];
            if(bc){
                e=bitext(jd,bc);
                if(e<0)return (Jpeg_result)(0-e);
                bc=1<<(bc-1);
                if(!(e&bc))e-=(bc<<1)-1;
                d+=e;
                jd->dcv[cmp]=(int16_t)d;
            }
            dqf=jd->qttbl[jd->qtid[cmp]];
            tmp[0]=d*dqf[0]>>8;
            memset(&tmp[1],0,63*sizeof(int32_t));
            z=1;
            do{
                d=huffext(jd,id,1);
                if(d==0)break;
                if(d<0)return (Jpeg_result)(0-d);
                bc=(uint32_t)d;z+=bc>>4;
                if(z>=64) return Jpeg_FMT1;
                if(bc&=0x0F){
                    d=bitext(jd,bc);
                    if(d<0)return (Jpeg_result)(0-d);
                    bc=1<<(bc-1);
                    if(!(d&bc))d-=(bc<<1)-1;
                    i=Zig[z];
                    tmp[i]=d*dqf[i]>>8;
                }
            }while(++z<64);
            if(1||!cmp){
                if(z==1||jd->scale==3){
                    d=(int16_t)((*tmp/256)+128);
                    for(i=0;i<64;bp[i++]=d);
                }
                else{
                    block_idct(tmp,bp);
                }
            }
        }
        bp+=64;
    }
    return Jpeg_OK;
}
static Jpeg_result mcu_output_pre(struct Jpeg_decoding* jd,uint32_t x,uint32_t y,struct Jpeg_size_rect* rect){
    const int CVACC=(sizeof(int)>2)?1024:128;
    uint32_t ix,iy,mx,my,rx,ry;int yy,cb,cr;
    int16_t *py,*pc;uint8_t *pix;
    mx=jd->msx*8;my=jd->msy*8;
    rx=(x+mx<=jd->width)?mx:jd->width-x;
    ry=(y+my<=jd->height)?my:jd->height-y;
    rx>>=jd->scale;ry>>=jd->scale;
    if(!rx||!ry)return Jpeg_OK;
    x>>=jd->scale;y>>=jd->scale;
    rect->left=x;rect->right=x+rx-1;
    rect->top=y;rect->bottom=y+ry-1;
    if(jd->scale!=3){
        pix=(uint8_t *)jd->workbuf;
        for(iy=0;iy<my;iy++){
            pc=py=jd->mcubuf;
            if(my==16){
                pc+=64*4+(iy>>1)*8;
                if(iy>=8)py+=64;
            }
            else{
                pc+=mx*8+iy*8;
            }
            py+=iy*8;
            for(ix=0;ix<mx;ix++){
                cb=pc[0]-128;cr=pc[64]-128;
                if(mx==16){
                    if(ix==8)py+=64-8;
                    pc+=ix&1;
                }
                else{
                    pc++;
                }
                yy=*py++;
                *pix++=BYTECLIP(yy+((int)(1.402*CVACC)*cr)/CVACC);
                *pix++=BYTECLIP(yy-((int)(0.344*CVACC)*cb+(int)(0.714*CVACC)*cr)/CVACC);
                *pix++=BYTECLIP(yy+((int)(1.772*CVACC)*cb)/CVACC);
            }
        }
        if(jd->scale){
            uint32_t x,y,r,g,b,s,w,a;uint8_t *op;
            s=jd->scale*2;
            w=1<<jd->scale;
            a=(mx-w)*3;
            op=(uint8_t*)jd->workbuf;
            for(iy=0;iy<my;iy+=w){
                for(ix=0;ix<mx;ix+=w){
                    pix=(uint8_t*)jd->workbuf+(iy*mx+ix)*3;
                    r=g=b=0;
                    for(y=0;y<w;y++){
                        for(x=0;x<w;x++){
                            r+=*pix++;
                            g+=*pix++;
                            b+=*pix++;
                        }
                        pix+=a;
                    }
                    *op++=(uint8_t)(r>>s);
                    *op++=(uint8_t)(g>>s);
                    *op++=(uint8_t)(b>>s);
                }
            }
        }
    }
    else{
        pix=(uint8_t*)jd->workbuf;
        pc=jd->mcubuf+mx*my;
        cb=pc[0]-128;cr=pc[64]-128;
        for(iy=0;iy<my;iy+=8){
            py=jd->mcubuf;
            if(iy==8)py+=128;
            for(ix=0;ix<mx;ix+=8){
                yy=*py;py+=64;
                *pix++=BYTECLIP(yy+((int)(1.402*CVACC)*cr/CVACC));
                *pix++=BYTECLIP(yy-((int)(0.344*CVACC)*cb+(int)(0.714*CVACC)*cr)/CVACC);
                *pix++=BYTECLIP(yy+((int)(1.772*CVACC)*cb/CVACC));
            }
        }
    }
    mx>>=jd->scale;
    if(rx<mx){
        uint8_t *s,*d;uint32_t x,y;
        s=d=(uint8_t*)jd->workbuf;
        for(y=0;y<ry;y++){
            for(x=0;x<rx;x++){
                *d++=*s++;
                *d++=*s++;
                *d++=*s++;
            }
            s+=(mx-rx)*3;
        }
    }
    uint8_t *sjp=(uint8_t*)jd->workbuf;
    uint16_t wjp,*djk=(uint16_t*)sjp;
    uint32_t nsb=rx*ry;
    do{
        wjp=(*sjp++&0xF8)<<8;
        wjp|=(*sjp++&0xFC)<<3;
        wjp|=*sjp++>>3;
        *djk++=wjp;
    }while(--nsb);
    return Jpeg_OK;

}
static Jpeg_result jd_prepare(struct Jpeg_decoding* jd,void* pool,size_t sz_pool,void* dev){
    uint8_t *seg,b;
    uint16_t marker;
    uint32_t n,i,ofs;
    size_t len;
    Jpeg_result rc;
    memset(jd,0,sizeof(struct Jpeg_decoding));
    jd->pool=pool;
    jd->sz_pool=sz_pool;
    jd->device=dev;
    jd->inbuf=seg=alloc_pool(jd,1024);
    if(!seg)return Jpeg_MEM1;
    ofs=marker=0;
    do{
        if(jpeg_in_func(jd,seg,1)!=1)return Jpeg_INP;
        ofs++;marker=marker<<8|seg[0];
    }while(marker!=0xFFD8);
    for(;;){
        if(jpeg_in_func(jd,seg,4)!=4)return Jpeg_INP;
        marker=LDB_WORD(seg);
        len=LDB_WORD(seg+2);
        if(len<=2||(marker>>8)!=0xFF)return Jpeg_FMT1;
        len-=2;ofs+=4+len;
        switch(marker&0xFF){
            case 0xC0:
                if(len>1024)return Jpeg_MEM2;
                if(jpeg_in_func(jd,seg,len)!=len)return Jpeg_INP;
                jd->width=LDB_WORD(&seg[3]);
                jd->height=LDB_WORD(&seg[1]);
                jd->ncomp=seg[5];
                if(jd->ncomp!=3&&jd->ncomp!=1)return Jpeg_FMT3;
                for(i=0;i<jd->ncomp;i++){
                    b=seg[7+3*i];
                    if(i==0){
                        if(b!=0x11&&b!=0x22&&b!=0x21){
                            return Jpeg_FMT3;
                        }
                        jd->msx=b>>4;jd->msy=b&15;
                    }
                    else{
                        if(b!=0x11)return Jpeg_FMT3;
                    }
                    jd->qtid[i]=seg[8+3*i];
                    if(jd->qtid[i]>3)return Jpeg_FMT3;
                }
                break;
            case 0xDD:
                if(len>1024)return Jpeg_MEM2;
                if(jpeg_in_func(jd,seg,len)!=len)return Jpeg_INP;
                jd->nrst=LDB_WORD(seg);
                break;
            case 0xC4:
                if(len>1024)return Jpeg_MEM2;
                if(jpeg_in_func(jd,seg,len)!=len)return Jpeg_INP;
                rc=create_huffman_tbl(jd,seg,len);
                if(rc)return rc;
                break;
            case 0xDB:
                if(len>1024)return Jpeg_MEM2;
                if(jpeg_in_func(jd,seg,len)!=len)return Jpeg_INP;
                rc=create_qt_tbl(jd,seg,len);
                if(rc)return rc;
                break;
            case 0xDA:
                if(len>1024)return Jpeg_MEM2;
                if(jpeg_in_func(jd,seg,len)!=len)return Jpeg_INP;
                if(!jd->width||!jd->height)return Jpeg_FMT1;
                if(seg[0]!=jd->ncomp)return Jpeg_FMT3;
                for(i=0;i<jd->ncomp;i++){
                    b=seg[2+2*i];
                    if(b!=0x00&&b!=0x11)return Jpeg_FMT3;
                    n=i?1:0;
                    if(!jd->huffbits[n][0]||!jd->huffbits[n][1]){
                        return Jpeg_FMT1;
                    }
                    if(!jd->qttbl[jd->qtid[i]]){
                        return Jpeg_FMT1;
                    }
                }
                n=jd->msy*jd->msx;
                if(!n)return Jpeg_FMT1;
                len=n*64*2+64;
                if(len<256)len=256;
                jd->workbuf=alloc_pool(jd,len);
                if(!jd->workbuf)return Jpeg_MEM1;
                jd->mcubuf=alloc_pool(jd,(n+2)*64*sizeof(int16_t));
                if(!jd->mcubuf)return Jpeg_MEM1;
                if(ofs%=1024){
                    jd->dctr=jpeg_in_func(jd,seg+ofs,(size_t)(1024-ofs));
                }
                jd->dptr=seg+ofs;
                return Jpeg_OK;
            case 0xC1:
            case 0xC2:
            case 0xC3:
            case 0xC5:
            case 0xC6:
            case 0xC7:
            case 0xC9:
            case 0xCA:
            case 0xCB:
            case 0xCD:
            case 0xCE:
            case 0xCF:
            case 0xD9:
                return Jpeg_FMT3;
            default:
                if(jpeg_in_func(jd,0,len)!=len)return Jpeg_INP;
        }
    }
}
static void jpeg_out_func_point(struct Jpeg_decoding* jd,void* rgbbuf,struct Jpeg_size_rect* rect,uint32_t S_XOFF,uint32_t S_YOFF,uint32_t Div_Fac){
    uint16_t realx=rect->left,realy=0;
    uint16_t *pencolor=rgbbuf;
    uint16_t width=rect->right-rect->left+1;
    uint16_t height=rect->bottom-rect->top+1;
    uint32_t staticx=0xFFFF,staticy=0xFFFF;
    for(uint16_t i=0;i<height;i++){
        realy=(Div_Fac*(rect->top+i))>>13;
        if(realx==staticx&&realy==staticy){
            pencolor+=width;continue;
        }
        for(uint16_t j=0;j<width;j++){
            realx=(Div_Fac*(rect->left+j))>>13;
            if(realx==staticx&&realy==staticy){
                pencolor++;continue;
            }
            else{
                staticx=realx;staticy=realy;
            }
            Lcd_set_point(realx+S_XOFF,realy+S_YOFF,*pencolor);
            pencolor++;
        }
    }
}
static Jpeg_result jd_decomp(struct Jpeg_decoding* jd,uint8_t scale,uint8_t out_func_mode,uint32_t S_XOFF,uint32_t S_YOFF,uint32_t Div_Fac){
    uint32_t x,y,mx,my;
    uint16_t rst,rsc;
    Jpeg_result rc;
    if(scale>3)return Jpeg_PAR;
    jd->scale=scale;
    mx=jd->msx*8;my=jd->msy*8;
    jd->dcv[2]=jd->dcv[1]=jd->dcv[0]=0;
    rst=rsc=0;rc=Jpeg_OK;
    for(y=0;y<jd->height;y+=my){
        for(x=0;x<jd->width;x+=mx){
            if(jd->nrst&&rst++==jd->nrst){
                rc=restart(jd,rsc++);
                if(rc!=Jpeg_OK)return rc;
                rst=1;
            }
            rc=mcu_load(jd);    
            if(rc!=Jpeg_OK)return rc;
            struct Jpeg_size_rect rect;
            rc=mcu_output_pre(jd,x,y,&rect);
            if(rc!=Jpeg_OK)return rc;
            if(out_func_mode){
                Lcd_set_point_list(rect.left+S_XOFF,rect.top+S_YOFF,rect.right-rect.left+1,rect.bottom-rect.top+1,(uint16_t*)(jd->workbuf));
            }
            else{
                jpeg_out_func_point(jd,jd->workbuf,&rect,S_XOFF,S_YOFF,Div_Fac);
            }
        }
    }
    return rc;
}
void Lcd_draw_jpeg_pic(char *file_path,uint16_t x,uint16_t y,uint16_t width,uint16_t height){
    uint8_t scale,outfun=0;float temp,temp1;
    if(width==0||height==0||(x+width>480)||(y+height>800)){return;}
    uint32_t S_YOFF=y,S_XOFF=x,ImgHeight,ImgWidth;
    struct Jpeg_decoding jpeg_dev;FIL f_jpeg;
    if(f_open(&f_jpeg,(const TCHAR*)file_path,FA_READ)!=FR_OK){return;}
    if(jd_prepare(&jpeg_dev,jpg_buffer,10240,&f_jpeg)!=Jpeg_OK){return;}
    for(scale=0;scale<4;scale++){
        if((jpeg_dev.width>>scale)<=width&&(jpeg_dev.height>>scale)<=height){
            if(((jpeg_dev.width>>scale)!=width)&&((jpeg_dev.height>>scale)!=height&&scale)){scale=0;}
            else{outfun=1;}
            break;
        }
    }
    if(scale==4)scale=0;
    ImgHeight=jpeg_dev.height>>scale;
    ImgWidth=jpeg_dev.width>>scale;
    temp=(float)width/ImgWidth;
    temp1=(float)height/ImgHeight;
    if(temp<temp1)temp1=temp;
    if(temp1>1)temp1=1;
    S_XOFF+=(width-temp1*ImgWidth)/2;
    S_YOFF+=(height-temp1*ImgHeight)/2;
    temp1*=8192;
    jd_decomp(&jpeg_dev,scale,outfun,S_XOFF,S_YOFF,temp1);
    f_close(&f_jpeg);
    return;
}
