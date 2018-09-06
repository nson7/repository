/*
 *
 *
 */

#include <common.h>
#include <command.h>
#include <nand.h>
#include <asm/io.h>
#include <s3c2416.h>
#ifdef CONFIG_GZSD_LCD
#include <lcd/s3c2416_fb.h>
#endif /* CONFIG_GZSD_LCD */
#define GZSD_MODIFY_MENU	1

extern char console_buffer[];
extern int readline (const char *const prompt);
extern char awaitkey(unsigned long delay, int* error_p);

#ifdef CONFIG_GZSD_LCD
void lcd_menu_usage()
{
    printf("\r\n##### LCD select Menu #####\r\n");
    printf("[1] T43\" screen.\r\n");
    printf("[2] A70TN92\" screen.\r\n");
    printf("[3] A104\" screen.\r\n");
    printf("[4] VGA1024768\" screen.\r\n");
    printf("[5] W35\" screen.\r\n");
    printf("[6] VGA640480\" screen.\r\n");
    printf("[r] Reboot u-boot\r\n");
    printf("[q] Return Parameter Menu \r\n");
    printf("Enter your selection: ");
}
void lcd_menu_shell()
{
    char c;
    char cmd_buf[256];
    while (1)
    {
        lcd_menu_usage();
        c = getc();
        printf("%c\n", c);
        switch (c)
        {

            case '1':
                {
                    sprintf(cmd_buf, "setenv lcdtype X480Y272;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                }

            case '2':
                {
                    sprintf(cmd_buf, "setenv lcdtype X800Y480;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                }

            case '3':
                {
                    sprintf(cmd_buf, "setenv lcdtype X800Y600;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                }

            case '4':
                {
                    sprintf(cmd_buf, "setenv lcdtype X1024Y768;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                }

            case '5':
                {
                    sprintf(cmd_buf, "setenv lcdtype X320Y240;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                }

            case '6':
                {
                    sprintf(cmd_buf, "setenv lcdtype X640Y480;");
                    run_command(cmd_buf, 0);
                    run_command("saveenv;", 0);
                    break;
                    break;
                }

            case 'R':
            case 'r':
                {
                    strcpy(cmd_buf, "reset");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'Q':
            case 'q':
                {
                    return;
                    break;
                }

        }
    }
}
#endif /* CONFIG_GZSD_LCD */


void param_menu_usage(void)
{
    printf("\r\n##### Parameter Menu #####\r\n");
    printf("[1] Set NFS boot parameter \r\n");
    printf("[2] Set Yaffs boot parameter for Standard Linux \r\n");
    printf("[3] Set UBIfs boot parameter for Standard Linux \r\n");
    printf("[4] Set parameter \r\n");
    printf("[5] View the parameters \r\n");
    printf("[d] Delete parameter \r\n");
    printf("[s] Save the parameters to Nand Flash \r\n");
    printf("[q] Return main Menu \r\n");
    printf("Enter your selection: ");
}


void param_menu_shell(void)
{
    char c;
    char cmd_buf[256];
    char name_buf[20];
    char val_buf[256];
    char param_buf1[25];
    char param_buf2[25];
    char param_buf3[25];
    char param_buf4[64];
    char param_buf5[10];

    while (1)
    {
        param_menu_usage();
#if GZSD_MODIFY_MENU
        c = getc();
#else
        c = awaitkey(-1, NULL);
#endif
        printf("%c\n", c);
        switch (c)
        {
            case '1':
                {
                    sprintf(cmd_buf, "setenv bootargs ");

                    printf("Enter the PC IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf1,console_buffer);
                    printf("Enter the gzsd2416 IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf2,console_buffer);
                    printf("Enter the Mask IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf3,console_buffer);
                    printf("Enter NFS directory:(eg: /gzsd/nfs)\n");
                    readline(NULL);
                    strcpy(param_buf4,console_buffer);
                    sprintf(cmd_buf, "setenv bootargs noinitrd init=/linuxrc console=ttySAC0 root=/dev/nfs nfsroot=%s:%s ip=%s:%s:%s:%s::eth0:off", param_buf1, param_buf4, param_buf2, param_buf1, param_buf2, param_buf3);
                    printf("bootargs: noinitrd init=/linuxrc console=ttySAC0 root=/dev/nfs nfsroot=%s:%s ip=%s:%s:%s:%s::eth0:off\n", param_buf1, param_buf4, param_buf2, param_buf1, param_buf2, param_buf3);

                    run_command(cmd_buf, 0);
                    break;
                }

            case '2':
                {
                    sprintf(cmd_buf, "setenv bootargs noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/linuxrc console=ttySAC0");
                    printf("bootargs: noinitrd root=/dev/mtdblock3 rootfstype=yaffs2 init=/linuxrc console=ttySAC0\n");

                    run_command(cmd_buf, 0);
                    break;
                }

            case '3':
                {
                    sprintf(cmd_buf, "setenv bootargs noinitrd ubi.mtd=3 root=ubi0:rootfs rootfstype=ubifs init=/linuxrc console=ttySAC0");
                    printf("bootargs: noinitrd ubi.mtd=3 root=ubi0:rootfs rootfstype=ubifs init=/linuxrc console=ttySAC0\n");

                    run_command(cmd_buf, 0);
                    break;
                }

            case '4':
                {
                    sprintf(cmd_buf, "setenv ");

                    printf("Name: ");
                    readline(NULL);
                    strcat(cmd_buf, console_buffer);

                    printf("Value: ");
                    readline(NULL);
                    strcat(cmd_buf, " ");
                    strcat(cmd_buf, console_buffer);
                    printf("%s\n",cmd_buf);

                    run_command(cmd_buf, 0);
                    break;
                }

            case '5':
                {
                    strcpy(cmd_buf, "printenv ");
                    printf("Name(enter to view all paramters): ");
                    readline(NULL);
                    strcat(cmd_buf, console_buffer);
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'D':
            case 'd':
                {
                    sprintf(cmd_buf, "setenv ");

                    printf("Name: ");
                    readline(NULL);
                    strcat(cmd_buf, console_buffer);

                    run_command(cmd_buf, 0);
                    break;
                }

            case 'S':
            case 's':
                {
                    sprintf(cmd_buf, "saveenv");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'Q':
            case 'q':
                {
                    //sprintf(cmd_buf, "saveenv");
                    //run_command(cmd_buf, 0);
                    return;
                    break;
                }
        }
    }
}


#define USE_USB_DOWN		1
#define USE_TFTP_DOWN		2
#define USE_SD_DOWN			3

void main_menu_usage(char menu_type)
{
    printf("\r\n#####	 BootLoad for gzsd2416 v1.1	#####\r\n");
    printf("\r\n#####	 Boot for gzsd2416 Main Menu	#####\r\n");

    if( menu_type == USE_USB_DOWN)
    {
        printf("#####     Gzsdinfo USB download mode     #####\r\n\n");
    }
    else if( menu_type == USE_TFTP_DOWN)
    {
        printf("#####     Gzsdinfo TFTP download mode     #####\r\n\n");
    }
#ifdef CONFIG_CMD_MENU_SD
    else if( menu_type == USE_SD_DOWN)
    {
        printf("#####     Gzsdinfo SD download mode     #####\r\n\n");
#if 1
        printf("[h] init TF card,card size more than or equal to 4G\r\n");
        printf("[m] init TF card,card size less than 4G\r\n");
#endif
    }
#endif

    if( menu_type == USE_USB_DOWN)
    {
        printf("[d] Download u-boot or zImage or filesystem to Nand Flash\r\n");

    }
    else if(menu_type == USE_TFTP_DOWN)
    {
        printf("[1] Download u-boot.bin to Nand Flash\r\n");
    }

    if(menu_type == USE_SD_DOWN)
    {
        printf("[1] Download u-boot.bin to Nand Flash\r\n");
        printf("[2] Download zImage.bin to Nand Flash\r\n");
        printf("[3] Download root.bin to Nand Flash\r\n");
        printf("[4] Download all file to Nand Flash\r\n");
    }

    if( menu_type == USE_TFTP_DOWN)
    {
        printf("[2] Download Eboot (eboot.nb0) to Nand Flash\r\n");
        printf("[3] Download Linux Kernel (zImage.bin) to Nand Flash\r\n");
        printf("[4] Download LOGO Picture (logo.bin) to Nand  Flash\r\n");
        printf("[5] Download UBIFS image (root.ubi) to Nand Flash\r\n");
        printf("[6] Download YAFFS image (root.bin) to Nand Flash\r\n");
        printf("[7] Download Program to SDRAM and Run it\r\n");
    }

    printf("[b] Boot the system\r\n");
    printf("[9] Format the Nand Flash\r\n");
    printf("[0] Set the boot parameters\r\n");
#ifdef CONFIG_GZSD_LCD
    printf("[c] Choice lcd type.\r\n");
#endif /* CONFIG_GZSD_LCD */

    if( menu_type == USE_USB_DOWN)
    {
        //printf("[n] Enter TFTP download mode menu\r\n");
    }

    else if( menu_type == USE_TFTP_DOWN)
    {
        printf("[n] Set TFTP parameters(PC IP,gzsd2416 IP,Mask IP...)\r\n");
        printf("[d] User default TFTP parameters(192.168.1.30,192.168.1.15,255.255.255.0)\r\n");
    }

    if( menu_type == USE_TFTP_DOWN)
    {
        printf("[p] Test network (gzsd2416 Ping PC's IP)\r\n");
    }

    printf("[r] Reboot u-boot\r\n");

    if( menu_type == USE_USB_DOWN)
    {
#ifdef CONFIG_CMD_MENU_SD
        printf("[s] Enter SD download mode menu\r\n");
#endif
    }

    else if(menu_type == USE_TFTP_DOWN)
    {
        printf("[s] Download STEPLDR.nb1 to Nand Flash\r\n");
    }

    //printf("[t] Test Linux Image (zImage)\r\n");
#ifndef CONFIG_GZSD2416
    if( menu_type == USE_USB_DOWN)
    {
        printf("[q] quit from menu\r\n");
    }
    else
#endif
        if(( menu_type == USE_TFTP_DOWN) || ( menu_type == USE_SD_DOWN))
        {
            printf("[q] Return main Menu \r\n");
        }
    if(menu_type == USE_USB_DOWN)
    {
        printf("[p] Printf Environment\r\n");
        printf("[q] Quit main Menu \r\n");
        printf("[l] led on \r\n");
    }

    printf("Enter your selection: ");
}


void tftp_menu_shell(void)
{
    char keyselect;
    char cmd_buf[200];

    while (1)
    {
        main_menu_usage(USE_TFTP_DOWN);
#if 0
        while(!(((keyselect >= '0') && (keyselect <= '9')) ||
                    ((keyselect == 'a') || (keyselect == 'A')) ||
                    ((keyselect == 'n') || (keyselect == 'N')) ||
                    ((keyselect == 'p') || (keyselect == 'P')) ||
                    ((keyselect == 'r') || (keyselect == 'R')) ||
                    ((keyselect == 's') || (keyselect == 'S')) ||
                    ((keyselect == 't') || (keyselect == 'T')) ||
                    ((keyselect == 'q') || (keyselect == 'Q')) ))
        {
            keyselect = serial_getc();
        }
#else
#if GZSD_MODIFY_MENU
        keyselect = getc();
#else
        keyselect = awaitkey(-1, NULL);
#endif
#endif
        printf("%c\n", keyselect);
        switch (keyselect)
        {
            case '1':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "tftp 0xc0000000 u-boot.bin; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)");
#else
                    strcpy(cmd_buf, "tftp 0xc0000000 u-boot.bin; nand erase 0x0 0x80000; nand write.jffs2 0xc0000000 0x0 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '2':
                {
                    sprintf(cmd_buf, "tftp 0xc0000000 eboot.nb0; nand erase 0x80000 0x80000; nand write.jffs2 0xc0000000 0x80000 $(filesize)");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '3':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "tftp 0xc0000000 zImage.bin; nand erase kernel; nand write.jffs2 0xc0000000 kernel $(filesize)");
#else
                    strcpy(cmd_buf, "tftp 0xc0000000 zImage.bin; nand erase 0x300000 0x400000; nand write.jffs2 0xc0000000 0x300000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '4':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "tftp 0xc0000000 logo.bin; nand erase logo; nand write.jffs2 0xc0000000 logo $(filesize)");
#else
                    strcpy(cmd_buf, "tftp 0xc0000000 logo.bin; nand erase 0x100000 0x200000; nand write.jffs2 0xc0000000 0x100000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '5':
                {
                    strcpy(cmd_buf, "tftp 0xc0000000 root.ubi;nand erase root;ubi part root;ubi create rootfs;ubi write 0xc0000000 rootfs $(filesize)");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '6':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "tftp 0xc0000000 root.bin; nand erase root; nand write.yaffs 0xc0000000 root $(filesize)");
#else
                    strcpy(cmd_buf, "tftp 0xc0000000 root.bin; nand erase 0x700000 0xF8D0000; nand write.yaffs 0xc0000000 0x700000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '7':
                {
                    char tftpaddress[12];
                    char filename[32];

                    printf("Enter downloads to SDRAM address:\n");
                    readline(NULL);
                    strcpy(tftpaddress, console_buffer);

                    printf("Enter program name:\n");
                    readline(NULL);
                    strcpy(filename, console_buffer);

                    sprintf(cmd_buf, "tftp %s %s", tftpaddress, filename);
                    printf("tftp %s %s\n", tftpaddress, filename);
                    run_command(cmd_buf, 0);

                    sprintf(cmd_buf, "go %s", tftpaddress);
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'B':
            case 'b':
                {
                    printf("Start Linux ...\n");
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 kernel;bootm 0xc0008000");
#else
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 0x300000 0x400000;bootm 0xc0008000");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '9':
                {
                    strcpy(cmd_buf, "nand scrub ");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '0':
                {
                    param_menu_shell();
                    break;
                }

            case 'A':
            case 'a':
                {
                    char filename[32];

                    printf("Enter program name:\n");
                    readline(NULL);
                    strcpy(filename, console_buffer);

                    sprintf(cmd_buf, "tftp 0xc0000000 %s; nand erase 0x0 $(filesize+1); nand write.jffs2 0xc0000000 0x0 $(filesize+1)", filename);
                    run_command(cmd_buf, 0);
                    break;
                }

#ifdef CONFIG_GZSD_LCD
            case 'C':
            case 'c':
                {
                    lcd_menu_shell();
                    break;
                }
#endif /* CONFIG_GZSD_LCD */

            case 'N':
            case 'n':
                {
                    char param_buf1[25];
                    char param_buf2[25];
                    char param_buf3[25];

                    printf("Enter the TFTP Server(PC) IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf1,console_buffer);
                    sprintf(cmd_buf, "setenv serverip %s",param_buf1);
                    run_command(cmd_buf, 0);

                    printf("Enter the gzsd2416 IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf2,console_buffer);
                    sprintf(cmd_buf, "setenv ipaddr %s",param_buf2);
                    run_command(cmd_buf, 0);

                    printf("Enter the Mask IP address:(xxx.xxx.xxx.xxx)\n");
                    readline(NULL);
                    strcpy(param_buf3,console_buffer);
                    sprintf(cmd_buf, "setenv netmask %s",param_buf3);
                    run_command(cmd_buf, 0);

                    printf("Save TFTP IP parameters?(y/n)\n");
                    if (getc() == 'y' )
                    {
                        printf("y");
                        getc() == '\r';
                        printf("\n");
                        sprintf(cmd_buf, "saveenv");
                        run_command(cmd_buf, 0);
                    }
                    else
                    {
                        printf("Not Save it!!!\n");
                    }
                    break;
                }

            case 'D':
            case 'd':
                {
                    sprintf(cmd_buf, "setenv serverip %s","192.168.1.30");
                    run_command(cmd_buf, 0);

                    sprintf(cmd_buf, "setenv ipaddr %s","192.168.1.15");
                    run_command(cmd_buf, 0);

                    sprintf(cmd_buf, "setenv netmask %s","255.255.255.0");
                    run_command(cmd_buf, 0);

                    sprintf(cmd_buf, "saveenv");
                    run_command(cmd_buf, 0);
                    break;

                }
            case 'P':
            case 'p':
                {
                    char *serverip;
                    serverip=getenv("serverip");
                    printf("gzsd2416 ping PC IP:ping %s\n",serverip);
                    sprintf(cmd_buf, "ping %s",serverip);
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'R':
            case 'r':
                {
                    strcpy(cmd_buf, "reset");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 's':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "tftp 0xc0000000 stepldr.nb1; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)");
#else
                    strcpy(cmd_buf, "tftp 0xc0000000 stepldr.nb1; nand erase 0x0 0x80000; nand write.jffs2 0xc0000000 0x0 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'T':
            case 't':
                {
                    strcpy(cmd_buf, "tftp 0xc0008000 zImage.bin; bootm 0xc0008000");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'Q':
            case 'q':
                {
                    return;
                    break;
                }
        }
    }

}

#ifdef CONFIG_CMD_MENU_SD
void sd_menu_shell(void)
{
    char keyselect;
    char cmd_buf[200];

    while (1)
    {
        main_menu_usage(USE_SD_DOWN);
        keyselect = awaitkey(-1, NULL);
        printf("%c\n", keyselect);
        switch (keyselect)
        {
#if 1
            case 'H':
            case 'h':
                {
                    strcpy(cmd_buf, "movi init mmc 1");
                    run_command(cmd_buf, 0);
                    break;
                }
            case 'M':
            case 'm':
                {
                    strcpy(cmd_buf, "movi init mmc 0");
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
#if 0
            case '1':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/u-boot.bin; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)");
#else
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/u-boot.bin; nand erase 0x0 0x80000; nand write.jffs2 0xc0000000 0x0 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '2':
                {
                    sprintf(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/eboot.nb0; nand erase 0x80000 0x80000; nand write.jffs2 0xc0000000 0x80000 $(filesize)");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '3':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/zImage.bin; nand erase kernel; nand write.jffs2 0xc0000000 kernel $(filesize)");
#else
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/zImage.bin; nand erase 0x300000 0x400000; nand write.jffs2 0xc0000000 0x300000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '4':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/logo.bin; nand erase logo; nand write.jffs2 0xc0000000 logo $(filesize)");
#else
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/logo.bin; nand erase 0x100000 0x200000; nand write.jffs2 0xc0000000 0x100000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '5':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/root.ubi;nand erase root;ubi part root;ubi create rootfs;ubi write 0xc0000000 rootfs $(filesize)");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '6':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/root.bin; nand erase root; nand write.yaffs 0xc0000000 root $(filesize)");
#else
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/root.bin; nand erase 0x700000 0xF8D0000; nand write.yaffs 0xc0000000 0x700000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '7':
                {
                    char sdaddress[12];
                    char filename[32];
                    printf("Enter downloads to SDRAM address:\n");
                    readline(NULL);
                    strcpy(sdaddress, console_buffer);

                    printf("Enter program name:\n");
                    readline(NULL);
                    strcpy(filename, console_buffer);

                    sprintf(cmd_buf, "fatload mmc 0 %s %s", sdaddress, filename);
                    printf("fatload mmc 0 %s %s\n", sdaddress, filename);
                    run_command(cmd_buf, 0);

                    sprintf(cmd_buf, "go %s", sdaddress);
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
            case 'B':
            case 'b':
                {
                    printf("Start Linux ...\n");
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 kernel;bootm 0xc0008000");
#else
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 0x300000 0x400000;bootm 0xc0008000");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '9':
                {
                    strcpy(cmd_buf, "nand scrub ");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '0':
                {
                    param_menu_shell();
                    break;
                }
#if 0
            case 'A':
            case 'a':
                {
                    char filename[32];

                    printf("Enter program name:\n");
                    readline(NULL);
                    strcpy(filename, console_buffer);

                    sprintf(cmd_buf, "fatload mmc 0 0xc0000000 %s; nand erase 0x0 $(filesize+1); nand write.jffs2 0xc0000000 0x0 $(filesize+1)", filename);
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
#ifdef CONFIG_GZSD_LCD
            case 'C':
            case 'c':
                {
                    lcd_menu_shell();
                    break;
                }
#endif /* CONFIG_GZSD_LCD */

            case 'R':
            case 'r':
                {
                    strcpy(cmd_buf, "reset");
                    run_command(cmd_buf, 0);
                    break;
                }
#if 0
            case 's':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/stepldr.nb1; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)");
#else
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/stepldr.nb1; nand erase 0x0 0x80000; nand write.jffs2 0xc0000000 0x0 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'T':
            case 't':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/zImage.bin; bootm 0xc0008000");
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
            case '1':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/u-boot.bin"); //hclydao
                    run_command(cmd_buf, 0);
                    break;
                }
            case '2':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/zImage.bin");
                    run_command(cmd_buf, 0);
                    break;
                }
            case '3':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/root.bin");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '4':
                {
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/u-boot.bin"); //hclydao
                    run_command(cmd_buf, 0);
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/zImage.bin");
                    run_command(cmd_buf, 0);
                    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/root.bin");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'Q':
            case 'q':
                {
                    return;
                    break;
                }
        }
    }

}
#endif

void gzsd_status(int status)
{
    u32 reg;
    int i;
    reg = readl(GPBCON);
    reg &= ~(0x3 << 2);
    reg |= (0x1 << 2);
    writel(reg,GPBCON);
    reg = readl(GPBDAT);
    if(status > 0) {
        for(i=0;i<status;i++)
        {
            reg |= (1 << 1);
            writel(reg,GPBDAT);
            udelay(100000);
            reg &= ~(1 << 1);
            writel(reg,GPBDAT);
            udelay(100000);
        }
    }
    else if(status == 0)
    {
        for(i=0;i<4;i++)
        {
            reg |= (1 << 1);
            writel(reg,GPBDAT);
            udelay(100000);
            udelay(100000);
            reg &= ~(1 << 1);
            writel(reg,GPBDAT);
            udelay(1000000);
            udelay(1000000);
        }
        while(1);
    }
    else if(status < 0)
    {
        while(1) {
            reg |= (1 << 1);
            writel(reg,GPBDAT);
            udelay(100000);
            reg &= ~(1 << 1);
            writel(reg,GPBDAT);
            udelay(100000);
        }
    }

}

void gzsd_auto()
{
#if 0
    char cmd_buf[200];
    printf("start load filesystem file\n");
    gzsd_status(1);
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/u-boot.bin; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)");
    run_command(cmd_buf, 0);
    gzsd_status(2);
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/zImage.bin; nand erase kernel; nand write.jffs2 0xc0000000 kernel $(filesize)");
    run_command(cmd_buf, 0);
    gzsd_status(3);
    //strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/root.bin; nand erase root; nand write.yaffs 0xc0000000 root $(filesize)");
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd/root.ubi;nand erase root;ubi part root;ubi create rootfs;ubi write 0xc0000000 rootfs $(filesize)");
    run_command(cmd_buf, 0);
    printf("\n***load filesystem file success,now you can start from nand flash***\n");
    gzsd_status(0);
#else
    char cmd_buf[200];
    printf("\n********start load filesystem file************\n");
    printf("first format nandflash\n");
    strcpy(cmd_buf, "nand erase 0");
    run_command(cmd_buf, 0);
    gzsd_status(1);
    printf("++load bootload...\n");
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/u-boot.bin");
    run_command(cmd_buf, 0);
    gzsd_status(2);
    printf("++load kernel...\n");
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/zImage.bin");
    run_command(cmd_buf, 0);
    gzsd_status(3);
    printf("++load filesystem...\n");
    strcpy(cmd_buf, "fatload mmc 0 0xc0000000 /gzsd2416/root.bin");
    run_command(cmd_buf, 0);
    printf("\n***load filesystem file success,now you can start from nand flash***\n");
    gzsd_status(0);
#endif
}

static inline void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n" "bne 1b":"=r" (loops):"0"(loops));
}

void menu_shell(void)
{
    char keyselect;
    char cmd_buf[200];
    while (1)
    {
        main_menu_usage(USE_USB_DOWN);
#if 0
        while(!(((keyselect >= '0') && (keyselect <= '9')) ||
                    ((keyselect == 'a') || (keyselect == 'A')) ||
                    ((keyselect == 'n') || (keyselect == 'N')) ||
                    ((keyselect == 'r') || (keyselect == 'R')) ||
                    ((keyselect == 't') || (keyselect == 'T')) ||
                    ((keyselect == 'q') || (keyselect == 'Q')) ))
        {
            keyselect = serial_getc();
        }
#else

#if GZSD_MODIFY_MENU
        keyselect = getc();
#else
        keyselect = awaitkey(-1, NULL);
#endif
#endif
        printf("%c\n", keyselect);
        switch (keyselect)
        {
#if 0
            case '1':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase bios; nand write.jffs2 0xc0000000 bios $(filesize)"); //0x80000");//
#else
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase 0x0 0x80000; nand write.jffs2 0xc0000000 0x0 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '2':
                {
                    sprintf(cmd_buf, "dnw 0xc0000000; nand erase 0x80000 0x80000; nand write.jffs2 0xc0000000 0x80000 $(filesize)");
                    run_command(cmd_buf, 0);
                    break;
                }

            case '3':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "dnw 0xc0008000; nand erase kernel; nand write.jffs2 0xc0008000 kernel $(filesize)");//0x400000");//
#else
                    strcpy(cmd_buf, "dnw 0xc0008000; nand erase 0x300000 0x400000; nand write.jffs2 0xc0008000 0x300000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '4':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase logo; nand write.jffs2 0xc0000000 logo  $(filesize)");
#else
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase 0x100000 0x200000; nand write.jffs2 0xc0000000 0x100000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '5':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "dnw 0xc0000000;nand erase root;ubi part root;ubi create rootfs;ubi write 0xc0000000 rootfs  $(filesize)");
#else
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase 0x700000 0xF8D0000; nand write.jffs2 0xc0000000 0x700000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '6':
                {
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase root; nand write.yaffs 0xc0000000 root $(filesize)");
#else
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase 0x700000 0xF8D0000; nand write.yaffs 0xc0000000 0x700000 $(filesize)");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '7':
                {
                    char addr_buff[12];
                    printf("Enter download address:(eg: 0xc0000000)\n");
                    readline(NULL);
                    strcpy(addr_buff,console_buffer);
                    sprintf(cmd_buf, "dnw %s;go %s", addr_buff, addr_buff);
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
            case 'B':
            case 'b':
                {
                    printf("Start Linux ...\n");
#ifdef CONFIG_MTD_DEVICE
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 kernel;bootm 0xc0008000");
#else
                    strcpy(cmd_buf, "nand read.jffs2 0xc0008000 0x300000 0x400000;bootm 0xc0008000");
#endif
                    run_command(cmd_buf, 0);
                    break;
                }

            case '9':
                {
                    strcpy(cmd_buf, "nand scrub ");
                    run_command(cmd_buf, 0);
                    break;
                }
            case '0':
                {
                    param_menu_shell();
                    break;
                }
#if 0
            case 'A':
            case 'a':
                {
                    strcpy(cmd_buf, "dnw 0xc0000000; nand erase 0x0 $(filesize+1); nand write.jffs2 0xc0000000 0x0 $(filesize+1)");
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
#ifdef CONFIG_GZSD_LCD
            case 'C':
            case 'c':
                {
                    lcd_menu_shell();
                    break;
                }
#endif /* CONFIG_GZSD_LCD */

            case 'N':
            case 'n':
                {
                    tftp_menu_shell();
                    break;
                }

            case 'R':
            case 'r':
                {
                    strcpy(cmd_buf, "reset");
                    run_command(cmd_buf, 0);
                    break;
                }
#if 1
#ifdef CONFIG_CMD_MENU_SD
            case 'S':
            case 's':
                {
                    //movi_set_capacity();
                    //movi_set_ofs(0x40004000 - 0x4);
                    //movi_init();
                    sd_menu_shell();
                    break;
                }
#endif
#endif
#if 0
            case 'T':
            case 't':
                {
                    strcpy(cmd_buf, "dnw 0xc0008000; bootm 0xc0008000");
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
#if 0
            case 'C':
            case 'c':
                {
                    strcpy(cmd_buf, "dnw 0xc1000000; relocateNK 0xc1000000 $(filesize); wince");
                    run_command(cmd_buf, 0);
                    break;
                }

            case 'F':
            case 'f':
                {
                    strcpy(cmd_buf, "sdread 0xc0008000; bootm 0xc0008000");
                    run_command(cmd_buf, 0);
                    break;
                }
#endif
#if 0
            case 'L':
            case 'l':
                {
                    char *t_ma=NULL ;
                    t_ma=(char *)malloc(1024*512);
                    if(!t_ma)
                    {
                        printf("malloc failed!\n");
                    }
                    else{
                        printf("malloc success!\n");
                        free(t_ma);
                        t_ma=NULL;
                    }
                    break;
                }
#endif
            case 'd':
            case 'D':
                {
                    strcpy(cmd_buf, "dnw 0xc0000000");
                    run_command(cmd_buf, 0);
                    break;
                }
            case 'P':
            case 'p':
                {
                    strcpy(cmd_buf, "printenv");
                    run_command(cmd_buf, 0);
                    break;
                }
            case 'Q':
            case 'q':
                {
                    return;
                }
            case 'L':
            case 'l':
                {
                    printf("led!\n");

#define rGPACON   	(*(volatile unsigned *)0x56000000)	//Configure the pins of port A
#define rGPADAT   	(*(volatile unsigned *)0x56000004)	//The data for port A

                    rGPACON &= ~(1 << 15);
                    rGPACON &= ~(1 << 16);

                    rGPADAT &= ~(1 << 15);
                    rGPADAT &= ~(1 << 16);
                    delay(30);
                    rGPADAT |= (1 << 15);
                    rGPADAT |= ~(1 << 16);
                    delay(30);
                    return;
                }
        }

    }
}

int do_menu (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
    menu_shell();
    return 0;
}

U_BOOT_CMD(
        menu,	3,	0,	do_menu,
        "display a menu, to select the items to do something",
        "\n"
        "\tdisplay a menu, to select the items to do something"
        );

