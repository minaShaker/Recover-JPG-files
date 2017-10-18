#include <stdio.h>
#include <stdint.h>

int main (int argc , char ** argv)
{
    typedef uint8_t byte;
    
    //make sure of the usage
    if (argc != 2)
    {
        fprintf(stderr,"usage: ./recover <name of the file to be recovered>\n");
        return 1;
    }
    
    //open file to recover from
    
    FILE * recover ;
    
    if ((recover = fopen( *(argv+1) ,"r" ))  == NULL)
    {
        fprintf(stderr , "the file cannot be opened\n");
        return 2;
    }
    
    //intitalizing copy
    byte copier[512] = {0};
    
    //buting recover in the start of the file
    fseek(recover, 0 , SEEK_SET);
    
    //initializing name
    int name[999] ;
    for (int x =0 ; x < 999 ; x++)
        name[x]=x;
    char nameC[3];
    
    int temp;
    for (int z =0 ; z < 999 ; z++)
    {
        //iterating to reach the start of the JPEG file 
        do
        { 
            if ((fread(copier , sizeof(byte), 512 , recover))!= 512)
            {
                printf("end of file\n");
                goto u;
            }
            temp = copier[3] & 240;
        }
        while (!(copier[0] == 0xff && copier[1] == 0xd8 && copier[2] == 0xff && temp == 0xe0  ));
    
        //creating file name
        if (name[z] <= 9)
            sprintf(nameC, "00%i.jpg" , name[z]);
        else if (name[z] <= 99)
            sprintf(nameC, "0%i.jpg", name[z]);
        else if (name[z] <= 999)
            sprintf(nameC , "%i.jpg" , name[z]);
        
        //open file to retrive to
        FILE * retrived;
        retrived = fopen(nameC,"w");
    
    
        //writing from recover to retrived
        do
        {
            fwrite(copier , sizeof(byte) , 512 , retrived);
            if (fread(copier , sizeof(byte), 512 , recover)!=512)
            {
                printf("end of file\n");
                goto u;
            }
            temp = copier[3] & 240;
        }
        while (!(copier[0] == 0xff && copier[1] == 0xd8 && copier[2] == 0xff && temp == 0xe0  ));
        
        
        fseek(recover, -512 , SEEK_CUR);
        
        fclose(retrived);
    }
    
    //closing files
    u : fclose(recover);
    return 0; 

}