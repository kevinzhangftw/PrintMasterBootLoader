//
//  main.c
//  mbr
//
//  Created by Kai Zhang on 2017-03-27.
//  Copyright © 2017 Kai Zhang. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct partition{
    unsigned char boot_flag;
    unsigned char chs_begin[3];
    unsigned char sys_type;
    unsigned char chs_end[3];
    unsigned char start_sector[4];
    unsigned char numbytesread_sector[4];
};

int i = 1;
int Numbytesread = 0;
int position = -1;
int filedes = 0;
char  buffer[512] ;
struct partition *partitionpointer;
int ret = 0;

void chkdevsda();
void getpos();
void readbuf();
void     closefd();
void string_in_hex(void *in_string, int in_string_size);
void     dumpmbr();
void     dumpMainPartition();
void dump_partition(struct partition *part, int partition_number);

int main(int argc, const char * argv[]) {
    chkdevsda();
    getpos();
    readbuf();
    closefd();
    dumpmbr();
    dumpMainPartition();
    return 0;
}


void     chkdevsda(){
        if ((filedes = open("/dev/sda", O_RDONLY | O_SYNC)) == -1){
            perror("Open");
            exit(1);
        }else{
            printf("fd is %d\n", filedes);
        }
}

void getpos(){
    position = lseek (filedes, 0, SEEK_CUR);
    printf("Position of pointer is :%d\n", position);
}

void readbuf(){
    if ((Numbytesread=read(filedes, buffer, sizeof(buffer))) == -1){
        perror("Read");
        exit(1);
    }
}

void     closefd(){
    ret = close(filedes);
    if (ret == -1){
        perror("close");
        exit(1);
    }
}

void checknextline(int j){
    if (j == 16){
        printf("\n");
        j = 0;
    }
    
}

void string_in_hex(void *in_string, int in_string_size){
    int i;
    int j = 0;
    for (i = 0; i < in_string_size; i++){
        printf("%02x ", ((char *)in_string)[i]& 0xFF);
        j = j + 1;
        checknextline(j);
    }
    printf("\n");
}

void     dumpmbr(){
    string_in_hex(buffer, 512);
    printf("Buffer Size = %d - Numbytesread: %d\n", sizeof(buffer), Numbytesread);
}

void     dumpMainPartition(){
    for (i = 0 ; i < 4 ; i++){
        partitionpointer = (struct partition *)(buffer + 446 + (16 * i));
        //putchar(sp->boot_flag);
        dump_partition(partitionpointer, i);
    }
}

void dump_partition(struct partition *part, int partition_number){
    printf("Partition /dev/sda%d\n", partition_number + 1);
    printf("numbytesread_sector = ");
    string_in_hex(part->numbytesread_sector, 4);
    printf("boot_flag = %02X\n", part->boot_flag);
    printf("chs_begin = ");
    string_in_hex(part->chs_begin, 3);
    printf("sys_type = %02X\n", part->sys_type);
    printf("chs_end = ");
    string_in_hex(part->chs_end, 3);
    printf("start_sector = ");
    string_in_hex(part->start_sector, 4);
}
