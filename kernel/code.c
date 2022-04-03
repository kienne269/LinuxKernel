#include<linux/init.h>
#include<linux/module.h>

// khoi tao chuoi s1
char s1[] = "Xin chao moi nguoi";

int i = 0;

static int p02_init(void)
{
    	printk(KERN_ALERT "Khoi tao ok\n");
            
        // su dung vong lap while tinh do dai cua chuoi
        // '\0' la ky tu NULL ket thuc cua mot chuoi
        while(s1[i] != '\0') {
            i++;
        }
            
        printk("Do dai cua chuoi '%s' la: %d", s1, i);

        return 0;
}

static void p02_exit(void)
{
    printk(KERN_ALERT "Ket thuc ok\n");

}

module_init(p02_init);
module_exit(p02_exit);

MODULE_LICENSE("GPL");