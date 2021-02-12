#include<stdio.h>
#include<string.h>
#include<stdint.h>


void main()
{
	
	uint32_t * data=(uint32_t *)"123456";  //1-base
	
	//uint32_t data1[]={0x31323334,0xffff3635};  //34-base
	
	uint32_t write[]={0xffffffff,0xffffffff};
	
	uint32_t * raddr=write;
	
	uint32_t read[2];
	
	static char str[10];
	
	int i=0;  
	
	int len= (strlen((char*)data)/4 )+  (((strlen((char*)data))%4 )!=0);  
	
	//printf("%d ", ((strlen((char*)data))%4 ));
	
	//printf("%d",len);
	
/////////write	/////////
	for(i=0;i<len;i++)
	{
		write[i]=data[i];  //34333231 3635
	}
/////////	
 

////read
//i=0;
//while(1)
//{
//	read[i]=*raddr;
//	if(i<len)
//	{
//		break;
//	}
//	raddr+=4;
//	i++;
//	
//}
//
//	for(i=0;i<len;i++)
//	{
//		printf("%x ",read[i]);
//	}
//		

//len = ((strlen((char *)write)/4) + ((strlen((char *)write) % 4) != 0)) *4;

//	for (i=0; i<len; i++)
	{
		//str[i] = write[i/4]>>(8*(i%4));
	}
	
	
	
	
	int idx=0;
	len=(strlen((char*)write)/4) + ((strlen((char*)write)%4)!=0 );  //length for 4 byte
	len =strlen((char*)write);  ////length for 1 byte
//	printf("%d\n",len);
	
	for(i=0;i<len;i++)
	{
		str[i] =( (write[i/4])>>(8*(i%4)));		
	}
		
	for(i=0;i<len;i++)
	{	
		printf("%c ",str[i]);
	}
	

	
	
}
