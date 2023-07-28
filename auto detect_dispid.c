//#include <i2c/smbus.h>
#include <sys/fcntl.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>      
#include <unistd.h>     
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

int dsi_id,dp_id,dp1_id,edp_id,display_count=0;
int fd_dsi,fd_dp,fd_dp1,fd_edp,p,q,r,s;  /* The fd for i2c busses */
int mcu_add=0x5;

int i2c_write_data(int fd,char *reg,int data)
{
    if(write(fd,reg,1)!=1)
    printf("\nfailed to write to reg:%s : data:%d",reg,data);
    
    return 0;
}

int read_display_id(int fd,char *buf)
{
    char disp_id;
    if (ioctl(fd, I2C_SLAVE, mcu_add) < 0) 
    printf("\nUnable to access the slave :%d",mcu_add);
    
    if(disp_id=read(fd,buf,1)!=1)
    {
         printf("\nfailed to read mcu add on bus");
         return -1;
    }
    else
    printf("mcu read succesfull");

    return disp_id;
}

int slave_settings(int fd)
{
   printf("\n slave settings and enable I2C passthrough");
   i2c_write_data(fd,"0x70",0x58);   
   i2c_write_data(fd,"0x78",0x58); 
   i2c_write_data(fd,"0x88",0x00); 
   i2c_write_data(fd,"0x71",0x24); 
   i2c_write_data(fd,"0x79",0x24); 
   i2c_write_data(fd,"0x89",0x00); 
   i2c_write_data(fd,"0x7",0x88);     //Enable I2C Pass Through
   
   return 0;
}


int dsi(int addr)
{
   fd_dsi = open("/dev/i2c3", O_RDWR);   //O_RDWR  Open for reading and writing of i2c3
   if(fd_dsi < 0 )
    printf("Unable to open the I2c3 bus");
   else
   {
    printf("\nI2C3 is avialable");
    if (ioctl(fd_dsi, I2C_SLAVE, addr) < 0)           //device address you want to communicate:
    printf("\nUnable to access the slave :%x",addr);
    else{
         slave_settings(fd_dsi);
         if(dsi_id=read_display_id(fd_dsi,"0x12")!=1)
         display_count++;
    }
   }
   close(fd_dsi);
   return dsi_id;
}


int dp(int addr)
{
   fd_dp = open("/dev/i2c5", O_RDWR);   //O_RDWR  Open for reading and writing of i2c3
   if(fd_dp < 0 ){ printf("\nUnable to open the I2c5 bus");}
   else{ 
    printf("I2C5 is avialable");
    if (ioctl(fd_dp, I2C_SLAVE, addr) < 0) 
    printf("\nUnable to access the slave :%x",addr);
    else{
    slave_settings(fd_dp);
    if(dp_id=read_display_id(fd_dp,"0x12")!=-1)
    display_count++; }
   }
   close(fd_dp);
   return dp_id;
   
}

int dp1(int addr)
{
   fd_dp1 = open("/dev/i2c6", O_RDWR);   //O_RDWR  Open for reading and writing of i2c3
   if(fd_dp1 < 0 )
   printf("\nUnable to open the I2c6 bus");
   else{
       printf("\nI2C6 is avialable");

   if (ioctl(fd_dp1, I2C_SLAVE, addr) < 0) 
   printf("\nUnable to access the slave :%x",addr);
   else{
      slave_settings(fd_dp1);
      if(dp1_id=read_display_id(fd_dp,"0x12") == -1)
      {
        printf("\nchecking with mcu add 0x13");
        if(dp1_id=read_display_id(fd_dp,"0x13") !=-1)
        display_count++;
      }
      else
      display_count++;
      }
   }
   close(fd_dp1);
   return dp1_id;
   
}

int edp(int addr)
{
   fd_edp = open("/dev/i2c4", O_RDWR);   //O_RDWR  Open for reading and writing of i2c3
   if(fd_edp < 0) 
   printf("\nUnable to open the I2c4 bus");
   
   else{
   printf("\nI2C4 is avialable");
        if (ioctl(fd_edp, I2C_SLAVE, addr) < 0) 
        printf("\nUnable to access the slave :%x",addr);
        else{
             slave_settings(fd_edp);
             if(edp_id=read_display_id(fd_dp,"0x12")!= -1)
             display_count++;
       }
   }
   close(fd_edp);
   return edp_id;
}

int main()
{
   p=dsi(0xc);      //p stores  display id of dsi interface
   q=dp(0xc);       //q  display id of dp interface
   r=dp1(0x11);
   s=edp(0x1A);
   
   if(p!=0 || q!=0 || r!=0 || s!=0  && display_count!=0)
   {
       if(p==2 && q ==5 )
       printf("\n Topologies available with the connected displays : Topology-15 & Topology-16");
       else if(q ==9 && display_count == 1)
       printf("\n Topologies available with the connected displays : Topology-17");
       else if(p==0 && q ==5 )
       printf("\nTopologies available with the connected displays : Topology-18");
       else if(p==19 && q ==10 )
       printf("\nTopologies available with the connected displays :Topology-19 &  Topology 40 ");
       else if(p==0 && q ==9)
       printf("\nTopologies available with the connected displays :Topology-22");
       else if(p==18 && q ==12 )
       printf("\nTopologies available with the connected displays :Topology-24 & Topology-25");
       else if(q==14 && r ==16 )
       printf("\nTopologies available with the connected displays :Topology-31 & Topology-32");
       else if(q==14 && r ==16 && s==0 )
       printf("\nTopologies available with the connected displays :Topology-33");
       else if(q==14 && r ==16 && s==2 )
       printf("\nTopologies available with the connected displays :Topology-34");
       else if(p==6 && q ==8 )
       printf("\nTopologies available with the connected displays : Topology-36");
       else if(p==2 && q ==8 )
       printf("\nTopologies available with the connected displays : Topology-37");
       else if(p==19 && q ==11 )
       printf("\nTopologies available with the connected displays :Topology-41");
       else if(p==2 && q ==9 )
       printf("\n Topologies available with the connected displays : Topology-44");
       else if(p==3 && q ==9 )
       printf("\n Topologies available with the connected displays : Topology-45");
       else if(p==0 && q ==9 )
       printf("\n Topologies available with the connected displays : Topology-46");
       else if(p==3 && q ==8 )
       printf("\n Topologies available with the connected displays : Topology-49");
       else if(q ==9 )
       printf("\n Topologies available with the connected displays : Topology-50");
       else if(p==3 && q ==6 )
       printf("\nTopologies available with the connected displays : Topology-57 & Topology-59");
       
   }
   else
   printf("\nNo display's was connected");
   return 0;
}
