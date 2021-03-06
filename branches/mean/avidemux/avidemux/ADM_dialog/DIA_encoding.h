/*

*/
#include "ADM_toolkit/TLK_clock.h"
class DIA_encoding
{
private:
		Clock	clock;
		uint32_t  _lastTime;
		uint32_t  _lastFrame;
		uint32_t _fps_average;
		uint32_t _totalSize;
		uint32_t _bitrate[60];
		uint32_t _roundup;
		uint32_t _current;
		void setBitrate(uint32_t br);
		void setSize(int size);
public:
		DIA_encoding( uint32_t fps1000 );
		~DIA_encoding( );
		
		void reset( void );
		void setPhasis(const char *n);
		void setCodec(const char *n);
		void feedFrame(uint32_t br);		
		void setQuant(uint32_t quant);
		void setFrame(uint32_t nb,uint32_t total);
		
 		uint8_t isAlive(void);
};
